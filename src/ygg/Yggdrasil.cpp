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
 * File:   Yggdrasil.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 18, 2021, 9:32 PM
 */

#include "Yggdrasil.h"
#include <map>
#include <string>
#include <iostream>

namespace ygg
{
  /** The default error handler.
   * @param error The error to handle.
   */
  static void defaultHandler( ygg::Yggdrasil::Error error ) ;

  /** Function to convert an Yggdrasil error to a string.
   * @return A string representation of the error.
   */
  static std::string toString( Yggdrasil::Error error ) ;
  
  /** Function to find the severity of an Yggdrasil error.
   * @param error The error to find the severity of.
   * @return The severity of the error.
   */
  static Yggdrasil::Severity severity( Yggdrasil::Error error ) ;
  
  /** Structure to contain all of the Yggdrasil library's static data.
   */
  struct YggdrasilData
  {
    typedef void ( *Callback )( Yggdrasil::Error ) ;
    
    Callback                 error_cb ;
    Yggdrasil::ErrorHandler* handler  ;
    
    /** Default constructor.
     */
    YggdrasilData() ;
  };
  
  /** Static container for the Yggdrasil library's data.
   */
  static YggdrasilData data ;

  void defaultHandler( ygg::Yggdrasil::Error error )
  {
    std::cout << "-- Yggdrasil Error: " << error.toString() << "." << std::endl ;
    if( error.severity() == Yggdrasil::Severity::Fatal ) exit( -1 ) ;
  }
  
  std::string toString( Yggdrasil::Error error )
  {
    switch( error.error() )
    {
      case Yggdrasil::Error::SocketCreationFailure :
        return "Socket Creation Failure" ;
        
      case Yggdrasil::Error::SslFailure :
        return "SSL Failed" ;
        
      case Yggdrasil::Error::SslContextFailure :
        return "SSL Context Failure" ;
        
      case Yggdrasil::Error::SslConnectionFailure :
        return "SSL Connection Failure" ;
        
      case Yggdrasil::Error::SslFDFailure : 
        return "SSL File Descriptor Failure" ;

      case Yggdrasil::Error::SslPrivateKeyCheckFailure :
        return "SSL Private Key Failure" ;
        
      case Yggdrasil::Error::SendFailure :
        return "Connection Send Failure" ;
        
      case Yggdrasil::Error::RecieveFailure :
        return "Connection Recieve Failure" ;
        
      case Yggdrasil::Error::ConnectionFailure :
        return "Error Establishing Connection" ;

      case Yggdrasil::Error::InvalidRead :
        return "Bad Read" ;

      case Yggdrasil::Error::InvalidIP :
        return "Could not find the IP of the hostname" ;

      case Yggdrasil::Error::None :
        return "None" ;

      default : return "Unknown" ;
    }
  }
  
  Yggdrasil::Severity severity( Yggdrasil::Error error )
  {
    switch( error.error() )
    {
      case Yggdrasil::Error::RecieveFailure : return Yggdrasil::Severity::Fatal ;
      case Yggdrasil::Error::None :
      default : return Yggdrasil::Severity::None ;
    }
  }
  
  YggdrasilData::YggdrasilData()
  {
    this->error_cb = &ygg::defaultHandler ;
    this->handler  = nullptr              ;
  }

  Yggdrasil::Error::Error()
  {
    this->val = Yggdrasil::Error::None ;
  }
  
  Yggdrasil::Error::Error( unsigned error ) 
  {
    this->val = error ;
  }
  
  Yggdrasil::Error& Yggdrasil::Error::operator=( unsigned error )
  {
    this->val = error ;
    
    return *this ;
  }
  
  Yggdrasil::Error::operator unsigned() const
  {
    return this->val ;
  }
  
  void Yggdrasil::Error::set( unsigned error )
  {
    this->val = error ;
  }
  
  unsigned Yggdrasil::Error::error()
  {
    return this->val ;
  }
  
  const char* Yggdrasil::Error::toString() const
  {
    static std::string str ;
    
    str = ygg::toString( *this ) ;

    return str.c_str() ;
  }
  
  Yggdrasil::Severity Yggdrasil::Error::severity() const
  {
    return ygg::severity( *this ) ;
  }

  void Yggdrasil::addError( Yggdrasil::Error error )
  {
    if( data.error_cb != nullptr )
    {
      ( data.error_cb )( error ) ;
    }
    
    if( data.handler != nullptr )
    {
      data.handler->handleError( error ) ;
    }
  }
  
  void Yggdrasil::setErrorHandler( void ( *error_handler )( Yggdrasil::Error ) )
  {
    data.error_cb = error_handler ;
  }
  
  void Yggdrasil::setErrorHandler( ygg::Yggdrasil::ErrorHandler* handler )
  {
    data.handler = handler ;
  }
}


