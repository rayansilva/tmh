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

#ifndef _tmp_tmhdserver_
#define _tmp_tmhdserver_

#include <string>
#include <thread>
#include <atomic>
#include <list>

#include "business/PlayerSession.h"

namespace tmp
{
   class TMHD
   {
   private:
      TMHD( uint16_t port = 2525 );

   public:
      typedef std::list< business::PlayerSession* > SessionList;

      ~TMHD();

      static TMHD& GetInstance();
      static int32_t GetTMHDPID();

      SessionList& GetActiveSessions();

      void SetDaemonMode();
      void SetPort( uint16_t port );
      bool IsTerminated() const;
      void Terminate();

      bool RunTMHD();
      void ListenForConnections();

   private:
      pid_t m_pid;
      bool m_daemonMode;
      bool m_terminated;
      uint16_t m_port;
      std::unique_ptr< std::thread > m_listenerThread;
      std::unique_ptr< std::thread > m_monitorSessionListThread;
      SessionList m_sessionThreadList;

      business::PlayerSession* createSession( void* arg );

      bool startListenThread();
      static void listenThreadEnterPoint( TMHD* tmhdServer );
      static void sessionThreadEnterPoint( business::PlayerSession* session );
      static void monitorSessionListThread( TMHD* tmhdServer );
   };
}
#endif