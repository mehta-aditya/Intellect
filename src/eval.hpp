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

const int MG_PIECE_VALUES[6] = {83, 329, 379, 469, 1018, 20000};
const int EG_PIECE_VALUES[6] = {89, 277, 302, 498, 929, 20000};
//middle game and opening pst
const int MG_PST_VALUES[6][64] = {
    //pawn
    {
           0,   0,   0,   0,   0,   0,   0,   0,
          82, 122,  61,  83,  53, 115,  21,  -4, 
          11,  24,  41,  45,  48,  70,  40, -37, 
         -31,  -4,  -5,  18,   9,   8,   0,  -6, 
         -10,  12,  11,  -5,   0,  -8,  25, -42, 
         -41, -18,  11,   3,  16,  -5,  48, -26, 
         -33,  13,  -6,  -6,   2,  14,  54, -19, 
           0,   0,   0,   0,   0,   0,   0,   0
    },

    // knight
    {
        -183, -89, -34, -35,  74,-108, -15,-107, 
         -60, -27,  74,  19,  35,  65,  17, -17, 
         -30,  77,  54,  54,  67, 123,  89,  59, 
           8,  30,  36,  39,  24,  52,  35,  36, 
         -30, -11,  -1,  15,  20,  18,  35,   9, 
         -17, -23,  -5,  23,  35,   0,  42,  -1, 
         -34, -58,   0, -20,  11,   1,   1,  -4, 
        -105, -38, -51, -17, -34, -14,  -2, -23
    },

    // bishop
    {
         -12,  -6, -69, -22, -36, -54,   2,  -8, 
         -35,  32,  -1,  -3,  14,  44,   8, -31, 
         -33,  52,  26,  34,  24,  33,  20,  15, 
         -19, -12,  35,  55,  20,  53,  24, -17, 
          11,  30,  20,  43,  20,  -1,  21, -13, 
         -16,  -2,  29,   0,  31,  10,  35,  -6,
          21,   9,  11,  17,  19,  38,  50,  12,
         -16,  14, -31,  -6,  -2,   5, -41,  -4    
    },

    // rook
    {
          41,  30,  49,  44,  51,  -6,  40,  32, 
          21,  25,  46,  76,  70,  83,  39,  29, 
         -14,  15,  42,  53,  24,  34,  73,   2, 
         -39,   2,  -8,  12,  12,  19,   6,  -8, 
         -29, -14,   5, -18,  23,   7, -11,  -8,
         -31, -22,   0, -33,  18,  16, -15, -50,
         -60, -26, -34, -21,  14,  -6, -21, -64, 
         -18,  -4, -16,  28,  30,  22, -21, -22
    },

    // queen
    {
         -25,  -8,  15,  24,  48,  27,  43,  59,
          -8, -56,  12, -16, -26,  73,  43,  70,
         -14, -33,  -4,  -9,  22,  66,  32,  73,
         -11, -40,  -4, -31,  15,  29,  -3,  18,
         -26, -12, -13, -19, -19, -17, -14,  -7,
         -31,  19,   6,  15,   4,  19,  31, -11,
         -23, -25,  -1, -15,  21,  24,   9,   1,
          14,  -1, -16,  -5,  -2, -42, -15, -64 
    },

    // king
    {
         -65,  23,  16, -15, -56, -34,   2,  13,
          29,   9, -31,   4,   2,   0, -32, -29,
         -22,  10,   2,  -4, -10,  11,   7, -22,
         -21,  -7, -15, -38, -43, -12, -29, -48,
         -49, -10, -15, -49, -60, -30, -41, -44,
         -14, -10, -25, -38, -48, -43,  -4, -41,
         -16,  23,  -9, -79, -30, -10,  -7,  -8,
          -6,  53,  23, -37,  14, -12,  14,  22
    }
};

//endgame pst table
const int EG_PST_VALUES[6][64] = {
    // pawn
    {
           0,   0,   0,   0,   0,   0,   0,   0,
         162, 169, 142, 119, 130, 119, 153, 203,
         108,  91,  97,  84,  47,  44,  67,  67,
          15,  10,   3, -10,   9,  -8,   4,   5,
          28,   5,  14, -16, -19, -23,  17, -16,
          12,   1,  -4,  17,  -4, -18, -12,  -3,
           0,  23,  24,   1,   3,  13,   7, -21,
           0,   0,   0,   0,   0,   0,   0,   0   
    },

    // knight
    {
         -68, -38, -13, -11, -16, -40, -63, -99,
         -11,   6,  -8, -18,   6, -39, -10, -52,
          -8,  -5,  27,  -8, -13, -20, -34, -30,
         -28, -11,  39,   9,  11,  -4,  24,  -7,
         -34, -23,  27,   9,   0,  29,  21,  -6,
          -7,  12, -18,  31,  -3,  10,  -5,  -6,
         -56, -16,   7,  -9,   8, -26, -36, -37,
         -29, -56, -39, -27, -36,  -7, -36, -64
    },

    // bishop
    {
          30, -49,  32,  28, -41, -36, -56, -24, 
         -10,  19,  27, -54,  28, -55,  42,  32, 
         -14, -26,   8,  33, -37, -26, -24,  -2, 
          38, -38,  25,  27,  -7,  47,   4, -17, 
          37,  23,  14,  52, -24,  32,  20, -55, 
          30, -47,  28,  32,  10, -27, -38, -24, 
         -33,  -5,  32,  39,  -7, -17, -36,  -6, 
          24,  39, -40,  23, -17, -35,  30,   4     
    },

    // rook
    {
          30,  -7,   9,   1,  -5,  -4,  25,  -7,
          17,   6,   6,  -2,  -5,   8,   3, -14,
          -8,  21,  22,  -6, -10, -20,  12,  -9,
          -2,  19,  24, -15, -13,  -8,  -8,  17,
         -14,  22,   5, -11,  10,  -4, -25,  -3,
           8,   9,   9, -10,  10,   5, -20, -30,
           6, -18, -11,  -9,   6, -21, -25,  10,
           4, -12,  -9,   3,  -3,   0,  19, -21 
    },

    // queen
    {
          -9,  28,  34,  41,  15,   3,  10,  22,
          -4,   8,  47,  29,  70,  41,  42,   6,
         -32,  -2,  19,  33,  40,  48,   8,  24,
          17,   9,  24,  29,  67,  30,  44,  47,
         -17,  44,  31,  35,  15,  21,  25,  14,
         -29, -13,  23,   1,  14,  31,  24,   0,
         -23, -35, -21, -20, -25, -38, -23, -32,
         -19, -13, -28, -48,   6, -47, -10, -46  
    },

    // king
    {
         -74, -35, -18, -18, -11,  15,   4, -17,
         -12,  32,   0,  33,  32,  37,  10,  11,
          -7,   0,  23,  30,  36,  29,  30,  13,
         -18,  38,  10,  26,   9,  47,   9, -10,
         -35,   5,  12,   8,  10,  33,  15, -20,
         -19,  14,  -6,  14,  40,   7,  -9, -10,
         -42,   6,  -2,  28,  29,  16, -17, -28,
         -42, -18,  -6,   4, -39,   2, -37, -31 
    }
};


inline int flip_board(int square) {
    return (square^56);
}
#endif