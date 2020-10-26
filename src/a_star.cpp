#include <cmath>
#include <set>
#include <utility>
#include "a_star.h"
#include "15_puzzle.h"

a_star::a_star(std::unordered_map<ull, state> a, std::unordered_map<ull, state> f,
               std::unordered_map<ull, state> s, std::unordered_map<ull, state> t)
        : A(std::move(a)),
          F(std::move(f)),
          S(std::move(s)),
          T(std::move(t)) {}

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

int a_star::h2(state s) {
    int out_of_pos = 0;
    std::pair<int, int> actual, next;
    short actual_v, next_v;

    const static std::pair<int, int> board_ordered_positions[] = { // ordered by for loop, asc ordered by value
            {0, 0},                                          // of final board 1;2;3;4...W
            {0, 1},
            {0, 2},
            {0, 3},
            {1, 3},
            {2, 3},
            {3, 3},
            {3, 2},
            {3, 1},
            {3, 0},
            {2, 0},
            {1, 0},
            {1, 1},
            {1, 2},
            {2, 2},
            {2, 1}
    };

    for (int i = 0; i < 15; ++i) {
        actual = board_ordered_positions[i];
        actual_v = s.board[actual.first][actual.second];
        next = board_ordered_positions[i + 1];
        next_v = s.board[next.first][next.second];

        if (actual_v != 0
            && ((actual_v != 15 && next_v != actual_v + 1)
                || (actual_v == 15 && next_v != 0))) {
            out_of_pos++;
        }
    }

    return out_of_pos;
}

int a_star::calc_manhattan_distance(int value, int i, int j) {
    const static std::pair<int, int> final_board_postions[] = { // get by value ex: value[1]: i=0;j=0;
            {2, 1},                                       // ex: value[2]: i=0;j=1;
            {0, 0},
            {0, 1},
            {0, 2},
            {0, 3},
            {1, 3},
            {2, 3},
            {3, 3},
            {3, 2},
            {3, 1},
            {3, 0},
            {2, 0},
            {1, 0},
            {1, 1},
            {1, 2},
            {2, 2}
    };

    auto correct_pos = final_board_postions[value];
    int manhattan_distance = std::abs(correct_pos.first - i)
                             + std::abs(correct_pos.second - j);
    return manhattan_distance;
}

int a_star::h3(state s) {
    int heuristc_v = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (s.board[i][j] != FINAL_BOARD_STATE[i][j]) {
                heuristc_v += calc_manhattan_distance(s.board[i][j], i, j);
            }
        }
    }

    return heuristc_v;
}

int a_star::calc_heuristic(const heuristc_t type, state s) {
    switch (type) {
        case heuristc_t::H1:
            return h1(s);
        case heuristc_t::H2:
            return h2(s);
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

void state::add_next_changing_bord_piece(int i, int j, change_t dir) const {
    state _s;
    short aux;

    _s.g = g + 1;
    _s.copy_board(board);

    switch (dir) {
        case UP:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i - 1][j];
            _s.board[i - 1][j] = aux;
            break;
        case DOWN:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i + 1][j];
            _s.board[i + 1][j] = aux;
            break;
        case LEFT:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i][j - 1];
            _s.board[i][j - 1] = aux;
            break;
        case RIGHT:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i][j + 1];
            _s.board[i][j + 1] = aux;
            break;
        default:
            break;
    }

    _s.generate_hash_key();
    t->insert({_s.hash_key, _s});
}

void state::calc_t() const {
    t->clear();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                if (i != 0) {
                    add_next_changing_bord_piece(i, j, change_t::UP);
                }
                if (i != 3) {
                    add_next_changing_bord_piece(i, j, change_t::DOWN);
                }
                if (j != 0) {
                    add_next_changing_bord_piece(i, j, change_t::LEFT);
                }
                if (j != 3) {
                    add_next_changing_bord_piece(i, j, change_t::RIGHT);
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

void state::calc_heuristic(heuristc_t type) {
    heuristic_value = a_star::calc_heuristic(type, *this);
}

std::pair<bool, int> a_star::run() {
    state v;
    std::unordered_map<ull, state>::iterator it;

    for (auto &s : S) {
        s.second.calc_heuristic(heuristc_t::H3);
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
        F.insert({v.hash_key, v});

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
                m.second.calc_heuristic(heuristc_t::H3);
                m.second.calc_f();
                A.insert(m);
            }
        }

        v = find_min_f(A);
    }

    if (v.f >= 0 && T.find(v.hash_key) != T.end()) {
        return {true, v.g};
    }

    return {false, -1};
}

a_star::~a_star() = default;

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
