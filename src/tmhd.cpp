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

#include "tmhd.h"
#include "utility/Socket.h"
#include "protocol/protocol.h"
#include "utility/Logger.h"

#include <signal.h>
#include <time.h>
#include <getopt.h>
#include <iterator>
#include <unistd.h>
#include <algorithm>

#define OUT( message ) utility::Logger::GetInstance()( message );
#define INFO( message ) utility::Logger::GetInstance()( utility::Logger::Info,  __FILE__, __LINE__, message );
#define WARNING( message ) utility::Logger::GetInstance()( utility::Logger::Warning,  __FILE__, __LINE__, message );
#define ERROR( message ) utility::Logger::GetInstance()( utility::Logger::Error,  __FILE__, __LINE__, message );

void TerminateSignal( int32_t signal )
{
   tmp::TMHD::GetInstance().Terminate();
   OUT( "... terminated!" );
}

namespace tmp
{
   TMHD::TMHD( unsigned short port ) :
      m_pid( 0 ),
      m_daemonMode( false ),
      m_terminated( true ),
      m_port( port ),
      m_listenerThread(),
      m_monitorSessionListThread(),
      m_sessionThreadList()
   {
   }

   TMHD::~TMHD()
   {
   }

   TMHD& TMHD::GetInstance()
   {
      static TMHD server;
      return server;
   }

   int32_t TMHD::GetTMHDPID()
   {
      FILE *fd = popen( "ps -A -o pid,comm --sort start_time | grep '\\<tmhd\\>' | sed 's:^\\s*::' | cut -d ' ' -f 1", "r" );
      char buf[ 100] = "";
      fread( buf, 1, 100, fd );
      pclose( fd );
      return atoi( buf );
   }

   TMHD::SessionList& TMHD::GetActiveSessions()
   {
      return m_sessionThreadList;
   }

   void TMHD::SetDaemonMode()
   {
      m_daemonMode = true;
   }

   void TMHD::SetPort( unsigned short port )
   {
      m_port = port;
   }

   bool TMHD::IsTerminated() const
   {
      return m_terminated;
   }

   void TMHD::Terminate()
   {
      m_terminated = true;
   }

   bool TMHD::RunTMHD()
   {
      // Set terminate signal function
      signal( SIGTERM, TerminateSignal );

      if( m_daemonMode )
      {
         // Fork off the parent process
         m_pid = fork();
         if( m_pid < 0  )
         {
            // error
            OUT( "Could not start server ..." );
            return EXIT_FAILURE;
         }
         if( m_pid > 0 )
         {
            // got a good pid: exit parent process
            return EXIT_SUCCESS;
         }
      }

      if( !startListenThread() )
      {
         ERROR( "Failed!" );
         return false;
      }

      while( !IsTerminated() )
      {
         // do nothing until the main thread is finished (terminated)
         sleep( 1 );
      }

      return true;
   }

   void TMHD::ListenForConnections()
   {
      utility::Socket serverSocket;

      while( !IsTerminated() )
      {
         try
         {
            uint16_t maxRetries = 5;

            if( !serverSocket.Open( m_port ) )
            {
               // Terminate program
               OUT( "Could not start server: socket error while openning!" )
               Terminate();
               return;
            }

            while( !serverSocket.Listen() && maxRetries > 0 )
            {
               usleep( 500 );
               maxRetries--;
            }

            if( maxRetries == 0 )
            {
               // Terminate program
               OUT( "Could not start server: socket error while listening!" );
               Terminate();
               return;
            }

            while( !IsTerminated() )
            {
               sleep( 1 );

               utility::Socket* clientSocket = NULL;
               try
               {
                  clientSocket = new utility::Socket();
                  if( serverSocket.Accept( *clientSocket ) )
                  {
                     std::string data;
                     if( clientSocket->Recv( data ) && data == "$" )
                     {
                        INFO( "TMHD UP!!!" );
                        continue;
                     }

                     // create session
                     business::PlayerSession* session = new business::PlayerSession( clientSocket );

                     // received a connection from a client: create a session

                     //verificar se o ip já está connectado
//                      for ( auto iter : m_sessionThreadList )
//                      {
//                         if ( (iter)->GetIpAddress() == session->GetIpAddress() )
//                         {
//                            (iter)->Terminate();
//                            break;
//                         }
//                      }

                     std::unique_ptr< std::thread > sessionThread( new std::thread( sessionThreadEnterPoint, session ) );
                     sessionThread->detach();

                     m_sessionThreadList.push_back( session );
                  }
               }
               catch( std::exception e )
               {
                  // do nothing: repeat process
                  continue;
               }
            }
         }
         catch( std::exception e )
         {
            // error creating socket
            ERROR( e.what() );
            Terminate();
         }
      }

      serverSocket.Close();
   }

   bool TMHD::startListenThread()
   {
      std::stringstream ss;
      ss << "Running tmhd server on port " << m_port << " ... ";
      OUT( ss.str() );

      m_terminated = false;

      m_listenerThread = std::unique_ptr< std::thread >( new std::thread( listenThreadEnterPoint, this ) );
      m_listenerThread->detach();

      m_monitorSessionListThread = std::unique_ptr< std::thread >( new std::thread( monitorSessionListThread, this ) );
      m_monitorSessionListThread->detach();

      return true;
   }

   void TMHD::listenThreadEnterPoint( TMHD* server )
   {
      if( server == NULL )
      {
         return;
      }

      try
      {
         server->ListenForConnections();
      }
      catch ( std::exception& e )
      {
         ERROR( e.what() );
      }
   }

   void TMHD::sessionThreadEnterPoint( business::PlayerSession* session )
   {
      INFO( "Sessao criada (" + session->GetIpAddress() + ")" );

      session->Handle();

      INFO( "Sessao terminada (" + session->GetIpAddress() + ")" );
   }

   void TMHD::monitorSessionListThread( TMHD* /*server*/ )
   {
//       uint32_t logTime = 0;
//
//       while( !server->IsTerminated() )
//       {
//          SessionList& activeSessions = server->GetActiveSessions();
//
//          if( logTime == 5 )
//          {
//             struct tm tm;
//             time_t dataTime;
//             time( &dataTime );
//             localtime_r( &dataTime, &tm );
//
//             std::stringstream ss;
//             ss << tm.tm_year+1900 <<":"<< tm.tm_mon+1 <<":"<< tm.tm_mday
//                << " - " << tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec
//                << " Number of active sessions: " << activeSessions.size();
//             INFO( ss.str() );
//             logTime = 0;
//          }
//
//          SessionList::iterator iter;
//          for( iter = activeSessions.begin() ; iter != activeSessions.end(); iter++ )
//          {
//             unsigned short cont = 0;
//             for( auto iterMonitor : activeSessions )
//             {
//                if( (*iterMonitor).GetUser().GetUsername() == (*iter)->GetUser().GetUsername() )
//                {
//                   if( cont == 1 )
//                   {
//                      ( *std::find( activeSessions.begin(), activeSessions.end(), *iter ) )->Terminate();
//                      continue;
//                   }
//                   cont = 1;
//                }
//             }
//
//             if( (*iter)->IsTerminated() )
//             {
//                iter = activeSessions.erase( iter );
//             }
//          }
//          logTime++;
//
//          usleep( 500000 );
//       }
   }
}

void printHelp( std::string const& name )
{
   OUT( "Usage: " + name + " [OPTIONS] COMMAND" );
   OUT( "" );
   OUT( "OPTIONS:" );
   OUT( "   -h, --help                       show this help" );
   OUT( "   -d, --daemon                     set to run in daemon mode" );
   OUT( "   -p <port>, --port=<port>         set which port to listen for connections" );
   OUT( "" );
   OUT( "COMMAND:" );
   OUT( "   start" );
   OUT( "   status" );
   OUT( "   stop" );
   OUT( "" );
}


int main( int argc, char* argv[] )
{
   // instantiate server singleton class
   tmp::TMHD& server = tmp::TMHD::GetInstance();

   while( true  )
   {
      option long_options[] = { {"help",   0, 0, 'h'},
                                {"daemon", 0, 0, 'd'},
                                {"port",   1, 0, 'p'} };
      int32_t option_index = 0;
      char c = getopt_long( argc, argv, "hdp:", long_options, &option_index );
      if( c == -1 )
      {
         break;
      }

      switch( c )
      {
         case '?':
            return EXIT_FAILURE;
            break;
         case ':':
            OUT( "Syntax error!" );
            OUT( "" );
         case 'h':
            printHelp( argv[0] );
            return EXIT_FAILURE;
            break;
         case 'd':
         {
            server.SetDaemonMode();
         } break;
         case 'p':
         {
            char *endptr;
            server.SetPort( strtoul( optarg, &endptr, 10 ) );
            if( *endptr != '\0' )
            {
               OUT( "Invalid value for option -- p" );
               return EXIT_FAILURE;
            }
         } break;
         default:
            OUT( "ERROR: An unknown error occurred while parsing command line arguments" );
            return EXIT_FAILURE;
            break;
      }
   }

   if( optind == argc - 1 )
   {
      std::string arg( argv[ optind ] );
      if( arg == "stop" || arg == "restart" )
      {
         OUT( "Stopping TMHD Server ..." );
         int32_t pid = tmp::TMHD::GetTMHDPID();
         if( pid != 0 && pid != getpid() && kill( pid, SIGTERM ) == 0 )
         {
            OUT( "... success!" );
         }
         else
         {
            OUT( "... failed!" );
            return EXIT_FAILURE;
         }
      }
      if( arg == "start" || arg == "restart" )
      {
         int32_t pid = tmp::TMHD::GetTMHDPID();
         if( pid != getpid() )
         {
            OUT( "TMHD is already running ..." );
            return EXIT_FAILURE;
         }

         OUT( "Starting TMHD Server ..." );
         server.RunTMHD();

         return EXIT_SUCCESS;
      }
      if( arg == "status" )
      {
         int32_t pid = tmp::TMHD::GetTMHDPID();
         if( pid != getpid() )
         {
            OUT( "TMHD server is running with PID " );
            OUT( "Number of active sessions/connections: " + tmp::TMHD::GetInstance().GetActiveSessions().size() );
         }
         else
         {
            OUT( "TMHD server is not running ..." );
         }
         return EXIT_SUCCESS;
      }

      if( arg != "stop" )
      {
         printHelp( argv[0] );
      }

      return EXIT_FAILURE;
   }
   else
   {
      printHelp( argv[0] );
      return EXIT_FAILURE;
   }

   return EXIT_FAILURE;
}
