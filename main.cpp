#include "attacks.hpp"
#include "engine.hpp"

int main(){
  Attacks::init();
  
  Board board;
  Engine engine;
  string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  string TESTING_FEN = "r1bqkb1r/1ppp1ppp/p1n2n2/4p3/B3P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 5";
  board.set_fen(TESTING_FEN); 


  board.render();
  engine.root(board, 6);
  board.render();
  return 0;
}
