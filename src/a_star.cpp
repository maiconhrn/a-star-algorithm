#include <cmath>
#include <set>
#include <utility>
#include <algorithm>
#include <string>
#include "a_star.h"
#include "15_puzzle.h"

a_star::a_star(std::unordered_map<std::string, state> a, std::unordered_map<std::string, state> f,
               std::unordered_map<std::string, state> s, std::unordered_map<std::string, state> t)
        : A(std::move(a)),
          F(std::move(f)),
          S(std::move(s)),
          T(std::move(t)) {}

int a_star::h1(const state &s) {
    int out_of_pos = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (s.board[i][j] != 0
                && s.board[i][j] != FINAL_BOARD_STATE[i][j]) {
                out_of_pos++;
            }
        }
    }

    return out_of_pos;
}

int a_star::h2(const state &s) {
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
    int manhattan_distance = abs(j - correct_pos.second)
                             + abs(correct_pos.first - i);
    return manhattan_distance;
}

int a_star::h3(const state &s) {
    int heuristc_v = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (s.board[i][j] != 0
                && s.board[i][j] != FINAL_BOARD_STATE[i][j]) {
                heuristc_v += calc_manhattan_distance(s.board[i][j], i, j);
            }
        }
    }

    return heuristc_v;
}

int a_star::h4(const state &s) {
    return (int) (0.08 * h1(s) + 0.02 * h2(s) + 0.9 * h3(s));
};

int a_star::h5(const state &s) {
    return std::max({h1(s), h2(s), h3(s)});
};

int a_star::calc_heuristic(const heuristc_t type, state s) {
    switch (type) {
        case heuristc_t::H1:
            return h1(s);
        case heuristc_t::H2:
            return h2(s);
        case heuristc_t::H3:
            return h3(s);
        case heuristc_t::H4:
            return h4(s);
        case heuristc_t::H5:
            return h5(s);
        default:
            return 0;
    }
}

state::state() = default;

state::state(short board[4][4]) {
    copy_board(board);
    generate_hash_key();
}

void state::calc_f() {
    f = g + heuristic_value;
}

state state::generate_next_seccessor(int i, int j, change_t dir) {
    state _s;
    short aux;

    _s.copy_board(board);

    switch (dir) {
        case UP:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i - 1][j];
            _s.board[i - 1][j] = aux;
            _s.from_change = change_t::UP;
            break;
        case DOWN:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i + 1][j];
            _s.board[i + 1][j] = aux;
            _s.from_change = change_t::DOWN;
            break;
        case LEFT:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i][j - 1];
            _s.board[i][j - 1] = aux;
            _s.from_change = change_t::LEFT;
            break;
        case RIGHT:
            aux = _s.board[i][j];
            _s.board[i][j] = _s.board[i][j + 1];
            _s.board[i][j + 1] = aux;
            _s.from_change = change_t::RIGHT;
            break;
        default:
            break;
    }

    _s.generate_hash_key();
    if (!_s.p) {
        _s.p = new state;
    }
    *(_s.p) = *this;
    _s.g = g + 1;
    _s.calc_heuristic(a_star::h_t);
    _s.calc_f();

    return _s;
}

std::unordered_map<std::string, state> state::generate_seccessors() {
    state s;
    auto successors = std::unordered_map<std::string, state>();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                if (i != 0 && (p == nullptr || from_change != change_t::DOWN)) {
                    s = generate_next_seccessor(i, j, change_t::UP);
                    successors.insert({s.hash_key, s});
                }

                if (i != 3 && (p == nullptr || from_change != change_t::UP)) {
                    s = generate_next_seccessor(i, j, change_t::DOWN);
                    successors.insert({s.hash_key, s});
                }

                if (j != 0 && (p == nullptr || from_change != change_t::RIGHT)) {
                    s = generate_next_seccessor(i, j, change_t::LEFT);
                    successors.insert({s.hash_key, s});
                }

                if (j != 3 && (p == nullptr || from_change != change_t::LEFT)) {
                    s = generate_next_seccessor(i, j, change_t::RIGHT);
                    successors.insert({s.hash_key, s});
                }

                return successors;
            }
        }
    }

    return {};
}

void state::generate_hash_key() {
    hash_key = "";
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            hash_key += std::to_string(board[i][j]);
        }
    }
}

bool state::operator==(const state &rhs) const {
    return hash_key == rhs.hash_key;
}

bool state::operator!=(const state &rhs) const {
    return !(rhs == *this);
}

bool state::operator<(const state &rhs) const {
    return f == rhs.f
           ? heuristic_value < rhs.heuristic_value
           : f < rhs.f;
}

bool state::operator>(const state &rhs) const {
    return rhs < *this;
}

bool state::operator<=(const state &rhs) const {
    return !(rhs < *this);
}

bool state::operator>=(const state &rhs) const {
    return !(*this < rhs);
}

void state::copy_board(short b[4][4]) {
    for (int i = 0; i < 4; ++i) {
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
    std::unordered_map<std::string, state>::iterator it;
    std::multiset<state>::iterator it1;
    auto A_aux = std::multiset<state>();
    auto successors = std::unordered_map<std::string, state>();

    for (auto &s : S) {
        s.second.calc_heuristic(heuristc_t::H3);
        s.second.g = 0;
        s.second.calc_f();
    }

    A.clear();
    A.insert(S.begin(), S.end());
    A_aux.clear();
    A_aux.insert(A.begin()->second);
    F.clear();

    while (!(v = *(it1 = A_aux.begin())).hash_key.empty()
           && T.find(v.hash_key) == T.end()) {
        v.it = it1;
        A_aux.erase(it1);
        A.erase(v.hash_key);
        F[v.hash_key] = v;

        successors = v.generate_seccessors();
        for (auto &m : successors) {
            if ((it = A.find(m.first)) != A.end()
                && m.second.g < it->second.g) {
                A_aux.erase(it->second.it);
                A.erase(it);
            }

            if ((it = F.find(m.first)) != F.end()
                && m.second.g < it->second.g) {
                F.erase(it);
            }

            if (A.find(m.first) == A.end()
                && F.find(m.first) == F.end()) {
                it1 = A_aux.insert(m.second);
                m.second.it = it1;
                A.insert(m);
            }
        }

//        std::cout << A.size() << std::endl;

        successors.clear();
    }

    if (T.find(v.hash_key) != T.end()) {
        return {true, v.g};
    }

    return {false, -1};
}

a_star::~a_star() {
    A.clear();
    F.clear();
    S.clear();
    T.clear();
}

state::~state() = default;
