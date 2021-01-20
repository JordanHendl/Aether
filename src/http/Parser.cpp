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
 * File:   Parser.cpp
 * Author: Jordan Hendl
 * 
 * Created on January 19, 2021, 1:08 AM
 */

#include "Parser.h"
#include <map>
#include <string>
#include <sstream>
#include <iostream>

namespace ygg
{
  namespace http
  {
    
    /** Function to recieve the next valid character from the stream.
     * @param stream Reference to the stringstream to search through.
     * @return char The character that is valid.
     */
    static char getNextValidCharacter( std::stringstream& stream ) ;

    /** Helper function to retrieve the rest of a string when a string is seen in the data
     * @param strean Reference to the stringstream to search through.
     * @return std::string The string value without the double quotes.
     */
    static std::string getKey( std::stringstream& stream ) ;
    
    /** Helper function to retrieve the rest of a string when a string is seen in the data
     * @param strean Reference to the stringstream to search through.
     * @return std::string The string value without the double quotes.
     */
    static std::string getValue( std::stringstream& stream ) ;
    
    /** Data structure to contain a http parser's data.
     */
    struct ParserData
    {
      using HeaderTokenMap = std::map<std::string, std::string> ;
      
      HeaderTokenMap map       ;
      std::string    version   ;
      std::string    command   ;
      std::string    code_desc ;
      unsigned       code      ;
      char*          body_ptr  ;
      
      ParserData() = default ;
      
      /** Method to parse the starting line of the HTTP message.
       * @param data The data to parse.
       * @return The index of the buffer to start reading the rest of the header data from.
       */
      unsigned parseStart( const char* data ) ;
      
      /** Helper function to parse the input HTTP data buffer.
       * @param data The HTTP data buffer to parse.
       */
      void parse( const char* data, unsigned start ) ;
    };

    std::string getKey( std::stringstream& instream )
    {
      std::stringstream stream ;
      char              val    ;
      
      val = '\0' ;
      while( !instream.eof() && ( val = instream.get() ) != instream.eof() && val != '\r' && val != '\n' && val != ':' )
      {
        stream << val ;
      }

      return stream.str() ;
    }
    
    std::string getValue( std::stringstream& instream )
    {
      std::stringstream stream ;
      char              val    ;
      
      val = '\0' ;
      while( !instream.eof() && ( val = instream.get() ) != instream.eof() && val != '\r' && val != '\n' )
      {
        stream << val ;
      }

      return stream.str() ;
    }

    char getNextValidCharacter( std::stringstream& stream )
    {
      char val ;

      val = '\0' ;
      // While not eof, not whitespace, and not a line break.
      while( !stream.eof() && ( val = stream.get() ) != stream.eof() && val != -1 && ( val == ' ' || val == '\n' || val == '\r' || val == '#' || val == ':' ) )
      { 
        
      }
      if( !stream.eof() )
      stream.putback( val ) ;
      return val ;
    }
      
    unsigned ParserData::parseStart( const char* data )
    {
      std::stringstream stream  ;
      unsigned          index   ;
      char              token   ;
      
      index = 0 ;
      
      while( ( token = data[ index ] ) != '\r' && token != '\n' && token != '\0' )
      {
        if( token == ' ' && index != 0 )
        {
          if( stream.str() == std::string( "HTTP/1.1" ) )
          {
            this->version = stream.str() ;
            stream.str( std::string() ) ;
          }
          else if( isdigit( stream.str().at( 0 ) ) && this->version != std::string( "" ) )
          {
            this->code = std::atoi( stream.str().c_str() ) ;
            stream.str( std::string() ) ;
          }
          else if( this->version != std::string( "" ) )
          {
            this->code_desc = stream.str() ;
            stream.str( std::string() ) ;
          }
          else
          {
            this->command = stream.str() ;
            stream.str( std::string() ) ;
          }
        }
        else
        {
          stream << token ;
        }
        index++ ;
      }
      
      if( this->version != std::string( "" ) )
      {
        this->code_desc = stream.str() ;
      }
      else
      {
        this->command = stream.str() ;
      }
      
      return index ;
    }
    
    void ParserData::parse( const char* data, unsigned start )
    {
      std::stringstream header ;
      std::stringstream stream ;
      std::string       line   ;
      std::string       key    ;
      std::string       value  ;
      
      header << ( data + start ) ;

      while( std::getline( header, line ) )
      {
        stream.clear() ;
        stream.str( std::string() ) ;
        stream << line ;

        // Trim off any leftover invalid characters.
        key   = getKey( stream ) ;
        getNextValidCharacter( stream ) ;
        value = getValue( stream ) ;
        
        this->map[ key ] = value ;
      }
    }

    Parser::Parser()
    {
      this->parser_data = new ParserData() ;
    }
    
    Parser::~Parser()
    {
      delete this->parser_data ;
    }
    
    void Parser::initialize( const char* data )
    {
      this->data().version   = "" ;
      this->data().command   = "" ;
      this->data().code_desc = "" ;
      this->data().code      = 0  ;

      this->data().map.clear() ;
      
      this->data().parse( data, this->data().parseStart( data ) ) ;
    }
    
    const char* Parser::value( const char* key ) const
    {
      const auto iter = data().map.find( key ) ;
      
      if( iter != data().map.end() ) return iter->second.c_str() ;
      
      return "" ;
    }
    
    const char* Parser::body() const
    {
      return data().body_ptr ;
    }
    
    ParserData& Parser::data()
    {
      return *this->parser_data ;
    }
    
    const ParserData& Parser::data() const
    {
      return *this->parser_data ;
    }
  }
}
