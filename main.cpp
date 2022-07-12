#include "attacks.hpp"

int main(){
  Attacks::init();
  
  Board board;
  string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  string TESTING_FEN = "rn1q1rk1/pbpp1pbp/1p3np1/3Pp3/8/2NQPN1P/PPPBBPP1/R3K2R w KQ e6 0 10";
  board.set_fen(TESTING_FEN); 
  vector<Moves> moves = board.generate_piece_quiets();
  vector<Moves> capture_moves = board.generate_piece_captures();
  vector<Moves> castling_moves = board.generate_castling_moves();
  moves.insert(moves.end(), capture_moves.begin(), capture_moves.end());
  moves.insert(moves.end(), castling_moves.begin(), castling_moves.end());
  for(auto move : moves){
    cout << to_uci(move) << endl;
  }
  board.render();
  int x;
  cin >> x;
  board.push(moves[x]);
  board.render();
  board.pop();
  board.render();

  return 0;
}
