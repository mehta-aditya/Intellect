
#include "misc.hpp"
//Starting Eval Params
int MG_MOBILITY[6] = {0, 1, 3, 5, 1, 0};
int EG_MOBILITY[6] = {0, 2, 2, 2, 8, 4};
int MAX_PIECE_BITS[6] = {4, 8, 13, 14, 27, 8};

int PASSED_VALUE[8] = {0, 38, 23, 17, 12, 8, 5, 0, }; //From white point of view
int EG_PASSED_BONUS = 5;

int MG_PIECE_VALUES[6] = {102, 263, 330, 428, 951, 20000, };
int EG_PIECE_VALUES[6] = {168, 299, 345, 586, 975, 20000, };
int MG_PST_VALUES[6][64] = {
    //pawn
    {
      0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0
    },

    // knight
    {
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23
    },

    // bishop
    {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21   
    },

    // rook
    {
     32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26
    },

    // queen
    {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50
    },

    // king
    {
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14
    }
};

//endgame pst table
int EG_PST_VALUES[6][64] = {
    // pawn
    {
      0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0
    },

    // knight
    {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64
    },

    // bishop
    {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17 
    },

    // rook
    {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20
    },

    // queen
    {
     -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41 
    },

    // king
    {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
    }
};
bool improving;
double best_e;
double K_VAL = 0.45;
int POSITIONS_NUM = 5000;
int TUNING_DELTA = 10;

namespace Tuner {
U64 PASSED_MASK[2][64];

}

void Tuner::init_eval(){
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
float Tuner::evaluation_tune(Board &board){
    float value = 0, game_phase = 0;
    float mg_value[2] = {0,0};
    float eg_value[2] = {0,0};
    int square, mobility_squares;
    U64 piece_board; //copy of board

    for (int c = WHITE; c <= BLACK; c++) {
        for (int p = PAWN_I; p <= KING_I; p++){
            piece_board = board.piece_boards[c][p];
            while (piece_board) {
                square = pop_lsb(&piece_board);
                //mobility value
                mobility_squares = board.moves_at(square, c, p);
                mg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*MG_MOBILITY[p];
                eg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*EG_MOBILITY[p];   
                //material value
                mg_value[c] += MG_PIECE_VALUES[p];
                eg_value[c] += EG_PIECE_VALUES[p];

                //positional value
                mg_value[c] += MG_PST_VALUES[p][flip_board[c][square]];
                eg_value[c] += EG_PST_VALUES[p][flip_board[c][square]];        
                game_phase += PHASE_VALUES[p];
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
    return value;
    //return (board.turn == WHITE) ? value : -value;
}

//mean square error used for tuner
double Tuner::mean_square_error(float K=K_VAL, int positions=POSITIONS_NUM) {
    Board board;
    float value;
    double error = 0.0, result, sigmoid;
    int pos = 1; // keeps track of position number
    fstream fen_file; 
    //positions.txt should have fens and at the end the game result
    //open the file and read fens to calculate mean square error
    fen_file.open("src/tools/positions.txt",ios::in);
    if (fen_file.is_open()) {
        string fen;
        while(getline(fen_file, fen)){ 
            string outcome = fen.substr(fen.length()-4, 3);
            result = std::stof(outcome);
            fen = fen.substr(0, fen.length()-5); //remove outcome from fen 
            board.set_fen(fen);
            value = evaluation_tune(board);
            sigmoid = 1/(1 + pow(10, -K*value/400.0));

            error += pow(result - sigmoid, 2);
            //stop if we are done with the positions
            if (pos >= positions) {break;}
            pos++;
        }
        fen_file.close(); 
    }
    return error/positions;
}

//tune a specific parameter we pass through
double Tuner::tune_parameter(int &arr) {
    int delta_val = TUNING_DELTA; //value to change parameters by each iteration  
    arr += delta_val;
    double error = mean_square_error();
    cout << "New Error: ";
    printf("%.10f", error);
    cout << endl;
    //check if parameter increase helps
    if (error < best_e) {
        best_e = error;
        improving = true;
        cout << "parameter+" << endl;
    }
    else {
        arr -= delta_val*2;
        error = mean_square_error();
        //check if parameter decrease helps
        if (error < best_e) {
            best_e = error;
            improving = true;
            cout << "parameter-" << endl;
        }
        //keep parameter the same
        else {
            arr += delta_val;
        }
    }
    return error;
}

double Tuner::tune_parameter(int arr[], int i) {
    int delta_val = TUNING_DELTA; //value to change parameters by each iteration  
    arr[i] += delta_val;
    double error = mean_square_error();
    cout << "New Error: ";
    printf("%.10f", error);
    cout << endl;
    //check if parameter increase helps
    if (error < best_e) {
        best_e = error;
        improving = true;
        cout << "parameter+" << endl;
    }
    else {
        arr[i] -= delta_val*2;
        error = mean_square_error();
        //check if parameter decrease helps
        if (error < best_e) {
            best_e = error;
            improving = true;
            cout << "parameter-" << endl;
        }
        //keep parameter the same
        else {
            arr[i] += delta_val;
        }
    }
    return error;
}

double Tuner::tune_parameter(int arr[][64], int i, int j) {
    int delta_val = TUNING_DELTA; //value to change parameters by each iteration
    arr[i][j] += delta_val;

    double error = mean_square_error();
    cout << "New Error: ";
    printf("%.10f", error);
    cout << endl;
    //check if parameter increase helps
    if (error < best_e) {
        best_e = error;
        improving = true;
        cout << "parameter+" << endl;
    }
    else {
        arr[i][j] -= delta_val*2;
        error = mean_square_error();
        //check if parameter decrease helps
        if (error < best_e) {
            best_e = error;
            improving = true;
            cout << "parameter-" << endl;
        }
        //keep parameter the same
        else {
            arr[i][j] += delta_val;
        }
    }
    return error;
}


//tuner for evaluation paramters
void Tuner::tune(int time) {
    init_eval();
    improving = true; //value to check if parameter is improving
    best_e = mean_square_error();
    cout << "Starting error: ";
    printf("%.10f", best_e);
    cout << endl;
    auto start_time = steady_clock::now();  

    while (improving && duration_cast<seconds>(steady_clock::now() - start_time).count() < time) {
        improving = false;
        //run through parameters
        for (int p = PAWN_I; p <= KING_I; p++) {
            tune_parameter(MG_PIECE_VALUES, p); 
            tune_parameter(EG_PIECE_VALUES, p); 
            // tune_parameter(MG_MOBILITY, p); 
            // tune_parameter(EG_MOBILITY, p); 
            for (int s = 0; s < 64; s++) {
                tune_parameter(MG_PST_VALUES, p, s);
                tune_parameter(EG_PST_VALUES, p, s);
            }
        }
        // for (int f = 0; f < 8; f++) {
        //     tune_parameter(PASSED_VALUE[f]); 
        // }
        // tune_parameter(EG_PASSED_BONUS);
        if (TUNING_DELTA > 2) {TUNING_DELTA--;}
        if (POSITIONS_NUM <= 95000) {POSITIONS_NUM += 5000;}
    }
    //show table
    int i = 0;
    //run through parameters
    //MG MOBILITY
    cout << "{";
    for (int &val : MG_MOBILITY) {
        cout << val << ", ";
    }
    cout << "};" << endl;
    //EG MOBILITY
    cout << "{";
    for (int &val : EG_MOBILITY) {
        cout << val << ", ";
    }
    cout << "};" << endl;

    //MG PIECE
    cout << "{";
    for (int &val : MG_PIECE_VALUES) {
        cout << val << ", ";
    }
    cout << "};" << endl;
    //EG PIECE
    cout << "{";
    for (int &val : EG_PIECE_VALUES) {
        cout << val << ", ";
    }
    cout << "};" << endl;
    //MG PST
    cout << "{" << endl;
    for (int p = PAWN_I; p <= KING_I; p++) {
        cout << "{" << endl;
        for (int &val : MG_PST_VALUES[p]) {
            cout << val;  
            if (i >= 7) {
                i = 0;
                cout << ", " << endl;
            }
            else {
                cout << ", ";
                i++;
            }
        }
        cout << "}, " << endl;
    }
    cout << "};" << endl;
    //EG PST
    cout << "{" << endl;
    for (int p = PAWN_I; p <= KING_I; p++) {
        cout << "{" << endl;
        for (int &val : EG_PST_VALUES[p]) {
            cout << val;
            
            if (i >= 7) {
                i = 0;
                cout << ", " << endl;
            }
            else {
                cout << ", ";
                i++;
            }
            
        }
        cout << "}, " << endl;
    }
    cout << "};" << endl;
    //PASSED
    cout << "{";
    for (int &val : PASSED_VALUE) {
        cout << val << ", ";
    }
    cout << "};" << endl;
    //EG BONUS
    cout << EG_PASSED_BONUS;

}

void calculate_k() {
    float best_k = 1;
    double best_e = 1e9;
    double res;
    for ( float k = 0.01; k < 2; k+=0.01)
    {
	res = Tuner::mean_square_error(k, 100000);
    if (res < best_e) {
        best_k = k;
        best_e = res;
    }
	printf("%.10f: %.10f\n", k, res);
    }
    printf("Best K: %.10f: %.10f\n", best_k, best_e);
    cout << "Best K: " << best_k;
}