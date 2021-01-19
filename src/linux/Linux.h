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
 * File:   Linux.h
 * Author: Jordan Hendl
 *
 * Created on January 18, 2021, 6:44 AM
 */

#ifndef AETHER_LINUX_H
#define AETHER_LINUX_H

#include "Connection.h"

namespace ae
{
  namespace lx
  {
    class Connection ;
    /** Library class to handle all things linux in the Aether library.
     */
    class Linux
    {
      public:
        
        /** The connection in use by this library.
         */
        using Connection = ae::lx::Connection ;
        
        /** Method to initialize this library with a valid certificate and private key.
         * @param certificate_file The path to the certificate file on disk.
         * @param private_key_file The path to the private key file on disk.
         */
        static void initialize( const char* certificate_file, const char* private_key_file ) ;
        
        /** Static method to retrieve the certificate file location of this library.
         * @return The C-string representation of this library's SSL certificate.
         */
        static const char* certificate() ;
        
        /** Static method to retrieve the private key file location of this library.
         * @return The C-string representation of this library's SSL private key.
         */
        static const char* key() ;
    };
  }
}
#endif /* LINUX_H */

