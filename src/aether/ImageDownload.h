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
 * File:   ImageDownload.h
 * Author: Jordan Hendl
 *
 * Created on January 18, 2021, 10:34 PM
 */

#ifndef AETHER_IMAGE_DOWNLOAD_H
#define AETHER_IMAGE_DOWNLOAD_H

namespace ae
{
  /** Class to download an image from an HTTP/HTTPS webserver.
   */
  class ImageDownloader
  {
    public:
      
      /** Default constructor.
       */
      ImageDownloader() ;
      
      /** Default deconstructor.
       */
      ~ImageDownloader() ;
      
      /** Method to download the image.
       * @param image_url The URL associated with the image.
       */
      void download( const char* image_url ) ;
      
      /** Method to retrieve the width of the input image.
       * @return The image width in pixels.
       */
      unsigned width() const ;
      
      /** Method to retrieve the height of the downloaded image.
       * @return The height of the image in pixels.
       */
      unsigned height() const ;
      
      /** Method to retrieve the bytes associated with the image.
       * @note The image is default in RGB/RGBA with each channel being represented by a single byte.
       * @return The byte data of the downloaded image.
       */
      const unsigned char* image() const ;
    private:
      
      /** The forward declared structure containing this object's data.
       */
      struct ImageDownloaderData* image_data ;
      
      /** Method to retrieve a reference to this object's internal data structure.
       * @return A reference to this object's internal data structure.
       */
      ImageDownloaderData& data() ;
      
      /** Method to retrieve a reference to this object's internal data structure.
       * @return A reference to this object's internal data structure.
       */
      const ImageDownloaderData& data() const ;
  };
}

#endif /* IMAGEDOWNLOAD_H */

