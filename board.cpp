#include "board.hpp"


//Set board position using the FEN (Forsyth–Edwards Notation)
void Board::set_fen(string fen_set){
  //Split the fen into a vector
  vector<string> fen_list;
  stringstream ss(fen_set);
  string part;
  while (ss >> part) {
      fen_list.emplace_back(part);
  }
  //Parse through the fen
  int iter = 0;
  for(int i = 0; i < fen_list.size(); i++) {
    //parse board fen
    if (i == 0) {
      for (std::string::size_type j = 0; j < fen_list[i].size(); j++) {
        if (fen_list[i][j] == '/') {
          continue;
        }
        else if (isdigit(fen_list[i][j])) {
          int index_counter = fen_list[i][j]-48;
          for(int k = 0; k < index_counter; k++) {
            iter++;
        }
      }
      else {
          for(int l = WHITE; l <= BLACK; l++) { 
            for (int m = PAWN_I; m <= KING_I; m++) {
              if (fen_list[i][j] == FEN_PIECE_STRINGS[l][m][0]){
                SET_BIT(piece_boards[l][m], iter);
                break;
              }
            }
        }
        iter++;
      }
    }
  }
    // Side to move fen
    else if (i == 1) {
      if (fen_list[i] == "w") {
        turn = WHITE;
      }
      else {
        turn = BLACK;
      }
    }
    //Castling rights fen
    else if (i == 2) {
      if (fen_list[i] == "-") {
        castling_rights[0][0] = false; castling_rights[0][1] = false; castling_rights[1][0] = false; castling_rights[1][1] = false;
      }
      else {
        if (fen_list[i].find("K") != string::npos) {
          castling_rights[0][0] = true;
        } else {castling_rights[0][0] = false;}
        if (fen_list[i].find("Q") != string::npos) {
          castling_rights[0][1] = true;
        } else {castling_rights[0][1] = false;}
        if (fen_list[i].find("k") != string::npos) {
          castling_rights[1][0] = true;
        } else {castling_rights[1][0] = false;}
        if (fen_list[i].find("q") != string::npos) {
          castling_rights[1][1] = true;
        } else {castling_rights[1][1] = false;}
      }
    }
    //Ep square fen
    else if (i == 3) {
      if (fen_list[i] == "-") {
        ep_square = NO_SQ;
      }
      else {
        for (int j = 0; j < 64; j++) {
          if (SQUARES[j] == fen_list[i]) {
            ep_square = j;
            break;
          }
        }
      }
    }
  }

  for (int piece = PAWN_I; piece <= KING_I; piece++) {
    BITMASK_SET(piece_co[WHITE], piece_boards[WHITE][piece]);
    BITMASK_SET(piece_co[BLACK], piece_boards[BLACK][piece]);
  }
} 

//Helper Function

void Board::render(){
  Piece render_list[64] = {Piece()};
  for (int c = WHITE; c <= BLACK; c++) {
    for (int p = PAWN_I; p <= KING_I; p++){
      for (int s = 0; s < 64; s++) {
        if (CHECK_BIT(piece_boards[c][p], s)) {
          render_list[s] = Piece(c, p);
        }
      }
    }
  }

  for (int row = 0; row < 8; row++) {
    cout << abs(row - 8) << " ";
    for (int col = 0; col < 8; col++) {
      int square = row * 8 + col;
      if (render_list[square].type == NO_PIECE){
        cout << "." << " ";
      }
      else {
        cout << UNICODE_PIECES[render_list[square].color][render_list[square].type] << " ";
      }
    }
  cout << endl;
  }
  cout << "  a b c d e f g h" << endl;
}

void Board::reset(){
  //reset all board values
  while (!position_history.empty() ){position_history.pop();}
  turn = WHITE;
  memset(castling_rights, true, sizeof(castling_rights[0][0]) * 2 * 2); 
  memset(castling_rights, true, sizeof(castling_rights[0][0]) * 2 * 2); 
  int ep_square = NO_SQ;
  memset(piece_boards, EMPTY_BB, sizeof(piece_boards[0][0]) * 2 * 6); 
  memset(piece_co, EMPTY_BB, sizeof(piece_co)); 
}

//Show given bitboard
void bb_rendering(U64 bitboard) {
  cout << bitboard << endl;
  cout << "" << endl;
  for (int row = 0; row < 8; row++) {
    cout << abs(row - 8) << " ";
    for (int col = 0; col < 8; col++) {
      int square = row * 8 + col;
      cout << CHECK_BIT(bitboard, square) << " ";
    }
  cout << endl;
  }
  cout << "  a b c d e f g h";
}   

