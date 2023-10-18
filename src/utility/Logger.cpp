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

#include "utility/Logger.h"

#include <iomanip>

namespace utility
{
   Logger::Logger()
   {
   }

   Logger& Logger::GetInstance()
   {
      static Logger logger;
      return logger;
   }

   void Logger::operator()( std::string const& description )
   {
      std::cout << description << std::endl;
   }

   void Logger::operator()( uint16_t logType, std::string const& file, uint32_t line, std::string const& description )
   {
      std::stringstream ss;

      switch( logType )
      {
         case Info      : ss << "Information;"; break;
         case Warning   : ss << "Warning;"; break;
         case Error     : ss << "Error;"; break;
      }
      ss << excludePath( file ) << ":" << std::setw(1) << std::dec << line << ";";

      std::string descriptionStr = description;
      descriptionStr = excludeChar( descriptionStr, ';' );
      descriptionStr = excludeChar( descriptionStr, '\r' );
      descriptionStr = excludeChar( descriptionStr, '\n' );

      ss << descriptionStr;

      std::cout << ss.str() << std::endl;
   }

   std::string Logger::excludePath( std::string const& filename )
   {
      std::string str_ret = filename;
      std::string::size_type pos;

      if( ( pos = str_ret.rfind( '/' ) ) != std::string::npos )
      {
         str_ret.erase( 0, pos+1 );
      }

      return str_ret;
   }

   std::string Logger::excludeChar( std::string const& data, unsigned char toExclude )
   {
      std::string str_ret = data;
      std::string::size_type pos;

      while( ( pos = str_ret.find( toExclude ) ) != std::string::npos )
      {
         str_ret.replace( pos, 1, " - " );
      }

      return str_ret;
   }
}
