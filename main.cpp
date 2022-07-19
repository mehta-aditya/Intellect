#include "attacks.hpp"
#include "engine.hpp"


int main(){
  Attacks::init();
  //Attacks::init_magic_numbers();
  Board board;
  Engine engine;
  string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  string TESTING_FEN = "r1bqkb1r/1ppp1ppp/p1n2n2/4p3/B3P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 5";
  board.set_fen(TESTING_FEN); 
  vector<Moves> moves = board.generate_psuedolegal_moves();

  board.render();  
  auto start = chrono::high_resolution_clock::now();
  cout << engine.root(board, 7);
  auto stop = chrono::high_resolution_clock::now();
  board.render();
  auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
  cout << "Time taken by function: " << duration.count() << " milliseconds" << endl;

  return 0;
}
