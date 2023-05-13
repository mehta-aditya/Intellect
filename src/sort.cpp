#include "engine.hpp"
#include "eval.hpp"
#include "attacks.hpp"

enum ORDER {
    CAPTURE_O = 4000,
    PROMOTION_O = 3500,
    KILLER_O = 3500, 
    COUNTERMOVE_O = 200,
    TT_O = 5000,
    PV_O = 6000,
    SEE_O = 1000 //not a bonus as this is subtracted if SEE is negative
};
//table used to compute move ordering for captures
constexpr int MVV_LVA_TABLE[6][6] = {
    {15, 14, 13, 12, 11, 10},  //victim P, attacker P, N, B, R, Q, K
    {25, 24, 23, 22, 21, 20},  //victim N, attacker P, N, B, R, Q, K
    {35, 34, 33, 32, 31, 30},  //victim B, attacker P, N, B, R, Q, K
    {45, 44, 43, 42, 41, 40},  //victim R, attacker P, N, B, R, Q, K
    {55, 54, 53, 52, 51, 50},  //victim Q, attacker P, N, B, R, Q, K
    {100, 100, 100, 100, 100, 100} //victim K, attacker P, N, B, R, Q, K
}; 

//score moves for negamax movegen
void Engine::score_moves(Board &board, vector<Moves> &moves, Moves tt_move, int ply){
    for (Moves &move : moves) {
        if (move == pv[0][ply]) {move.order = PV_O;}
        else if (move == tt_move) {move.order = TT_O;}
        else if (move.flag == CAPTURE_F || move.flag == EN_PASSANT_F) {
            move.captured = board.piece_at(move.to_square, board.turn^1);
            move.order = (CAPTURE_O + MVV_LVA_TABLE[move.captured][move.piece]);
            }
        else if (move == killers[0][ply]) {move.order = KILLER_O;}
        else if (move == killers[1][ply]) {move.order = KILLER_O-1;}
        else {
            if (move.flag == NO_FLAG || move.flag == DOUBLE_PAWN_F) {move.order = history[board.turn][move.piece][move.to_square] + MG_PST_VALUES[move.piece][move.to_square];}
            else if (move.flag == PROMOTE_F) {move.order = PROMOTION_O + PIECE_VALUES[move.promoted];}
            else if (move.flag == PROMOTE_CAP_F) {move.order = PROMOTION_O + PIECE_VALUES[move.promoted] + MVV_LVA_TABLE[board.piece_at(move.to_square, board.turn^1)][move.piece];}
            //give countermove bonus
            if (countermoves[board.turn][move.from_square][move.to_square] == move) {move.order += COUNTERMOVE_O;}   
        }
    }
}

//score moves for quiesce movegen
void Engine::score_quiesce_moves(Board &board, vector<Moves> &moves, Moves tt_move) {
    for (Moves &move : moves) {
        if (move == tt_move) {move.order = TT_O;}
        else if (move.flag == CAPTURE_F || move.flag == EN_PASSANT_F) {move.order = CAPTURE_O + MVV_LVA_TABLE[board.piece_at(move.to_square, board.turn^1)][move.piece];}
        else if (move.flag == PROMOTE_CAP_F) {move.order = PROMOTION_O + PIECE_VALUES[move.promoted] + MVV_LVA_TABLE[board.piece_at(move.to_square, board.turn^1)][move.piece];}
    }
}

//static engine evaluation
//used to prune captures in quiesce search
bool Engine::see(Board &board, Moves move, int threshold){
    int color = board.turn^1;
    int value = PIECE_VALUES[board.piece_list[color][move.to_square]];
    if (move.flag == PROMOTE_CAP_F) {value += PIECE_VALUES[move.promoted] - PIECE_VALUES[move.piece];}
    value -= threshold;
    if (value < 0) {return false;}
    value -= PIECE_VALUES[move.piece];
    if (value >= 0) {return true;}
    int depth = 0;
    int from_square = move.from_square, to_square = move.to_square;
    U64 blockers = board.piece_co[WHITE] | board.piece_co[BLACK];
    //play the move
    blockers ^= SQUARES_BB[move.from_square];
    blockers |= SQUARES_BB[move.to_square];
    if (move.flag == EN_PASSANT_F) {blockers ^= SQUARES_BB[board.ep_square];}
    int captured;
    U64 temp;
    U64 attack_board = (board.attackers_to(move.to_square, board.turn, blockers) | board.attackers_to(move.to_square, color, blockers)) & blockers;
    U64 diag_pieces = board.piece_boards[WHITE][BISHOP_I] | board.piece_boards[WHITE][QUEEN_I] | board.piece_boards[BLACK][BISHOP_I] | board.piece_boards[BLACK][QUEEN_I];
    U64 line_pieces = board.piece_boards[WHITE][ROOK_I] | board.piece_boards[WHITE][QUEEN_I] | board.piece_boards[BLACK][ROOK_I] | board.piece_boards[BLACK][QUEEN_I];

    while (true) {
        temp = attack_board & board.piece_co[color];
        if (!temp) {break;}
        for (captured = PAWN_I; captured <= KING_I; captured++) {
        if (temp & board.piece_boards[color][captured]) {break;}
        }
        blockers ^= SQUARES_BB[get_lsb(temp & board.piece_boards[color][captured])];

        if (captured == PAWN_I || captured == BISHOP_I || captured == QUEEN_I) {
        attack_board |= Attacks::get_diag_attacks(move.to_square, blockers) & diag_pieces;
        }
            
        if (captured == ROOK_I || captured == QUEEN_I) {
        attack_board |= Attacks::get_line_attacks(move.to_square, blockers) & line_pieces;
        }
        attack_board &= blockers;
        color ^= 1;
        value = -value - 1 - PIECE_VALUES[captured];
        if (value >= 0) {
        if (captured == KING_I && (attack_board & board.piece_co[color])) {
            color ^= 1;
        }
        break;
        }
    }
    return board.turn != color;
}