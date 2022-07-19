#include "board.hpp"
#include <algorithm>
#include <cstring>
#ifndef ENGINE_HPP
#define ENGINE_HPP

const int MAX_DEPTH = 32;
enum VALUES {
    MAX = 20000,
    MATE_V = 10000,
    DRAW_V = 0
};

//Use this structure to sort
struct move_sort {
    inline bool operator() (const Moves& move1, const Moves& move2) const {
        return (move1.order > move2.order);
    }
};

class Engine{
    public: 
        //engine.cpp
        //used for triangular pv table
        int pv_len[MAX_DEPTH];
        Moves pv[MAX_DEPTH][MAX_DEPTH];
        //used for killer heuristic
        Moves killers[2] = {Moves()};
        
        inline void update_pv(Moves &move, int ply);
        int search(Board &board, int alpha, int beta, int depth, int ply);
        string root(Board &board, int depth);
        
        //eval.cpp
        int evaluation(Board &board);
        void move_ordering(vector<Moves> &moves);

        //sort.cpp
        void score_moves(vector<Moves> &moves);
};


#endif