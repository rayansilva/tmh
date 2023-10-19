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

#include "protocol/TPUser.h"

#include "protocol/TPHeader.h"

namespace protocol
{
   TPUser::TPUser( TPUser const& other ) :
      m_id( other.m_id ),
      m_typeUser( other.m_typeUser ),
      m_username( other.m_username ),
      m_password( other.m_password ),
      m_email( other.m_email )
   {
   }

   TPUser::TPUser() :
      m_id( 0 ),
      m_typeUser( 0 ),
      m_username( "" ),
      m_password( "" ),
      m_email( "" )
   {
   }

   TPUser::~TPUser()
   {
   }

   protocol::TPUser TPUser::operator=( protocol::TPUser const& other )
   {
      m_id           = other.m_id;
      m_typeUser     = other.m_typeUser;
      m_username     = other.m_username;
      m_password     = other.m_password;
      m_email        = other.m_email;

      return *this;
   }

   bool TPUser::operator==( TPUser const& other )
   {
      return ( ( m_id == other.m_id ) &&
               ( m_typeUser == other.m_typeUser ) &&
               ( m_username == other.m_username ) &&
               ( m_password == other.m_password ) &&
               ( m_email == other.m_email ) );
   }

   uint32_t TPUser::GetId() const
   {
      return m_id;
   }

   void TPUser::SetId( uint32_t const& id )
   {
      m_id = id;
   }

   uint16_t TPUser::GetTypeUser() const
   {
      return m_typeUser;
   }

   void TPUser::SetTypeUser( uint16_t const& typeUser )
   {
      m_typeUser = typeUser;
   }

   std::string TPUser::GetUsername() const
   {
      return m_username;
   }

   void TPUser::SetUsername( const std::string& username )
   {
      m_username = username;
   }

   std::string TPUser::GetPassword() const
   {
      return m_password;
   }

   void TPUser::SetPassword( const std::string& password )
   {
      m_password = password;
   }

   std::string TPUser::GetEmail() const
   {
      return m_email;
   }

   void TPUser::SetEmail( const std::string& email )
   {
      m_email = email;
   }

   bool TPUser::ToString( std::stringstream& streamBuffer )
   {
      putNumeric( m_id, streamBuffer );
      putNumeric( m_typeUser, streamBuffer );
      putString( m_username, streamBuffer );
      putString( m_password, streamBuffer );
      putString( m_email, streamBuffer );

      return true;
   }

   bool TPUser::FromString( std::stringstream& streamBuffer )
   {
      getNumeric( m_id, streamBuffer );
      getNumeric( m_typeUser, streamBuffer );
      getString( m_username, streamBuffer );
      getString( m_password, streamBuffer );
      getString( m_email, streamBuffer );

      return true;
   }
}