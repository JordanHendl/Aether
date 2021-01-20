/*
 * Copyright (C) 2020 Jordan Hendl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   Connection.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 18, 2021, 6:05 AM
 */

#include "Connection.h"
#include "Linux.h"
#include <ygg/Connection.h>
#include <ygg/Yggdrasil.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <array>
#include <assert.h>
#include <algorithm>

namespace ygg
{
  namespace lx
  {
    static bool ssl_initialized = false ;
    
    /** Structure to contain a linux connection's data.
     */
    struct ConnectionData
    {
      using Message    = std::vector<char>        ;
      using StringList = std::vector<std::string> ;
      using Buffer     = std::array<char, 20000>  ;
      

      SSL               *ssl               ;
      SSL_CTX           *context           ;
      BIO               *read_bio          ;
      BIO               *write_bio         ;
      unsigned           port              ;
      int                socket_descriptor ;
      Buffer             reply_buffer      ;
      sockaddr_in        server            ;
      Message            message           ;
      std::string        ip_address        ;
      std::string        host_name         ;
      ygg::ConnectionType type              ;
      bool               valid             ;

      /** Default constructor.
       */
      ConnectionData() ;
      
      /** Method to connect to the server provided.
       */
      void connect() ;
      
      /** Method to get an ip address from an input host name.
       * @param host_name The host name to connect to.
       * @return The string representation of an IP address to connect to.
       */
      std::string ipFromHostname( const char* host_name ) ;
      
      /** Method to initialize the SSL Context.
       */
      void initialize() ;
    };
    
    ConnectionData::ConnectionData()
    {
      this->valid             = false                      ;
      this->port              = 80                         ;
      this->socket_descriptor = 0x0                        ;
      this->type              = ygg::ConnectionType::Client ;
      this->ip_address        = ""                         ;
      this->host_name         = ""                         ;
    }
    
    void ConnectionData::initialize()
    {
      if( !ygg::lx::ssl_initialized )
      {
        SSL_library_init          () ;
        SSL_load_error_strings    () ;
        ERR_load_CRYPTO_strings   () ;
        SSL_load_error_strings    () ; 

        ygg::lx::ssl_initialized = true ;
      }
      
      this->context = SSL_CTX_new( SSLv23_method() ) ;
      this->ssl     = SSL_new    ( this->context   ) ;
      
      if( !this->context )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::SslContextFailure ) ;
        this->valid = false ;
        return ;
      };
        
      if( this->type == ygg::ConnectionType::Client ) 
      {
        SSL_set_connect_state( this->ssl ) ;
      }
      else
      {
        SSL_set_accept_state( this->ssl ) ;
      }
      
      SSL_CTX_use_certificate_file( this->context, Linux::certificate(), SSL_FILETYPE_PEM ) ;
      SSL_CTX_use_PrivateKey_file ( this->context, Linux::key()        , SSL_FILETYPE_PEM ) ;
      SSL_CTX_set_verify_depth( this->context, 1 ) ;

      if( SSL_connect( this->ssl ) != 1 )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::SslConnectionFailure ) ;
        this->valid = false ;
        return ;
      }
      
      if( SSL_set_fd( this->ssl , this->socket_descriptor ) != 1 )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::SslFDFailure ) ;
        this->valid = false ;
        return ;
      }
      
      if( SSL_CTX_check_private_key( this->context ) != 1 )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::SslPrivateKeyCheckFailure ) ;
        this->valid = false ;
        return ;
      }
      
      SSL_CTX_set_options( this->context, SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 ) ;

      this->read_bio  = BIO_new( BIO_s_mem() ) ;
      this->write_bio = BIO_new( BIO_s_mem() ) ;
      SSL_set_bio( this->ssl, this->read_bio, this->write_bio ) ;

    }
    
    void ConnectionData::connect()
    {
      this->server.sin_addr.s_addr = inet_addr( this->ip_address.c_str() ) ;
      this->server.sin_family      = AF_INET                               ;
      this->server.sin_port        = htons( this->port )                   ;
      
      if( ::connect( this->socket_descriptor, reinterpret_cast<sockaddr*>( &this->server ), sizeof( this->server ) ) < 0 )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::ConnectionFailure ) ;
        this->valid = false ;
      }
    }
    
    std::string ConnectionData::ipFromHostname( const char* host_name )
    {
      std::string ip           ;
      hostent    *host         ;
      in_addr   **address_list ;
      unsigned    index        ;
      
      if( ( host = gethostbyname( host_name ) ) == nullptr )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::InvalidIP ) ;
        this->valid = false ;
      }
      
      address_list = reinterpret_cast<in_addr**>( host->h_addr_list ) ;
      
      for( index = 0; address_list[ index ] != nullptr; index++ )
      {
        ip = std::string( ( inet_ntoa( *address_list[ index ] ) ) ) ;
      }
      
      return ip ;
    }
  
    Connection::Connection()
    {
      this->connection_data = new ConnectionData() ;
    }
    
    Connection::~Connection()
    {
      delete this->connection_data ;
    }
    
    void Connection::connect( const char* host_name, ygg::ConnectionType type, unsigned port )
    {
      data().port              = port                               ;
      data().socket_descriptor = socket( AF_INET, SOCK_STREAM, 0 )  ;
      data().ip_address        = data().ipFromHostname( host_name ) ;
      data().type              = type                               ;
      data().host_name         = host_name                          ;
      data().valid             = true                               ;

      data().connect()    ;
      data().initialize() ;
    }
    
    void Connection::sendHttp( const char* http_cmd )
    {
      std::stringstream cmd    ;
      std::string       string ;
      
      // Build command with hostname.
      cmd << std::string( http_cmd ) ;
      cmd << "HOST: " << data().host_name << "\r\n\r\n" ;
      
      string = cmd.str() ;
      
      // Send data.
      if( ::send( data().socket_descriptor, string.c_str(), string.size(), 0 ) < 0 )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::SendFailure ) ;
        data().valid = false ;
      }
    }
    
    bool Connection::valid() const
    {
      return data().valid ;
    }
    
    const char* Connection::recieve()
    {
      std::fill( data().reply_buffer.begin(), data().reply_buffer.end(), 0 ) ;
      if( ::recv( data().socket_descriptor, data().reply_buffer.data(), data().reply_buffer.size(), 0 ) < 0 )
      {
        ygg::Yggdrasil::addError( Yggdrasil::Error::RecieveFailure ) ;
        data().valid = false ;
      }
      
      data().reply_buffer.back() = '\0' ;
      return data().reply_buffer.data() ;
    }

    ConnectionData& Connection::data()
    {
      return *this->connection_data ;
    }
    
    const ConnectionData& Connection::data() const
    {
      return *this->connection_data ;
    }
  }
}

