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
 * File:   Yggdrasil.h
 * Author: Jordan Hendl
 *
 * Created on January 18, 2021, 9:32 PM
 */

#ifndef YGGDRASIL_H
#define YGGDRASIL_H

namespace ygg
{
  class Yggdrasil
  {
    public:
      
      class Error ;
      
      /** Abstract class for an error handler.
       * If you wish to have an object handle errors, inherit this class and implement the handleError() function.
       */
      class ErrorHandler
      {
        public:
          
          /** Virtual method to handle a library error.
           * @param error The error to handle.
           */
          virtual void handleError( ygg::Yggdrasil::Error error ) = 0 ;
          
          /** Virtual deconstructor for inheritance.
           */
          virtual ~ErrorHandler() ;
      };
      
      /** The severity of an error.
       */
      enum class Severity
      {
        None,
        Warning,
        Fatal
      };

      /** Reflective enumeration for containing a library error.
       */
      class Error
      {
        public:
          enum
          {
            /** No error.
             */
            None,
            
            /** Failure while creating a socket.
             */
            SocketCreationFailure,
            
            /** Generic SSL failure.
             */
            SslFailure,
            
            /** Failure when generating an SSL context.
             */
            SslContextFailure,
            
            /** Error when connecting SSL to a server & performing the handshake.
             */
            SslConnectionFailure,
            
            /** Error when setting the file descriptor of SSL.
             */
            SslFDFailure,
            
            /** Error when performing the private key check of SSL.
             */
            SslPrivateKeyCheckFailure,
            
            /** Error when sending data over a connection.
             */
            SendFailure,
            
            /** Error when recieving data over a connection.
             */
            RecieveFailure,
            
            /** Error when connecting a socket.
             */
            ConnectionFailure,
            
            /** Error when reading has failed.
             */
            InvalidRead,
            
            /** Error when looking up the IP.
             */
            InvalidIP
          };
          
          /** Default constructor.
           */
          Error() ;

          /** Copy constructor. Copies value into this object.
           * @param error The error to place inside this one.
           */
          Error( unsigned error ) ;
          
          /** Method to retrieve a string representation of this error.
           * @return The C-string representation of this error.
           */
          const char* toString() const ;
          
          /** Method to retrieve the severity of this error.
           */
          Yggdrasil::Severity severity() const ;

          /** Assignment operator. Assigns this error to the input.
           * @param error The error to assign to this object.
           * @return This object after assignment.
           */
          Error& operator=( unsigned error ) ;
          
          /** Conversion operator to convert an error into it's underlying unsigned value.
           * @return This error's underlying value enumeration.
           */ 
          operator unsigned() const ;
          
          /** Method to set this object's underlying enumeration.
           * @param error The enumeration to set this error to.
           */
          void set( unsigned error ) ;
          
          /** Method to retrieve the underlying error of this object/
           * @return The underlying enumeration error of this object.
           */
          unsigned error() ;

        private:
          unsigned val ; ///< The internal container for the enumeration.
      };
      
      /** Static method to push an error onto this library.
       * @param error The error to handle by this library
       */
      static void addError( Yggdrasil::Error error ) ;
      
      /** Static method to allow a custom error handler to be set for this library.
       * @param error_handler The error handler to be used by this library.
       */
      static void setErrorHandler( void ( *error_handler )( Yggdrasil::Error ) ) ;
      
      /** Static method to allow a custom error handler to be set for this library.
       * @param handler The error handler to be used by this library.
       */
      static void setErrorHandler( ErrorHandler* handler ) ;
  };
}
#endif /* YGGDRASIL_H */

