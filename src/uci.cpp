#include "uci.hpp"

namespace UCI {
    Board board;
    Engine engine = Engine();
}
int UCI::perft(int depth){ 
  int nodes = 0;

  if (depth == 0) {return 1;}

  vector<Moves> moves = board.generate_psuedolegal_moves();
  for (Moves &move : moves) {
    board.push(move);
    if (!board.in_check(board.turn^1)) {
        nodes += perft(depth - 1);
    }
    board.pop();
  }
  return nodes;
}

void UCI::divide(int depth) {
  int nodes = 0, n = 0;

  if (depth == 0) {return;}

  vector<Moves> moves = board.generate_psuedolegal_moves();
  for (Moves &move : moves) {
    board.push(move);
    if (!board.in_check(board.turn^1)) {
      n = perft(depth - 1);
      cout << to_uci(move) << ":" << n << endl;
      nodes += n;
    }
    board.pop();
  }
    cout << endl;
    cout << "Nodes searched: " << nodes << endl << endl;
}

//change a uci string to a move
Moves UCI::to_move(string &uci_move) {
    //only for promotions; make the promotion piece lowercase
    if (uci_move.length() == 5){
      uci_move[4] = char(tolower(uci_move[4])); 
    }
    //search legal moves for same move
    vector<Moves> moves = board.generate_psuedolegal_moves();
    for (Moves &move : moves){
        if (uci_move == to_uci(move)){
          return move;
        }
    }
}

//position command
void UCI::set_position(istringstream &is) {
    string cin_part;
    is >> cin_part;
    //set to starting position
    if (cin_part == "startpos") {
        board.set_fen(STARTING_FEN);
        is >> cin_part; //skip the moves part
    }
    //set to a specific fen
    else if (cin_part == "fen") {
        string fen;
        //go through the the fen list until we run out or hit "moves" part of string
        while (is >> cin_part && cin_part != "moves") {
            fen += cin_part + " ";
            board.set_fen(fen);
        }
    }
    //parse through the other moves
    while (is >> cin_part) {
        board.push(to_move(cin_part));
    }
}
//go command
void UCI::go(istringstream &is) {
    EngineLimits limits = EngineLimits();
    string cin_part;
    int perft_depth = 0;
    while (is >> cin_part) {
        if (cin_part == "perft") {is >> perft_depth; break;}
        //only search these specific moves
        if (cin_part == "searchmoves") {
            while (is >> cin_part) {
                limits.search_moves.push_back(to_move(cin_part));
            }
        }
        else if (cin_part == "wtime") {is >> limits.co_time[WHITE];}
        else if (cin_part == "btime") {is >> limits.co_time[BLACK];}
        else if (cin_part == "winc") {is >> limits.co_inc[WHITE];}
        else if (cin_part == "binc") {is >> limits.co_inc[BLACK];}
        else if (cin_part == "movestogo") {is >> limits.moves_to_go;}
        else if (cin_part == "depth") {is >> limits.depth;}
        else if (cin_part == "movetime") {is >> limits.move_time;}
        else if (cin_part == "infinite") {limits.infinite = true;}
    }    
    if (!perft_depth) {
        //the engine should start thinking
        engine.search(board, limits);
    }
    else {
        auto start_time = steady_clock::now();
        cout << perft(perft_depth) << endl;
        int elapsed_time = duration_cast<milliseconds>(steady_clock::now() - start_time).count();
        cout << "time " << elapsed_time << endl;
    }

}

//start uci engine
void UCI::init() {
    engine.reset();
    engine.init_eval();
    //start off with the identifying info
    cout << "Intellect " << ENGINE_VER << " by Aditya Mehta" << endl;
    string cin_line,cin_part;
    bool searching = false;
    board.reset();

    while (getline(cin, cin_line)) {
        istringstream is(cin_line);
        is >> cin_part;
        //quit uci
        if (cin_part == "quit") {
            if (searching) {engine.stop = true;}
            break;
        }
        //stop search
        //*add in feature to stop in middle of search
        else if (cin_part == "stop") {
            if (searching) {engine.stop = true;}
        }
        //identifying info
        else if (cin_part == "uci") {
            cout << "id name Intellect " << ENGINE_VER << endl;
            cout << "id author Aditya Mehta" << endl;
            //add options here
            cout << "uciok" << endl;
        }
        //reply to gui command
        else if (cin_part == "isready") { 
            cout << "readyok" << endl;
        }
        else if (cin_part == "ucinewgame") {
            engine.reset();
            board.set_fen(STARTING_FEN);
        }
        else if (cin_part == "position") {
            engine.reset();
            set_position(is);
        }
        else if (cin_part == "go") {
            searching = true;
            go(is);
            searching = false;
        }
        //for debugging
        else if (cin_part == "render") {
            board.render();
        }

        //setoption commands
    }
}
