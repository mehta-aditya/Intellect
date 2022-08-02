#include "attacks.hpp"


const U64 CASTLING_BB[2][2] = {{(1ULL<<(64-3) | 1ULL<<(64-2)), (1ULL<<(64-7) | 1ULL<<(64-6) | 1ULL<<(64-5)) }, 
      {(1ULL<<(3) | 1ULL<<(2)), (1ULL<<(7) | 1ULL<<(6) | 1ULL<<(5)) }};
const U64 CASTLING_SQUARES[2][2][3] = {{{60, 61, 62}, {60, 59, 58}}, {{4, 5, 6}, {4, 3, 2}}};
//Check if the square is attacked (useful for checking for checks checks)
bool Board::is_square_attacked(int square, int color) {
  U64 attack_board;
  int opp_col = color^1;
  U64 both_blockers = piece_co[color] | piece_co[opp_col];

  //pawn attacks
  attack_board = Attacks::PAWN_ATTACKS[opp_col][square];
  if (attack_board & piece_boards[opp_col][PAWN_I]) {
    return true;
  }
  //knight attacks
  attack_board = Attacks::KNIGHT_ATTACKS[square];
  if (attack_board & piece_boards[opp_col][KNIGHT_I]) {
    return true;
  }
  //king attacks
  attack_board = Attacks::KING_ATTACKS[square];
  if (attack_board & piece_boards[opp_col][KING_I]) {
    return true;
  }
  //diag attacks
  attack_board = Attacks::get_diag_attacks(square, both_blockers);
  if (attack_board & (piece_boards[opp_col][BISHOP_I] | piece_boards[opp_col][QUEEN_I])) {
    return true;
  }
  //line attacks
  attack_board = Attacks::get_line_attacks(square, both_blockers);
  if (attack_board & (piece_boards[opp_col][ROOK_I] | piece_boards[opp_col][QUEEN_I])) {
    return true;
  }
  return false;
}

void Board::generate_castling_moves(vector<Moves>& move_list){
  //If the turn is white
  if (castling_rights[turn][KINGSIDE_I] == true) {
    //Check if pieces in between are clear and castling is allowed
    if (!(CASTLING_BB[turn][KINGSIDE_I] & piece_co[turn])) {
      if (!is_square_attacked(CASTLING_SQUARES[turn][KINGSIDE_I][0], turn) && !is_square_attacked(CASTLING_SQUARES[turn][KINGSIDE_I][1], turn) && !is_square_attacked(CASTLING_SQUARES[turn][KINGSIDE_I][2], turn)) {
        move_list.push_back(Moves(CASTLING_SQUARES[turn][KINGSIDE_I][0], CASTLING_SQUARES[turn][KINGSIDE_I][2], KING_I, KS_F));
      }
    }
  }
  if (castling_rights[turn][QUEENSIDE_I] == true) {
    //Check if pieces in between are clear and castling is allowed
    if (!(CASTLING_BB[turn][QUEENSIDE_I] & piece_co[turn])) {
      if (!is_square_attacked(CASTLING_SQUARES[turn][QUEENSIDE_I][0], turn) && !is_square_attacked(CASTLING_SQUARES[turn][QUEENSIDE_I][1], turn) && !is_square_attacked(CASTLING_SQUARES[turn][KINGSIDE_I][2], turn)) {
        move_list.push_back(Moves(CASTLING_SQUARES[turn][QUEENSIDE_I][0], CASTLING_SQUARES[turn][QUEENSIDE_I][2], KING_I, QS_F));
      }
    }
  }
}
void Board::generate_piece_quiets(vector<Moves>& move_list){
  int from_square, to_square;
  int pawn_shift;
  U64 piece_board, attack_board, double_attack_board;
  U64 blockers = piece_co[turn] | piece_co[(turn^1)];
  turn == WHITE ? pawn_shift = 8 : pawn_shift = -8;
  
  //pawn moves
  piece_board = piece_boards[turn][PAWN_I];
  //white pawn moves
  if (turn == WHITE) {
    attack_board = piece_board >> 8;
    BITMASK_CLEAR(attack_board, blockers);
    double_attack_board = attack_board >> 8;
    BITMASK_INTERSECT(double_attack_board, RANK_4);
    BITMASK_CLEAR(double_attack_board, blockers);
  }
  else {
    attack_board = piece_board << 8;
    BITMASK_CLEAR(attack_board, blockers);
    double_attack_board = attack_board << 8;
    BITMASK_INTERSECT(double_attack_board, RANK_5);
    BITMASK_CLEAR(double_attack_board, blockers);
  }
  
  //single pawn push
  while (attack_board) {
    to_square = pop_lsb(&attack_board);
    from_square = to_square + pawn_shift;
    //promotion
    if ((to_square >= 56) || (to_square <= 7)) {
      move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_F, QUEEN_I));
      move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_F, ROOK_I));
      move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_F, KNIGHT_I));
      move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_F, BISHOP_I));
    }
    else {
      move_list.push_back(Moves(from_square, to_square, PAWN_I));
    }
  }
  //double pawn push 
  while (double_attack_board) {
    to_square = pop_lsb(&double_attack_board);
    from_square = to_square + (pawn_shift << 1);
    move_list.push_back(Moves(from_square, to_square, PAWN_I, DOUBLE_PAWN_F));
  }

  //knight moves 
  piece_board = piece_boards[turn][KNIGHT_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::KNIGHT_ATTACKS[from_square];
    BITMASK_CLEAR(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, KNIGHT_I));
    }
  }
  //bishop moves
  piece_board = piece_boards[turn][BISHOP_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::get_diag_attacks(from_square, blockers);
    BITMASK_CLEAR(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, BISHOP_I));
    }
  }
  //rook moves
  piece_board = piece_boards[turn][ROOK_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::get_line_attacks(from_square, blockers);
    BITMASK_CLEAR(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, ROOK_I));
    }
  }
  //queen moves
  piece_board = piece_boards[turn][QUEEN_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::get_queen_attacks(from_square, blockers);
    BITMASK_CLEAR(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, QUEEN_I));
    }
  }
  //king moves 
  piece_board = piece_boards[turn][KING_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::KING_ATTACKS[from_square];
    BITMASK_CLEAR(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, KING_I));
    }
  }
}
void Board::generate_piece_captures(vector<Moves>& move_list){
  int from_square, to_square;
  int opp_col = turn ^ 1;
  U64 piece_board, attack_board;
  U64 both_blockers = piece_co[turn] | piece_co[opp_col];
  U64 blockers = piece_co[opp_col];
  
  //pawn captures 
  piece_board = piece_boards[turn][PAWN_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::PAWN_ATTACKS[turn][from_square];
    BITMASK_INTERSECT(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      //promotion
      if ((to_square >= 56) || (to_square <= 7)) {
        move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_CAP_F, QUEEN_I));
        move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_CAP_F, ROOK_I));
        move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_CAP_F, KNIGHT_I));
        move_list.push_back(Moves(from_square, to_square, PAWN_I, PROMOTE_CAP_F, BISHOP_I));
      }
      else {
        move_list.push_back(Moves(from_square, to_square, PAWN_I, CAPTURE_F));
      }
    }
    //en passant!
    if (ep_square != NO_SQ) {
      attack_board = Attacks::PAWN_ATTACKS[turn][from_square] & SQUARES_BB[ep_square];
      if (attack_board) {
        int to_square = get_lsb(attack_board);
        move_list.push_back(Moves(from_square, to_square, PAWN_I, EN_PASSANT_F));
      }
    }
  }
  //knight captures 
  piece_board = piece_boards[turn][KNIGHT_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::KNIGHT_ATTACKS[from_square];
    BITMASK_INTERSECT(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, KNIGHT_I, CAPTURE_F));
    }
  }
  //bishop captures
  piece_board = piece_boards[turn][BISHOP_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::get_diag_attacks(from_square, both_blockers);
    BITMASK_INTERSECT(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, BISHOP_I, CAPTURE_F));
    }
  }
  //rook captures
  piece_board = piece_boards[turn][ROOK_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::get_line_attacks(from_square, both_blockers);
    BITMASK_INTERSECT(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, ROOK_I, CAPTURE_F));
    }
  }
  //queen captures
  piece_board = piece_boards[turn][QUEEN_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::get_queen_attacks(from_square, both_blockers);
    BITMASK_INTERSECT(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, QUEEN_I, CAPTURE_F));
    }
  }
  //king captures 
  piece_board = piece_boards[turn][KING_I];
  while(piece_board) {
    from_square = pop_lsb(&piece_board);
    attack_board = Attacks::KING_ATTACKS[from_square];
    BITMASK_INTERSECT(attack_board, blockers);
    while (attack_board) {
      to_square = pop_lsb(&attack_board);
      move_list.push_back(Moves(from_square, to_square, KING_I, CAPTURE_F));
    }
  }
}

vector<Moves> Board::generate_psuedolegal_moves(){
  vector<Moves> move_list;
  move_list.reserve(MOVE_LIST_RESERVE);
  generate_piece_captures(move_list);
  generate_castling_moves(move_list);
  
  generate_piece_quiets(move_list);
  return move_list;
}