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
int Engine::negamax(Board &board, int alpha, int beta, int depth, int ply=0, bool null=true) {
    int value;
    Moves best_m = Moves();
    int legal_moves = 0; // number of legal moves tested in this node
    int original_alpha = alpha;
    int flag = TT_ALPHA;
    bool is_pv = beta - alpha > 1; 
    bool in_check = board.in_check(board.turn);

    pv_len[ply] = ply;

    //*Check for draw (threefold repition and 50 move)
    if (ply && board.is_repetition()) {return DRAW_V;}

    //check transposition table: if it is not root, 
    if(ply && !is_pv && tt_table.find(board.zobrist_hash) != tt_table.end() && tt_table[board.zobrist_hash].depth >= depth) {   
        if(tt_table[board.zobrist_hash].flag == TT_EXACT) {return tt_table[board.zobrist_hash].value;}
        else if (tt_table[board.zobrist_hash].flag == TT_ALPHA && tt_table[board.zobrist_hash].value <= alpha) {return alpha;}
        else if (tt_table[board.zobrist_hash].flag == TT_BETA && tt_table[board.zobrist_hash].value >= beta) {return beta;}
        best_m = tt_table[board.zobrist_hash].move;
    }
    
    //eval/quiesce
    if (depth <= 0) {
        return quiesce(board, alpha, beta);
    }
    //count node
    nodes++;

    //null move pruning
    // *add methods to stop zugzwang 
    int R;
    depth > 6 ? R = 3 : R = 2;
    if (depth >= 2 && !in_check && !is_pv) {
        board.push_null();
        value = -negamax(board, -beta, -beta+1, depth-R, ply+1, false);
        board.pop_null();
        //check for beta cutoff
        if (value >= beta) {return beta;}
    }

    //move ordering
    vector<Moves> moves = board.generate_psuedolegal_moves();
    score_moves(board, moves, best_m, ply);
    sort(moves.begin(), moves.end(), move_sort());

    //search moves
    for (Moves &move : moves) {
        board.push(move); // play move
        //check move legality; if it isn't legal skip the move
        if(board.in_check(board.turn^1)) {
            board.pop();
            continue;
        }
        //if legal increase legal move count
        legal_moves++;  
        R = 0;
        //use late move reduction (LMR)
        if (depth >= 3 && legal_moves >= 2 && move.captured == NO_PIECE && move.promoted == PAWN_I && !in_check) {
            R = 1;
        }
        //zero window
        if (legal_moves > 1) {  
            value = -negamax(board, -alpha-1, -alpha, depth-1-R, ply+1); //search with lmr
            //search again if late move didn't fail low
            if (R && value > alpha) {
                value = -negamax(board, -alpha-1, -alpha, depth-1, ply+1);
            }

            //search again using a full window
            if (value > alpha && value < beta) {
                value = -negamax(board, -beta, -alpha, depth-1, ply+1);
            }
                     
        }
        //full window
        else {
            value = -negamax(board, -beta, -alpha, depth-1, ply+1); //search
        }
        board.pop(); //unmake move

        if (check_limits()) {return 0;}


        //alpha beta pruning
        if (value > alpha) {
            alpha = value;
            //update best move
            best_m = move;
            update_pv(move, ply);
            //change other variables
            flag = TT_EXACT;
            //update history heuristic
            if (move.captured == NO_PIECE) {
                history[board.turn][move.from_square][move.to_square] += depth;
            }
        }
        //fail high
        if(value >= beta) {
            //update tt table
            if (tt_table.size() >= TT_MAX_SIZE) {
                tt_table.erase(tt_table.begin());
            }
            tt_table[board.zobrist_hash] = TTEntry(value, best_m, depth, TT_BETA);

            //update killer move heuristic
            if (move.captured == NO_PIECE) {
                killers[1] = killers[0];
                killers[0] = move;
            }
            return beta;
        }
        //check for stalemate and checkmate
        if (legal_moves == 0) {
            //-MATE because current player does not want to get checkmated + ply (so we look for fastest mate)
            if(board.in_check(board.turn)) {return -MATE_V + ply;}
            //STALEMATE
            else {return DRAW_V;}
        }
    }

    //store transposition table values
    if (tt_table.size() >= TT_MAX_SIZE) {
        tt_table.erase(tt_table.begin());
    }
    tt_table[board.zobrist_hash] = TTEntry(alpha, best_m, depth, flag);

    //update pv
    return alpha;
}
//iterative deepening loop
void Engine::iterative_deepening(Board& board) {
    Moves best_move; //holds best move from previous play
    start_time = steady_clock::now();   
    for (int depth = 1; depth <= search_depth; depth++) {       
        //check engine limits
        if (check_limits()) {break;}
        //calculate
        int score = negamax(board, -MAX, MAX, depth, 0);
        if (check_limits()) {break;}
        int elapsed_time = duration_cast<milliseconds>(steady_clock::now() - start_time).count();
        //print uci info
        cout << "info";
        cout << " score cp " << score;
        cout << " depth " << depth;
        cout << " time " << elapsed_time;
        cout << " nodes " << nodes;
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
    //reset important values
    stop = false;
    nodes = 0;
    memset(killers, 0, sizeof(killers));
    memset(history, 0, sizeof(history));
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
