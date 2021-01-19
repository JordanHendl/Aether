/*
 * Copyright (C) 2021 Jordan Hendl
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
 * File:   Connection.h
 * Author: Jordan Hendl
 *
 * Created on January 18, 2021, 9:56 PM
 */

#ifndef AETHER_CONNECTION_H
#define AETHER_CONNECTION_H

namespace ae
{
  /** The type of connections available.
   */
  enum class ConnectionType
  {
    None,
    Client,
    Server
  };
  
  /** Library object to handle a connection.
   */
  template<typename Impl>
  class Connection
  {
    public:
      
      /** Default constructor.
       */
      Connection() = default ;
      
      /** Default deconstructor.
       */
      ~Connection() = default ;
      
      /** Conversion operator to convert this object to the underlying implementation's version.
       * @return The underlying implementation's connection object.
       */
      operator const typename Impl::Connection&() ;
      
      /** Method to start a connection.
       * @param host The C-string representation of the host name to connect to.
       * @param port The port number to use.
       * @param type The type of connection to make.
       */
      void connect( const char* host, ae::ConnectionType type = ae::ConnectionType::Client, unsigned port = 80 ) ;
      
      /** Method to retrieve whether or not this connection is successfully connected.
       * @return Whether or not this connection is valid & working correctly.
       */
      bool valid() const ;

      /** Method to send a message over the connection & retrieve a response.
       * @param command The HTTP command to send.
       */
      void sendHttp( const char* command ) ;
      
      /** Method to retrieve a message from the connection.
       * @return The data recieved from the connection.
       */
      const char* recieve() ;
      
      /** Method to reset this connection & end all traffic.
       */
      void reset() ;
    private:
      
      /** The underlying connection object.
       */
      typename Impl::Connection connection ;
  };
  
  template<typename Impl>
  Connection<Impl>::operator const typename Impl::Connection&()
  {
    return this->connection ;
  }
  
  template<typename Impl>
  void Connection<Impl>::connect( const char* host, ae::ConnectionType type, unsigned port )
  {
    this->connection.connect( host, type, port ) ;
  }
  
  template<typename Impl>
  void Connection<Impl>::sendHttp( const char* command )
  {
    this->connection.sendHttp( command ) ;
  }
  
  template<typename Impl>
  bool Connection<Impl>::valid() const
  {
    return this->connection.valid() ;
  }
  
  template<typename Impl>
  const char* Connection<Impl>::recieve()
  {
    return this->connection.recieve() ;
  }
  
  template<typename Impl>
  void Connection<Impl>::reset()
  {
    this->connection.reset() ;
  }
}

#endif /* CONNECTION_H */

