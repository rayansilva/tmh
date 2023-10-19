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

#include "Persistence.h"

namespace persistence
{
   Persistence::Persistence( std::string connectionString ):
      m_connectionString( connectionString )
   {
   }

   pqxx::result Persistence::ExecuteQuery( std::string query )
   {
      pqxx::connection conn( m_connectionString );
      pqxx::work txn( conn );
      pqxx::result result_set = txn.exec( query );
      txn.commit();

      return result_set;
   }

   void Persistence::ExecuteCommand( std::string command )
   {
      pqxx::connection conn( m_connectionString );
      pqxx::work txn(conn);
      txn.exec(command);
      txn.commit();
   }

   pqxx::result Persistence::Select( std::string table, std::string condition )
   {
      std::string query = "SELECT * FROM " + table;

      if( !condition.empty() )
      {
         query += " WHERE " + condition;
      }

      return ExecuteQuery(query);
   }

   void Persistence::Insert( std::string table, std::string values )
   {
      std::string query = "INSERT INTO " + table + " VALUES (" + values + ")";
      ExecuteCommand(query);
   }

   void Persistence::Update( std::string table, std::string set_values, std::string condition )
   {
      std::string query = "UPDATE " + table + " SET " + set_values + " WHERE " + condition;
      ExecuteCommand(query);
   }

   void Persistence::Delete( std::string table, std::string condition )
   {
      std::string query = "DELETE FROM " + table + " WHERE " + condition;
      ExecuteCommand(query);
   }
}