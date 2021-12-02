// Fhourstones 3.0 bit_board Logic
// (http://www.cwi.nl/~tromp/c4/fhour.html)

#include "bit_board.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <bitset>

bool bit_board::check_gravity(){
  for(int i = 0; i < WIDTH; i++){
    if(!this->check_gravity_col(i)){
      std::cout << "Gravity error in col: " << i << std::endl;
      return false;
    };
  }

  uint64_t board = this->m_boards[0] | this->m_boards[1];
  int count = 0;
  while (board) {
    count += board & 1;
    board >>= 1;
  }

  if(count != this->m_pieces_on_board){
    std::cout << "Bit error" << std::endl;
    return false;
  }

  return true;
}

bool bit_board::check_gravity_col(int col){
  int height = (int)this->m_heights[col] - (int)(H1*col);

  for(int i = 0; i < HEIGHT; i++){
    char pos = (H1*col) + i;
    uint64_t filter = ((uint64_t)1 << (char)pos);
    uint64_t has_piece = (this->m_boards[0] & filter) != 0 || (this->m_boards[1] & filter) != 0;


    if(has_piece && i > height){
      std::cout << "Gravity error, top" << std::endl;
      this->print();
      return false;
    }

    if(!has_piece && (i+1) < height){
      std::cout << "Gravity error, bottom" << std::endl;
      this->print();
      return false;
    }
  }

  return true;
}

void bit_board::makemove_setup_only_dont_use(int col, int player)
{
  this->m_boards[player] ^= (uint64_t)1<<(char)this->m_heights[col]++;
}

void bit_board::mean_move(int from_col, int end_col)
{
  assert(this->m_heights[from_col] > H1*from_col);
  assert(this->m_move_num > 0);

  this->shift_pieces_down(from_col, 0);
  this->shift_pieces_down(from_col, 1);

  this->m_heights[from_col]--;

  this->normal_move(end_col, !(bool)(this->m_move_num & 1));
}

void bit_board::normal_move(int col, int board)
{
  this->m_boards[board] ^= (uint64_t)1<<(char)this->m_heights[col]++;
}

int bit_board::popcount()
{
  return this->m_pop_count;
}

void bit_board::makemove(int move){
  //assert(this->isplayable(move));
  assert(move > 0);
  assert(move != 255);

  if(move < 10){
    this->m_pieces_on_board++;
    this->normal_move(move-1, this->m_move_num & 1);
  } else {
    int from = (move / 10)-1;
    int end = (move % 10)-1;

    assert(from != end);
    assert(from > -1 && end > -1);
    assert(from < WIDTH && end < WIDTH);

    this->mean_move(from, end);

    if((this->m_move_num & 1) == 0){
      this->m_pop_count++;
    }
  }

  this->m_moves[this->m_move_num++] = move;

  //std::cout << "Move: " << move << std::endl;
}

void bit_board::undo_mean_move(int from, int end){
  //We moved a piece from bottom of from to top of end
  int other_board = !(bool)(this->m_move_num & 1);

  //In order to undo it, we need to take from the top of end
  this->m_boards[other_board] ^= (uint64_t)1<<(char)(--this->m_heights[end]);

  //Shift up the from 
  this->shift_pieces_up(from, 0);
  this->shift_pieces_up(from, 1);

  //And put the piece back
  this->m_boards[other_board] ^= (uint64_t)1<<(char)(H1*from);
  this->m_heights[from]++;
}

void bit_board::undomove()
{
  assert(this->m_move_num > 0);
  
  int move_num = --this->m_move_num;
  int move = this->m_moves[this->m_move_num];

  if(move < 10){
    assert(this->m_pieces_on_board > 0);
    this->m_pieces_on_board--;
    this->m_boards[move_num & 1] ^= (uint64_t)1<<(char)(--this->m_heights[move-1]);
  } else {
    int from = (move / 10) - 1;
    int end = (move % 10) - 1;

    this->undo_mean_move(from, end);

    if((this->m_move_num & 1) == 0){
      assert(this->m_pop_count > 0);
      this->m_pop_count--;
    }
  }
}

void bit_board::shift_pieces_up(int col, int board)
{
  char starting_col_height = col*H1;

  uint64_t col_filter = (uint64_t)COL1 << (char)starting_col_height;
  uint64_t curr_col = this->m_boards[board] & col_filter;
  
  //Clear column
  this->m_boards[board] ^= curr_col;

  //Set to one shifted version
  this->m_boards[board] ^= curr_col << 1;
}


void bit_board::shift_pieces_down(int col, int board)
{
  char starting_col_height = col*H1;

  uint64_t col_filter = (uint64_t)COL1 << (char)starting_col_height;
  uint64_t curr_col = this->m_boards[board] & col_filter;
  
  //Clear column
  this->m_boards[board] ^= curr_col;

  //Clear bottom bit
  curr_col &= ~((uint64_t)1 << (char)starting_col_height);

  //Set to one shifted version
  this->m_boards[board] ^= curr_col >> 1;
}

void horiz_line(){
  std::cout << "--";
  for(int i = 0; i < WIDTH; i++){
    std::cout << "--";
  }
  std::cout << "-";
  std::cout << std::endl; 
}

void bit_board::print()
{
  std::cout << "Board0: " << this->m_boards[0] << std::endl;
  std::cout << "Board1: " << this->m_boards[1] << std::endl;
  std::cout << "Pieces on board: " << this->m_pieces_on_board << std::endl;
  std::cout << "Pop moves used: " << this->m_pop_count << std::endl;

  std::cout << "Heights: ";
  for(int i = 0; i < WIDTH; i++){
    std::cout << (int)this->m_heights[i] << ",";
  }
  std::cout << std::endl;

  std::cout << "Num: ";
  for(int i = 0; i < WIDTH; i++){
    std::cout << (int)this->m_heights[i] - (int)(H1*i) << ",";
  }
  std::cout << std::endl;

  horiz_line();

 for(int row = HEIGHT-1; row >= 0; row--){
    std::cout << "| ";
    for(int col = 0; col < WIDTH; col++){
      uint64_t bit = (uint64_t)1 << ((char)(H1 * col)+ row);

      if((m_boards[0] & bit) != 0){
        std::cout << "r ";
      } else if ((m_boards[1] & bit) != 0){
        std::cout << "y ";
      } else {
        std::cout << "  ";
      }
    }
    std::cout << "|";
    std::cout << std::endl;
  }
  horiz_line();
}


bool bit_board::isfull()
{
  return FULL == (FULL & (m_boards[0] | m_boards[1]));
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

bool bit_board::isplayable_normal(int col)
{
  return (TOP & ((uint64_t)1 << this->m_heights[col])) == 0;
}

bool bit_board::isplayable_mean(int from, int end)
{
  //need to see if opposite player has piece at bottom 
  int other_board = !(this->m_move_num & 1);

  char starting_col_height = from*H1;
  uint64_t first_pos = (uint64_t)1 << (char)starting_col_height;

  bool is_placed = (this->m_boards[other_board] & first_pos) != 0;

  if(!is_placed){
    return false;
  }

  //and need to see if there is room in end column
  return this->isplayable_normal(end);
}


bool bit_board::isplayable(int move)
{ 
  assert(move > 0);

  if(move < 10){
    return this->isplayable_normal(move-1);
  } else {
    int from = (move / 10) - 1;
    int end = (move % 10) - 1;

    return this->isplayable_mean(from, end);
  }
}