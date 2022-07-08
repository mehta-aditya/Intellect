#include "attacks.hpp"

int main(){
  Attacks::init();
  
  Board board;
  string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  board.set_fen(STARTING_FEN); 
  vector<Moves> moves = board.generate_piece_quiets();
  cout << board.turn;
  for(auto move : moves){
    cout << to_uci(move) << endl;
  }

  return 0;
}
