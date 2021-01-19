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
 * File:   Aether.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 18, 2021, 9:32 PM
 */

#include "Aether.h"
#include <map>
#include <string>
#include <iostream>

namespace ae
{
  /** The default error handler.
   * @param error The error to handle.
   */
  static void defaultHandler( ae::Aether::Error error ) ;

  /** Function to convert an Aether error to a string.
   * @return A string representation of the error.
   */
  static std::string toString( Aether::Error error ) ;
  
  /** Function to find the severity of an Aether error.
   * @param error The error to find the severity of.
   * @return The severity of the error.
   */
  static Aether::Severity severity( Aether::Error error ) ;
  
  /** Structure to contain all of the Aether library's static data.
   */
  struct AetherData
  {
    typedef void ( *Callback )( Aether::Error ) ;
    
    Callback              error_cb ;
    Aether::ErrorHandler* handler  ;
    
    /** Default constructor.
     */
    AetherData() ;
  };
  
  /** Static container for the Aether library's data.
   */
  static AetherData data ;

  void defaultHandler( ae::Aether::Error error )
  {
    std::cout << "-- Aether Error: " << error.toString() << "." << std::endl ;
  }
  
  std::string toString( Aether::Error error )
  {
    switch( error.error() )
    {
      case Aether::Error::SocketCreationFailure :
        return "Socket Creation Failure" ;
        
      case Aether::Error::SslFailure :
        return "SSL Failed" ;
        
      case Aether::Error::SslContextFailure :
        return "SSL Context Failure" ;
        
      case Aether::Error::SslConnectionFailure :
        return "SSL Connection Failure" ;
        
      case Aether::Error::SslFDFailure : 
        return "SSL File Descriptor Failure" ;

      case Aether::Error::SslPrivateKeyCheckFailure :
        return "SSL Private Key Failure" ;
        
      case Aether::Error::SendFailure :
        return "Connection Send Failure" ;
        
      case Aether::Error::RecieveFailure :
        return "Connection Recieve Failure" ;
        
      case Aether::Error::ConnectionFailure :
        return "Error Establishing Connection" ;

      case Aether::Error::InvalidRead :
        return "Bad Read" ;

      case Aether::Error::InvalidIP :
        return "Could not find the IP of the hostname" ;

      case Aether::Error::None :
        return "None" ;

      default : return "Unknown" ;
    }
  }
  
  Aether::Severity severity( Aether::Error error )
  {
    switch( error.error() )
    {
      case Aether::Error::None :
      default : return Aether::Severity::None ;
    }
  }
  
  AetherData::AetherData()
  {
    this->error_cb = &ae::defaultHandler ;
    this->handler  = nullptr             ;
  }

  Aether::Error::Error()
  {
    this->val = Aether::Error::None ;
  }
  
  Aether::Error::Error( unsigned error ) 
  {
    this->val = error ;
  }
  
  Aether::Error& Aether::Error::operator=( unsigned error )
  {
    this->val = error ;
    
    return *this ;
  }
  
  Aether::Error::operator unsigned() const
  {
    return this->val ;
  }
  
  void Aether::Error::set( unsigned error )
  {
    this->val = error ;
  }
  
  unsigned Aether::Error::error()
  {
    return this->val ;
  }
  
  const char* Aether::Error::toString() const
  {
    static std::string str ;
    
    str = ae::toString( *this ) ;

    return str.c_str() ;
  }
  
  Aether::Severity Aether::Error::severity() const
  {
    return ae::severity( *this ) ;
  }

  void Aether::addError( Aether::Error error )
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
  
  void Aether::setErrorHandler( void ( *error_handler )( Aether::Error ) )
  {
    data.error_cb = error_handler ;
  }
  
  void Aether::setErrorHandler( ae::Aether::ErrorHandler* handler )
  {
    data.handler = handler ;
  }
}


