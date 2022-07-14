#include "attacks.hpp"
#include "engine.hpp"

int main(){
  Attacks::init();
  
  Board board;
  Engine engine;
  string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  string TESTING_FEN = "rn1q1rk1/pbpp1pbp/1p3np1/3Pp3/8/2NQPN1P/PPPBBPP1/R3K2R w KQ e6 0 10";
  board.set_fen(TESTING_FEN); 

  board.render();
  cout << engine.root(board, 3);
  return 0;
}
