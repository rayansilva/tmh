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

#include "utility/Card.h"

namespace utility
{
   Card::Card( Card::Suit suit, Card::Rank rank ) :
      m_suit( suit ),
      m_rank( rank )
   {
   }

   Card::~Card()
   {
   }

   uint16_t Card::GetRank() const
   {
      return m_rank;
   }

   void Card::SetRank( Card::Rank rank )
   {
      m_rank = rank;
   }

   uint16_t Card::GetSuit() const
   {
      return m_suit;
   }

   void Card::SetSuit( Card::Suit suit )
   {
      m_suit = suit;
   }
}
