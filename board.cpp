#include "board.hpp"

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
      


//Show given bitboard
void basic_rendering(U64 bitboard) {
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
