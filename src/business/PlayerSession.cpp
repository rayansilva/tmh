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

#include "business/PlayerSession.h"

#include <exception>
#include <iostream>
#include <thread>
#include <list>
#include <unistd.h>

#include "persistence/Persistence.h"
#include "protocol/TPAuthenticateAck.h"
#include "utility/Logger.h"
#include "utility/CommonMethod.h"
#include "utility/PackageManager.h"

#define INFO( message ) utility::Logger::GetInstance()( utility::Logger::Info,  __FILE__, __LINE__, message );
#define ERROR( message ) utility::Logger::GetInstance()( utility::Logger::Error,  __FILE__, __LINE__, message );

namespace business
{
   PlayerSession::PlayerSession( utility::Socket* socket ) :
     m_socket( socket ),
     m_user(),
     m_terminated( false ),
     m_authenticated( false ),
     m_active( false ),
     m_timeOut( 0 )
   {
   }

   PlayerSession::~PlayerSession()
   {
      Terminate();
      m_socket->Close();
      delete m_socket;
   }

   business::BusUser PlayerSession::GetUser() const
   {
      return m_user;
   }

   void PlayerSession::SetUser( business::BusUser const& user )
   {
      m_user = user;
   }

   bool PlayerSession::IsAuthenticated() const
   {
      return m_authenticated;
   }

   void PlayerSession::SetAuthenticated( bool const& authenticated )
   {
      m_authenticated = authenticated;
   }

   bool PlayerSession::IsTerminated() const
   {
      return m_terminated;
   }

   void PlayerSession::Terminate()
   {
      m_terminated = true;
   }

   bool PlayerSession::IsActived() const
   {
      return m_active;
   }
   void PlayerSession::SetActived( bool const& active )
   {
      m_active = active;
   }

   std::string PlayerSession::GetIpAddress()
   {
      if( m_socket != NULL )
      {
         return m_socket->GetIpAddress();
      }

      return "";
   }

   void PlayerSession::Handle()
   {
      uint16_t timeOut = 0;

      while( !IsTerminated() )
      {
         std::list< std::string > packets;
         if( readPacket( packets ) )
         {
            if( m_timeOut >= 240 )
            {
               INFO ( " - Time out session!" );
               Terminate();
            }
            timeOut = 0;
            // ok: valid data received
            try
            {
               // now process packet
               for( auto data : packets )
               {
                  ( *this )( data );
               }
            }
            catch( std::exception& e )
            {
               // an error happened
               ERROR( e.what() );
            }
            catch( ... )
            {
               // an error happened
               std::stringstream ss;
               ss << "An unknown error happened while trying to process packet ";
               ERROR( ss.str() );
            }
         }
         else
         {
            timeOut++;
            if( timeOut == 600 )
            {
               INFO ( "TimeOut session!!!" );
               Terminate();
            }
         }
         sleep( 1 );
      }

      m_socket->Close();
   }

   bool PlayerSession::readPacket( std::list< std::string>& packet )
   {
      std::string dataRead = "";
      try
      {
         // try to read data from socket
         if( !m_socket->Recv( dataRead ) )
         {
            return false;
         }
      }
      catch( ... )
      {
         return false;
      }

      // check for valid data
      if( dataRead.empty() )
      {
         return false;
      }
      else
      {
         serialization::PackageManager manager;
         manager.DecodeStreamPacket( dataRead, packet );

         std::stringstream ss;
         ss << "Received packet from user '" << m_user.GetUsername() << "' (" << m_socket->GetIpAddress()
            << ":" << m_socket->GetPort() << ") : ";

         for( auto iter : packet )
         {
            ss << datatype::FromStringToHexa( iter ) << " - ";
         }

         INFO( ss.str() );
      }

      return true;
   }

   bool PlayerSession::sendPacket( std::string const& packet )
   {
      serialization::PackageManager manager;

      std::stringstream encodedPacket;
      encodedPacket << ( manager.EncodeStreamPacket( packet ) );

      std::stringstream ss;
      ss << "Sended packet from user '" << m_user.GetUsername() << "' (" << m_socket->GetIpAddress()
         << ":" << m_socket->GetPort() << ") : " << datatype::FromStringToHexa( encodedPacket.str() ) << std::endl;

      INFO( ss.str() );

      return m_socket->Send( encodedPacket.str() );
   }

   uint32_t PlayerSession::getCurrentDate()
   {
      timeval tv;
      gettimeofday( &tv, 0 );
      return tv.tv_sec;
   }

   void PlayerSession::operator()( std::string const& packet )
   {
      protocol::TPHeader header;

      if( !header.FromString( packet ) )
      {
         throw "Error decoding packet";
      }

      if( !tryHandle< protocol::TPAuthenticate >( header.GetMessageType(), packet ) )
      {
         if( !IsAuthenticated() )
         {
            std::stringstream ss;

            ss << "Received packet: " << datatype::FromStringToHexa( packet ) << " was identified as " << header.GetMessageType()
               << " but could not be handled.";

            // TODO: Enviar um pacote que informe o erro
            sendPacket( "Error: Could not decoded the packet." );
            Terminate();

            throw ss.str();
         }
      }
   }

   void PlayerSession::operator()( protocol::TPAuthenticate& packet )
   {
      protocol::TPAuthenticateAck authAck;

      try
      {
         business::BusUser userAuthenticate;
         userAuthenticate.SetId( packet.GetUser().GetId() );
         userAuthenticate.SetUsername( packet.GetUser().GetUsername() );
         userAuthenticate.SetPassword( packet.GetUser().GetPassword() );
         userAuthenticate.SetEmail( packet.GetUser().GetEmail() );

         m_user = userAuthenticate;

         // TODO: Pegar na base de dados as informações do usuario/jogador
//          persistence::Persistence ServerPersistence;
//          if( ServerPersistence.Select( userAuthenticate, m_user ) )
//          {
            SetAuthenticated( true );
            authAck.SetStatusCode( protocol::TPAuthenticateAck::Success );

            authAck.GetUser().SetTypeUser( m_user.GetTypeUser() );
            authAck.GetUser().SetId( m_user.GetId() );
            authAck.GetUser().SetUsername( m_user.GetUsername() );
            authAck.GetUser().SetPassword( m_user.GetPassword() );
            authAck.GetUser().SetEmail( m_user.GetEmail() );

            INFO( " USER " + userAuthenticate.GetUsername() + " AUTHENTICATED!" );
//          }
//          else
//          {
//             Terminate();
//             SetAuthenticated( false );
//             authAck.SetStatusCode( protocol::TPAuthenticateAck::InvalidUser );
//             INFO( " - USER NOT AUTHENTICATED!" );
//          }
      }
      catch( ... )
      {
         SetAuthenticated( false );
         authAck.SetStatusCode( protocol::TPAuthenticateAck::Unknown );

         std::stringstream ss;
         ss << " - UNKNOWN ERROR WHILE AUTHENTICATING USER!" << packet.GetUser().GetUsername();
         ERROR( ss.str() );
      }

      authAck.SetDateTime( getCurrentDate() );
      authAck.SetVersion( 0 );

      sendPacket( authAck.ToString() );
   }
}