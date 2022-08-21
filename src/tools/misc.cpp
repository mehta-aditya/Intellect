
#include "misc.hpp"

// void Tuner::update_weights(string json_name, int arr[], int piece = NO_PIECE, int square = NO_PIECE){
//     if (square > NO_PIECE) {
//         arr[piece][square] = weights[json_name][piece][square];
//     }
//     else {
//         arr[piece] = weights[json_name][piece];
//     }
// }

// void Tuner::write_weights(int val, string json_name, int piece = NO_PIECE, int square = NO_PIECE){
//     if (square > NO_PIECE) {
//         weights[json_name][piece][square] = val;
//     }
//     else {
//         weights[piece][piece] = val;
//     }
// }

//mean square error used for tuner
float Tuner::mean_square_error(int K, int positions) {
    Board board;
    int value;
    float error = 0.0, result;
    int pos = 1; // keeps track of position number
    fstream fen_file; 
    //positions.txt should have fens and at the end the game result
    //open the file and read fens to calculate mean square error
    fen_file.open("positions.txt",ios::in);
    if (fen_file.is_open()) {
        string fen;
        while(getline(fen_file, fen)){ 
            string outcome = fen.substr(fen.length()-4, 3);
            result = std::stof(outcome);
            fen = fen.substr(0, fen.length()-5); //remove outcome from fen 
            cout << fen;
            board.set_fen(fen);
            value = Engine::evaluation(board);
            float sigmoid = (1/(1 + pow(10, -K*value/400)));
            cout << fen;
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
float Tuner::tune_parameter(int *param) {
    int delta_val = 1; //value to change parameters by each iteration
    *param = *param + delta_val; //update value
    float error = mean_square_error(1, 1000);
    cout << "New Error: ";
    printf("%.6f", error);
    cout << endl;
    //check if parameter increase helps
    if (error < best_e) {
        best_e = error;
        improving = true;
        cout << "parameter+" << endl;
    }
    else {
        *param = *param - 2*delta_val;
        error = mean_square_error(1, 1000);
        //check if parameter decrease helps
        if (error < best_e) {
            best_e = error;
            improving = true;
            cout << "parameter-" << endl;
        }
        //keep parameter the same
        else {
            *param = *param + delta_val;
        }
    }
    return error;
}

//tuner for evaluation paramters
void Tuner::tune(int time) {
    improving = true; //value to check if parameter is improving
    best_e = mean_square_error(1, 1000);
    auto start_time = steady_clock::now();  

    while (improving && duration_cast<seconds>(steady_clock::now() - start_time).count() < time) {
        //run through parameters
        for (int &val : MG_PIECE_VALUES) {
            tune_parameter(&val); 
        }
        for (int &val : EG_PIECE_VALUES) {
            tune_parameter(&val); 
        }
        for (int p = PAWN_I; p <= KING_I; p++) {
            for (int &val : MG_PST_VALUES[p]) {
                tune_parameter(&val);
            }
            for (int &val : EG_PST_VALUES[p]) {
                tune_parameter(&val);
            }
        }
    }
    //show table
    int i;
    //run through parameters
    cout << "{";
    for (int &val : MG_PIECE_VALUES) {
        cout << val << ", ";
    }
    cout << "}" << endl;

    cout << "{";
    for (int &val : EG_PIECE_VALUES) {
        cout << val << ", ";
    }
    cout << "}" << endl;

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
            }
            i++;
        }
        cout << "}" << endl;
    }
    cout << "}" << endl;

    cout << "{" << endl;
    for (int p = PAWN_I; p <= KING_I; p++) {
        cout << "{" << endl;
        for (int &val : EG_PST_VALUES[p]) {
            cout << val;
            i++;
            if (i >= 7) {
                i = 0;
                cout << ", " << endl;
            }
            else {
                cout << ", ";
            }
        }
        cout << "}" << endl;
    }
    cout << "}" << endl;
}

