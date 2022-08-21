#include "board.hpp"
#include <cstring>
#include <math.h>
#include <unordered_map>

#ifndef ENGINE_HPP
#define ENGINE_HPP

using TTFLAG = uint_fast8_t;

//Key Engine Parameters
const int MAX_DEPTH = 64;
const int MAX_TIME = 300000000;
const int TIME_DIVIDER = 50;
const int OVERHEAD_TIME = 50;
const int DEFUALT_TT_MB = 128;
const int QUIESCE_MAX_DEPTH = 5;
//Values for different pruning and reduction methods
const int REVERSE_FUTILITY_MARGIN = 100;
const int RAZORING_MARGIN = 200;
const int DELTA_MARGIN = 1000;
const int LMP_TABLE[8] = {0, 8, 10, 12, 15, 20, 22, 24}; //lmp move cutoff
const int FUTILITY_MARGIN[8] = {0, 300, 450, 600, 750, 900, 1050, 1200}; //futility margin
const int MAX_HISTORY_V = 3000; //based on move ordering values

//Values used in engine eval
enum VALUES {
    MAX = 20000,
    MATE_V = 10000,
    DRAW_V = 0
};

enum TTFlags : TTFLAG{
    TT_EXACT = 0,
    TT_BETA = 1,
    TT_ALPHA = 2
};

//Use this structure to sort
struct move_sort {
    inline bool operator() (const Moves& move1, const Moves& move2) const {
        return (move1.order > move2.order);
    }
};

//ponder, mate and nodes has not been implemented yet
struct EngineLimits {
    vector<Moves> search_moves;
    bool ponder, infinite;
    int co_time[2], co_inc[2];
    int move_time;
    int moves_to_go, depth, nodes, mate;
     //Constructor
    EngineLimits() {
        ponder = false, infinite = false;
        co_time[WHITE] = 0, co_time[BLACK] = 0, co_inc[WHITE] = 0, co_inc[BLACK] = 0;
        move_time = 0;
        moves_to_go = 0, depth = 0, nodes = 0, mate = 0;
    }  
};

//Use this structure for the transposition table
struct TTEntry{
    int value, depth;
    TTFLAG flag;
    Moves move;
    TTEntry(int v, Moves m, int d, uint_fast8_t f) {
        value = v;
        move = m;
        depth = d;
        flag = f;
    }
    TTEntry() {}
};
typedef unordered_map<U64, TTEntry> TTMAP;

class Engine{
    public: 
        //init
        bool stop = false;
        int nodes = 0;
        //used for triangular pv table
        int pv_len[MAX_DEPTH+1];
        Moves pv[MAX_DEPTH+1][MAX_DEPTH+1];
        //various heuristic tables and pruning
        Moves killers[2][MAX_DEPTH];
        Moves countermoves[64][64];
        int history[2][6][64]; //[turn][piece][to]
        int LMR_TABLE[MAX_DEPTH][64];

        //transposition table
        int TT_MAX_SIZE = 1e7;
        TTMAP tt_table;
        int set_tt_memory(TTMAP &tt_table, int megabytes);

        //search parameters
        int search_depth;
        int time_for_move;
        time_point<steady_clock> start_time;

        //engine.cpp
        //initialize engine table
        Engine() {
            TT_MAX_SIZE = set_tt_memory(tt_table, DEFUALT_TT_MB);
            for (int d = 0; d < MAX_DEPTH; d++) {
                for (int m = 0; m < 64; m++) {
                    LMR_TABLE[d][m] = max(d/5, 2) + m/10;
                }
            }
        }
        inline void update_pv(Moves &move, int ply);
        inline bool check_limits();

        inline int quiesce(Board &board, int alpha, int beta, int depth);
        inline int negamax(Board &board, int alpha, int beta, int depth, int ply, bool null);
        inline void iterative_deepening(Board& board);
        void search(Board& board, EngineLimits &limits);
        
        //eval.cpp
        static int evaluation(Board &board);
        void move_ordering(vector<Moves> &moves);

        //sort.cpp
        void score_moves(Board &board, vector<Moves> &moves, Moves tt_move, int ply);
        void score_quiesce_moves(vector<Moves> &moves, Moves tt_move);  
        bool see(Board &board, Moves move, int threshold);

        void halt() {
            stop = true;
        }
};

#endif