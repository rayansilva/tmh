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

#ifndef _business_PlayerSession_
#define _business_PlayerSession_

#include <sys/time.h>
#include <list>

#include "utility/Socket.h"
#include "business/BusUser.h"
#include "protocol/protocol.h"

namespace business
{
   class PlayerSession
   {
   public:
      PlayerSession( utility::Socket* socket );
      ~PlayerSession();

      business::BusUser GetUser() const;
      void SetUser( business::BusUser const& user );

      bool IsAuthenticated() const;
      void SetAuthenticated( bool const& authenticated );

      bool IsActived() const;
      void SetActived( bool const& active );

      bool IsTerminated() const;
      void Terminate();

      std::string GetIpAddress();

      void Handle();

   private:
      utility::Socket* m_socket;
      business::BusUser m_user;
      bool m_terminated;
      bool m_authenticated;
      bool m_active;
      uint16_t m_timeOut;

      bool readPacket( std::list< std::string>& packet );
      bool sendPacket( std::string const& packet );

      uint32_t getCurrentDate();

      void operator()( std::string const& packet );
      void operator()( protocol::TPAuthenticate& packet );

      template< typename Type >
      bool tryHandle( uint16_t type, std::string const& packet )
      {
         Type object;
         if ( object.GetMessageType() != type )
         {
            return false;
         }

         if ( !object.FromString( packet ) )
         {
            return false;
         }

         // call typed operator
         ( *this )( object );

         return true;
      }
   };
}

#endif