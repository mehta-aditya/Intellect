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

//check engine limits
inline bool Engine::check_limits() {
    int elapsed_time = duration_cast<milliseconds>(steady_clock::now() - start_time).count();
    if (elapsed_time >= (time_for_move/2)) {stop = true;}
    return stop;
}

//quiesce search (run till position is stable)
int Engine::quiesce(Board &board, int alpha, int beta, int depth = QUIESCE_MAX_DEPTH) {
    int capture_moves = 0;
    int value;
    if (check_limits()) {return 0;}
    if (depth == 0) {return evaluation(board);}
    //check eval
    int stand_pat = evaluation(board);
    if (stand_pat >= beta) {return beta;}
    if (alpha < stand_pat) {alpha = stand_pat;}

    //iterate through moves
    vector<Moves> moves;
    moves.reserve(MOVE_LIST_RESERVE);
    board.generate_piece_captures(moves);
    score_quiesce_moves(moves);
    sort(moves.begin(), moves.end(), move_sort());
    //run through capture moves
    for (Moves &move : moves) {
        board.push(move); // play move
        //check move legality; if it isn't legal skip the move
        if(board.in_check(board.turn^1)) {
            board.pop();
            continue;
        }
        capture_moves++;
        value = -quiesce(board, -beta, -alpha, depth-1);
        board.pop();
        //alpha beta pruning
        if (value >= beta) {return beta;}
        if (value > alpha) {alpha = value;}
    }
    if (capture_moves == 0) {return stand_pat;}
    return alpha;
}

//run a negamax (PVS) search
int Engine::negamax(Board &board, int alpha, int beta, int depth, int ply=0) {
    int value;
    int legal_moves = 0; // number of legal moves tested in this node
    bool pvs = false; //if true do zero window search

    //Moves best_m;
    pv_len[ply] = ply;

    //Check for draw (threefold repition and 50 move)

    //eval/quiesce
    if (depth <= 0) {
        return quiesce(board, alpha, beta);
    }

    //iterate through moves
    vector<Moves> moves = board.generate_psuedolegal_moves();
    score_moves(moves);
    sort(moves.begin(), moves.end(), move_sort());
    //move ordering

    //search moves
    for (Moves &move : moves) {
        board.push(move); // play move
        //check move legality; if it isn't legal skip the move
        if(board.in_check(board.turn^1)) {
            board.pop();
            continue;
        }
        
        //full window
        if (!pvs) {
            legal_moves++;      
            value = -negamax(board, -beta, -alpha, depth-1, ply+1); //search         
        }
        //zero window
        else {
            legal_moves++;
            value = -negamax(board, -alpha - 1, -alpha, depth-1, ply+1); //search 
            //search again using negamax (fail-soft)
            if (value > alpha && value < beta) {
                value = -negamax(board, -beta, -alpha, depth-1, ply+1);
            }   
        }
        board.pop(); //unmake move
       
        if (check_limits()) {return 0;}

        //alpha beta pruning
        if (value > alpha) {
            update_pv(move, ply);
            alpha = value;
            pvs = true;
        }

        if(value >= beta) {
            //update killer move heuristic
            if (move.captured == NO_PIECE) {
                killers[1] = killers[0];
                killers[0] = move;
            }
            return value;
        }
    }

    //Stalemate and checkmate
    if (legal_moves == 0) {
        //-MATE because current player does not want to get checkmated + ply (so we look for fastest mate)
        if(board.in_check(board.turn)) {return -MATE_V + ply;}
        //STALEMATE
        else {return DRAW_V;}
    }
    //update pv
    return alpha;
}
//iterative deepening loop
void Engine::iterative_deepening(Board& board) {
    Moves best_move; //holds best move from previous play
    start_time = steady_clock::now();   
    for (int depth = 1; depth <= search_depth; depth++) {
        int elapsed_time = duration_cast<milliseconds>(steady_clock::now() - start_time).count();
        //check engine limits
        if (check_limits()) {break;}
        //calculate
        int score = negamax(board, -MAX, MAX, depth, 0);
        if (check_limits()) {break;}

        //print uci info
        cout << "info";
        cout << " score cp " << score;
        cout << " depth " << depth;
        cout << " time " << elapsed_time;
        cout << " pv ";

        // loop over the moves within a PV line
        best_move = pv[0][0];
        for (int count = 0; count < pv_len[0]; count++){
            // print PV move
            cout << to_uci(pv[0][count]) << "  ";
        }     
        cout << endl;  
    }
    //bestmove for uci
    if (stop == false) {cout << "bestmove " << to_uci(best_move) << endl;}
    else {cout << "bestmove " << to_uci(best_move) << endl;}
}

void Engine::search(Board& board, EngineLimits &limits) {
    memset(killers, 0, sizeof(killers));
    memset(pv, 0, sizeof(pv));
    memset(pv_len, 0, sizeof(pv_len));
    //infinite
    search_depth = MAX_DEPTH;
    time_for_move = MAX_TIME;
    if (limits.depth != 0) {
        search_depth = limits.depth;
    }
    else if (limits.move_time != 0) {
        time_for_move = limits.move_time;
    }
    else if (limits.co_time[board.turn] != 0) {
        //there are no moves until next time control (ie. sudden death): use TIME_DIVIDER
        int divider;
        limits.moves_to_go == 0 ? divider = TIME_DIVIDER : divider = limits.moves_to_go;

        time_for_move = (limits.co_time[board.turn] + limits.co_time[board.turn]/divider)/divider + limits.co_inc[board.turn] - OVERHEAD_TIME;
    }
    iterative_deepening(board);
}
