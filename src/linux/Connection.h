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
 * File:   Connection.h
 * Author: Jordan Hendl
 *
 * Created on January 18, 2021, 6:05 AM
 */

#ifndef AETHER_LINUX_CONNECTION_H
#define AETHER_LINUX_CONNECTION_H

namespace ae
{
  /** The type of connections available.
   */
  enum class ConnectionType ;
  
  namespace lx
  {
    class Connection
    {
      public:
        Connection() ;
        ~Connection() ;
        void connect( const char* url_path, ae::ConnectionType type, unsigned port = 80 ) ;
        void sendHttp( const char* http_cmd ) ;
        bool valid() const ;
        const char* recieve() ;
      private:
        struct ConnectionData *connection_data ;
        ConnectionData& data() ;
        const ConnectionData& data() const ;
    };
  }
}

#endif