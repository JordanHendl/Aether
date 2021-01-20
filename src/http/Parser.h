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
 * File:   Parser.h
 * Author: Jordan Hendl
 *
 * Created on January 19, 2021, 1:08 AM
 */

#ifndef YGGDRASIL_PARSER_H
#define YGGDRASIL_PARSER_H

namespace ygg
{
  namespace http
  {
    class Parser
    {
      public:
        /** Default constructor.
         */
        Parser() ;
        
        /** Default deconstructor.
         */
        ~Parser() ;
        
        /** Method to initialize this object.
         * @param data The input HTTP data block to parse.
         */
        void initialize( const char* data ) ;
        
        /** Method to retrieve the value of a given key.
         * @param key The key to retrieve a value of.
         * @return The value of the input key, if available.
         */
        const char* value( const char* key ) const ;
        
        /** Method to retrieve a const pointer to the beginning of the body of the currenly parsed message.
         * @return The const pointer pointing to the beginning of the body of the HTTP message, if available.
         */
        const char* body() const ;

      private:
        
        /** The forward declared structure containing this object's data.
         */
        struct ParserData *parser_data ;
        
        /** Method to retrieve a reference to this object's internal data structure.
         * @return A reference to this object's internal data structure.
         */
        ParserData& data() ;
        
        /** Method to retrieve a reference to this object's internal data structure.
         * @return A reference to this object's internal data structure.
         */
        const ParserData& data() const ;
    };
  }
}
#endif /* PARSER_H */

