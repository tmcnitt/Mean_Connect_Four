// Fhourstones 3.0 bit_board Logic
// (http://www.cwi.nl/~tromp/c4/fhour.html)
//
// implementation of the well-known game
// usually played on a vertical board of 7 columns by 6 rows,
// where 2 players take turns in dropping counters in a column.
// the first player to get four of his counters
// in a horizontal, vertical or diagonal row, wins the game.
// if neither player has won after 42 moves, then the game is drawn.
//
// This software is copyright (c) 1996-2008 by
//      John Tromp
//      600 Route 25A
//      East Setauket
//      NY 11733
// E-mail: john.tromp@gmail.com
//
// This notice must not be removed.
// This software must not be sold for profit.
// You may redistribute if your distributees have the
// same rights and restrictions.
#include "bit_board.h"
#include <stdio.h>
#include <stdlib.h>


void bit_board::makemove(int player, int col)
{
    this->m_boards[player] ^= (uint64_t)1<<this->m_heights[col]++;
}

bool bit_board::isfull()
{
  return (FULL & (m_boards[0] | m_boards[1])) != 0;
}

bool bit_board::haswon(int player)
{
  uint64_t board = this->m_boards[player];

  uint64_t diag1 = board & (board>>HEIGHT);
  uint64_t hori = board & (board>>H1);
  uint64_t diag2 = board & (board>>H2);
  uint64_t vert = board & (board>>1);

  return ((diag1 & (diag1 >> 2*HEIGHT)) |
          (hori & (hori >> 2*H1)) |
          (diag2 & (diag2 >> 2*H2)) |
          (vert & (vert >> 2))) != 0;
}

bool bit_board::isplayable(int col)
{
  return (TOP & ((uint64_t)1 << this->m_heights[col])) == 0;
}
