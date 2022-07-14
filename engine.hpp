#include "board.hpp"
#include <cstring>
#ifndef ENGINE_HPP
#define ENGINE_HPP

const int MAX_DEPTH = 32;
enum V {
    MAX = 20000,
    MATE_V = 10000,
    DRAW_V = 0
};

class Engine{
    public: 
        int pv_len[MAX_DEPTH];
        Moves pv[MAX_DEPTH][MAX_DEPTH];
        //engine.cpp
        inline void update_pv(Moves &move, int ply);
        int search(Board &board, int alpha, int beta, int depth, int ply);
        string root(Board &board, int depth);
        
        //eval.cpp
        int evaluation(Board &board);
};


#endif