#include "engine.hpp"
#include "eval.hpp"

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
inline bool Engine::check_limits(int depth) {
    int elapsed_time = duration_cast<milliseconds>(steady_clock::now() - start_time).count();
    if (elapsed_time >= (time_for_move) || (depth >= MAX_DEPTH-1)) {stop = true;}
    return stop;
}

int Engine::set_tt_memory(TTMAP &tt_table, int megabytes) {
    auto entry_size = sizeof(U64) + sizeof(TTEntry) + sizeof(void*);
    auto bucket_size = sizeof(void*);
    auto admin_size = 3 * sizeof(void*) + sizeof(size_t);
    int entries = megabytes * 1e6 - tt_table.max_bucket_count() * bucket_size - admin_size;
    entries /= entry_size;
    return entries;
}

//quiesce search (run till position is stable)
inline int Engine::quiesce(Board &board, int alpha, int beta, int depth = QUIESCE_MAX_DEPTH) {
    int capture_moves = 0;
    int value;
    Moves best_m, tt_move;
    int best_v = -MAX_V-MAX_DEPTH-1;
    bool is_pv = beta - alpha != 1;
    TTFLAG flag = TT_ALPHA;
    //count nodes
    nodes++;
    
    if (nodes & 2047 && check_limits(depth)) {return 0;}
    //if this quiesce is really deep then just return eval
    if (depth <= 0) {return evaluation(board);} 

    //check transposition table: if it is not root, 
    if(!is_pv && tt_table.find(board.zobrist_hash) != tt_table.end()) {   
        if(tt_table[board.zobrist_hash].flag == TT_EXACT) {return tt_table[board.zobrist_hash].value;}
        else if (tt_table[board.zobrist_hash].flag == TT_ALPHA && tt_table[board.zobrist_hash].value <= alpha) {return tt_table[board.zobrist_hash].value;}
        else if (tt_table[board.zobrist_hash].flag == TT_BETA && tt_table[board.zobrist_hash].value >= beta) {return tt_table[board.zobrist_hash].value;}
        tt_move = tt_table[board.zobrist_hash].move;
    }

    int stand_pat = evaluation(board);

    //eval pruning for quiesce search
    //if the static eval is above beta we don't need to keep searching
    if (stand_pat >= beta) {return stand_pat;}
    
    //delta pruning
    //if we cannot improve our position even with a massive gain then this node can be pruned
    if (stand_pat + DELTA_MARGIN < alpha) {return stand_pat;}

    if (stand_pat > alpha) {alpha = stand_pat;}
    best_v = stand_pat;

    //iterate through moves
    vector<Moves> moves;
    moves.reserve(MOVE_LIST_RESERVE);
    board.generate_piece_captures(moves);
    score_quiesce_moves(moves, tt_move);
    sort(moves.begin(), moves.end(), move_sort());
    //run through capture moves
    for (Moves &move : moves) {
        //static engine evaluation pruning (SEE pruning)
        //this prunes bad captures right away
        if (!see(board, move, 0)) {continue;}
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
        if (value > best_v) {
            best_v = value;
            best_m = move;
            if (value >= beta) {
                flag = TT_BETA;
                break;
            }
            if (value > alpha) {
                alpha = value;
            }
        }        
    }

    //store transposition table values
    if (tt_table.size() >= TT_MAX_SIZE) {
        tt_table.erase(tt_table.begin());
    }
    tt_table[board.zobrist_hash] = TTEntry(best_v, best_m, -1, flag);

    return best_v;
}

//run a negamax (PVS) search
inline int Engine::negamax(Board &board, int alpha, int beta, int depth, int ply=0, bool null=true) {
    int value;
    int best_v = -MAX_V-MAX_DEPTH-1;
    Moves best_m, tt_move;
    int legal_moves = 0; // number of legal moves tested in this node
    TTFLAG flag = TT_ALPHA; //presume we will fail high
    bool is_pv = beta - alpha != 1; 
    bool not_tactical;
    bool in_check = board.in_check(board.turn);
    
    pv_len[ply] = ply;

    //Check for draw (threefold repition and 50 move)
    if (ply && (board.is_repetition() | (board.halfmove_clock >= 100 && !(in_check)))) {return DRAW_V;}

    //search extension for being in check
    if (in_check) {depth++;}
    //eval/quiesce
    if (depth <= 0) {
        return quiesce(board, alpha, beta);
    }
    //count node
    nodes++;

    bool is_tt = tt_table.find(board.zobrist_hash) != tt_table.end(); //checks if the position is in the transposition table
    //check transposition table if the position has appeared before
    if(is_tt) {
        if(ply && tt_table[board.zobrist_hash].depth >= depth && !is_pv) {   
            if(tt_table[board.zobrist_hash].flag == TT_EXACT) {return tt_table[board.zobrist_hash].value;}
            else if (tt_table[board.zobrist_hash].flag == TT_ALPHA && tt_table[board.zobrist_hash].value <= alpha) {return tt_table[board.zobrist_hash].value;}
            else if (tt_table[board.zobrist_hash].flag == TT_BETA && tt_table[board.zobrist_hash].value >= beta) {return tt_table[board.zobrist_hash].value;}   
        }
        tt_move = tt_table[board.zobrist_hash].move;
    }   

    //static evaluation of position
    //use transposition table eval if it's available
    int eval;
    if (is_tt && !in_check) {eval = tt_table[board.zobrist_hash].value;}
    else {
        eval = evaluation(board);
    }

    //reverse futility pruning/static null move pruning
    //if we are sufficiently above beta we can prune
    if (!in_check && !is_pv
        && eval - REVERSE_FUTILITY_MARGIN * depth >= beta 
        && eval >= beta && !(beta >= MATE_V || beta <= -MATE_V)) {
        return eval - REVERSE_FUTILITY_MARGIN * depth;
    }

    //null move pruning
    //if we don't play a move but still have a good position we can prune
    // *add methods to stop zugzwang 
    int R;
    if (null && !in_check && depth >= 3 && eval >= beta && !is_pv) {
        R = 3 + (depth/6);
        board.push_null();
        value = -negamax(board, -beta, -beta+1, depth-R-1, ply+1, false);
        board.pop_null();
        //check for beta cutoff
        if (value >= beta) {
            return value;
        }
    } 

    //razoring
    //if we are sufficiently below alpha then we can prune
    if (depth <= 3 && !is_pv && !in_check && eval + RAZORING_MARGIN*depth <= alpha) {
        value = quiesce(board, alpha, beta);
        if (value < alpha) {return value;}
    }

    //IID reduction technique
    //If there was no value in transposition table and we meet other requirements we can reduce depth
    if (depth >= 5 && !is_pv && !is_tt) {depth--;}

    //move ordering
    vector<Moves> moves = board.generate_psuedolegal_moves();
    score_moves(board, moves, tt_move, ply);
    sort(moves.begin(), moves.end(), move_sort());

    //search moves
    for (Moves &move : moves) {
        //check if a move is tactical or related to checks
        not_tactical = move.captured == NO_PIECE && !in_check && move.promoted == PAWN_I;

        //late move pruning (LMP) 
        //we can prune moves that are late in the node cause they are probably not as good
        if (depth <= 7 && not_tactical && !is_pv && legal_moves > LMP_TABLE[depth]) {
            continue;
        }

        //futility pruning
        //if we are far enough below alpha we can prune
        if (depth <= 7 && legal_moves > 3 && not_tactical && !is_pv && eval + FUTILITY_MARGIN[depth] < alpha) {
            continue;
        }

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
        //Reduces search depth for moves that are late in the node
        if (depth >= 3 && legal_moves >= 3 && not_tactical) {
            R = LMR_TABLE[depth][legal_moves];
            //don't reduce as much if PV or has good history
            R -= is_pv;
            R -= history[board.turn][move.piece][move.to_square]/1200;
        }

        //zero window
        if (legal_moves > 1 || !is_pv) {  
            value = -negamax(board, -alpha-1, -alpha, depth-1-R, ply+1); //search with lmr
            //research the move at full depth if late move didn't fail low
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

        //make sure we don't have to stop
        if (nodes & 2047 && (check_limits(depth))) {return 0;}

        //alpha beta pruning
        if (value > best_v) {
            //update best move
            best_v = value; 
            best_m = move;   

            //fail high/cut node: this is so good that the opponent wouldn't let us do this
            if(value >= beta) {
                flag = TT_BETA; //fail low
                //update various heuristics (killer, countermove, history)
                if (move.captured == NO_PIECE) {
                    killers[1][ply] = killers[0][ply];
                    killers[0][ply] = move;
                    countermoves[move.from_square][move.to_square] = move;
                    history[board.turn][move.piece][move.to_square] += depth*depth;
                    //don't go over maximum
                    if (history[board.turn][move.piece][move.to_square] > MAX_HISTORY_V) {history[board.turn][move.piece][move.to_square] /= 2;}
                }
                break; //cutoff
            }

            //this is good so it is our new minimium
            if (value > alpha) {
                alpha = value;
                flag = TT_EXACT; //we were in alpha-beta bounds
                update_pv(move, ply);
                //update history heuristic
                if (move.captured == NO_PIECE) {
                    history[board.turn][move.piece][move.to_square] += depth*depth;
                    //don't go over maximum
                    if (history[board.turn][move.piece][move.to_square] > MAX_HISTORY_V) {history[board.turn][move.piece][move.to_square] /= 2;}
                }
            }
            //decrease history heuristic
            else {
                history[board.turn][move.piece][move.to_square] -= 1; 
            }
        }       
    }
    
    //check for stalemate and checkmate
    if (legal_moves == 0) {
        //-MATE because current player does not want to get checkmated + ply (so we look for fastest mate)
        if(in_check) {return -MATE_V + ply;}
        //STALEMATE
        else {return DRAW_V;}
    }

    if (board.halfmove_clock >= 100) {}

    //store transposition table values
    if (tt_table.size() >= TT_MAX_SIZE) {
        tt_table.erase(tt_table.begin());
    }
    tt_table[board.zobrist_hash] = TTEntry(best_v, best_m, depth, flag);

    return best_v;
}

//iterative deepening loop
inline void Engine::iterative_deepening(Board& board) {
    Moves best_move; //holds best move from previous play
    start_time = steady_clock::now();   
    int alpha = -MAX_V, beta = MAX_V;
    int value = 0;
    int upperbound, lowerbound;
    int delta_v = 20;
    for (int depth = 1; depth <= search_depth; depth++) {       
        //check engine limits
        if (check_limits(depth)) {break;}
        //value = negamax(board, alpha, beta, depth);
        //use aspiration windows
        if (depth <= 4) {
           value = negamax(board, alpha, beta, depth);
        }
        else {
            while (!stop) {
                lowerbound = max(alpha, value-delta_v);
                upperbound = min(beta, value+delta_v);
                value = negamax(board, lowerbound, upperbound, depth);
                //we failed low at root
                if (value <= lowerbound) {
                    upperbound = (upperbound+lowerbound)/2;
                    lowerbound = max(value - delta_v, alpha);
                }
                //we failed high at root
                else if (value >= upperbound) {
                    upperbound = min(beta, value+delta_v);
                }
                else {break;}
                //increase the window if we were outside alpha and beta
                delta_v += delta_v/3;
            }
        }

    
        if (check_limits(depth)) {break;}
        int elapsed_time = duration_cast<milliseconds>(steady_clock::now() - start_time).count();

        //print uci info
        cout << "info";
        if (value < -MATE_V+MAX_DEPTH+1) {cout << " score mate " << (int)((-MATE_V-value+1)/2);}
        else if (value > MATE_V-MAX_DEPTH-1) {cout << " score mate " << (int)((MATE_V-value+1)/2);}
        else {cout << " score cp " << value;}
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
    tt_table.clear();
    memset(killers, 0, sizeof(killers));
    memset(countermoves, 0, sizeof(countermoves));
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
        time_for_move = limits.move_time - OVERHEAD_TIME;
    }
    else if (limits.co_time[board.turn] != 0) {
        //there are no moves until next time control (ie. sudden death): use TIME_DIVIDER
        int divider;
        limits.moves_to_go == 0 ? divider = TIME_DIVIDER : divider = limits.moves_to_go;

        time_for_move = limits.co_time[board.turn]/divider - OVERHEAD_TIME;
    }
    time_for_move = max(time_for_move, 10);
    iterative_deepening(board);
}
