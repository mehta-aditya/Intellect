
#include "misc.hpp"
#include <cmath>
//Starting Eval Params
int MG_MOBILITY[6] = {1, 4, 4, 2, 1, 0};
int EG_MOBILITY[6] = {0, 2, 2, 2, 2, 0};
int MAX_PIECE_BITS[6] = {4, 8, 13, 14, 27, 8};
int MG_DOUBLED_VALUE = 6;
int EG_DOUBLED_VALUE = 10;
int MG_ISOLATED_VALUE = 15;
int EG_ISOLATED_VALUE = 20;

int KING_ATTACKS[6] = {0, 4, 4, 6, 10, 0};

int PASSED_VALUE[8] = {0, 20, 12, 7, 4, 3, 2, 0}; //From white point of view
int EG_PASSED_BONUS = 10;

int MG_PIECE_VALUES[6] = {96, 312, 330, 530, 999, 20000, };
int EG_PIECE_VALUES[6] = {105, 308, 335, 540, 992, 20000, };

int MG_BISHOP_PAIR = 1;
int EG_BISHOP_PAIR = 20;

float MG_PST_VALUES[6][64] = 
{
{
0, 0, 0, 0, 0, 0, 0, 0,
28, 68, 7, 67, 55, 61, -8, -32,
-43, -20, -8, 0, -2, 75, -14, -47, 
-26, -4, -15, -7, -2, -28, -16, -22,
-31, -5, -18, -4, -6, 0, -19, -47,
-27, -17, -8, -11, -1, -16, 5, -16, 
-28, -4, -30, -20, -20, 19, 23, -24,
0, 0, 0, 0, 0, 0, 0, 0,
}, 
{
-237, -106, -78, -84, 112, -90, -49, -63,
-72, 7, 68, 51, 70, 102, -37, -6,
9, 27, 25, 70, 111, 167, 97, 32, 
0, 13, 8, 40, 25, 24, 11, 42,
9, -10, 27, 19, 31, 10, 9, -13, 
-18, 2, 17, 21, 1, 14, 6, -17,
-26, -8, 14, 11, 21, 28, 4, 1,
-51, -8, -46, -7, -18, -5, -10, -10, 
},
{
-34, -50, -82, -66, -80, -38, 37, -40, 
15, 14, -4, 41, -26, 73, 0, 13,
-17, -2, 48, 21, 57, 77, 64, 15, 
-17, 12, 27, 29, 25, -1, 11, 22,
-14, 20, 9, 45, 32, 9, 20, -8, 
38, 42, 27, 18, 18, 39, 15, 32,
-13, 40, 14, 13, 28, 69, 62, 23, 
38, 27, 13, 22, 16, 20, 3, 15, 
},
{
-9, 24, -5, 47, 79, 38, 0, 21,
-23, -29, 1, 22, 16, 102, -11, -5, 
-9, -35, -12, 18, 25, 31, 47, 39,
-51, -22, -36, 27, -42, 24, -18, -25, 
-49, -45, -23, -62, -13, -47, -55, -37,
-7, -58, -11, -5, -26, 7, 21, 4,
-21, -46, -22, -21, -30, -18, -10, -87, 
-20, -4, -2, 6, 4, 2, -50, -19,
},
{
-58, 25, 55, 31, 35, 62, 26, 46,
-53, -46, -33, -70, -76, 32, 28, 91,
-19, -50, -34, -2, 19, 51, -15, 23, 
-16, -35, -14, -46, -14, -21, 16, -9,
-3, -44, -33, -26, -17, -14, 9, -25,
-45, -8, -2, -7, -10, -5, -13, -22, 
-8, -1, -1, 3, 14, 28, 45, 10,
4, 3, 10, 9, -12, 7, -54, -10,
},
{
-110, 67, 9, 29, -63, -16, 46, -23, 
-25, -7, 13, -36, -39, 24, -86, 7,
22, 5, -11, 11, -31, 55, 40, -1,
-27, 17, -48, -45, -85, -34, -14, -51, 
-10, 14, 39, -31, -47, -63, -52, -34,
-2, 15, 2, -53, -67, -33, -11, 4, 
-23, -22, -22, -64, -59, -3, 9, 7,
-50, 29, 17, -66, 11, -26, 19, 13,
},
};
//endgame pst table
float EG_PST_VALUES[6][64] = 
{
{
0, 0, 0, 0, 0, 0, 0, 0,
188, 154, 162, 138, 104, 153, 181, 183,
114, 105, 83, 69, 59, 30, 68, 96, 
32, 17, 9, 0, -14, 4, 14, 3,
6, 1, -14, -22, -23, -18, -3, -4, 
7, 14, -27, 1, -17, -8, -11, -13,
9, 7, 10, 12, 5, -10, -14, -16,
0, 0, 0, 0, 0, 0, 0, 0, 
},
{
-61, -45, -67, 11, -42, -8, -19, -59, 
35, -26, -62, -61, -17, -36, 0, -26,
-53, -21, 11, -30, -6, -37, -5, -18,
-1, 11, 19, 36, 32, 36, 19, -42, 
0, 17, 10, 22, 14, 1, 13, -22,
-36, -4, -11, 20, 26, -2, -2, 14, 
-38, -50, -4, 14, 2, 19, -6, 0,
-55, -12, 15, 11, -5, -26, -18, -10, 
},
{
-6, -46, -12, 4, -22, 1, -2, 5,
-45, -19, -11, -32, 1, -15, -7, -2,
-7, 3, -25, -1, -2, -2, -35, 8, 
14, 7, -11, 20, 3, -7, -3, -26,
-17, -18, -1, 9, -2, 10, -19, -40,
-10, -3, -9, 15, 9, 1, 16, -7, 
21, -19, -8, 25, -11, 3, -16, -8,
-20, -5, -6, 9, 4, -20, -14, -36,
},
{
20, 7, -15, -2, 2, -5, -4, -2,
-7, 11, 1, 1, -3, -21, 7, -5, 
6, 23, 3, -7, -25, -32, -24, -22,
9, -2, 9, -20, 6, -5, -22, -7,
5, 15, -19, 8, -13, -9, -14, -16, 
-3, 20, -11, -11, 2, -10, -58, -14,
16, 2, 11, 6, -5, -10, -3, -20,
-5, 0, -5, -12, -12, -15, 3, -24, 
},
{
35, 53, 2, 33, -29, 19, 50, 62,
40, 17, 25, 73, 64, 58, 34, -48, 
-9, -23, 57, 1, 69, 33, 41, -30, 
-8, 24, -25, 55, 17, 20, 36, 2, 
-15, 30, 59, 6, 20, 1, 60, 46,
7, -24, 21, 2, 18, 20, 48, 45, 
27, 9, -11, 13, -18, -28, -56, 22, 
-5, -18, 22, -12, 31, -45, 39, 8,
},
{
-124, 9, -33, -31, -32, 23, -1, -71,
-7, 27, -35, -21, 17, -2, 19, 4,
-38, -5, 32, -7, 12, 17, 35, 19, 
-34, 1, 14, 31, 9, 23, 14, 12, 
-43, -27, -1, 22, 10, 18, 0, -15,
1, -8, 9, 28, 25, 12, -14, -14, 
-32, 1, 27, 33, 33, 11, -7, -13,
-18, -39, -11, 7, -12, -19, -42, -49, 
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

namespace Tuner {
U64 DOUBLED_MASK[64];
U64 ISOLATED_MASK[64];
U64 PASSED_MASK[2][64];
U64 HALF_BOARD[2];
}

bool improving;
double best_e;
double K_VAL = 1.6;
int POSITIONS_NUM = 1000000;
int TUNING_DELTA = 1;
const int THREADS = 4;

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
    HALF_BOARD[WHITE] = Attacks::RANKS_BB[0] | Attacks::RANKS_BB[1] | Attacks::RANKS_BB[2] | Attacks::RANKS_BB[3];
    HALF_BOARD[BLACK] = Attacks::RANKS_BB[4] | Attacks::RANKS_BB[5] | Attacks::RANKS_BB[6] | Attacks::RANKS_BB[7];
}

//simple hand crafted eval
//currently has material, tuned piece square tables and mobility 
float Tuner::evaluation_tune(Board &board){
    float value = 0, game_phase = 0;
    float mg_value[2] = {0,0};
    float eg_value[2] = {0,0};
    int king_safety;
    U64 mobility_board;
    int square, mobility_squares;
    U64 piece_board; //copy of piece board
    
    int king_squares[2] = {get_lsb(board.piece_boards[WHITE][KING_I]),
                            get_lsb(board.piece_boards[BLACK][KING_I])};

    for (int c = WHITE; c <= BLACK; c++) {
        for (int p = PAWN_I; p <= KING_I; p++){
            piece_board = board.piece_boards[c][p];
            //bishop pair *retune values
            // if (p == BISHOP_I && Attacks::count_bits(piece_board) == 2) {
            //     mg_value[c] += MG_BISHOP_PAIR;
            //     eg_value[c] += EG_BISHOP_PAIR;
            // }

            while (piece_board) {
                square = pop_lsb(&piece_board);
                
                if (p != KING_I) {
                    mobility_board = board.attackers_from(square, c, p);
                    //mobility value
                    if (p == PAWN_I) {
                        mobility_squares = Attacks::count_bits(mobility_board & HALF_BOARD[c]);
                        mg_value[c] += (mobility_squares-1)*MG_MOBILITY[p];
                        eg_value[c] += (mobility_squares-1)*EG_MOBILITY[p]; 
                    }
                    else {
                        mobility_squares = Attacks::count_bits(mobility_board);
                        mg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*MG_MOBILITY[p];
                        eg_value[c] += (mobility_squares-MAX_PIECE_BITS[p]/2)*EG_MOBILITY[p]; 
                    }

 
                    //king safety values
                    king_safety = Attacks::count_bits(mobility_board & Attacks::KING_ATTACKS[king_squares[c^1]]) * KING_ATTACKS[p];
                    mg_value[c] += king_safety;
                    eg_value[c] += king_safety/2;
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
            mg_value[c] += KING_EDGE[king_squares[c]];
            eg_value[c] += KING_EDGE[king_squares[c]];
        }    
    }

    float phase = (game_phase * 256 + (MAX_PHASE / 2)) / MAX_PHASE;
    value = ((mg_value[WHITE]-mg_value[BLACK]) * phase + (eg_value[WHITE]-eg_value[BLACK]) * (256-phase))/256;
    return (board.turn == WHITE) ? value : -value;
}

//mean square error used for tuner
double Tuner::mean_square_error(float K, vector<TunerEntry> positions) {
    Board board;
    float value;
    double error = 0.0, result, sigmoid;
    int pos = 1; // keeps track of position number
    
    for (TunerEntry entry : positions) {
        board.set_fen(entry.fen);
        value = evaluation_tune(board);
        sigmoid = 1/(1 + pow(10, -K*value*0.01));

        error += pow(entry.result - sigmoid, 2);   
    }
    return error/positions.size();
}

// double Tuner::thread_mse(float K, vector<TunerEntry> positions, double* error) {
//     Board board;
//     float value;
//     double result, sigmoid;
//     int pos = 1; // keeps track of position number
    
//     for (TunerEntry entry : positions) {
//         board.set_fen(entry.fen);
//         value = evaluation_tune(board);
//         sigmoid = 1/(1 + pow(10, -K*value*0.01));

//         *error += pow(entry.result - sigmoid, 2);   
//     }
// }

// double Tuner::threaded_mse(float K, vector<TunerEntry> positions) {
//     double errors[THREADS] = {0};
//     vector<TunerEntry> batches[THREADS];

//     int split_size = positions.size() / THREADS;
//     int current = 0;
//     for (int t = 0; t < THREADS; t++) {

//         int i = 0;

//         while (i <= split_size && (i + current) < positions.size()) {
//             batches[t].push_back(positions[current + i]);
//             i++;
//         }

//         current += i;
//     }

//     if (THREADS > 1) {
//         std::vector<std::thread> workers;

//         for (int t = 0; t < THREADS; t++) {
//             workers.push_back(std::thread(thread_mse, K_VAL, &batches[t], &errors[t]));
//         }

//         // Wait for the workers to complete their work, and then join them
//         for (int t = 0; t < THREADS; t++) {
//             workers[t].join();
//         }

//     }
//     else { // Don't start up any workers. Just run the thread_batch function
//         thread_mse(K_VAL, &batches[0], &errors[0]);
//     }
//     double avg = 0;
//     for (int i = 0; i < THREADS; i++) {
//         avg += errors[i];
//     }
// 	return (avg / double(positions.size()));
// }


//tune a specific parameter we pass through
double Tuner::tune_parameter(int &arr, vector<TunerEntry> positions) {
    int delta_val = TUNING_DELTA; //value to change parameters by each iteration
    arr += delta_val;

    double error = mean_square_error(K_VAL, positions);
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
        error = mean_square_error(K_VAL, positions);
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

double Tuner::tune_parameter(int arr[], int i, vector<TunerEntry> positions) {
    int delta_val = TUNING_DELTA; //value to change parameters by each iteration
    arr[i] += delta_val;

    double error = mean_square_error(K_VAL, positions);
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
        error = mean_square_error(K_VAL, positions);
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

double Tuner::tune_parameter(float arr[][64], int i, int j, vector<TunerEntry> positions) {
    int delta_val = TUNING_DELTA; //value to change parameters by each iteration
    arr[i][j] += delta_val;

    double error = mean_square_error(K_VAL, positions);
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
        error = mean_square_error(K_VAL, positions);
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

vector<TunerEntry> Tuner::read_txt_file(int positions=POSITIONS_NUM) {
    vector<TunerEntry> position_fens;
    float value;
    fstream fen_file; 
    //positions.txt should have fens and at the end the game result
    //open the file and read fens to calculate mean square error
    int pos = 0;
    fen_file.open("src/tools/positions.txt",ios::in);
    if (fen_file.is_open()) {
        string fen;
        while(getline(fen_file, fen)){ 
            string outcome = fen.substr(fen.length()-5, 3);

            float result = std::stof(outcome);
            fen = fen.substr(0, fen.length()-5); //remove outcome from fen 
            if (fen.find("1-0") != std::string::npos) {result = 1.0;}
            else if (fen.find("0-1") != std::string::npos) {result = 0.0;}
            else {result = 0.5;}

            fen = fen.substr(0, fen.find("\"")-3);
            position_fens.push_back(TunerEntry(fen, result));
            //stop if we are done with the positions
            if (pos >= positions) {break;}
            pos++;
        }
        fen_file.close(); 
    }
    return position_fens;
}

//tuner for evaluation paramters
void Tuner::tune(int time) {
    init_eval();
    improving = true; //value to check if parameter is improving

    vector<TunerEntry> positions = read_txt_file(POSITIONS_NUM);
    K_VAL = calculate_k(positions);

    best_e = mean_square_error(K_VAL, positions);
    cout << "Starting error: ";
    printf("%.10f", best_e);
    cout << endl;
    auto start_time = steady_clock::now();  


    TUNING_DELTA = 1;
    while (improving && duration_cast<seconds>(steady_clock::now() - start_time).count() < time) {
        improving = false;
        //run through parameters
        for (int p = PAWN_I; p <= KING_I; p++) {
            // tune_parameter(MG_PIECE_VALUES, p); 
            // tune_parameter(EG_PIECE_VALUES, p); 
            tune_parameter(MG_MOBILITY, p, positions); 
            tune_parameter(EG_MOBILITY, p, positions); 
            // tune_parameter(KING_ATTACKS, p);
        //     for (int s = 0; s < 64; s++) {
        //         if (p == PAWN_I && (s < 8 || s > 64-8)) {
        //             continue;
        //         }

        //         float error = tune_parameter(MG_PST_VALUES, p, s, positions);
        //         printf("%.10f", error);
        //         cout << endl; 
        //         error = tune_parameter(EG_PST_VALUES, p, s, positions);
        //         printf("%.10f", error);
        //         cout << endl;
        //     }
        }


        for (int f = 0; f < 8; f++) {
            tune_parameter(PASSED_VALUE, f, positions); 
        }
        tune_parameter(EG_PASSED_BONUS, positions);
        tune_parameter(MG_DOUBLED_VALUE, positions);
        tune_parameter(EG_DOUBLED_VALUE, positions);
        tune_parameter(MG_ISOLATED_VALUE, positions);
        tune_parameter(EG_ISOLATED_VALUE, positions);
        // tune_parameter(MG_BISHOP_PAIR);
        // tune_parameter(EG_BISHOP_PAIR);
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
        for (float &val : MG_PST_VALUES[p]) {
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
        for (float &val : EG_PST_VALUES[p]) {
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

double Tuner::calculate_k(vector<TunerEntry> positions) {
    double best_k = 0.45;
    double best_e = Tuner::mean_square_error(best_k, positions);
    double res;
    for ( double k = 0.5; k < 2; k+=0.05) {
        res = Tuner::mean_square_error(k, positions);
        if (res < best_e) {
            best_k = k;
            best_e = res;
        }
    }
    return best_k;
}
