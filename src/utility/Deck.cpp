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

#include "utility/Deck.h"

#include <ctime>
#include <cstdlib>

namespace utility
{
   Deck::Deck( Deck::Type type ) :
      m_cards()
   {
      for( uint16_t suit = Card::Spades; suit <= Card::Clubs; suit++ )
      {
         for( uint16_t rank = Card::Ace; rank <= Card::King; rank++ )
         {
            if( ( rank != Card::Eight && rank != Card::Nine && rank != Card::Ten ) )
            {
               utility::Card card( static_cast< Card::Suit >( suit ), static_cast< Card::Rank >( rank ) );
               if( type == Deck::EmptyDeck && ( rank == Card::Five || rank == Card::Six || rank == Card::Seven ) )
               {
                  continue;
               }

               AddCard( card );
            }
         }
      }

      utility::Card card( Card::Suit::SuitJk, Card::Rank::RankJk );
      AddCard( card );
   }

   Deck::~Deck()
   {
   }

   void Deck::AddCard( Card const& card )
   {
      m_cards.push_back( card );
   }

   const std::vector< Card > Deck::GetCards() const
   {
      return m_cards;
   }

   const std::vector< Card > Deck::GetHand()
   {
      std::vector< utility::Card > hand;

      // Use o tempo atual como semente para o gerador de números aleatórios
      std::srand( static_cast< unsigned >( std::time( 0 ) ) );

      // Sortea 3 cartas
      for( uint16_t iter = 0; iter < 3; iter++ )
      {
         uint32_t randomIndex = std::rand() % m_cards.size();

         hand.push_back( m_cards[ randomIndex ] );
         m_cards.erase( m_cards.begin() + randomIndex );
      }

      return hand;
   }

}
