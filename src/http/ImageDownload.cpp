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
#include "Parser.h"
#include "stb_image.h"
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
  
namespace ygg
{
  
  struct ImageDownloaderData
  {
    using ImageData = std::vector<unsigned char> ;
    
    ygg::Connection<Impl> connection ; ///< The connection to make to the server.
    ygg::http::Parser     parser     ; ///< The parser for the HTTP header.
    ImageData             data       ; ///< The data container of the image bytes.
    ImageData             img_bytes  ; ///< The data container of the image bytes.
    std::string           host       ; ///< The hostname of the image provider.
    std::string           location   ; ///< The location in the hostname the image is held.
    unsigned              width      ; ///< The width of the image.
    unsigned              height     ; ///< The height of the image.
    unsigned              channels   ; ///< The number of channels in the image.

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
    
    stream << "GET "   << this->location << " HTTP/1.1\r\n" ;
    stream << "HOST: " << this->host     << "\r\n\r\n"      ;

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
    ygg::Packet    packet       ;
    unsigned       content_size ;
    unsigned       request_amt  ;
    int            width        ;
    int            height       ;
    int            chan         ;
    unsigned char* bytes        ;

    data().parseURL( image_url ) ;
    data().connection.connect( data().host.c_str() ) ;        
    data().connection.send( data().message().c_str(), data().message().size() ) ;
    
    // Parse the HTTP header.
    while( !data().parser.parsed() ) 
    {
      packet = data().connection.recieve() ;
      data().parser.parse( packet ) ;
    }
    
    // Find out how big our image is & reserve that much data.
    content_size = std::atoi( data().parser.value( "Content-Length" ) ) ;
    data().data.reserve( content_size ) ;
    
    // Grab any data accidentally grabbed from the header packets.
    packet = data().parser.leftover() ;
    for( unsigned i = 0; i < packet.size(); i++ )
    {
      data().data.push_back( packet.payload()[ i ] ) ;
    }
    
    // Now, keep requesting until we've gotten all our data.
    request_amt = content_size - data().data.size() ;
    while( data().data.size() < content_size && packet.size() != 0 )
    {
      packet = data().connection.recieve( std::min( request_amt, PACKET_SIZE ) ) ;
      
      for( unsigned i = 0; i < packet.size(); i++ )
      {
        data().data.push_back( packet.payload()[ i ] ) ;
        request_amt-- ;
      }
    }
    
    // Now we have the .png/jpeg/whatever data, use STB to generate raw bytes * channels from it.
    bytes = stbi_load_from_memory( data().data.data(), data().data.size(), &width, &height, &chan, 4 ) ;
    
    data().data = ImageDownloaderData::ImageData( bytes, bytes + ( width * height * chan ) ) ;
      
    data().width    = static_cast<unsigned>( width  ) ;
    data().height   = static_cast<unsigned>( height ) ;
    data().channels = static_cast<unsigned>( chan   ) ;
   }
  
  unsigned ImageDownloader::width() const
  {
    return data().width ;
  }
  
  unsigned ImageDownloader::height() const
  {
    return data().height ;
  }
  
  unsigned ImageDownloader::channels() const
  {
    return data().channels ;
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

