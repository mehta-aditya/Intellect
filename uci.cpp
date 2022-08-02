#include "uci.hpp"

namespace UCI {
    Board board;
    Engine engine;
}


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
    EngineLimits limits;
    string cin_part;
    while (is >> cin_part) {
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

    //the engine should start thinking
    engine.search(board, limits);
}

//start uci engine
void UCI::init() {
    //start off with the identifying info
    cout << "Intellect" << ENGINE_VER << " by Aditya Mehta" << endl;
    string cin_line,cin_part;
    bool searching = false;
    
    board.reset(); //needs to be PROGRAMMED

    while (getline(cin, cin_line)) {
        istringstream is(cin_line);
        is >> cin_part;
        //quit uci
        if (cin_part == "quit") {
            if (searching) {engine.stop = true;}
            break;
        }
        //stop search
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
            board.set_fen(STARTING_FEN);
        }
        else if (cin_part == "position") {
            set_position(is);
        }
        else if (cin_part == "go") {
            searching = true;
            go(is);
            searching = false;
        }
        //setoption commands
    }
}
