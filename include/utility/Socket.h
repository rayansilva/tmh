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

#ifndef _utility_socket_
#define _utility_socket_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

const int32_t MAXHOSTNAME = 200;
const int32_t MAXCONNECTIONS = 20;
const int32_t MAXRECV = 500;
const int32_t BUFFERSIZE = 8192;

namespace utility
{
   class Socket
   {
   public:
      Socket();
      ~Socket();

      bool Open( uint16_t port );
      bool Create( std::string const& host, uint16_t port );
      bool Listen( int32_t maxConnections = MAXCONNECTIONS );
      bool Accept( Socket& ) const;
      bool Close();

      bool IsValid() const;
      bool CheckConnection();
      bool SocketConnect( const std::string host );

      uint16_t GetPort() const;
      std::string GetIpAddress();

      bool Recv( std::string& );
      bool Send( std::string ) const;

   private:
      int32_t m_sockfd;
      sockaddr_in m_addr;
      uint16_t m_port;
      std::string m_ipAddress;

      bool getSOError( int32_t& err );
   };
}
#endif