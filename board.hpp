#include <iostream>
#include <time.h> 
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <chrono>
#include <cstring>
#include <algorithm>

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

#ifndef BOARD_HPP
#define BOARD_HPP

enum I {
  PAWN_I = 0, KNIGHT_I = 1, BISHOP_I = 2, ROOK_I = 3, QUEEN_I = 4, KING_I = 5, 
  WHITE = 0, BLACK = 1,
  KINGSIDE_I = 0, QUEENSIDE_I = 1,
  NO_SQ = -1, NO_PIECE = -1
};
//flags for moves structure
enum FLAGS {
  NO_FLAG = -1, CAPTURE_F = 0, KS_F = 1, QS_F = 2, PROMOTE_F = 3, PROMOTE_CAP_F = 4, EN_PASSANT_F = 5, DOUBLE_PAWN_F = 6
};

extern U64 SQUARES_BB[64];
extern U64 CASTLING_BB[2][2];
const U64 EMPTY_BB = 0ULL;
const U64 ALL_BB = 0xFFFFFFFFFFFFFFFFULL;

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

const string UNICODE_PIECES[2][6] = {{"\u2654", "\u2655", "\u2656", "\u2657", "\u2658", "\u2659"}, {"\u265A", "\u265B", "\u265C", "\u265D", "\u265E", "\u265F"}};
const int MOVE_LIST_RESERVE = 218;

//Structure for a move
struct Moves {
  int from_square = NO_SQ, to_square = NO_SQ, flag = NO_FLAG, promoted = NO_PIECE, piece = NO_PIECE;
  int order; //used for move ordering
  int captured = NO_PIECE; //captured piece
  Moves(int from = NO_SQ, int to = NO_SQ, int p = NO_PIECE, int f = NO_FLAG, int pro = PAWN_I) {
    from_square = from;
    to_square = to;
    piece = p;
    flag = f;
    promoted = pro;
  }
};
//Structure for a piece
struct Piece {
  int color, type;
  Piece(int col = WHITE, int i = NO_PIECE) {
    color = col;
    type = i;
  }
};
//Attributes for the position
struct Position{
  Moves move;
  bool castling_rights[2][2];
  int ep_square;
  Position(Moves m, bool cr[2][2], int ep) {
    move = m;
    castling_rights[WHITE][KINGSIDE_I] = cr[WHITE][KINGSIDE_I];
    castling_rights[WHITE][QUEENSIDE_I] = cr[WHITE][QUEENSIDE_I];
    castling_rights[BLACK][KINGSIDE_I] = cr[BLACK][KINGSIDE_I];
    castling_rights[BLACK][QUEENSIDE_I] = cr[BLACK][QUEENSIDE_I];
    ep_square = ep;
  }
};

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

//Renders a bitboard (a tool)
void bb_rendering(U64 bitboard);

//The chess board
class Board {
  public:
    stack<Position> position_history;
    int turn = WHITE;
    bool castling_rights[2][2]; //[color][types] .//[0:White or 1: Black][0:Kingside or 1:Queenside]
    int ep_square;
    //No Piece, WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK
    U64 piece_boards[2][6] = {EMPTY_BB};
    U64 piece_co[2] = {EMPTY_BB};
    //board.cpp
    void set_fen(string fen_set);
    void render();
    void reset();

    //movegen.cpp
    bool is_square_attacked(int move, int color);
    void generate_castling_moves(vector<Moves>& move_list);
    void generate_piece_quiets(vector<Moves>& move_list);
    void generate_piece_captures(vector<Moves>& move_list);
    vector<Moves> generate_psuedolegal_moves();

    //move.cpp
    inline int piece_at(int square, int color);
    inline void add_piece(int square, int color, int type);
    inline void remove_piece(int square, int color, int type);
    inline void move_piece(int from, int to, int color, int type);
    void push(Moves move);
    void pop();
    bool in_check(int color) { 
      int square = get_lsb(piece_boards[color][KING_I]);
      if (square < 64) {return is_square_attacked(square, color);}
      else {return true;}
    }    
};
                    


//Turns a move into a uci string
inline string to_uci(Moves &move) {
  string uci_str = SQUARES[move.from_square] + SQUARES[move.to_square];
  if (move.promoted > PAWN_I) {
    uci_str += "=" + FEN_PIECE_STRINGS[BLACK][move.promoted];
  }
  return uci_str;
}
#endif
