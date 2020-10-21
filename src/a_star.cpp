#include <cmath>
#include <set>
#include "a_star.h"
#include "15_puzzle.h"

a_star::a_star(const std::unordered_map<ull, state> &a, const std::unordered_map<ull, state> &f,
               const std::unordered_map<ull, state> &s, const std::unordered_map<ull, state> &t)
        : A(a),
          F(f),
          S(s),
          T(t) {}

int a_star::h1(const state s) {
    int out_of_pos = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (s.board[i][j] != FINAL_BOARD_STATE[i][j]) {
                out_of_pos++;
            }
        }
    }

    return out_of_pos;
}

int a_star::h3(state s) {
    int out_of_pos = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            short pos = s.board[i][j];
            if (pos == 0 || s.board[i][j] == FINAL_BOARD_STATE[i][j]) {
                continue;
            }
            out_of_pos += (short) (abs((short) (floor((pos - 1) % 4)) - i)
                                   + abs((short) floor(pos / 4.1) - j));
        }
    }
    return out_of_pos;
}

int a_star::calc_heuristic(const heuristc_t type, state s) {
    switch (type) {
        case heuristc_t::H1:
            return h1(s);
        case heuristc_t::H2:
            return 0;
        case heuristc_t::H3:
            return h3(s);
        case heuristc_t::H4:
            return 0;
        case heuristc_t::H5:
            return 0;
        default:
            return 0;
    }
}

state::state() = default;

state::state(short **board) : board(board) {
    generate_hash_key();
}

void state::calc_f() {
    f = g + heuristic_value;
}

void state::calc_t() const {
    t->clear();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                if (i != 0) { //trocar com a peça de cima
                    state up;
                    up.g = g + 1;
                    up.copy_board(board);

                    short aux = up.board[i][j];
                    up.board[i][j] = up.board[i - 1][j];
                    up.board[i - 1][j] = aux;

                    up.generate_hash_key();
                    t->insert(std::make_pair(up.hash_key, up));
                }
                if (i != 3) { //trocar com a peça de  baixo
                    state down;
                    down.g = g + 1;
                    down.copy_board(board);

                    short aux = down.board[i][j];
                    down.board[i][j] = down.board[i + 1][j];
                    down.board[i + 1][j] = aux;

                    down.generate_hash_key();
                    t->insert(std::make_pair(down.hash_key, down));
                }
                if (j != 0) { //trocar com a peça da esquerda
                    state left;
                    left.g = g + 1;
                    left.copy_board(board);

                    short aux = left.board[i][j];
                    left.board[i][j] = left.board[i][j - 1];
                    left.board[i][j - 1] = aux;

                    left.generate_hash_key();
                    t->insert(std::make_pair(left.hash_key, left));
                }
                if (j != 3) { //trocar com a peça da direita
                    state right;
                    right.g = g + 1;
                    right.copy_board(board);

                    short aux = right.board[i][j];
                    right.board[i][j] = right.board[i][j + 1];
                    right.board[i][j + 1] = aux;

                    right.generate_hash_key();
                    t->insert(std::make_pair(right.hash_key, right));
                }
            }
        }
    }
}

void state::generate_hash_key() {
    hash_key = 0;
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            short aux = board[i][j];
            hash_key += aux << (count * 4);
            count++;
        }
    }
}

bool state::operator==(const state &rhs) const {
    return hash_key == rhs.hash_key;
}

bool state::operator!=(const state &rhs) const {
    return !(rhs == *this);
}

void state::copy_board(short **b) {
    board = new short *[4];

    for (int i = 0; i < 4; ++i) {
        board[i] = new short[4];
        for (int j = 0; j < 4; ++j) {
            board[i][j] = b[i][j];
        }
    }
}

std::pair<bool, int> a_star::run() {
    state v;
    std::unordered_map<ull, state>::iterator it;

    for (auto &s : S) {
        s.second.heuristic_value = calc_heuristic(heuristc_t::H1, s.second);
        s.second.g = 0;
        s.second.p = nullptr;
        s.second.calc_f();
    }

    A.clear();
    A.insert(S.begin(), S.end());
    F.clear();

    v = A.begin()->second;
    while (T.find(v.hash_key) == T.end() && v.hash_key != -1) {
        A.erase(v.hash_key);
        F.insert(std::make_pair(v.hash_key, v));

        v.calc_t();
        for (auto &m : *(v.t)) {
            if ((it = A.find(m.first)) != A.end()
                && m.second.g < it->second.g) {
                A.erase(it->first);
            }

            if ((it = F.find(m.first)) != F.end()
                && m.second.g < it->second.g) {
                F.erase(it->first);
            }

            if (A.find(m.first) == A.end()
                && F.find(m.first) == F.end()) {
                m.second.p = &v;
                m.second.heuristic_value = calc_heuristic(heuristc_t::H1, m.second);
                m.second.calc_f();
                A.insert(m);
            }
        }

        v = find_min_f(A);
    }

    return (v.f >= 0 && T.find(v.hash_key) != T.end())
           ? std::pair<bool, int>(true, v.g)
           : std::pair<bool, int>(false, -1);
}

a_star::~a_star() {
    //delete[] &A;
    //delete[] &F;
    //delete[] &S;
    //delete[] &T;
}

state a_star::find_min_f(std::unordered_map<ull, state> &map) {
    if (!map.empty()) {
        state s = map.begin()->second;
        for (auto &i : map) {
            if (i.second.f < s.f
                || (i.second.f == s.f && i.second.heuristic_value < s.heuristic_value)) {
                s = i.second;
            }
        }

        return s;
    }

    return {};
}
