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
 * File:   Linux.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 18, 2021, 6:44 AM
 */

#include "Linux.h"
#include <string>
namespace ae
{
  namespace lx
  {
    struct LinuxData
    {
      std::string cert_file ;
      std::string key_file  ;
    };
    
    static LinuxData data ;

    void Linux::initialize( const char* certificate_file, const char* private_key_file )
    {
      data.cert_file = certificate_file ;
      data.key_file  = private_key_file ;
    }
    
    const char* Linux::certificate()
    {
      return data.cert_file.c_str() ;
    }
    
    const char* Linux::key()
    {
      return data.key_file.c_str() ;
    }
  }
}

