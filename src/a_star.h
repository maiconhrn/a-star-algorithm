#ifndef A_STAR_H
#define A_STAR_H

#include<iostream>
#include <unordered_map>

typedef unsigned long long ull;

typedef struct state {
    ull hash_key = -1;
    short **board; // 4X4
    int heuristic_value = -1;

    state *p = nullptr; // parent
    int g = -1;
    int f = -1;
    std::unordered_map<ull, struct state> *t = new std::unordered_map<ull, struct state>(); //successors

    state();

    explicit state(short **board);

    void calc_f();

    void calc_t() const;

    void generate_hash_key();

    void copy_board(short **b);

    bool operator==(const state &rhs) const;

    bool operator!=(const state &rhs) const;
} state;

typedef enum {
    H1, H2, H3, H4, H5
} heuristc_t;

class a_star {
private:
    std::unordered_map<ull, state> A; //opened
    std::unordered_map<ull, state> F; //closed
    std::unordered_map<ull, state> S; //initial
    std::unordered_map<ull, state> T; //final

    state find_min_f(std::unordered_map<ull, state> &map);

    static int h1(state s);

    static int h2(state s);

    static int h3(state s);

    static int h4(state s);

    static int h5(state s);

    static int calc_heuristic(heuristc_t type, state s);

public:
    a_star(const std::unordered_map<ull, state> &a, const std::unordered_map<ull, state> &f,
           const std::unordered_map<ull, state> &s, const std::unordered_map<ull, state> &t);

    ~a_star();

    std::pair<bool, int> run();
};

#endif //A_STAR_H
