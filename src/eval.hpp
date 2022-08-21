#include "engine.hpp"

#ifndef EVAL_HPP
#define EVAL_HPP

const int PIECE_VALUES[6] = {100, 320, 330, 500, 900, 20000};
const int PHASE_VALUES[6] = {0, 1, 1, 2, 4, 0};
const int MAX_PHASE = 24;

//Mobility
const int MG_MOBILITY[6] = {0, 1, 3, 5, 1, 0};
const int EG_MOBILITY[6] = {0, 2, 2, 2, 8, 4};
const int MAX_PIECE_BITS[6] = {4, 8, 13, 14, 27, 8};

const int PASSED_VALUE[8] = {0, 5, 10, 20, 30, 50, 80, 0};

extern int MG_PIECE_VALUES[6];
extern int EG_PIECE_VALUES[6];
extern int MG_PST_VALUES[6][64];
extern int EG_PST_VALUES[6][64];


inline int flip_board(int square) {
    return (square^56);
}
#endif