
#ifndef EVAL_HPP
#define EVAL_HPP
constexpr int PIECE_VALUES[6] = {100, 300, 300, 500, 900, 20000};
constexpr int PHASE_VALUES[6] = {0, 1, 1, 2, 5, 0};
constexpr int MAX_PHASE = 26;

//Mobility
constexpr int MG_MOBILITY[6] = {0, 4, 4, 2, 1, 0};
constexpr int EG_MOBILITY[6] = {0, 2, 2, 2, 2, 0};
constexpr int MOBILITY[6] = {0, 4, 4, 2, 1, 0};
constexpr int MAX_PIECE_BITS[6] = {4, 8, 13, 14, 27, 8};

constexpr int PASSED_VALUE[8] = {0, 38, 26, 20, 15, 5, 2, 0, }; //From white point of view
constexpr int EG_PASSED_BONUS = 5;

constexpr int MG_PIECE_VALUES[6] = {96, 312, 330, 530, 999, 20000, };
constexpr int EG_PIECE_VALUES[6] = {105, 308, 335, 540, 992, 20000, };


//middle game and opening pst
constexpr int MG_PST_VALUES[6][64] = 
{
{
0, 0, 0, 0, 0, 0, 0, 0,
78, 114, 41, 75, 60, 118, 14, -19,
-26, -9, 12, 11, 45, 76, 5, -26, 
-24, 3, -6, 1, 9, -8, -3, -15,
-47, -6, -19, 0, 5, -4, -10, -45,
-38, -24, -2, -18, 3, -17, 25, -8,
-31, 3, -30, -27, -11, 30, 40, -6,
0, 0, 0, 0, 0, 0, 0, 0, 
},
{
-187, -97, -54, -41, 81, -77, -23, -87,
-65, -21, 88, 50, 43, 82, -13, 1,
-39, 52, 49, 75, 104, 149, 81, 52,
5, 23, 21, 63, 39, 57, 10, 42,
7, 18, 36, 33, 48, 11, 29, -16, 
-27, -9, 28, 24, 9, 27, 19, -22,
-47, -45, 8, 5, 17, 26, -6, 1,
-97, -15, -70, -21, -27, -26, -19, -21,
},
{
-9, -16, -62, -57, -33, -24, 15, 12,
-18, 36, -2, 7, 10, 79, 26, -27,
-16, 17, 63, 48, 43, 70, 57, 6,
4, 1, 27, 58, 53, 23, -1, 6, 
-26, 21, 15, 34, 42, 14, 18, -16,
20, 29, 27, 27, 22, 41, 20, 18,
-4, 35, 8, 6, 27, 41, 53, 9,
-13, 5, 6, -1, -3, 2, -19, -17,
},
{
36, 44, 24, 59, 71, 23, 11, 39,
7, 12, 38, 52, 60, 87, 32, 36, 
15, 11, 12, 40, 37, 53, 69, 36,
-32, -9, -13, 38, 4, 55, -28, -12,
-52, -18, -20, -21, 7, -15, -14, -19,
-37, -41, -4, -11, -17, 10, 11, -13,
-24, -36, -14, -19, -21, 7, 14, -89,
-11, -5, 3, 15, 22, 15, -55, -12, 
},
{
-20, 20, 37, 32, 39, 64, 51, 65,
-18, -31, -15, -19, -24, 77, 36, 74,
1, -37, 15, 16, 37, 64, 27, 47,
-7, -35, -8, -22, 7, 3, 18, -9,
-9, -26, -23, -18, -4, -24, 19, -23,
-34, -2, -7, -16, -17, 2, 6, -3, 
-15, 10, 5, 4, 18, 23, 5, 21, 
-9, -6, 3, 18, -23, -17, -51, -30,
},
{
-85, 31, 8, 5, -76, -14, 22, 5, 
9, 19, 0, -15, -4, 16, -30, -21,
-1, 32, -6, 0, -40, 14, 42, -2,
-23, 0, -32, -25, -50, -17, 2, -26, 
-29, 19, -11, -47, -60, -64, -41, -43,
-6, -6, -32, -54, -64, -26, -13, -7,
-19, -13, -16, -84, -63, -6, 7, 18, 
-27, 44, 32, -74, 10, -42, 44, 18,
},
};

//endgame pst table
constexpr int EG_PST_VALUES[6][64] =
{
{
0, 0, 0, 0, 0, 0, 0, 0,
166, 165, 156, 118, 127, 134, 171, 169,
114, 114, 85, 67, 52, 45, 70, 102,
42, 32, 17, -3, -8, 6, 25, 15, 
17, 11, -3, -15, -15, -10, 3, -3,
10, 25, -18, 7, -8, 3, -5, -6,
25, 20, 16, 18, 17, 6, 2, -3,
0, 0, 0, 0, 0, 0, 0, 0, 
},
{
-38, -34, -21, -20, -37, -19, -43, -79,
-17, -2, -33, -22, 11, -17, -16, -42,
-32, -28, 18, 1, 3, -15, -11, -21,
3, 21, 28, 30, 34, 19, 16, -22,
-10, 14, 16, 33, 30, 11, 12, -26,
-43, 5, -9, 27, 30, -3, -12, -2,
-44, -40, -4, 3, 2, 0, -3, -36,
-49, -31, -3, 5, -18, -26, -34, -44, 
},
{
6, -41, -13, 0, -15, 1, 3, -4,
-28, -4, -1, -2, 3, 5, 4, 6,
4, 0, -14, 1, 14, 14, -12, 12,
5, 7, 20, 17, 18, -10, 5, -4,
-14, -1, 19, 27, 15, 24, -11, -29, 
-4, 17, 6, 30, 31, 19, 9, -7,
6, 2, -13, 19, 8, 11, 5, -19,
-5, -21, -3, 3, 1, -12, -1, -25,
},
{
33, 28, 8, 11, 20, 20, 6, 15,
7, 21, 19, 19, 17, 17, 28, 11, 
17, 27, 25, 21, 4, -7, -1, 5,
24, 23, 21, 5, 10, 19, 7, 12,
23, 25, 0, 12, 11, 10, -4, -15,
14, 8, 3, 5, 13, 8, -28, 4, 
14, 2, 20, 10, -5, -9, 1, -7,
11, 6, 13, 7, -11, 5, 4, -12,
},
{
11, 30, 30, 47, 7, 39, 26, 40,
3, 38, 40, 49, 50, 45, 50, -8,
-26, -2, 17, 41, 55, 55, 27, -11, 
9, 14, 4, 55, 37, 46, 63, 28,
-14, 32, 27, 37, 39, 18, 59, 31,
-28, -23, 35, 14, 17, 25, 30, -3,
-2, -3, -26, -8, -8, -15, -28, -12, 
-13, -8, -2, -23, 15, -40, 0, -33,
},
{
-94, -27, -24, -18, -31, 35, 16, -25,
8, 27, 6, 9, 25, 30, 31, 19,
-10, -1, 31, 3, 16, 37, 52, 33, 
-28, 14, 10, 33, 6, 31, 26, 23,
-26, -24, 13, 34, 23, 31, 15, 7,
-7, -9, 5, 21, 35, 28, 5, -3,
-45, -1, 10, 23, 22, 12, 3, -11, 
-35, -44, -23, -15, -22, -18, -38, -55,
},
};

constexpr int flip_board[2][64] = {
{
		0, 1, 2, 3, 4, 5, 6, 7,
		8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 39,
		40, 41, 42, 43, 44, 45, 46, 47,
		48, 49, 50, 51, 52, 53, 54, 55,
		56, 57, 58, 59, 60, 61, 62, 63
},
{
  56, 57, 58, 59, 60, 61, 62, 63,
  48, 49, 50, 51, 52, 53, 54, 55,
  40, 41, 42, 43, 44, 45, 46, 47,
  32, 33, 34, 35, 36, 37, 38, 39,
  24, 25, 26, 27, 28, 29, 30, 31,
  16, 17, 18, 19, 20, 21, 22, 23,
  8, 9, 10, 11, 12, 13, 14, 15,
  0, 1, 2, 3, 4, 5, 6, 7
}
};




#endif



