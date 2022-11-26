#include "board.hpp"
#include "eval.hpp"
#include "zobrist.hpp"

//Gets the piece at a square from the bitboards
int Board::piece_at(int square, int color){
    for (int p = PAWN_I; p <= KING_I; p++){
        if (SQUARES_BB[square] & piece_boards[color][p]) {
            return p;
        }
    }
    return NO_PIECE;
}
//add the piece to the bitboards
inline void Board::add_piece(int square, int color, int type) {
    U64 mask = SQUARES_BB[square];
    piece_boards[color][type] |= mask;
    piece_co[color] |= mask;
    zobrist_hash ^= PIECE_ZOBRIST[color][type][square];
    piece_list[color][square] = type;
}
//removing the piece from the bitboards
inline void Board::remove_piece(int square, int color, int type) {
    U64 mask = SQUARES_BB[square];
    piece_boards[color][type] ^= mask;
    piece_co[color] ^= mask;
    zobrist_hash ^= PIECE_ZOBRIST[color][type][square];
    piece_list[color][square] = NO_PIECE;
}
//move the piece in the bitboards
inline void Board::move_piece(int from, int to, int color, int type) {
    U64 mask = SQUARES_BB[from] | SQUARES_BB[to];
    piece_boards[color][type] ^= mask;
    piece_co[color] ^= mask;
    zobrist_hash ^= PIECE_ZOBRIST[color][type][from];
    zobrist_hash ^= PIECE_ZOBRIST[color][type][to];
    piece_list[color][from] = NO_PIECE;
    piece_list[color][to] = type;
}

//add the piece to the bitboards
inline void Board::nz_add_piece(int square, int color, int type) {
    U64 mask = SQUARES_BB[square];
    piece_boards[color][type] |= mask;
    piece_co[color] |= mask;
    piece_list[color][square] = type;
}
//removing the piece from the bitboards
inline void Board::nz_remove_piece(int square, int color, int type) {
    U64 mask = SQUARES_BB[square];
    piece_boards[color][type] ^= mask;
    piece_co[color] ^= mask;
    piece_list[color][square] = NO_PIECE;
}
//move the piece in the bitboards
inline void Board::nz_move_piece(int from, int to, int color, int type) {
    U64 mask = SQUARES_BB[from] | SQUARES_BB[to];
    piece_boards[color][type] ^= mask;
    piece_co[color] ^= mask;
    piece_list[color][from] = NO_PIECE;
    piece_list[color][to] = type;
}

//play the move
void Board::push(Moves move){
    int opp_col = turn ^ 1;
    //add position to position history (before changing the position)
    Position pos = Position(castling_rights, ep_square);
    zobrist_history.push_back(zobrist_hash);
    //Reset en passant square   
    if (ep_square != NO_SQ) {
        zobrist_hash ^= EP_ZOBRIST[(ep_square & 7)];
        ep_square = NO_SQ;
    } 
    switch(move.flag) {
        //No flag
        case NO_FLAG:
            move_piece(move.from_square, move.to_square, turn, move.piece);
            break;
        
        //Capture flag
        case CAPTURE_F:
            move.captured = piece_list[opp_col][move.to_square];
            remove_piece(move.to_square, opp_col, move.captured);
            move_piece(move.from_square, move.to_square, turn, move.piece);
            break;
        
        //Promote (without capture) flag
        case PROMOTE_F:
            remove_piece(move.from_square, turn, move.piece);
            add_piece(move.to_square, turn, move.promoted);
            break;
        
        //Promote (with capture) flag
        case PROMOTE_CAP_F:
            move.captured = piece_list[opp_col][move.to_square];
            remove_piece(move.to_square, opp_col, move.captured);
            remove_piece(move.from_square, turn, move.piece);
            add_piece(move.to_square, turn, move.promoted);
            break;
        
        //En passant flag
        case EN_PASSANT_F:
            move.captured = PAWN_I;
            int pawn_shift;
            turn == WHITE ? pawn_shift = 8 : pawn_shift = -8;
            move_piece(move.from_square, move.to_square, turn, move.piece);
            remove_piece(move.to_square + pawn_shift, opp_col, move.captured);
            break;
        
        //Double Pawn Push flag (needed to update en passant square)
        case DOUBLE_PAWN_F:
            move_piece(move.from_square, move.to_square, turn, move.piece);
            turn == WHITE ? ep_square = move.to_square + 8 : ep_square = move.to_square - 8;
            zobrist_hash ^= EP_ZOBRIST[(ep_square & 7)];
            break;
        
        
        //Kingside Castle flag
        case KS_F:
            move_piece(move.from_square, move.to_square, turn, move.piece);
            move_piece(move.to_square+1, move.from_square+1, turn, ROOK_I);
            if (castling_rights[turn][KINGSIDE_I]) {
                castling_rights[turn][KINGSIDE_I] = false;
                zobrist_hash ^= CASTLING_ZOBRIST[turn][KINGSIDE_I];
            }
            if (castling_rights[turn][QUEENSIDE_I]) {
                castling_rights[turn][QUEENSIDE_I] = false;
                zobrist_hash ^= CASTLING_ZOBRIST[turn][KINGSIDE_I];
            }
            break;
        
        //Queenside Castle flag
        case QS_F:
            move_piece(move.from_square, move.to_square, turn, move.piece);
            move_piece(move.to_square-2, move.from_square-1, turn, ROOK_I);
            if (castling_rights[turn][KINGSIDE_I]) {
                castling_rights[turn][KINGSIDE_I] = false;
                zobrist_hash ^= CASTLING_ZOBRIST[turn][KINGSIDE_I];
            }
            if (castling_rights[turn][QUEENSIDE_I]) {
                castling_rights[turn][QUEENSIDE_I] = false;
                zobrist_hash ^= CASTLING_ZOBRIST[turn][KINGSIDE_I];
            }
            break;
    }

    //Switch Castling Rights
    //Rooks moved or captured check
    //h1 rook
    if (castling_rights[WHITE][KINGSIDE_I] && !(piece_boards[WHITE][ROOK_I] & SQUARES_BB[63])) {
        castling_rights[WHITE][KINGSIDE_I] = false;
        zobrist_hash ^= CASTLING_ZOBRIST[WHITE][KINGSIDE_I];
    }
    //a1 rook
    if (castling_rights[WHITE][QUEENSIDE_I] && !(piece_boards[WHITE][ROOK_I] & SQUARES_BB[56])) {
        castling_rights[WHITE][QUEENSIDE_I] = false;
        zobrist_hash ^= CASTLING_ZOBRIST[WHITE][QUEENSIDE_I];
    }
    //h8 rook
    if (castling_rights[BLACK][KINGSIDE_I] && !(piece_boards[BLACK][ROOK_I] & SQUARES_BB[7])) {
        castling_rights[BLACK][KINGSIDE_I] = false;
        zobrist_hash ^= CASTLING_ZOBRIST[BLACK][KINGSIDE_I];
    }
    //a8 rook
    if (castling_rights[BLACK][QUEENSIDE_I] && !(piece_boards[BLACK][ROOK_I] & SQUARES_BB[0])) {
        castling_rights[BLACK][QUEENSIDE_I] = false;
        zobrist_hash ^= CASTLING_ZOBRIST[BLACK][QUEENSIDE_I];
    }
    //white king moved
    if (!(piece_boards[WHITE][KING_I] & SQUARES_BB[60])) {
        if (castling_rights[WHITE][KINGSIDE_I]) {
            castling_rights[WHITE][KINGSIDE_I] = false;
            zobrist_hash ^= CASTLING_ZOBRIST[WHITE][KINGSIDE_I];
        }
        if (castling_rights[WHITE][QUEENSIDE_I]) {
            castling_rights[WHITE][QUEENSIDE_I] = false;
            zobrist_hash ^= CASTLING_ZOBRIST[WHITE][KINGSIDE_I];
        }
    }
    //black king moved
    if (!(piece_boards[BLACK][KING_I] & SQUARES_BB[4])) {
        if (castling_rights[BLACK][KINGSIDE_I]) {
            castling_rights[BLACK][KINGSIDE_I] = false;
            zobrist_hash ^= CASTLING_ZOBRIST[BLACK][KINGSIDE_I];
        }
        if (castling_rights[BLACK][QUEENSIDE_I]) {
            castling_rights[BLACK][QUEENSIDE_I] = false;
            zobrist_hash ^= CASTLING_ZOBRIST[BLACK][KINGSIDE_I];
        }
    }
    pos.move = move;
    pos.halfmove_clock = halfmove_clock;
    position_history.push(pos);
    //swap turn
    turn = opp_col;
    //reset halfmove clock
    if (move.piece == PAWN_I || move.flag == CAPTURE_F || move.flag == PROMOTE_CAP_F) {
        halfmove_clock = 0;
    }
    else {halfmove_clock++;}
    
    zobrist_hash ^= TURN_ZOBRIST;
}

//unmake the move
void Board::pop(){
    int opp_col = turn ^ 1;
    Position pos = position_history.top();
    position_history.pop();
    U64 pos_hash = zobrist_history.back();
    zobrist_history.pop_back();
    //switch castling rights
    castling_rights[WHITE][KINGSIDE_I] = pos.castling_rights[WHITE][KINGSIDE_I];
    castling_rights[WHITE][QUEENSIDE_I] = pos.castling_rights[WHITE][QUEENSIDE_I];
    castling_rights[BLACK][KINGSIDE_I] = pos.castling_rights[BLACK][KINGSIDE_I];
    castling_rights[BLACK][QUEENSIDE_I] = pos.castling_rights[BLACK][QUEENSIDE_I];
    //switch en passant square
    ep_square = pos.ep_square;
    switch(pos.move.flag) { 
        //No flag
        case NO_FLAG:
            nz_move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
            break;

        case DOUBLE_PAWN_F:
            nz_move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
            break;
        //Capture flag
        case CAPTURE_F:
            nz_add_piece(pos.move.to_square, turn, pos.move.captured);
            nz_move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
            break;
        
        //Kingside Castle flag
        case KS_F:
            nz_move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
            nz_move_piece(pos.move.from_square+1, pos.move.to_square+1, opp_col, ROOK_I);
            break;
        
        //Queenside Castle flag
        case QS_F:
            nz_move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
            nz_move_piece(pos.move.from_square-1, pos.move.to_square-2, opp_col, ROOK_I);
            break;
        
        //Promote (without capture) flag
        case PROMOTE_F:
            nz_add_piece(pos.move.from_square, opp_col, pos.move.piece);
            nz_remove_piece(pos.move.to_square, opp_col, pos.move.promoted);
            break;
        
        //Promote (with capture) flag
        case PROMOTE_CAP_F:
            nz_add_piece(pos.move.to_square, turn, pos.move.captured);
            nz_add_piece(pos.move.from_square, opp_col, pos.move.piece);
            nz_remove_piece(pos.move.to_square, opp_col, pos.move.promoted);
            break;
        
        //En passant flag
        case EN_PASSANT_F:
            int pawn_shift;
            opp_col == WHITE ? pawn_shift = 8 : pawn_shift = -8;
            nz_move_piece(pos.move.to_square, pos.move.from_square, opp_col, pos.move.piece);
            nz_add_piece(pos.move.to_square + pawn_shift, turn, pos.move.captured);
            break;      
    }

    turn = opp_col;
    halfmove_clock = pos.halfmove_clock;
    zobrist_hash = pos_hash;
}

void Board::push_null(){
    //switch the side moving
    position_history.push(Position(ep_square, zobrist_hash));
    turn ^= 1; 
    zobrist_hash ^= TURN_ZOBRIST;
    ep_square = NO_SQ;
}
void Board::pop_null(){
    //switch the side moving
    Position pos = position_history.top();
    position_history.pop();
    turn ^= 1; 
    ep_square = pos.ep_square;
    zobrist_hash = pos.zobrist_hash;
}