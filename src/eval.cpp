#include "eval.hpp"
#include "engine.hpp"
#include "attacks.hpp"

U64 DOUBLED_MASK[64];
U64 ISOLATED_MASK[64];
U64 PASSED_MASK[2][64];
U64 HALF_BOARD[2];

void Engine::init_eval(){
    for (int square = 0; square < 64; square++) {
        //passed pawn mask table
        PASSED_MASK[WHITE][square] = Attacks::FILES_BB[square&7] |
        ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[7]) << 1) |
         ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[0]) >> 1);

        PASSED_MASK[BLACK][square] = Attacks::FILES_BB[square&7] | 
        ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[7]) << 1) |
         ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[0]) >> 1);

        ISOLATED_MASK[square] = PASSED_MASK[WHITE][square] & ~Attacks::FILES_BB[square&7];
        DOUBLED_MASK[square] = Attacks::FILES_BB[square&7];

        int rank = square/8;
        //white
        for (int r = rank; r < 8; r++){
            PASSED_MASK[WHITE][square] &= ~Attacks::RANKS_BB[r];
        }
        //black
        for (int r = rank; r >= 0; r--){
            PASSED_MASK[BLACK][square] &= ~Attacks::RANKS_BB[r];
        }
    }
    HALF_BOARD[WHITE] = Attacks::RANKS_BB[0] | Attacks::RANKS_BB[1] | Attacks::RANKS_BB[2] | Attacks::RANKS_BB[3];
    HALF_BOARD[BLACK] = Attacks::RANKS_BB[4] | Attacks::RANKS_BB[5] | Attacks::RANKS_BB[6] | Attacks::RANKS_BB[7];
}

//simple hand crafted eval
int Engine::evaluation(Board &board){
    int value = 0, game_phase = 0;
    int mg_value[2] = {0,0};
    int eg_value[2] = {0,0};
    int king_safety;
    U64 mobility_board;
    int square, mobility_squares;
    U64 piece_board; //copy of piece board
    
    int king_squares[2] = {get_lsb(board.piece_boards[WHITE][KING_I]),
                            get_lsb(board.piece_boards[BLACK][KING_I])};

    for (int c = WHITE; c <= BLACK; c++) {
        for (int p = PAWN_I; p <= KING_I; p++){
            piece_board = board.piece_boards[c][p];
            //bishop pair bonus
            if (p == BISHOP_I && Attacks::count_bits(piece_board) == 2) {
                mg_value[c] += MG_BISHOP_PAIR;
                eg_value[c] += EG_BISHOP_PAIR;
            }

            while (piece_board) {
                square = pop_lsb(&piece_board);
                
                if (p != KING_I) {
                    //mobility values
                    if (p != PAWN_I) {
                        mobility_board = board.attackers_from(square, c, p);
                        mobility_squares = Attacks::count_bits(mobility_board);
                        mg_value[c] += (mobility_squares)*MG_MOBILITY[p];
                        eg_value[c] += (mobility_squares)*EG_MOBILITY[p]; 

                        //king safety values
                        king_safety = Attacks::count_bits(mobility_board & Attacks::KING_ATTACKS[king_squares[c^1]]) * KING_ATTACKS[p];
                        mg_value[c] += king_safety*2;
                        eg_value[c] += king_safety/2;
                    }

                    //pawn structure eval
                    //doubled pawn eval
                    if (p == PAWN_I) {
                        //doubled pawn values
                        if (DOUBLED_MASK[square] & board.piece_boards[c][PAWN_I]) {
                            mg_value[c] -= MG_DOUBLED_VALUE;
                            eg_value[c] -= EG_DOUBLED_VALUE;
                        }
                        //isolated pawn eval
                        if (!(ISOLATED_MASK[square] & board.piece_boards[c][PAWN_I])) {
                            mg_value[c] -= MG_ISOLATED_VALUE;
                            eg_value[c] -= EG_ISOLATED_VALUE;
                        }
                        
                        //passed pawn eval
                        if (!(PASSED_MASK[c][square] & board.piece_boards[c^1][PAWN_I])) {
                            int rank = flip_board[c][square]/8;
                            mg_value[c] += PASSED_VALUE[rank];
                            eg_value[c] += PASSED_VALUE[rank] + EG_PASSED_BONUS; 
                        }
                    }

                    //update phase value based on pieces present
                    game_phase += PHASE_VALUES[p];
                } 
                //extra king safety eval
                else { 
                    int king_file = (king_squares[c] & 7); 
                    
                    if (!(Attacks::FILES_BB[king_file] & board.piece_boards[c][PAWN_I])) {
                        mg_value[c] -= SEMIOPEN_KING_FILE_VALUE;
                    }
                    if (king_file != 0 && !(Attacks::FILES_BB[king_file-1] & board.piece_boards[c][PAWN_I])) {
                        mg_value[c] -= SEMIOPEN_KING_FILE_VALUE;
                    }
                    if (king_file != 7 && !(Attacks::FILES_BB[king_file+1] & board.piece_boards[c][PAWN_I])) {
                        mg_value[c] -= SEMIOPEN_KING_FILE_VALUE;
                    }
                }

                //material value
                mg_value[c] += MG_PIECE_VALUES[p];
                eg_value[c] += EG_PIECE_VALUES[p];

                //positional value
                mg_value[c] += MG_PST_VALUES[p][flip_board[c][square]];
                eg_value[c] += EG_PST_VALUES[p][flip_board[c][square]];        

            }
        }
    }

    //if no pawns are left or we have big advantage aka. king endgame; we push them to edge
    if ((!(board.piece_boards[WHITE][PAWN_I]) && eg_value[BLACK] > eg_value[WHITE]) || (eg_value[BLACK] - 1000 > eg_value[WHITE] && game_phase <= 5)) {
        mg_value[WHITE] += KING_EDGE[king_squares[WHITE]];
        eg_value[WHITE] += KING_EDGE[king_squares[WHITE]];
    }   
    else if ((!(board.piece_boards[BLACK][PAWN_I]) && eg_value[WHITE] > eg_value[BLACK]) || (eg_value[WHITE] - 1000 > eg_value[BLACK] && game_phase <= 5)) {
        mg_value[BLACK] += KING_EDGE[king_squares[BLACK]];
        eg_value[BLACK] += KING_EDGE[king_squares[BLACK]];
    }   

    float phase = (game_phase * 256 + (MAX_PHASE / 2)) / MAX_PHASE;
    value = ((mg_value[WHITE]-mg_value[BLACK]) * phase + (eg_value[WHITE]-eg_value[BLACK]) * (256-phase))/256;
    return (board.turn == WHITE) ? value : -value;
}


//checks for most common cases of insufficient material
bool Engine::is_insufficient(Board &board) {
    U64 no_kings = (board.piece_co[WHITE] & ~board.piece_boards[WHITE][KING_I]) |
                    (board.piece_co[BLACK] & ~board.piece_boards[BLACK][KING_I]);
    
    //NK v K
    if (!(no_kings & ~board.piece_boards[WHITE][KNIGHT_I])) {
        return true;
    }   
    else if (!(no_kings & ~board.piece_boards[BLACK][KNIGHT_I])) {
        return true;
    }  
    //BK v K
    else if (!(no_kings & ~board.piece_boards[WHITE][BISHOP_I])) {
        return true;
    }  
    else if (!(no_kings & ~board.piece_boards[BLACK][BISHOP_I])) {
        return true;
    }  
    return false;
}
