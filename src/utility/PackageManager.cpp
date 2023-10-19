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

#include "protocol/TPHeader.h"
#include "utility/PackageManager.h"

namespace serialization
{
   PackageManager::PackageManager()
   {
   }

   PackageManager::~PackageManager()
   {
   }

   void PackageManager::DecodeStreamPacket( std::string data, std::list< std::string >& packets )
   {
      uint8_t startOfFrame = 0x01;
      uint8_t endOfFrame   = 0x04;

      size_t startPosition = 0;
      while( ( startPosition = data.find( startOfFrame, startPosition ) ) != std::string::npos )
      {
         size_t endPosition = data.find( endOfFrame, startPosition );

         if( endPosition != std::string::npos )
         {
            packets.push_back( data.substr( startPosition, endPosition - startPosition + 1 ) );
            startPosition = endPosition + 1;
            continue;
         }
         break;
      }
   }

   std::string PackageManager::EncodeStreamPacket( std::string const& packet )
   {
      std::stringstream encodedPacket;

      encodedPacket << ( uint8_t ) StartOfFrame;
      for( std::string::size_type i = 1; i < ( packet.length() - 1 ); ++i )
      {
         int8_t byte = packet[ i ];
         if( byte == StartOfFrame || byte == EndOfFrame || byte == 0x10 )
         {
            encodedPacket << ( int8_t )0x10;
            encodedPacket << ( int8_t )( byte + 0x20 );
         }
         else
         {
            encodedPacket << byte;
         }
      }
      encodedPacket << ( uint8_t ) EndOfFrame;

      return encodedPacket.str();
   }
}
