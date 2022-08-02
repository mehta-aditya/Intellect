#include "engine.hpp"
#include "eval.hpp"


enum ORDER {
    CAPTURE_O = 4000,
    PROMOTION_O = 3500,
    CASTLING_O = 3000,
    KILLER_O = 4016 //killer moves are more important than pawn captures
};
//table used to compute move ordering for captures
const int MVV_LVA_TABLE[6][6] = {
    {15, 14, 13, 12, 11, 10},  //victim P, attacker P, N, B, R, Q, K
    {25, 24, 23, 22, 21, 20},  //victim N, attacker P, N, B, R, Q, K
    {35, 34, 33, 32, 31, 30},  //victim B, attacker P, N, B, R, Q, K
    {45, 44, 43, 42, 41, 40},  //victim R, attacker P, N, B, R, Q, K
    {55, 54, 53, 52, 51, 50},  //victim Q, attacker P, N, B, R, Q, K
    {100, 100, 100, 100, 100, 100} //victim K, attacker P, N, B, R, Q, K
}; 


void Engine::score_moves(vector<Moves> &moves){
    for (Moves &move : moves) {
        if (move.from_square == killers[0].from_square && move.to_square == killers[0].to_square) {move.order = KILLER_O;}
        else if (move.from_square == killers[1].from_square && move.to_square == killers[1].to_square) {move.order = KILLER_O;}
        else if (move.flag == NO_FLAG || move.flag == DOUBLE_PAWN_F) {move.order = 0;}
        else if (move.flag == CAPTURE_F || move.flag == EN_PASSANT_F) {move.order = CAPTURE_O + MVV_LVA_TABLE[move.captured][move.piece];}
        else if (move.flag == KS_F || move.flag == QS_F) {move.order = CASTLING_O;}
        else if (move.flag == PROMOTE_F) {move.order = PROMOTION_O + PIECE_VALUES[move.promoted];}
        else if (move.flag == PROMOTE_CAP_F) {move.order = PROMOTION_O + PIECE_VALUES[move.promoted] + MVV_LVA_TABLE[move.captured][move.piece];}
    }
}

void Engine::score_quiesce_moves(vector<Moves> &moves) {
    for (Moves &move : moves) {
        if (move.flag == CAPTURE_F || move.flag == EN_PASSANT_F) {move.order = CAPTURE_O + MVV_LVA_TABLE[move.captured][move.piece];}
        else if (move.flag == PROMOTE_CAP_F) {move.order = PROMOTION_O + PIECE_VALUES[move.promoted] + MVV_LVA_TABLE[move.captured][move.piece];}
    }
}

