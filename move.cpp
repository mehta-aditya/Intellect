#include "board.hpp"
//Gets the piece at a square from the bitboards
inline int Board::piece_at(int square, int color){
    int square_bb = SQUARES_BB[square];
    for (int i = PAWN_I; i <= KING_I; i++){
        if (square_bb & piece_boards[color][i]) {
            return i;
        }
    }
    return NO_PIECE;
}
//add the piece to the bitboards
inline void Board::add_piece(int square, int color, int type) {
    U64 mask = SQUARES_BB[square];
    piece_boards[color][type] |= mask;
    piece_co[color] |= mask;
}
//removing the piece from the bitboards
inline void Board::remove_piece(int square, int color, int type) {
    U64 mask = SQUARES_BB[square];
    piece_boards[color][type] ^= mask;
    piece_co[color] ^= mask;
}
//move the piece in the bitboards
inline void Board::move_piece(int from, int to, int color, int type) {
    U64 mask = SQUARES_BB[from] | SQUARES_BB[to];
    piece_boards[color][type] ^= mask;
    piece_co[color] ^= mask;
}


//play the move
void Board::push(Moves move){
    int opp_col = turn ^ 1;
    int past_ep = ep_square;
    //Reset en passant square    
    ep_square = NO_SQ;
    //No flag
    if (move.flag == NO_FLAG){
        move_piece(move.from_square, move.to_square, turn, move.piece);
    }
    //Capture flag
    else if (move.flag == CAPTURE_F) {
        move.captured = piece_at(move.to_square, opp_col);
        remove_piece(move.from_square, opp_col, move.captured);
        move_piece(move.from_square, move.to_square, turn, move.piece);
    }
    //Kingside Castle flag
    else if (move.flag == KS_F) {
        move_piece(move.from_square, move.to_square, turn, move.piece);
        move_piece(move.to_square+1, move.from_square+1, turn, ROOK_I);
    }
    //Queenside Castle flag
    else if (move.flag == QS_F) {
        move_piece(move.from_square, move.to_square, turn, move.piece);
        move_piece(move.to_square-2, move.from_square-1, turn, ROOK_I);
    }
    //Promote (without capture) flag
    else if (move.flag == PROMOTE_F) {
        remove_piece(move.from_square, turn, move.piece);
        add_piece(move.to_square, turn, move.promoted);
    }
    //Promote (with capture) flag
    else if (move.flag == PROMOTE_F) {
        move.captured = piece_at(move.to_square, opp_col);
        remove_piece(move.to_square, opp_col, move.captured);
        remove_piece(move.from_square, turn, move.piece);
        add_piece(move.to_square, turn, move.promoted);
    }
    //En passant flag
    else if (move.flag == EN_PASSANT_F) {
        move.captured = PAWN_I;
        int pawn_shift;
        turn == WHITE ? pawn_shift = 8 : pawn_shift = -8;
        move_piece(move.from_square, move.to_square, turn, move.piece);
        remove_piece(move.to_square + pawn_shift, opp_col, move.captured);
    }
    //Double Pawn Push flag (needed to update en passant square)
    else if (move.flag == DOUBLE_PAWN_F) {
        move_piece(move.from_square, move.to_square, turn, move.piece);
        turn == WHITE ? ep_square = move.to_square + 8 : ep_square = move.to_square - 8;
    }
    //add position to position history (before switching castling rights )
    position_history.push(Position(move, castling_rights, past_ep));

    //Switch Castling Rights
    //Rooks moved or captured check
    //h1 rook
    if (!(piece_boards[WHITE][ROOK_I] & SQUARES_BB[63])) {
        castling_rights[WHITE][KINGSIDE_I] = false;
    }
    //a1 rook
    if (!(piece_boards[WHITE][ROOK_I] & SQUARES_BB[56])) {
        castling_rights[WHITE][QUEENSIDE_I] = false;
    }
    //h8 rook
    if (!(piece_boards[BLACK][ROOK_I] & SQUARES_BB[7])) {
        castling_rights[BLACK][KINGSIDE_I] = false;
    }
    //a8 rook
    if (!(piece_boards[BLACK][ROOK_I] & SQUARES_BB[0])) {
        castling_rights[BLACK][QUEENSIDE_I] = false;
    }
    //white king moved
    if (!(piece_boards[WHITE][KING_I] & SQUARES_BB[60])) {
        castling_rights[WHITE][KINGSIDE_I] = false;
        castling_rights[WHITE][QUEENSIDE_I] = false;
    }
    //black king moved
    if (!(piece_boards[BLACK][KING_I] & SQUARES_BB[4])) {
        castling_rights[BLACK][KINGSIDE_I] = false;
        castling_rights[BLACK][QUEENSIDE_I] = false;
    }
    
    //swap turn
    turn = opp_col;

}

void Board::pop(){
    int opp_col = turn ^ 1;
    Position pos = position_history.top();
    //switch castling rights
    castling_rights[WHITE][KINGSIDE_I] = pos.castling_rights[WHITE][KINGSIDE_I];
    castling_rights[WHITE][QUEENSIDE_I] = pos.castling_rights[WHITE][QUEENSIDE_I];
    castling_rights[BLACK][KINGSIDE_I] = pos.castling_rights[BLACK][KINGSIDE_I];
    castling_rights[BLACK][QUEENSIDE_I] = pos.castling_rights[BLACK][QUEENSIDE_I];
    //switch en passant square
    ep_square = pos.ep_square;
    //No flag
    if (pos.move.flag == NO_FLAG || pos.move.flag == DOUBLE_PAWN_F){
        move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
    }
    //Capture flag
    else if (pos.move.flag == CAPTURE_F) {
        add_piece(pos.move.to_square, turn, pos.move.captured);
        move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
    }
    //Kingside Castle flag
    else if (pos.move.flag == KS_F) {
        move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
        move_piece(pos.move.from_square+1, pos.move.to_square+1, opp_col, ROOK_I);
    }
    //Queenside Castle flag
    else if (pos.move.flag == QS_F) {
        move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
        move_piece(pos.move.from_square-1, pos.move.to_square-2, opp_col, ROOK_I);
    }
    //Promote (without capture) flag
    else if (pos.move.flag == PROMOTE_F) {
        add_piece(pos.move.from_square, opp_col, pos.move.piece);
        remove_piece(pos.move.to_square, opp_col, pos.move.promoted);
    }
    //Promote (with capture) flag
    else if (pos.move.flag == PROMOTE_F) {
        add_piece(pos.move.to_square, turn, pos.move.captured);
        add_piece(pos.move.from_square, opp_col, pos.move.piece);
        remove_piece(pos.move.to_square, opp_col, pos.move.promoted);
    }
    //En passant flag
    else if (pos.move.flag == EN_PASSANT_F) {
        int pawn_shift;
        opp_col == WHITE ? pawn_shift = 8 : pawn_shift = -8;
        move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
        add_piece(pos.move.to_square + pawn_shift, turn, pos.move.captured);
    }
    turn = opp_col;
}