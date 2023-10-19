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

namespace protocol
{
   TPHeader::TPHeader():
      m_messageType( 0 ),
      m_version( 0 ),
      m_dateTime( 0 )
   {
      m_streamBuffer.str("");
   }

   TPHeader::TPHeader( uint16_t messageType )
      : m_messageType( messageType ),
      m_version( 0 ),
      m_dateTime( 0 )
      {
      }

   TPHeader::TPHeader( TPHeader const& other )
      : m_messageType( other.m_messageType ),
      m_version( other.m_version ),
      m_dateTime( other.m_dateTime )
      {
      }

   TPHeader::~TPHeader()
   {
   }

   TPHeader TPHeader::operator=( TPHeader const& other)
   {
      m_messageType  = other.m_messageType;
      m_version      = other.m_version;
      m_dateTime     = other.m_dateTime;
      return *this;
   }

   bool TPHeader::operator==( TPHeader const& other)
   {
      return ( ( m_messageType   == other.m_messageType ) &&
               ( m_version       == other.m_version ) &&
               ( m_dateTime      == other.m_dateTime ) );
   }

   uint16_t TPHeader::GetMessageType() const
   {
      return m_messageType;
   }

   uint16_t TPHeader::GetVersion() const
   {
      return m_version;
   }

   void TPHeader::SetVersion( uint16_t version )
   {
      m_version = version;
   }

   uint32_t TPHeader::GetDateTime() const
   {
      return m_dateTime;
   }

   void TPHeader::SetDateTime( uint32_t dateTime )
   {
      m_dateTime = dateTime;
   }

   std::string TPHeader::putEndOfFrame()
   {
      putNumeric( static_cast< uint8_t >( EndOfFrame ), m_streamBuffer );
      return m_streamBuffer.str();
   }

   bool TPHeader::checkEndOfFrame()
   {
      uint8_t eof = 0;
      getNumeric( eof, m_streamBuffer );

      return ( eof == EndOfFrame );
   }

   bool TPHeader::ToString()
   {
      m_streamBuffer.str( "" );
      m_streamBuffer.flush();
      m_streamBuffer.clear();

      putNumeric( static_cast< uint8_t >( StartOfFrame ), m_streamBuffer);
      putNumeric( m_messageType, m_streamBuffer );
      putNumeric( m_version, m_streamBuffer );
      putNumeric( m_dateTime, m_streamBuffer );

      return true;
   }

   bool TPHeader::FromString( std::string const& packetStr )
   {
      m_streamBuffer.str( "" );
      m_streamBuffer.flush();
      m_streamBuffer.clear();

      m_streamBuffer << packetStr;

      //get startOfFrame
      uint8_t startOfFrame;
      getNumeric( startOfFrame, m_streamBuffer );

      //get TypeMessage
      uint16_t receivedMsgType = 0;
      getNumeric( receivedMsgType, m_streamBuffer );

      if ( m_messageType != 0 && ( receivedMsgType != m_messageType ) )
      {
         return false;
      }
      m_messageType = receivedMsgType;

      getNumeric( m_version, m_streamBuffer );
      getNumeric( m_dateTime, m_streamBuffer );

      return true;
   }
}