#include "15_puzzle.h"

std::pair<bool, int> calc_lowest_move_number_to_final_board(short board[4][4]) {
    state state1(board);
    state state2(FINAL_BOARD_STATE);

    a_star astar(std::unordered_map<std::string, state>(),
                 std::unordered_map<std::string, state>(),
                 std::unordered_map<std::string, state>({{state1.hash_key, state1}}),
                 std::unordered_map<std::string, state>({{state2.hash_key, state2}}));

    return astar.run();
}
