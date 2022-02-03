#include "attacks.h"

struct Moves {
  //0: No Flag 1:Castling 2: En Passant 3: Promotion
  int from_square = -1, to_square = -1, captured = 0, flag = 0, promoted = 0;
  Moves(int from = -1, int to = -1, int cap = 0, int f = 0, int pro = 0) {
    from_square = from;
    to_square = to;
    captured = cap;
    flag = f;
    promoted = pro;
  }
};

string to_uci(Moves &move) {
      string uci_str = SQUARES[move.from_square] + SQUARES[move.to_square];
      return uci_str;
    }

class Board{
    public:
      Board() {
        init_bb_values();
        init_step_tables();
        init_diag_tables();
        init_line_tables();
      }
    
      bool turn = true;
      bool castling_rights[2][2]; //[color][types] .//[0:White or 1: Black][0:Kingside or 1:Queenside]
      int ep_square;
      //No Piece, WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK
      U64 piece_boards[2][6] = {EMPTY_BB};
      U64 piece_co[2] = {EMPTY_BB};
      
      void set_fen(string fen_set){
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
              turn = true;
            }
            else {
              turn = false;
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
              ep_square = -1;
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
      
      vector<Moves> generate_piece_quiets(){
        int from_square, to_square;
        int color_to_move = !turn; //0 is white and 1 is black
        int pawn_shift;
        vector<Moves> move_list;
        move_list.reserve(218);
        Moves move;
        U64 piece_board, attack_board, double_attack_board;
        U64 blockers = piece_co[color_to_move] | piece_co[(color_to_move^1)];
        color_to_move == WHITE ? pawn_shift = 8 : pawn_shift = -8;
        
        //pawn moves
        piece_board = piece_boards[color_to_move][PAWN_I];
        //white pawn moves
        if (color_to_move == WHITE) {
          attack_board = piece_board >> 8;
          BITMASK_CLEAR(attack_board, blockers);
          double_attack_board = attack_board >> 8;
          BITMASK_INTERSECT(double_attack_board, RANK_5);
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
          to_square = pop_lsb(attack_board);
          from_square = to_square + pawn_shift;
          //promotion
          if ((to_square >= 56) || (to_square <= 7)) {
            move_list.push_back(Moves(from_square, to_square, 0, 3, QUEEN_I));
            move_list.push_back(Moves(from_square, to_square, 0, 3, ROOK_I));
            move_list.push_back(Moves(from_square, to_square, 0, 3, KNIGHT_I));
            move_list.push_back(Moves(from_square, to_square, 0, 3, BISHOP_I));
          }
          else {
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //double pawn push 
        while (double_attack_board) {
          to_square = pop_lsb(double_attack_board);
          from_square = to_square + (pawn_shift << 1);
          move_list.push_back(Moves(from_square, to_square));
        }

        //knight moves 
        piece_board = piece_boards[color_to_move][KNIGHT_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = KNIGHT_ATTACKS[from_square];
          BITMASK_CLEAR(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //bishop moves
        piece_board = piece_boards[color_to_move][BISHOP_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = get_diag_attacks(from_square, blockers);
          BITMASK_CLEAR(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //rook moves
        piece_board = piece_boards[color_to_move][ROOK_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = get_line_attacks(from_square, blockers);
          BITMASK_CLEAR(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //queen moves
        piece_board = piece_boards[color_to_move][QUEEN_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = get_queen_attacks(from_square, blockers);
          BITMASK_CLEAR(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //king moves 
        piece_board = piece_boards[color_to_move][KING_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = KING_ATTACKS[from_square];
          BITMASK_CLEAR(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        return move_list;
      }
      vector<Moves> generate_piece_captures(){
        int from_square, to_square;
        int color_to_move = !turn; //0 is white and 1 is black
        vector<Moves> move_list;
        
        Moves move;
        U64 piece_board, attack_board;
        U64 both_blockers = piece_co[color_to_move] | piece_co[(color_to_move^1)];
        U64 blockers = piece_co[(color_to_move^1)];
        move_list.reserve(60);
        
        //pawn captures 
        piece_board = piece_boards[color_to_move][PAWN_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = PAWN_ATTACKS[color_to_move][from_square];
          BITMASK_INTERSECT(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            //promotion
            if ((to_square >= 56) || (to_square <= 7)) {
              move_list.push_back(Moves(from_square, to_square, 0, 3, QUEEN_I));
              move_list.push_back(Moves(from_square, to_square, 0, 3, ROOK_I));
              move_list.push_back(Moves(from_square, to_square, 0, 3, KNIGHT_I));
              move_list.push_back(Moves(from_square, to_square, 0, 3, BISHOP_I));
            }
            else {
              move_list.push_back(Moves(from_square, to_square));
            }
          }
        }
        //knight captures 
        piece_board = piece_boards[color_to_move][KNIGHT_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = KNIGHT_ATTACKS[from_square];
          BITMASK_INTERSECT(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //bishop captures
        piece_board = piece_boards[color_to_move][BISHOP_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = get_diag_attacks(from_square, both_blockers);
          BITMASK_INTERSECT(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //rook captures
        piece_board = piece_boards[color_to_move][ROOK_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = get_line_attacks(from_square, both_blockers);
          BITMASK_INTERSECT(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //queen captures
        piece_board = piece_boards[color_to_move][QUEEN_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = get_queen_attacks(from_square, both_blockers);
          BITMASK_INTERSECT(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        //king captures 
        piece_board = piece_boards[color_to_move][KING_I];
        while(piece_board) {
          from_square = pop_lsb(piece_board);
          attack_board = KING_ATTACKS[from_square];
          BITMASK_INTERSECT(attack_board, blockers);
          while (attack_board) {
            to_square = pop_lsb(attack_board);
            move_list.push_back(Moves(from_square, to_square));
          }
        }
        return move_list;
      }


      //Show given bitboard
      void basic_rendering(U64 &bitboard) {
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
  };

int main(){
  Board board;
  string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  board.set_fen(STARTING_FEN); 
  vector<Moves> quiet_moves, capture_moves;
  auto start = steady_clock::now();
  quiet_moves = board.generate_piece_quiets();
  capture_moves = board.generate_piece_captures();
  auto stop = steady_clock::now();

  auto duration = duration_cast<nanoseconds>(stop - start);
  cout << duration.count() << endl;

  // vector<Moves> quiet_moves, capture_moves;
  // for (int i = 0; i < 100000; i++) {
  //   quiet_moves = board.generate_piece_quiets();
  //   capture_moves = board.generate_piece_captures();
  // }
  //vector<Moves> legal_moves = board.generate_piece_quiets();
  //vector<Moves> more_moves = board.generate_piece_captures();
  //lsb_index(RANK_4);
  // cout << endl;
  // for (int i = 0; i < legal_moves.size(); i++) {
  //   cout << to_uci(legal_moves[i]) << " ";
  // }
  // cout << endl;
  // for (int i = 0; i < more_moves.size(); i++) {
  //   cout << to_uci(more_moves[i]) << " ";
  // }
  return 0;
}

