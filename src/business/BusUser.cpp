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

#include "business/BusUser.h"

namespace business
{
   BusUser::BusUser() :
      m_id ( 0 ),
      m_typeUser( 0 ),
      m_username ( "" ),
      m_password ( "" ),
      m_email( "" )
   {
   }

   BusUser::BusUser( BusUser const& other )
   {
      m_id = other.m_id;
      m_typeUser = other.m_typeUser;
      m_username = other.m_username;
      m_password = other.m_password;
      m_email = other.m_email;
   }

   BusUser::~BusUser()
   {
   }

   BusUser BusUser::operator=( BusUser const& other )
   {
      m_id        = other.m_id;
      m_typeUser  = other.m_typeUser;
      m_username  = other.m_username;
      m_password  = other.m_password;
      m_email     = other.m_email;

      return *this;
   }

   bool BusUser::operator==( BusUser const& other )
   {
      return ( m_id == other.m_id &&
               m_typeUser == other.m_typeUser &&
               m_username == other.m_username &&
               m_password == other.m_password &&
               m_email == other.m_email );
   }

   bool BusUser::operator<(const BusUser& other)
   {
      return ( m_id < other.m_id );
   }

   uint32_t BusUser::GetId() const
   {
      return m_id;
   }

   void BusUser::SetId( uint32_t const& id )
   {
      m_id = id;
   }

   uint16_t BusUser::GetTypeUser() const
   {
      return m_typeUser;
   }

   void BusUser::SetTypeUser( uint16_t typeUser )
   {
      m_typeUser = typeUser;
   }

   std::string BusUser::GetUsername() const
   {
      return m_username;
   }

   void BusUser::SetUsername( std::string const& username )
   {
      m_username = username;
   }

   std::string BusUser::GetPassword() const
   {
      return m_password;
   }

   void BusUser::SetPassword( std::string const& password )
   {
      m_password = password;
   }

   std::string BusUser::GetEmail() const
   {
      return m_email;
   }

   void BusUser::SetEmail( const std::string& email )
   {
      m_email = email;
   }

   void BusUser::Reset()
   {
      m_id        = 0;
      m_typeUser  = 0;
      m_username  = "";
      m_password  = "";
      m_email     = "";
   }
}