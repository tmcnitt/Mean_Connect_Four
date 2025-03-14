#pragma once

#include <cstdint>

//https://minimax.dev/docs/ultimate/pn-search/

//Standard board
//#define WIDTH 7
//#define HEIGHT 6

#define WIDTH 7
#define HEIGHT 6
// bitmask corresponds to board as follows in 7x6 case:
//  .  .  .  .  .  .  .  TOP
//  5 12 19 26 33 40 47
//  4 11 18 25 32 39 46
//  3 10 17 24 31 38 45
//  2  9 16 23 30 37 44
//  1  8 15 22 29 36 43
//  0  7 14 21 28 35 42  BOTTOM
#define H1 (HEIGHT+1)
#define H2 (HEIGHT+2)
#define SIZE (HEIGHT*WIDTH)
#define SIZE1 (H1*WIDTH)

#define COL1 (((uint64_t)1<<H1)-(uint64_t)1)
#define ALL1 (((uint64_t)1<<SIZE1)-(uint64_t)1)
#define BOTTOM (ALL1 / COL1) // has bits i*H1 set
#define TOP (BOTTOM << HEIGHT)
#define FULL (BOTTOM << (HEIGHT-1))

class bit_board {
    private:
        void shift_pieces_down(int, int);
        void shift_pieces_up(int, int);

        void mean_move(int, int);
        void normal_move(int, int);
        void undo_mean_move(int, int);

        bool isplayable_normal(int);
        bool isplayable_mean(int, int);

        bool check_gravity_col(int);

    public:
        bit_board(){
            for(int i = 0; i < WIDTH; i++){
                this->m_heights[i] = (char)(H1*i);
            }
        }

        bool check_gravity();

        char m_heights[WIDTH];
        char m_moves[HEIGHT*WIDTH];

        uint32_t m_pieces_on_board = 0;
        uint32_t m_move_num = 0;
        uint32_t m_pop_counts[2] = {0,0};

        uint64_t m_boards[2] = {0,0};

        void makemove_setup_only_dont_use(int col, int player);

        void makemove(int move);
        void undomove();

        void print();
        bool isfull();
        bool haswon(int player);
        bool isplayable(int col);
};