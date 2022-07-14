#include "engine.hpp"

inline void Engine::update_pv(Moves &move, int ply) {
    pv[ply][ply] = move; //update pv table

    for (int next_ply = ply + 1; next_ply < pv_len[ply + 1]; next_ply++){
        // copy move from deeper ply into a current ply's line
        pv[ply][next_ply] = pv[ply + 1][next_ply];
    }
    // adjust PV length
    pv_len[ply] = pv_len[ply + 1];        
}
int Engine::search(Board &board, int alpha, int beta, int depth, int ply=0) {
    int best_v, value;
    int legal_moves = 0; // number of legal moves tested in this node
    Moves best_m;
    pv_len[ply] = ply;

    //Check for draw (threefold repition and 50 move)

    //eval/quiesce
    if (depth <= 0) {
        return evaluation(board);
    }

    //iterate through moves
    vector<Moves> moves = board.generate_psuedolegal_moves();
    //move ordering

    //search remaining moves
    for (Moves &move : moves) {
        board.push(move); // play move
        //check move legality; if it isn't legal skip the move
        if(board.in_check()) {
            continue;
        }

        //search the first move in node using negamax
        if (legal_moves == 0) {
            value = -search(board, -beta, -alpha, depth-1, ply+1); //search
            board.pop(); //unmake the move
            //alpha beta pruning (fail-soft)
            if (best_v > alpha) {
                update_pv(move, ply);
                if (best_v >= beta) {
                    return best_v;
                }
                alpha = best_v;
            }
        }
        //pvs search (uses zero window searches)
        else {
            value = -search(board, -alpha - 1, -alpha, depth-1, ply+1); //search 
            //search again using negamax (fail-soft)
            if (value > alpha && value < beta) {
                value = -search(board, -beta, -alpha, depth-1, ply+1);
                if (value > alpha) {alpha = value;}
            }
            board.pop(); //unmake move
            //alpha beta pruning
            if (value > best_v) {
                update_pv(move, ply);
                if(value >= beta) {return value;}
                best_v = value;
            }
        }
    }
    //Stalemate and checkmate
    if (legal_moves == 0) {
        //-MATE because current player does not want to get checkmated + ply (so we look for fastest mate)
        if(board.in_check()) {return -MATE_V + ply;}
        //STALEMATE
        else {return DRAW_V;}
    }
    //update pv
    return best_v;
}


string Engine::root(Board &board, int depth){   
    memset(pv, 0, sizeof(pv));
    memset(pv_len, 0, sizeof(pv_len));
    int value = search(board, -MAX, MAX, depth);
    return to_uci(pv[0][0]);
}

