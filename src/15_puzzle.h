#ifndef _15_PUZZLE_H
#define _15_PUZZLE_H

#include <string>
#include <istream>

#include "a_star.h"

static short FINAL_BOARD_STATE[4][4] = {
        {1,  2,  3,  4},
        {12, 13, 14, 5},
        {11, 0,  15, 6},
        {10, 9,  8,  7}
};

short **read_board(std::istream &in);

std::pair<bool, int> calc_lowest_move_number_to_final_board(short board[4][4]);

#endif //_15_PUZZLE_H
