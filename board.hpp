#include <iostream>
#include <time.h> 
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using U64 = uint64_t;

//Common bit formulas
#define SET_BIT(board,square) ((board) |= (1ULL<<(square)))
#define CLEAR_BIT(board,square) ((board) &= ~(1ULL<<(square)))
#define FLIP_BIT(board,square) ((board) ^= (1ULL<<(square)))
#define CHECK_BIT(board,square) (!!((board) & (1ULL<<(square)))) 

#define BITMASK_SET(board, mask) ((board) |= (mask))
#define BITMASK_CLEAR(board, mask) ((board) &= (~(mask)))
#define BITMASK_INTERSECT(board, mask) ((board) &= (mask))
#define BITMASK_FLIP(board, mask) ((board) ^= (mask))

#ifndef Board_HPP
#define Board_HPP

enum I {
  PAWN_I = 0, KNIGHT_I = 1, BISHOP_I = 2, ROOK_I = 3, QUEEN_I = 4, KING_I = 5, 
  WHITE = 0, BLACK = 1,
  KINGSIDE_I = 0, QUEENSIDE_I = 1,
  NO_SQ = -1
};

const U64 EMPTY_BB = 0ULL;
const U64 ALL_BB = 0xFFFFFFFFFFFFFFFFULL;
const U64 RANK_5 = 0x000000FF00000000ULL, RANK_4 = 0x00000000FF000000ULL;
const U64 RANK_8 = 0xFF00000000000000ULL,RANK_1 = 0x00000000000000FFULL;
const U64 FILE_A = 0x8080808080808080ULL, FILE_H = 0x0101010101010101ULL;
const U64 CORNERS = 0x8100000000000081ULL;
const U64 EDGES = RANK_1 | RANK_8 | FILE_A | FILE_H;
const U64 CASTLING_BB[2][2] = {{(1ULL<<(64-3) | 1ULL<<(64-2)), (1ULL<<(64-7) | 1ULL<<(64-6) | 1ULL<<(64-5)) }, 
      {(1ULL<<(3) | 1ULL<<(2)), (1ULL<<(7) | 1ULL<<(6) | 1ULL<<(5)) }};
const U64 CASTLING_SQUARES[2][2][3] = {{{60, 61, 62}, {60, 59, 58}}, {{4, 5, 6}, {4, 3, 2}}};


//Useful arrays
const string FEN_PIECE_STRINGS[2][6] = {{"P", "N", "B", "R", "Q", "K"}, {"p", "n", "b", "r", "q", "k"}};
const string SQUARES[64] = {"a8", "b8", "c8","d8","e8","f8","g8","h8",
                      "a7", "b7", "c7","d7","e7","f7","g7","h7",
                      "a6", "b6", "c6","d6","e6","f6","g6","h6",
                      "a5", "b5", "c5","d5","e5","f5","g5","h5",
                      "a4", "b4", "c4","d4","e4","f4","g4","h4",
                      "a3", "b3", "c3","d3","e3","f3","g3","h3",
                      "a2", "b2", "c2","d2","e2","f2","g2","h2",
                      "a1", "b1", "c1","d1","e1","f1","g1","h1"};

//Structure for a move
struct Moves {
  //0: No Flag 1:Castling 2: En Passant 3: Promotion
  int from_square = NO_SQ, to_square = NO_SQ, captured = 0, flag = 0, promoted = 0;
  Moves(int from = NO_SQ, int to = NO_SQ, int cap = -1, int f = 0, int pro = PAWN_I) {
    from_square = from;
    to_square = to;
    captured = cap;
    flag = f;
    promoted = pro;
  }
};

//The chess board
class Board {
  public:
    int turn = WHITE;
    bool castling_rights[2][2]; //[color][types] .//[0:White or 1: Black][0:Kingside or 1:Queenside]
    int ep_square;
    //No Piece, WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK
    U64 piece_boards[2][6] = {EMPTY_BB};
    U64 piece_co[2] = {EMPTY_BB};
    void set_fen(string fen_set);
    bool is_square_attacked(int move);

    vector<Moves> generate_castling_moves();
    vector<Moves> generate_piece_quiets();
    vector<Moves> generate_piece_captures();

};
                   
//Counts bits
inline int count_bits(U64 bitboard) {
  int count = 0;
  while (bitboard) {
    count++;
    bitboard &= bitboard - 1;
  }
  return count;
}
//Gets index of least lsb
inline int get_lsb(U64 bb) {
    int i = __builtin_ctzll(bb);
    return i;
}


//Gets index of least lsb and turns it to 0
inline int pop_lsb(U64* bb) {
    int i = __builtin_ctzll(*bb);
    *bb &= *bb - 1;
    return i;
}

//Renders a bitbaord
void basic_rendering(U64 bitboard);



//Turns a move into a uci string
inline string to_uci(Moves &move) {
  string uci_str = SQUARES[move.from_square] + SQUARES[move.to_square];
  return uci_str;
}

#endif
