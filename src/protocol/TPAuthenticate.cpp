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

#include "protocol/TPAuthenticate.h"

namespace protocol
{
   TPAuthenticate::TPAuthenticate():
      TPHeader( TPHeader::Authenticate ),
      m_user()
   {
   }

   TPAuthenticate::TPAuthenticate( TPAuthenticate const& other ):
      TPHeader( other ),
      m_user( other.m_user )
   {
   }

   TPAuthenticate::~TPAuthenticate()
   {
   }

   TPAuthenticate TPAuthenticate::operator=( TPAuthenticate const& other )
   {
      TPHeader::operator=( other );
      m_user = other.m_user;

      return *this;
   }

   bool TPAuthenticate::operator==( TPAuthenticate const& other )
   {
      return ( ( TPHeader::operator==( other) ) &&
               ( m_user == other.m_user ) );
   }

   protocol::TPUser& TPAuthenticate::GetUser()
   {
      return m_user;
   }

   std::string TPAuthenticate::ToString()
   {
      TPHeader::ToString();

      m_user.ToString( m_streamBuffer );

      return putEndOfFrame();
   }

   bool TPAuthenticate::FromString( std::string const& packetStr )
   {
      if( !TPHeader::FromString( packetStr ) )
      {
         return false;
      }

      m_user.FromString( m_streamBuffer );

      return checkEndOfFrame();
   }
}