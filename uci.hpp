
#include "engine.hpp"

#ifndef UCI_HPP
#define UCI_HPP

namespace UCI {
    const string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const string ENGINE_VER = "0.0.0"; //major.minor.patch

    extern Board board;
    extern Engine engine;
    
    Moves to_move(string &uci_move); //FIX to_uci promotion
    void set_position(istringstream &is);
    void go(istringstream &is);
    void init();
    
}

#endif