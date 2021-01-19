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
#include "Connection.h"

#ifdef _WIN32
  #include <win32/Win32.h>
  using Impl = ae::win32::Win32 ;
#elif __linux__ 
  #include <linux/Linux.h>
  using Impl = ae::lx::Linux ;
#endif

#include <vector>
#include <string>

namespace ae
{
  
  struct ImageDownloaderData
  {
    using ImageData = std::vector<unsigned char> ;
    
    ae::Connection<Impl> connection ; ///< The connection to make to the server.
    ImageData            data       ; ///< The data container of the image bytes.
    std::string          host       ; ///< The hostname of the image provider.
    std::string          location   ; ///< The location in the hostname the image is held.
    unsigned             width      ; ///< The width of the image.
    unsigned             height     ; ///< The height of the image.
    
    /** Default constructor.
     */
    ImageDownloaderData() ;
    
    /** Method to parse the incoming URL into host name and location.
     * @param url The C-string url to parse.
     */
    void parseURL( const char* url ) ;
  };
  
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
    
  }
  
  unsigned ImageDownloader::width() const
  {
    
  }
  
  unsigned ImageDownloader::height() const
  {
    
  }
  
  const unsigned char* ImageDownloader::image() const
  {
    
  }
  
  ImageDownloaderData& ImageDownloader::data()
  {
    
  }
  
  const ImageDownloaderData& ImageDownloader::data() const
  {
    
  }
}

