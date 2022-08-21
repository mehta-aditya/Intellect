#include "eval.hpp"

//simple hand crafted eval
//currently has material, tuned piece square tables and mobility 
int Engine::evaluation(Board &board){
    int value = 0, mg_value = 0, eg_value = 0;
    int game_phase = 0;
    int square, mobility_squares;
    U64 piece_board; //copy of board
    for (int p = PAWN_I; p <= KING_I; p++){
        //white eval
        piece_board = board.piece_boards[WHITE][p];
        while (piece_board) {
            square = pop_lsb(&piece_board);
            //mobility value
            //mobility_squares = board.moves_at(square, WHITE, p);
            //mg_value += (mobility_squares-MAX_PIECE_BITS[p]/2)*MG_MOBILITY[p];
            //eg_value += (mobility_squares-MAX_PIECE_BITS[p]/2)*EG_MOBILITY[p];   
            //material value
            mg_value += MG_PIECE_VALUES[p];
            eg_value += EG_PIECE_VALUES[p];
            //positional value
            mg_value += MG_PST_VALUES[p][square];
            eg_value += EG_PST_VALUES[p][square];        
            game_phase += PHASE_VALUES[p];
        }
        //black eval
        piece_board = board.piece_boards[BLACK][p];
        while (piece_board) {
            square = pop_lsb(&piece_board);
            //mobility value
            //mobility_squares = board.moves_at(square, BLACK, p);
            //mg_value -= (mobility_squares-MAX_PIECE_BITS[p]/2)*MG_MOBILITY[p];
            //eg_value -= (mobility_squares-MAX_PIECE_BITS[p]/2)*EG_MOBILITY[p];
            //material value
            mg_value -= MG_PIECE_VALUES[p];
            eg_value -= EG_PIECE_VALUES[p];
            //positional value
            square = flip_board(square);
            mg_value -= MG_PST_VALUES[p][square];
            eg_value -= EG_PST_VALUES[p][square];
            game_phase += PHASE_VALUES[p];
        }
    }

    float phase = (game_phase * 256 + (MAX_PHASE / 2)) / MAX_PHASE;
    value = (mg_value * phase + eg_value * (256-phase))/256;
    return (board.turn == WHITE) ? value : -value;
}

