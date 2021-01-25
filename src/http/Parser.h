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
  /** Forward declare for a Yggdrasil packet.
   */
  class Packet ;

  namespace http
  {
    
    /** Class to manage parsing an HTTP Header.
     */
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
        void parse( const ygg::Packet& packet ) ;
        
        /** Method to retrieve the value of a given key.
         * @param key The key to retrieve a value of.
         * @return The value of the input key, if available.
         */
        const char* value( const char* key ) const ;
        
        /** Method to return whether or not this parser has fully parsed a whole HTTP header.
         * @return Whether or not this parser has parsed a whole HTTP header.
         */
        bool parsed() const ;
        
        /** Method to reset this object and clear all parsed data.
         */
        void reset() ;

        /** Method to retrieve a packet containing the body-leftovers from the parsing operation, if any were found.
         * @return A Packet containing any HTTP-body section data claimed by this parsing.
         */
        ygg::Packet leftover() const ;

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

