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
 
#include "Parser.h"
#include <athena/Manager.h>
#include <string>
#include "ImageDownload.h"
#include "ygg/Connection.h"

#ifdef _WIN32
  #include <win32/Win32.h>
  using Impl = ygg::win32::Win32 ;
#elif __linux__ 
  #include <linux/Linux.h>
  using Impl = ygg::lx::Linux ;
#endif
  
static athena::Manager      manager    ;
static ygg::ImageDownloader downloader ;
static ygg::http::Parser    parser     ;

static const char http_message[] = 
{
  "HTTP/1.1 200 OK\r\n"
  "Accept-Ranges: bytes\r\n"
  "access-control-allow-origin: *\r\n"
  "access-control-expose-headers: Content-Length\r\n"
  "Age: 23917\r\n"
  "cache-control: max-age=604800, must-revalidate\r\n"
  "Content-Type: image/jpeg\r\n"
  "Date: Tue, 19 Jan 2021 08:48:00 GMT\r\n"
  "Last-Modified: Tue, 19 Jan 2021 02:07:04 GMT\r\n"
  "Server: ECS (dab/4BDE)\r\n"
  "strict-transport-security: max-age=631138519\r\n"
  "surrogate-key: media media/bucket/0 media/1351350936648691712\r\n"
  "X-Cache: HIT\r\n"
  "x-connection-hash: 1e6cb47c1d203cc162a5f6febd9c4b21\r\n"
  "x-content-type-options: nosniff\r\n"
  "x-response-time: 76\r\n"
  "x-tw-cdn: VZ\r\n"
  "x-tw-cdn: VZ\r\n"
  "Content-Length: 15713\r\n\r\n\0\0"
};

bool testImageDownload()
{
  downloader.download( "https://pbs.twimg.com/media/EsBb-LLXMAAjJ6p?format=png&name=900x900" ) ;
  downloader.download( "https://pbs.twimg.com/media/EsBb-LLXMAAjJ6p?format=png&name=900x900" ) ;
  
  if( downloader.width() != 770 || downloader.height() != 686 ) return false ;
  return true ;
}

bool testParser()
{
  std::string age = parser.value( "Age" ) ;
  if( age != std::string( "23917" ) ) return false ;
  
  return true ;
}

int main()
{
  athena::Manager manager ;
  
  ygg::Packet packet = ygg::makePacket( http_message, sizeof( http_message ) ) ;
  parser.parse( packet ) ;
  Impl::initialize( "/wksp/github/yggdrasil/cert/cert.pem", "/wksp/github/yggdrasil/cert/key.pem" ) ;
  
  manager.initialize( "Yggdrasil HTTP Library" ) ;
  manager.add( "1) HTTP Parser Value Test"  , &testParser        ) ;
  manager.add( "2) HTTP Image Download Test", &testImageDownload ) ;
  return manager.test( athena::Output::Verbose ) ;
}