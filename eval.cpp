#include "eval.hpp"

int Engine::evaluation(Board &board){
    int value = 0;
    int square;
    U64 piece_board; //copy of board
    for (int c = WHITE; c <= BLACK; c++) {
        for (int p = PAWN_I; p <= KING_I; p++){
            piece_board = board.piece_boards[c][p];
            while (piece_board) {
                square = pop_lsb(&piece_board);
                if (board.turn == WHITE) {
                    //material value
                    value += PIECE_VALUES[p];
                    //positional value
                    value += PST_VALUES[p][flip_board(square)];
                }
                else {
                    //material value
                    value -= PIECE_VALUES[p];
                    //positional value
                    value -= PST_VALUES[p][square];
                }
            }
        }
    }
    return (board.turn == WHITE) ? value : -value;
}
