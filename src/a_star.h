#ifndef A_STAR_H
#define A_STAR_H

#include<iostream>
#include <unordered_map>

typedef unsigned long long ull;

typedef enum {
    H1, H2, H3, H4, H5
} heuristc_t;

typedef enum {
    UP, DOWN, LEFT, RIGHT
} change_t;

typedef struct state {
    ull hash_key = -1;
    short **board{}; // 4X4
    int heuristic_value = -1;

    state *p = nullptr; // parent
    int g = -1;
    int f = -1;
    std::unordered_map<ull, struct state> *t = new std::unordered_map<ull, struct state>(); //successors

    state();

    explicit state(short **board);

    void calc_f();

    void calc_t() const;

    void calc_heuristic(heuristc_t type);

    void generate_hash_key();

    void copy_board(short **b);

    void add_next_changing_bord_piece(int i, int j, change_t dir) const;

    bool operator==(const state &rhs) const;

    bool operator!=(const state &rhs) const;
} state;

class a_star {
private:
    std::unordered_map<ull, state> A; //opened
    std::unordered_map<ull, state> F; //closed
    std::unordered_map<ull, state> S; //initial
    std::unordered_map<ull, state> T; //final

    static state find_min_f(std::unordered_map<ull, state> &map);

    static int calc_manhattan_distance(int value, int i, int j);

    static int h1(state s);

    static int h2(state s);

    static int h3(state s);

    static int h4(state s);

    static int h5(state s);

public:
    a_star(std::unordered_map<ull, state> a, std::unordered_map<ull, state> f,
           std::unordered_map<ull, state> s, std::unordered_map<ull, state> t);

    ~a_star();

    std::pair<bool, int> run();

    static int calc_heuristic(heuristc_t type, state s);
};

#endif //A_STAR_H
