////////////////////////////////////////////////////////////////////////////////////////////
//
//          Nome do Projeto: TMH - Truco Message Handler
//          Descrição: Servido de mensagens para o jogo de Truco
//          Autor: Rayan Silva
//          Ano: 2023
//          Licença: MIT (Consulte a seção abaixo para mais detalhes)
//
//          MIT License
//
//          Copyright (c) 2023 Rayan Silva
//
//          Permission is hereby granted, free of charge, to any person obtaining a copy
//          of this software and associated documentation files (the "Software"), to deal
//          in the Software without restriction, including without limitation the rights
//          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//          copies of the Software, and to permit persons to whom the Software is
//          furnished to do so, subject to the following conditions:
//
//          The above copyright notice and this permission notice shall be included in
//          all copies or substantial portions of the Software.
//
//          THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//          IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//          FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//          AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//          LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//          OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//          THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _protocol_TPHeader_
#define _protocol_TPHeader_

#include <time.h>
#include <iostream>
#include <sstream>

#define StartOfFrame 0x01
#define EndOfFrame 0x04

namespace protocol
{
   template< typename ValueType >
   inline void putNumeric( ValueType const& value, std::stringstream& output )
   {
      uint32_t const length( sizeof( ValueType ) );
      uint8_t const* const begin( reinterpret_cast< uint8_t const* >( &value ) );
      uint8_t const* const end( begin + length );
      uint8_t const* dest;

      // big endian conversion
      for( dest = (end - 1); dest >= begin; dest-- )
      {
         output << *dest;
      }
   }

   inline void putString( std::string const& value, std::stringstream& output )
   {
      uint16_t length = value.size();
      putNumeric( length, output );
      output << value;
   }

   template< typename ValueType >
   inline void getNumeric( ValueType& value, std::stringstream& input )
   {
      uint32_t const length( sizeof( ValueType ) );
      char* const begin( reinterpret_cast< char* >( &value ) );
      char* const end( begin + length );
      char* dest;

      for ( dest = (end - 1); dest >= begin; --dest )
      {
         input.get( *dest );
      }
   }

   inline void getString( std::string& value, std::stringstream& input )
   {
      uint16_t length = 0;
      getNumeric( length, input );

      value = "";
      value.resize( length );

      for( std::string::size_type i = 0; i < length; ++i  )
      {
         input.get( value[ i ] );
      }
   }

   class TPHeader
   {
   public:
      enum MessageTypes
      {
         Authenticate          = 1,
         AuthenticateAck       = 2,
      };

      TPHeader();
      TPHeader( uint16_t messageType );
      TPHeader( TPHeader const& other );
      virtual ~TPHeader();

      virtual TPHeader operator=( TPHeader const& other );
      virtual bool operator==( TPHeader const& other );

      uint16_t GetMessageType() const;

      uint16_t GetVersion() const;
      void SetVersion( uint16_t version );

      uint32_t GetDateTime() const;
      void SetDateTime( uint32_t dateTime );

      bool ToString();
      virtual bool FromString( std::string const& packetStr );

   protected:
      std::stringstream m_streamBuffer;

      std::string putEndOfFrame();
      bool checkEndOfFrame();

   private:
      uint16_t m_messageType;
      uint16_t m_version;
      uint32_t m_dateTime;
   };
}

#endif
