
#include "misc.hpp"
#include <cmath>
//Starting Eval Params
int MG_MOBILITY[6] = {0, 1, 3, 5, 1, 0};
int EG_MOBILITY[6] = {0, 2, 2, 2, 8, 4};
int MAX_PIECE_BITS[6] = {4, 8, 13, 14, 27, 8};

int PASSED_VALUE[8] = {0, 38, 26, 20, 15, 5, 2, 0, }; //From white point of view
int EG_PASSED_BONUS = 5;

int MG_PIECE_VALUES[6] = {96, 312, 330, 530, 999, 20000, };
int EG_PIECE_VALUES[6] = {105, 308, 335, 540, 992, 20000, };
int MG_PST_VALUES[6][64] = 
{
{
0, 0, 0, 0, 0, 0, 0, 0,
92, 128, 55, 89, 74, 132, 28, -5, 
-12, 1, 20, 25, 59, 62, 19, -14, 
-16, 7, 0, 15, 17, 6, 11, -17,
-33, 4, -7, 6, 11, 0, 4, -31, 
-32, -10, 2, -16, 9, -3, 35, -18,
-29, 5, -18, -17, -9, 30, 44, -16, 
0, 0, 0, 0, 0, 0, 0, 0,
},
{
-173, -83, -40, -55, 67, -91, -9, -101, 
-79, -35, 78, 42, 29, 68, 1, -11, 
-53, 66, 43, 71, 90, 135, 67, 50, 
-3, 11, 13, 59, 43, 63, 12, 28,
-7, 10, 22, 19, 34, 13, 15, -2,
-17, -15, 14, 16, 15, 15, 19, -22, 
-35, -59, -6, -9, 5, 12, -8, -13, 
-111, -15, -64, -27, -19, -28, -13, -17,
},
{
-23, -2, -76, -43, -19, -36, 1, -2,
-32, 22, -12, -7, 24, 65, 12, -41, 
-22, 31, 49, 34, 29, 56, 43, -8, 
-10, -1, 13, 44, 43, 31, 11, -8,
-12, 7, 7, 20, 28, 18, 4, -2,
6, 15, 15, 19, 20, 27, 24, 4, 
10, 21, 22, 6, 13, 27, 39, -5,
-27, -9, -8, -15, -7, -10, -33, -15, 
},
{
38, 42, 38, 45, 57, 9, 25, 37, 
21, 26, 52, 56, 74, 73, 32, 50, 
1, 13, 20, 42, 23, 51, 67, 22,
-18, -5, 1, 32, 18, 41, -14, -26, 
-42, -20, -6, -7, 11, -1, 0, -21, 
-51, -31, -10, -11, -3, 6, 1, -27,
-38, -22, -14, -15, -7, 17, 0, -77, 
-21, -7, -5, 11, 22, 13, -43, -24, 
},
{
-34, 6, 23, 18, 53, 50, 37, 51, 
-18, -45, -11, -5, -10, 63, 22, 60, 
-7, -23, 1, 14, 23, 50, 41, 53,
-21, -21, -22, -10, -7, 11, 4, -5, 
-11, -20, -15, -16, -8, -10, 5, -9, 
-20, 8, -5, -8, -11, -4, 20, 11,
-29, -2, 5, -4, 14, 9, -9, 7, 
5, -12, -11, 16, -21, -19, -37, -44, 
},
{
-71, 17, 22, -9, -62, -28, 8, 19, 
23, 5, -14, -1, -14, 2, -44, -35, 
-15, 30, -4, -10, -26, 12, 28, -16, 
-23, -14, -18, -21, -36, -19, -8, -30, 
-43, 5, -21, -33, -52, -50, -27, -57,
-20, -20, -28, -40, -50, -24, -21, -21, 
-5, 1, -2, -70, -49, -10, 3, 6,
-13, 30, 18, -60, 14, -30, 30, 8, 
},
};
//endgame pst table
int EG_PST_VALUES[6][64] =
{
{
0, 0, 0, 0, 0, 0, 0, 0, 
172, 179, 152, 128, 141, 138, 159, 181,
100, 102, 91, 73, 50, 59, 76, 88, 
38, 22, 17, -1, -8, 10, 21, 13, 
19, 15, -1, -13, -13, -14, -3, -7,
-2, 13, -12, 7, 2, -3, -7, -2, 
19, 14, 2, 16, 7, 6, 8, -1,
0, 0, 0, 0, 0, 0, 0, 0, 
},
{
-52, -44, -7, -34, -37, -33, -57, -93, 
-31, -14, -19, -8, -3, -31, -30, -46,
-18, -14, 16, 15, 5, -15, -25, -35, 
-11, 9, 16, 16, 20, 5, 2, -12,
-24, 0, 10, 31, 22, 11, -2, -12, 
-29, -9, -7, 21, 16, -9, -26, -16, 
-48, -26, -16, -11, 4, -14, -17, -50, 
-35, -45, -17, -9, -16, -12, -44, -58,
},
{
-8, -27, -5, -14, -1, -3, -11, -18,
-14, -10, 13, -6, -9, -7, 2, -8, 
-4, -14, -6, -5, 0, 0, -6, -2, 
-9, 3, 6, 3, 20, 4, 9, -4, 
0, 9, 15, 13, 1, 16, -9, -15,
-6, 3, 14, 16, 19, 7, -1, -21, 
-8, -12, -13, 5, 2, -3, -9, -33, 
-17, -15, -17, -11, -3, -10, 1, -23,
}, 
{
19, 16, 14, 9, 6, 6, 2, 1,
5, 7, 7, 5, 3, 9, 14, 9, 
3, 13, 13, 11, -2, 3, 1, -3,
10, 9, 7, 7, -4, 7, -7, 8, 
9, 11, 14, -2, 1, 0, -2, -17,
0, -6, -11, 5, -1, -6, -14, -10, 
0, -12, 6, -4, -15, -3, -13, -9, 
-3, 8, 9, -7, -11, -7, -2, -26, 
},
{
-3, 16, 16, 33, 21, 25, 12, 26,
-11, 26, 26, 35, 64, 31, 36, 6, 
-14, 12, 3, 55, 41, 41, 13, 3, 
9, 16, 18, 51, 51, 34, 63, 42, 
-12, 22, 13, 41, 25, 28, 45, 29,
-22, -21, 21, 12, 3, 11, 16, 11, 
-16, -17, -36, -22, -10, -29, -42, -26, 
-27, -22, -16, -37, 1, -26, -14, -47,
}, 
{
-80, -41, -12, -12, -17, 21, 10, -11,
-6, 23, 20, 23, 11, 44, 17, 5, 
4, 11, 17, 17, 14, 51, 38, 19, 
-14, 28, 18, 21, 20, 33, 32, 9,
-12, -10, 27, 30, 21, 29, 5, -5, 
-21, -9, 9, 27, 29, 16, 1, -3,
-33, -5, 10, 19, 8, 10, 1, -23, 
-47, -40, -15, -17, -22, -8, -30, -49, 
},
};

bool improving;
double best_e;
double K_VAL = 1;
int POSITIONS_NUM = 100000;
int TUNING_DELTA = 2;

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
            // string outcome = fen.substr(fen.length()-4, 3);
            // result = std::stof(outcome);
            // fen = fen.substr(0, fen.length()-5); //remove outcome from fen 
            if (fen.find("1-0") != std::string::npos) {result = 1.0;}
            else if (fen.find("0-1") != std::string::npos) {result = 0.0;}
            else {result = 0.5;}

            fen = fen.substr(0, fen.find("\"")-3);
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
            //tune_parameter(MG_PIECE_VALUES, p); 
            //tune_parameter(EG_PIECE_VALUES, p); 
            // tune_parameter(MG_MOBILITY, p); 
            // tune_parameter(EG_MOBILITY, p); 
            for (int s = 0; s < 64; s++) {
                if (p == PAWN_I && (s < 8 || s > 64-8)) {
                    continue;
                }
                tune_parameter(MG_PST_VALUES, p, s);
                tune_parameter(EG_PST_VALUES, p, s);
            }
        }
        // for (int f = 0; f < 8; f++) {
        //     tune_parameter(PASSED_VALUE[f]); 
        // }
        // tune_parameter(EG_PASSED_BONUS);
        //if (TUNING_DELTA >= 2) {TUNING_DELTA /= 2;}
        //if (POSITIONS_NUM <= 95000) {POSITIONS_NUM += 5000;}
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