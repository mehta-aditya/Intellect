
#include "misc.hpp"
#include <cmath>
//Starting Eval Params
int MG_MOBILITY[6] = {0, 4, 4, 3, 1, 0, };
int EG_MOBILITY[6] = {0, 2, 2, 3, 6, 0, };
int MAX_PIECE_BITS[6] = {4, 8, 13, 14, 27, 8};
int MG_DOUBLED_VALUE = 2;
int EG_DOUBLED_VALUE = 8;
int MG_ISOLATED_VALUE = 13;
int EG_ISOLATED_VALUE = 10;

int KING_ATTACKS[6] = {0, 4, 6, 8, 10, 0};

int PASSED_VALUE[8] = {0, 30, 25, 20, 15, 13, 10, 0}; //From white point of view
int EG_PASSED_BONUS = 20;

int MG_PIECE_VALUES[6] = {96, 312, 330, 530, 999, 20000, };
int EG_PIECE_VALUES[6] = {105, 308, 335, 540, 992, 20000, };

int MG_BISHOP_PAIR = 9;
int EG_BISHOP_PAIR = 39;

int SEMIOPEN_KING_FILE_VALUE = 10;

float MG_PST_VALUES[6][64] =
{
{
0, 0, 0, 0, 0, 0, 0, 0,
-2, 38, -23, 37, 25, 31, -38, -62,
-61, -30, -26, -26, -20, 61, -32, -65,
-32, -14, -21, -15, -8, -36, -22, -40,
-45, -19, -20, -10, -4, -14, -33, -57,
-41, -31, -12, -21, -9, -28, -1, -32,
-38, -12, -32, -26, -20, 7, 21, -34, 
0, 0, 0, 0, 0, 0, 0, 0,
},
{
-265, -136, -76, -54, 130, -120, -79, -75,
-64, 1, 70, 39, 62, 88, -39, -32,
-1, 23, 3, 52, 97, 171, 67, 10,
18, 9, -4, 24, 3, 6, -5, 36,
9, -16, 27, 17, 27, 14, 3, -9,
-14, 0, 19, 19, 5, 20, 16, -15,
-20, -18, 22, 19, 21, 26, 10, 3, 
-49, -6, -40, 3, -12, -13, -6, -24,
},
{
-44, -76, -72, -96, -110, -50, 31, -38,
11, 10, 6, 51, -36, 79, 14, 37,
-17, -4, 30, 35, 37, 63, 94, 21,
-11, 2, 21, 21, 23, -11, 19, 18,
-4, 26, 5, 45, 24, 21, 6, -10,
36, 44, 29, 14, 26, 33, 25, 14,
-11, 52, 4, 21, 28, 63, 58, 29, 
46, 27, 29, 16, 20, 10, 27, 13,
},
{
-23, 10, -35, 17, 53, 68, -30, -9,
-37, -59, -25, -8, -14, 132, -25, -35,
-15, -49, -30, 0, 15, 9, 45, 9,
-57, -16, -34, 13, -38, 10, -28, -25,
-45, -47, -17, -56, -17, -45, -85, -55,
-17, -40, -17, -21, -30, -19, 23, -4,
-35, -70, -36, -43, -48, -16, -24, -89, 
-22, -18, -8, 0, 4, 0, -46, -19,
},
{
-88, 35, 73, 25, 33, 64, -4, 16,
-55, -54, -59, -100, -106, 14, -2, 75,
-23, -42, -42, -26, -7, 21, -45, -5,
-26, -41, -24, -52, -20, -33, 8, -25,
-11, -50, -37, -22, -25, -14, -7, -27,
-53, -10, -2, -13, -10, -9, -27, -28,
-18, -7, -1, 3, 14, 26, 33, -20, 
-2, 1, 6, 11, -16, 15, -44, 2,
},
{
-124, 37, 39, 59, -65, -46, 76, -53,
-55, 23, 43, -6, -37, 26, -84, 37,
52, 35, -41, 41, -49, 53, 38, -15,
3, 47, -58, -31, -115, -4, 16, -81,
-20, 44, 69, -21, -25, -37, -50, -64,
-28, 45, 4, -67, -37, -15, -9, -6,
-53, -44, -36, -70, -63, -17, 17, -3, 
-80, 21, 19, -88, -1, -28, 21, -3,
},
};
//endgame pst table
float EG_PST_VALUES[6][64] = 
{
{
0, 0, 0, 0, 0, 0, 0, 0,
158, 124, 132, 108, 74, 123, 157, 153,
92, 75, 53, 39, 29, 0, 46, 70,
10, 11, -5, -20, -24, -8, -2, -9,
-16, -13, -26, -28, -37, -22, -17, -20,
-13, 0, -37, -23, -27, -20, -25, -27,
-5, -5, -4, -6, -9, -16, -26, -24,
0, 0, 0, 0, 0, 0, 0, 0, 
},
{
-49, -29, -53, 7, -22, -6, -17, -51,
29, -18, -56, -55, -13, -24, 2, -36,
-39, -21, 5, -18, -8, -23, -11, -16,
-5, 21, 23, 34, 30, 40, 13, -44,
-2, 35, 14, 24, 16, 7, 11, -12,
-42, 6, -5, 18, 26, 2, 0, 16,
-16, -54, -10, 10, -2, 25, -34, -10,
-53, -6, 13, 1, 1, 2, -8, 20, 
},
{
2, -42, -22, -4, -26, 5, 2, 15,
-49, -15, -17, -40, 5, -25, -11, -4,
-11, -9, -21, -13, 4, 6, -39, -6,
2, -1, -17, 14, -1, -23, -27, -26,
-35, -24, -5, 9, -2, -4, -19, -40,
-14, -7, -15, 11, 3, 3, 6, -5,
15, -33, -16, 3, -15, -1, -18, -10,
-36, -31, -32, -1, -14, -18, -8, -34,
}, 
{
18, 3, -15, -10, -10, -15, -4, -2,
-13, 17, -3, -1, -11, -33, 9, 1,
6, 5, -1, -11, -29, -38, -30, -22,
3, -12, 3, -20, -12, -9, -26, -23,
-1, 9, -23, 6, -17, -13, -14, -24,
-1, 6, -7, -11, 0, -8, -50, -6,
20, 8, 19, 4, 1, -20, 3, -18,
-3, -6, -7, -14, -22, -21, -21, -30,
},
{
65, 33, 4, 27, -59, 23, 56, 48, 
54, 41, 39, 103, 76, 32, 16, -78,
-21, -17, 53, 19, 99, 5, 57, -48,
22, 26, -23, 55, 3, 12, 36, -10,
3, 60, 69, 8, 28, 1, 66, 52,
3, -2, 29, 0, 28, 34, 72, 59,
47, 39, -13, 37, -16, -34, -54, 52,
-15, -36, 20, -6, 61, -75, 45, -4,
},
{
-116, 9, -19, -17, -26, 13, 15, -89,
1, 39, -19, -21, 31, 14, 31, 4, 
-60, -5, 22, -9, 10, 7, 19, 11,
-48, 1, 10, 23, 9, 15, 10, 16,
-35, -31, -3, 24, 14, 18, 16, 1,
-15, -8, 7, 24, 25, 20, -2, -14,
-20, 9, 27, 33, 29, 17, -5, -17,
-24, -33, -19, 7, -20, -19, -38, -49,
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
            //bishop pair *needs testing
            if (p == BISHOP_I && Attacks::count_bits(piece_board) == 2) {
                mg_value[c] += MG_BISHOP_PAIR;
                eg_value[c] += EG_BISHOP_PAIR;
            }

            while (piece_board) {
                square = pop_lsb(&piece_board);
                
                if (p != KING_I) {
                    mobility_board = board.attackers_from(square, c, p);
                    //mobility values
                    if (p != PAWN_I) {
                        mobility_squares = Attacks::count_bits(mobility_board);
                        mg_value[c] += (mobility_squares)*MG_MOBILITY[p];
                        eg_value[c] += (mobility_squares)*EG_MOBILITY[p]; 
                    }

                    //pawn structure eval
                    //doubled pawn eval
                    if (p == PAWN_I) {
                        //doubled pawn values
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
                    }

                    //king safety values
                    king_safety = Attacks::count_bits(mobility_board & Attacks::KING_ATTACKS[king_squares[c^1]]) * KING_ATTACKS[p];
                    mg_value[c] += king_safety*2;
                    eg_value[c] += king_safety/2;
                    //update phase value based on pieces present
                    game_phase += PHASE_VALUES[p];
                } 
                //extra king safety eval
                if (p == KING_I) {
                    int king_file = (king_squares[c] & 7); 
                    
                    if (!(Attacks::FILES_BB[king_file] & board.piece_boards[c][PAWN_I])) {
                        mg_value[c] -= SEMIOPEN_KING_FILE_VALUE;
                    }
                    if (king_file != 0 && !(Attacks::FILES_BB[king_file-1] & board.piece_boards[c][PAWN_I])) {
                        mg_value[c] -= SEMIOPEN_KING_FILE_VALUE;
                    }
                    if (king_file != 7 && !(Attacks::FILES_BB[king_file+1] & board.piece_boards[c][PAWN_I])) {
                        mg_value[c] -= SEMIOPEN_KING_FILE_VALUE;
                    }
                }

                //material value
                mg_value[c] += MG_PIECE_VALUES[p];
                eg_value[c] += EG_PIECE_VALUES[p];

                //positional value
                mg_value[c] += MG_PST_VALUES[p][flip_board[c][square]];
                eg_value[c] += EG_PST_VALUES[p][flip_board[c][square]];        

            }
        }
    }

    //if no pawns are left or we have big advantage aka. king endgame; we push them to edge
    if ((!(board.piece_boards[WHITE][PAWN_I]) && eg_value[BLACK] > eg_value[WHITE]) || (eg_value[BLACK] -  1000 > eg_value[WHITE] && game_phase <= 5)) {
        mg_value[WHITE] += KING_EDGE[king_squares[WHITE]];
        eg_value[WHITE] += KING_EDGE[king_squares[WHITE]];
    }   
    if ((!(board.piece_boards[BLACK][PAWN_I]) && eg_value[WHITE] > eg_value[BLACK]) || (eg_value[WHITE] - 1000 > eg_value[BLACK] && game_phase <= 5)) {
        mg_value[BLACK] += KING_EDGE[king_squares[BLACK]];
        eg_value[BLACK] += KING_EDGE[king_squares[BLACK]];
    }   

    float phase = (game_phase * 256 + (MAX_PHASE / 2)) / MAX_PHASE;
    value = ((mg_value[WHITE]-mg_value[BLACK]) * phase + (eg_value[WHITE]-eg_value[BLACK]) * (256-phase))/256;
    return value;
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
            if (fen.find("1-0") != std::string::npos) {result = 1.0;}
            else if (fen.find("0-1") != std::string::npos) {result = 0.0;}
            else {result = 0.5;}
            fen = fen.substr(0, fen.length()-5); //remove outcome from fen 


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
            // tune_parameter(MG_PIECE_VALUES, p, positions); 
            // tune_parameter(EG_PIECE_VALUES, p, positions); 
            // tune_parameter(MG_MOBILITY, p, positions); 
            // tune_parameter(EG_MOBILITY, p, positions); 
            //tune_parameter(KING_ATTACKS, p, positions);
            // for (int s = 0; s < 64; s++) {
            //     if (p == PAWN_I && (s < 8 || s > 64-8)) {
            //         continue;
            //     }

            //     tune_parameter(MG_PST_VALUES, p, s, positions);
            //     tune_parameter(EG_PST_VALUES, p, s, positions);
            // }
        }


        // for (int f = 0; f < 8; f++) {
        //     tune_parameter(PASSED_VALUE, f, positions); 
        // }
        // tune_parameter(EG_PASSED_BONUS, positions);
        tune_parameter(MG_DOUBLED_VALUE, positions);
        tune_parameter(EG_DOUBLED_VALUE, positions);
        tune_parameter(MG_ISOLATED_VALUE, positions);
        tune_parameter(EG_ISOLATED_VALUE, positions);
        tune_parameter(MG_BISHOP_PAIR, positions);
        tune_parameter(EG_BISHOP_PAIR, positions);
        if (TUNING_DELTA > 1) {TUNING_DELTA /= 2;}
        if (!improving) {break;}
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

