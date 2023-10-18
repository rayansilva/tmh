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

#include "utility/Socket.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

namespace utility
{
   Socket::Socket() :
      m_sockfd( -1 ),
      m_port( 0 ),
      m_ipAddress( "" )
   {
      memset( &m_addr, 0, sizeof ( m_addr ) );
   }

   Socket::~Socket()
   {
      Close();
   }

   bool Socket::Create( std::string const& host, uint16_t port )
   {
      // Armazena o endereço IP e a porta
      m_ipAddress = host;
      m_port = port;

      // Cria um novo socket usando o protocolo IPv4 e TCP
      m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
//       fcntl( m_sockfd, F_SETFL, O_NONBLOCK ); // tornar o socket não bloqueante (modo não bloqueante).

      // Verifica se o socket foi criado com sucesso
      if( m_sockfd == -1 )
      {
         std::cerr << "Could not create socket" << std::endl;
         return false;
      }

      // Configura a estrutura do endereço para o socket
      m_addr.sin_family = AF_INET;                              // Usa IPv4
      m_addr.sin_addr.s_addr = inet_addr(m_ipAddress.c_str());  // Define o endereço IP
      m_addr.sin_port = htons( m_port );                        // Define a porta (em ordem de bytes de rede)

      return true;
   }

   bool Socket::Open( uint16_t port )
   {
      m_sockfd = socket( AF_INET, SOCK_STREAM, 0 );
      if( !IsValid() )
      {
         return false;
      }

      // set socket to non-blocking mode
      fcntl( m_sockfd, F_SETFL, O_NONBLOCK );

      // set keep alive from tcp
      int set = 1;
      if( setsockopt( m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &set, sizeof(int) ) < 0 )
      {
         Close();
         return false;
      }

      m_port = port;
      m_addr.sin_family = AF_INET;
      m_addr.sin_addr.s_addr = INADDR_ANY;
      m_addr.sin_port = htons( m_port );

      if( bind( m_sockfd, (struct sockaddr *) &m_addr, sizeof(m_addr) ) < 0 )
      {
         Close();
         return false;
      }

      return true;
   }

   bool Socket::Listen( int maxConnections)
   {
      if( !IsValid() )
      {
         return false;
      }

      if( listen( m_sockfd, maxConnections ) != -1 )
      {
         // set legible ip address
         m_ipAddress = inet_ntoa( m_addr.sin_addr );
         return true;
      }

      return false;
   }

   bool Socket::Accept( Socket& newSocket ) const
   {
      int32_t addr_length = sizeof( newSocket.m_addr );
      newSocket.m_sockfd = accept( m_sockfd, (struct sockaddr *) &newSocket.m_addr, (socklen_t *) &addr_length );
      newSocket.m_ipAddress = inet_ntoa( newSocket.m_addr.sin_addr );
      fcntl( newSocket.m_sockfd, F_SETFL, O_NONBLOCK );

      return newSocket.IsValid();
   }

   bool Socket::Close()
   {
      if( IsValid() )
      {
         int32_t err;
         if( !getSOError( err ) )
         {
            return false;
         }

         if( shutdown( m_sockfd, SHUT_RDWR ) < 0 )
         {
            if( err != ENOTCONN && err != EINVAL )
            {
               return false;
            }
         }

         if( close( m_sockfd ) < 0 )
         {
            return false;
         }
      }

      return true;
   }

   bool Socket::IsValid() const
   {
      return m_sockfd != -1;
   }

   bool Socket::CheckConnection()
   {
      // try to send data and check error (ignoring signals)
      char buf[1] = { '$' };
      ssize_t retval = send( m_sockfd, buf, sizeof( buf ), MSG_NOSIGNAL );

      return ( retval >= 0 );
   }

   uint16_t Socket::GetPort() const
   {
      return m_port;
   }

   std::string Socket::GetIpAddress()
   {
      return m_ipAddress;
   }

   bool Socket::Recv( std::string& data )
   {
      data = "";

      size_t bytesToRead = BUFFERSIZE;
      char lpvBuffer[ BUFFERSIZE ];
      ssize_t bytesRead = 0;

      bytesRead = read( m_sockfd, lpvBuffer, bytesToRead );
      if( bytesRead < 0 )
      {
         return false;
      }

      data.append( lpvBuffer, bytesRead );

      return true;
   }

   bool Socket::getSOError( int& err )
   {
      err = -1;
      socklen_t len = sizeof err;

      if( getsockopt( m_sockfd, SOL_SOCKET, SO_ERROR, (char *)&err, &len ) == -1 )
      {
         return false;
      }

      return true;
   }

   bool Socket::Send( std::string data ) const
   {
      int status = ::send ( m_sockfd, data.c_str(), data.size(), MSG_NOSIGNAL );
      return !( status == -1 );
   }

   bool Socket::SocketConnect( const std::string host )
   {
      int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

      status = ::connect ( m_sockfd, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

      return !( status < 0 );
   }
}
