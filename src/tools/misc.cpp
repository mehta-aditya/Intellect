
#include "misc.hpp"
#include <cmath>
//Starting Eval Params
int MG_MOBILITY[6] = {0, 1, 3, 5, 1, 0};
int EG_MOBILITY[6] = {0, 2, 2, 2, 8, 4};
int MAX_PIECE_BITS[6] = {4, 8, 13, 14, 27, 8};
int MG_DOUBLED_VALUE = -20;
int EG_DOUBLED_VALUE = -3;
int MG_ISOLATED_VALUE = 29;
int EG_ISOLATED_VALUE = 20;

int KING_ATTACKS[6] = {0, -7, 11, 4, 3, 0};

int PASSED_VALUE[8] = {4, 16, 4, 3, 8, 10, -1, -22, }; //From white point of view
int EG_PASSED_BONUS = -16;

int MG_PIECE_VALUES[6] = {96, 312, 330, 530, 999, 20000, };
int EG_PIECE_VALUES[6] = {105, 308, 335, 540, 992, 20000, };

int MG_BISHOP_PAIR = 1;
int EG_BISHOP_PAIR = 20;

int MG_PST_VALUES[6][64] =
{
{
0, 0, 0, 0, 0, 0, 0, 0, 
33, 73, 12, 72, 60, 66, -3, -27,
-38, -15, -3, 5, 3, 80, -9, -42, 
-21, 1, -10, -2, 3, -23, -11, -17,
-36, -5, -13, 1, -1, -5, -24, -52, 
-32, -22, -3, -16, -1, -21, 5, -21,
-23, 1, -25, -25, -15, 24, 28, -19, 
0, 0, 0, 0, 0, 0, 0, 0,
},
{
-232, -111, -83, -79, 117, -85, -44, -58, 
-77, 12, 73, 56, 75, 107, -32, -1,
4, 32, 30, 75, 116, 172, 102, 37, 
5, 18, 13, 45, 30, 29, 16, 47, 
4, -5, 32, 24, 36, 10, 14, -18, 
-23, -3, 22, 21, 6, 19, 11, -22,
-31, -13, 14, 11, 16, 33, -1, 6, 
-56, -13, -51, -12, -23, -10, -15, -15,
},
{
-29, -55, -77, -71, -85, -43, 42, -35,
10, 19, -4, 46, -21, 78, 5, 18,
-17, 3, 53, 26, 62, 82, 69, 20, 
-12, 7, 32, 34, 30, 4, 6, 27, 
-19, 25, 14, 50, 37, 14, 15, -13,
33, 47, 27, 23, 23, 44, 20, 27, 
-18, 45, 9, 8, 33, 74, 67, 28,
33, 22, 18, 17, 11, 15, -2, 10, 
},
{
-4, 29, 0, 52, 84, 43, -5, 26,
-18, -24, 6, 27, 21, 107, -6, 0, 
-4, -30, -7, 23, 30, 36, 52, 44, 
-51, -17, -41, 32, -37, 29, -23, -20,
-54, -40, -28, -57, -8, -42, -60, -42, 
-12, -63, -16, -10, -31, 2, 26, -1,
-26, -51, -27, -26, -35, -13, -5, -92, 
-15, -9, 3, 11, 9, 7, -55, -24,
},
{
-53, 30, 60, 36, 30, 67, 31, 51, 
-48, -41, -28, -65, -71, 37, 33, 96,
-14, -55, -29, 3, 24, 56, -10, 28, 
-11, -40, -9, -41, -9, -16, 21, -9, 
-8, -39, -38, -26, -12, -19, 14, -30,
-50, -8, -7, -12, -15, -5, -18, -27, 
-13, -1, -1, 3, 19, 33, 40, 5, 
-1, -2, 5, 14, -17, 2, -59, -15,
}, 
{
-105, 72, 4, 34, -68, -11, 51, -28,
-20, -2, 18, -31, -34, 29, -81, 12, 
27, 10, -6, 16, -26, 60, 45, 4, 
-22, 22, -43, -40, -80, -29, -9, -46, 
-5, 19, 34, -26, -42, -58, -47, -29,
-7, 10, -3, -58, -72, -28, -6, -1, 
-28, -17, -27, -69, -64, -8, 4, 2, 
-55, 34, 22, -71, 6, -31, 24, 8,
},
};

//endgame pst table
int EG_PST_VALUES[6][64] =
{
{
0, 0, 0, 0, 0, 0, 0, 0, 
193, 159, 167, 143, 109, 158, 186, 188,
119, 110, 88, 74, 64, 35, 73, 101, 
37, 22, 14, 5, -9, 9, 19, 8,
11, 6, -9, -17, -18, -13, 2, -4, 
7, 19, -22, 6, -12, -3, -6, -8,
14, 12, 15, 17, 10, -5, -9, -11,
0, 0, 0, 0, 0, 0, 0, 0, 
},
{
-56, -40, -62, 16, -47, -3, -14, -64,
30, -21, -57, -56, -12, -31, 5, -31, 
-53, -26, 16, -25, -1, -32, 0, -13,
4, 16, 24, 41, 37, 41, 24, -37, 
5, 22, 15, 27, 19, 6, 18, -27,
-41, 1, -6, 25, 31, -2, -7, 9, 
-43, -55, -9, 14, -3, 14, -11, -5, 
-60, -17, 10, 6, -10, -31, -23, -15,
},
{
-1, -51, -17, -1, -27, -4, -7, 10, 
-50, -14, -16, -27, 6, -10, -2, -7, 
-2, -2, -20, 4, 3, 3, -30, 13,
19, 2, -6, 25, 8, -2, -8, -21, 
-12, -13, 4, 14, 3, 15, -19, -45,
-15, 2, -4, 20, 14, 6, 11, -12, 
16, -14, -13, 30, -6, 8, -11, -13, 
-25, -10, -1, 4, -1, -25, -19, -41,
},
{
25, 12, -10, -7, 7, -5, -9, 3, 
-2, 16, 6, 6, 2, -16, 12, 0,
11, 28, 8, -2, -20, -27, -19, -17, 
14, 3, 14, -15, 11, 0, -17, -2, 
10, 20, -14, 13, -8, -4, -9, -21,
2, 25, -6, -16, 7, -5, -53, -9, 
11, -3, 16, 1, -10, -15, 2, -25,
0, -5, -5, -7, -17, -10, -2, -19, 
},
{
40, 58, 7, 38, -34, 24, 55, 67,
45, 22, 30, 78, 69, 63, 39, -43, 
-9, -28, 62, 6, 74, 38, 46, -25,
-3, 19, -20, 60, 22, 25, 41, 7, 
-20, 35, 64, 11, 25, 6, 65, 41,
2, -19, 26, -3, 13, 25, 53, 40, 
22, 14, -16, 18, -23, -23, -61, 17, 
-10, -23, 17, -17, 26, -50, 34, 3,
},
{
-119, 14, -28, -26, -37, 28, 4, -66,
-2, 32, -30, -16, 22, 3, 24, 9,
-33, 0, 37, -2, 17, 22, 40, 24, 
-29, 6, 19, 36, 14, 28, 19, 17,
-48, -22, 4, 27, 15, 23, 5, -10, 
-4, -13, 4, 23, 30, 17, -9, -19,
-37, -4, 22, 28, 28, 6, -7, -18, 
-23, -44, -16, 2, -17, -24, -37, -54,
},
};

int KING_EDGE[64] = 
{
  -100,  -95,  -90,  -90,  -90,  -90,  -95,  -100,  
  -95,  -50,  -50,  -50,  -50,  -50,  -50,  -95,  
  -90,  -50,  -10,  -10,  -10,  -10,  -50,  -90,  
  -90,  -50,  -10,    0,    0,  -10,  -50,  -90,  
  -90,  -50,  -10,    0,    0,  -10,  -50,  -90,  
  -90,  -50,  -10,  -10,  -10,  -10,  -50,  -90,  
  -95,  -50,  -50,  -50,  -50,  -50,  -50,  -95,  
  -100,  -95,  -90,  -90,  -90,  -90,  -95,  -100,
};

bool improving;
double best_e;
double K_VAL = 1.6;
int POSITIONS_NUM = 100000;
int TUNING_DELTA = 1;

namespace Tuner {
    U64 DOUBLED_MASK[64];
    U64 ISOLATED_MASK[64];
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

        ISOLATED_MASK[square] = PASSED_MASK[WHITE][square] & ~Attacks::FILES_BB[square&7];
        DOUBLED_MASK[square] = Attacks::FILES_BB[square&7];

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
    float value = 0;
    int game_phase = 0;
    int mg_value[2] = {0,0};
    int eg_value[2] = {0,0};
    U64 mobility_board;
    int square, mobility_squares;
    U64 piece_board; //copy of piece board
    
    int king_squares[2] = {get_lsb(board.piece_boards[WHITE][KING_I]),
                            get_lsb(board.piece_boards[BLACK][KING_I])};

    for (int c = WHITE; c <= BLACK; c++) {
        for (int p = PAWN_I; p <= KING_I; p++){
            piece_board = board.piece_boards[c][p];
            //bishop pair
            if (p == BISHOP_I && Attacks::count_bits(piece_board) > 1) {
                mg_value[c] += MG_BISHOP_PAIR;
                eg_value[c] += EG_BISHOP_PAIR;
            }

            while (piece_board) {
                square = pop_lsb(&piece_board);
                
                if (p != PAWN_I && p != KING_I) {
                    mobility_board = board.attackers_from(square, c, p);
                    //mobility value
                    mobility_squares = Attacks::count_bits(mobility_board);
                    mg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*MG_MOBILITY[p];
                    eg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*EG_MOBILITY[p];  
                    //king safety values
                    mg_value[c] += Attacks::count_bits(mobility_board & Attacks::KING_ATTACKS[king_squares[c^1]]) * KING_ATTACKS[p];
                    //update phase value based on pieces present
                    game_phase += PHASE_VALUES[p];
                } 
                //material value
                mg_value[c] += MG_PIECE_VALUES[p];
                eg_value[c] += EG_PIECE_VALUES[p];

                //positional value
                mg_value[c] += MG_PST_VALUES[p][flip_board[c][square]];
                eg_value[c] += EG_PST_VALUES[p][flip_board[c][square]];        

            }
        }
        //pawn structure eval
        //doubled pawn eval
        if (DOUBLED_MASK[square] & board.piece_boards[c][PAWN_I]) {
            mg_value[c] -= MG_DOUBLED_VALUE;
            eg_value[c] -= EG_DOUBLED_VALUE;
        }
        //isolated pawn eval
        if (!(ISOLATED_MASK[square] & board.piece_boards[c][PAWN_I])) {
            mg_value[c] -= MG_ISOLATED_VALUE;
            eg_value[c] -= EG_ISOLATED_VALUE;
        }
        
        //passed pawn eval
        if (!(PASSED_MASK[c][square] & board.piece_boards[c^1][PAWN_I])) {
            int rank = flip_board[c][square]/8;
            mg_value[c] += PASSED_VALUE[rank];
            eg_value[c] += PASSED_VALUE[rank] + EG_PASSED_BONUS; 
        }
        
        //if only the king is left push the king to the edge of the board
        if (!(board.piece_co[c] ^ board.piece_boards[c][KING_I])) {
            eg_value[c] += KING_EDGE[king_squares[c]];
        }    
    }

    int phase = (game_phase * 256 + (MAX_PHASE / 2)) / MAX_PHASE;
    value = ((mg_value[WHITE]-mg_value[BLACK]) * phase + (eg_value[WHITE]-eg_value[BLACK]) * (256-phase))/256;
    return value;
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
        //for (int p = PAWN_I; p <= KING_I; p++) {
            // tune_parameter(MG_PIECE_VALUES, p); 
            // tune_parameter(EG_PIECE_VALUES, p); 
            // tune_parameter(MG_MOBILITY, p); 
            // tune_parameter(EG_MOBILITY, p); 
            // tune_parameter(KING_ATTACKS, p);
            // for (int s = 0; s < 64; s++) {
            //     if (p == PAWN_I && (s < 8 || s > 64-8)) {
            //         continue;
            //     }
            //     tune_parameter(MG_PST_VALUES, p, s);
            //     tune_parameter(EG_PST_VALUES, p, s);
            // }
        //}
        // for (int f = 0; f < 8; f++) {
        //     tune_parameter(PASSED_VALUE[f]); 
        // }
        // tune_parameter(EG_PASSED_BONUS);
        // tune_parameter(MG_DOUBLED_VALUE);
        // tune_parameter(EG_DOUBLED_VALUE);
        // tune_parameter(MG_ISOLATED_VALUE);
        // tune_parameter(EG_ISOLATED_VALUE);
        tune_parameter(MG_BISHOP_PAIR);
        tune_parameter(EG_BISHOP_PAIR);
        if (TUNING_DELTA > 1) {TUNING_DELTA--;}
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
    //KING SAFETY
    cout << "{";
    for (int &val : KING_ATTACKS) {
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
    cout << EG_PASSED_BONUS << endl;
    //PAWN STRUCTURE
    cout << MG_DOUBLED_VALUE << endl;
    cout << EG_DOUBLED_VALUE << endl;
    cout << MG_ISOLATED_VALUE << endl;
    cout << EG_ISOLATED_VALUE << endl;
    cout << MG_BISHOP_PAIR << endl;
    cout << EG_BISHOP_PAIR << endl;
}

void calculate_k() {
    float best_k = 1;
    double best_e = 1e9;
    double res;
    for ( float k = 0.6; k < 2; k+=0.05)
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
