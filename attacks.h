#include "board.h"

//Get blockers for diag and line tables
inline U64 get_blockers(int index, int bits, U64 mask) {
  U64 blockers = EMPTY_BB;
  for (int i = 0; i < bits; i++) {
    int square = pop_lsb(mask);
    if (index & (1 << i)) {
      SET_BIT(blockers, square);
    }
  }
  return blockers;
}   

//Used to cut off edges of the board which are not needed in diag or line masks
inline U64 get_edges(int square)  {
  int square_rank = RANKS_BB[(int)(square & 7)];
  int square_file = FILES_BB[(int)(square >> 3)];
  
  return ((FILE_A & !square_file) | (FILE_H & !square_file) \
    | (RANK_1 & !square_rank) | (RANK_8 & !square_rank));
}

//Checks which squares can be attacked. 
inline U64 sliding_attacks(int square, U64 occupied, vector<int> deltas) {
  U64 attack_bb = EMPTY_BB;
  for (auto delta : deltas) {
    int square_to = square;
    
    while (true) {
      square_to += delta;
      if (((square_to < 0) || (square_to >= 64)) || (board_distance(square_to, square_to-delta) > 2)) {
        break;
      }
      attack_bb |= SQUARES_BB[square_to];
      if (occupied & (SQUARES_BB[square_to])) {
        break;
      }
    }
  }
  return attack_bb;
}

//Initialize stepper pieces (K, N, P) attacks
void init_step_tables() {
  for (int square = 0; square < 64; square++) {
    KNIGHT_ATTACKS[square] = sliding_attacks(square, ALL_BB, KNIGHT_DELTAS);
    KING_ATTACKS[square] = sliding_attacks(square, ALL_BB, KING_DELTAS);
    PAWN_ATTACKS[WHITE][square] = sliding_attacks(square, ALL_BB, {-9, -7});
    PAWN_ATTACKS[BLACK][square] = sliding_attacks(square, ALL_BB, {9, 7});
  }
}

//Generate diag masks and attack table
void init_diag_tables() {
  for (int square = 0; square < 64; square++) {
    //Masks
    DIAG_MASKS[square] = sliding_attacks(square, EMPTY_BB, DIAG_DELTAS);
    BITMASK_CLEAR(DIAG_MASKS[square], EDGES);
    //Attack Table
    U64 blocker_bits = count_bits(DIAG_MASKS[square]);
    for (int i = 0; i < (1 << blocker_bits); i++) {
      U64 blockers = get_blockers(i, blocker_bits, DIAG_MASKS[square]);
      int magic_index = (blockers * diag_magics[square]) >> (64 - diag_relevant_bits[square]);
      DIAG_ATTACKS[square][magic_index] = sliding_attacks(square, blockers, DIAG_DELTAS);
    }
  }
}

//Generate line masks and attack table
void init_line_tables() {
  for (int square = 0; square < 64; square++) {
    //Masks
    U64 blocked_edges = get_edges(square);
    LINE_MASKS[square] = sliding_attacks(square, EMPTY_BB, LINE_DELTAS);
    BITMASK_CLEAR(LINE_MASKS[square], blocked_edges);
    BITMASK_CLEAR(LINE_MASKS[square], CORNERS);
    //Attack Table
    U64 blocker_bits = count_bits(LINE_MASKS[square]);
    for (int i = 0; i < (1 << blocker_bits); i++) {
      U64 blockers = get_blockers(i, blocker_bits, LINE_MASKS[square]);
      int magic_index = (blockers * line_magics[square]) >> (64 - line_relevant_bits[square]);
      LINE_ATTACKS[square][magic_index] = sliding_attacks(square, blockers, LINE_DELTAS);
    }
  }
}

inline U64 get_diag_attacks(int square, U64 blockers) {
  blockers &= DIAG_MASKS[square];
  blockers *= diag_magics[square];
  blockers >>= 64 - diag_relevant_bits[square];
  return DIAG_ATTACKS[square][blockers];
}

inline U64 get_line_attacks(int square, U64 blockers) {
  blockers &= LINE_MASKS[square];
  blockers *= line_magics[square];
  blockers >>= 64 - line_relevant_bits[square];
  
  return LINE_ATTACKS[square][blockers];
}

inline U64 get_queen_attacks(int square, U64 blockers) {
  return get_diag_attacks(square, blockers) | get_line_attacks(square, blockers);
}


