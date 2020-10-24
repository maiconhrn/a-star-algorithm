#include "15_puzzle.h"

typedef unsigned long long ull;

short **read_board(std::istream &in) {
    auto board = new short *[4];

    for (int i = 0; i < 4; ++i) {
        board[i] = new short[4];

        for (int j = 0; j < 4; ++j) {
            in >> board[i][j];
        }
    }

    return board;
}

std::pair<bool, int> calc_lowest_move_number_to_final_board(short **board) {
    state state1(board);
    state state2(new short *[4]{
            FINAL_BOARD_STATE[0],
            FINAL_BOARD_STATE[1],
            FINAL_BOARD_STATE[2],
            FINAL_BOARD_STATE[3],
    });

    a_star astar(std::unordered_map<ull, state>(),
                 std::unordered_map<ull, state>(),
                 std::unordered_map<ull, state>({{state1.hash_key, state1}}),
                 std::unordered_map<ull, state>({{state2.hash_key, state2}}));

    return astar.run();
}
