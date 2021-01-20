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
 * File:   ImageDownload.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 18, 2021, 10:34 PM
 */

#include "ImageDownload.h"
#include <ygg/Connection.h>

#ifdef _WIN32
  #include <win32/Win32.h>
  using Impl = ygg::win32::Win32 ;
#elif __linux__ 
  #include <linux/Linux.h>
  using Impl = ygg::lx::Linux ;
#endif

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
  
namespace ygg
{
  
  struct ImageDownloaderData
  {
    using ImageData = std::vector<unsigned char> ;
    
    ygg::Connection<Impl> connection ; ///< The connection to make to the server.
    ImageData            data       ; ///< The data container of the image bytes.
    std::string          host       ; ///< The hostname of the image provider.
    std::string          location   ; ///< The location in the hostname the image is held.
    unsigned             width      ; ///< The width of the image.
    unsigned             height     ; ///< The height of the image.
    
    /** Default constructor.
     */
    ImageDownloaderData() ;
    
    /** Method to build the HTTP image request message to send.
     */
    std::string message() const ;
    
    
    /** Method to parse the incoming URL into host name and location.
     * @param url The C-string url to parse.
     */
    void parseURL( const char* url ) ;
  };
  
  std::string ImageDownloaderData::message() const
  {
    std::string       ret    ;
    std::stringstream stream ;
    
    stream << "GET " << this->location << " HTTP/1.1\r\nHOST: " << this->host << "\r\n\r\n" ;
    ret = stream.str() ;
    return ret ;
  }
  ImageDownloaderData::ImageDownloaderData()
  {
    this->width    = 0  ;
    this->height   = 0  ;
    this->host     = "" ;
    this->location = "" ;
  }
  
  void ImageDownloaderData::parseURL( const char* url )
  {
    std::string full  ;
    std::size_t begin ;
    std::size_t end   ;
    
    full  = url                 ;
    begin = full.find( "://"  ) ;
    end   = full.find( ".com" ) ;
    
    if( begin != std::string::npos ) begin += 3 ; else begin = 0 ;
    if( end   != std::string::npos ) end   += 4 ; else end   = 0 ;
    
    this->host     = full.substr( begin, end - begin ) ;
    this->location = full.substr( end, full.size()   ) ;
  }

  ImageDownloader::ImageDownloader()
  {
    this->image_data = new ImageDownloaderData() ;
  }
  
  ImageDownloader::~ImageDownloader()
  {
    delete this->image_data ;
  }
  
  void ImageDownloader::download( const char* image_url )
  {
    data().parseURL( image_url ) ;
    
    data().connection.connect( data().host.c_str() ) ;        
    if( data().connection.valid() )
    {
      data().connection.sendHttp( data().message().c_str() ) ;
      
      std::cout << data().connection.recieve() << std::endl ;
    }
  }
  
  unsigned ImageDownloader::width() const
  {
    return data().width ;
  }
  
  unsigned ImageDownloader::height() const
  {
    return data().height ;
  }
  
  const unsigned char* ImageDownloader::image() const
  {
    return data().data.data() ;
  }
  
  ImageDownloaderData& ImageDownloader::data()
  {
    return *this->image_data ;
  }
  
  const ImageDownloaderData& ImageDownloader::data() const
  {
    return *this->image_data ;
  }
}

