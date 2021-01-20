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
 * File:   Test.cpp
 * Author: Jordan Hendl
 *
 * Created on January 18, 2021, 6:37 AM
 */

#include <ygg/Connection.h>
#include "Linux.h"
#include <iostream>

int main()
{
  using Impl = ygg::lx::Linux ;
  
  ygg::Connection<Impl> connection ;
  
  Impl::initialize( "/wksp/github/yggdrasil/cert/cert.pem", "/wksp/github/yggdrasil/cert/key.pem" ) ;
  
  connection.connect( "pbs.twimg.com" ) ;
  connection.sendHttp( "GET /media/EsD2hGHWMAAq8D9?format=jpg&name=360x360 HTTP/1.1\r\n" ) ;
  
  std::cout << connection.recieve() << std::endl ;
  return 0;
}

