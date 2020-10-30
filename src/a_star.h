#ifndef A_STAR_H
#define A_STAR_H

#include<iostream>
#include <unordered_map>
#include <map>
#include <set>

typedef enum {
    H1, H2, H3, H4, H5
} heuristc_t;

typedef enum {
    UP, DOWN, LEFT, RIGHT
} change_t;

typedef struct state {
    std::string hash_key;
    short board[4][4]; // 4X4
    int heuristic_value = -1;

    std::multiset<state>::iterator it;

    int g = -1;
    int f = -1;

    change_t from_change;

    state *p = nullptr;

    state();

    explicit state(short board[4][4]);

    virtual ~state();

    void calc_f();

    std::unordered_map<std::string, state> generate_seccessors();

    void calc_heuristic(heuristc_t type);

    void generate_hash_key();

    void copy_board(short b[4][4]);

    state generate_next_seccessor(int i, int j, change_t dir);

    bool operator==(const state &rhs) const;

    bool operator!=(const state &rhs) const;

    bool operator<(const state &rhs) const;

    bool operator>(const state &rhs) const;

    bool operator<=(const state &rhs) const;

    bool operator>=(const state &rhs) const;
} state;

class a_star {
private:
    std::unordered_map<std::string, state> A; //opened
    std::unordered_map<std::string, state> F; //closed
    std::unordered_map<std::string, state> S; //initial
    std::unordered_map<std::string, state> T; //final

    static int calc_manhattan_distance(int value, int i, int j);

    static int h1(const state &s);

    static int h2(const state &s);

    static int h3(const state &s);

    static int h4(const state &s);

    static int h5(const state &s);

public:
    a_star(std::unordered_map<std::string, state> a, std::unordered_map<std::string, state> f,
           std::unordered_map<std::string, state> s, std::unordered_map<std::string, state> t);

    ~a_star();

    std::pair<bool, int> run();

    static int calc_heuristic(heuristc_t type, state s);

    const static heuristc_t h_t = heuristc_t::H3;
};

#endif //A_STAR_H
