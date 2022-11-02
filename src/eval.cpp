#include "eval.hpp"
#include "engine.hpp"
#include "attacks.hpp"

U64 PASSED_MASK[2][64];

void Engine::init_eval(){
    for (int square = 0; square < 64; square++) {
        //passed pawn mask table
        PASSED_MASK[WHITE][square] = Attacks::FILES_BB[square&7] |
        ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[7]) << 1) |
         ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[0]) >> 1);

        PASSED_MASK[BLACK][square] = Attacks::FILES_BB[square&7] | 
        ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[7]) << 1) |
         ((Attacks::FILES_BB[square&7] & ~Attacks::FILES_BB[0]) >> 1);

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
}

//simple hand crafted eval
//currently has material, tuned piece square tables and mobility 
int Engine::evaluation(Board &board){
    int value = 0, game_phase = 0;
    int mg_value[2] = {0,0};
    int eg_value[2] = {0,0};
    int square, mobility_squares;
    U64 piece_board; //copy of board

    for (int c = WHITE; c <= BLACK; c++) {
        for (int p = PAWN_I; p <= KING_I; p++){
            piece_board = board.piece_boards[c][p];
            while (piece_board) {
                square = pop_lsb(&piece_board);
                //mobility value
                if (p != PAWN_I && p != KING_I) {
                    // mg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*MG_MOBILITY[p];
                    // eg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*EG_MOBILITY[p];  
                    mobility_squares = board.moves_at(square, c, p);
                    mg_value[c] += mobility_squares*MG_MOBILITY[p];
                    eg_value[c] += mobility_squares*EG_MOBILITY[p];
                    game_phase += PHASE_VALUES[p];
                } 
  
                //material value
                mg_value[c] += MG_PIECE_VALUES[p];
                eg_value[c] += EG_PIECE_VALUES[p];

                //positional value
                mg_value[c] += MG_PST_VALUES[p][flip_board[c][square]];
                eg_value[c] += EG_PST_VALUES[p][flip_board[c][square]];        
                
                //passed pawn eval
                if (p == PAWN_I && !(PASSED_MASK[c][square] & board.piece_boards[c^1][PAWN_I])) {
                    int rank = flip_board[c][square]/8;
                    mg_value[c] += PASSED_VALUE[rank];
                    eg_value[c] += PASSED_VALUE[rank] + EG_PASSED_BONUS;     
                }
            }
        }
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
    else if (!(no_kings & ~board.piece_boards[BLACK][BISHOP_I])) {
        return true;
    }  
    else if (!(no_kings & ~board.piece_boards[BLACK][BISHOP_I])) {
        return true;
    }  
    return false;
}
