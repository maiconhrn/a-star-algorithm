#include <iostream>

#include "15_puzzle.h"

/*
 * Acadêmico: Maicon Henrique Rodrigues do Nascimento RA102504
 *
 * Programa desenvolvido com base em sistema Linux.
 *
 */
int main() {
//    getline(cin, in);

    auto board = read_board(std::cin);

    auto result = calc_lowest_move_number_to_final_board(board);

    std::cout << result.second << std::endl;

    return 0;
}