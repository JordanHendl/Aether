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
 * File:   Connection.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 18, 2021, 9:56 PM
 */

#include "Connection.h"
#include <algorithm>

namespace ygg
{
  Packet::Packet()
  {
    std::fill( this->data_ptr, this->data_ptr + sizeof( this->data_ptr ), 0 ) ;
    this->data_size = 0 ;
  }
  
  Packet::Packet( const Packet& packet )
  {
    *this = packet ;
  }
  
  Packet::~Packet()
  {
    std::fill( this->data_ptr, this->data_ptr + sizeof( this->data_ptr ), 0 ) ;
    this->data_size = 0 ;
  }
  
  Packet& Packet::operator=( const Packet& packet )
  {
    std::copy( packet.data_ptr, packet.data_ptr + sizeof( packet.data_ptr ), this->data_ptr ) ;
    this->data_size = packet.data_size ;
    
    return *this ;
  }
  
  Packet makePacket( const char* data, unsigned data_amt )
  {
    Packet packet ;
    
    std::copy( data, data + data_amt, packet.data_ptr ) ;
    packet.data_size = data_amt ;
    
    return packet ;
  }
  
  unsigned Packet::size() const
  {
    return this->data_size ;
  }
  
  const char* Packet::payload() const
  {
    return this->data_ptr ;
  }
}

