
// #include "../engine.hpp"
// #include "../eval.hpp"
#include "../attacks.hpp"
#include "../board.hpp"
#include <fstream>

#ifndef MISC_HPP
#define MISC_HPP

struct TunerEntry { 
    string fen; 
    float result; 
    TunerEntry(string f, float r) {
        fen = f; 
        result = r;
    }
};

namespace Tuner {
void init_eval();

const int PIECE_VALUES[6] = {100, 300, 300, 500, 900, 20000};
const int PHASE_VALUES[6] = {0, 1, 1, 2, 5, 0};
const int MAX_PHASE = 26;
const int flip_board[2][64] = {
{
  0, 1, 2, 3, 4, 5, 6, 7,
  8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23,
  24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 42, 43, 44, 45, 46, 47,
  48, 49, 50, 51, 52, 53, 54, 55,
  56, 57, 58, 59, 60, 61, 62, 63
},
{
  56, 57, 58, 59, 60, 61, 62, 63,
  48, 49, 50, 51, 52, 53, 54, 55,
  40, 41, 42, 43, 44, 45, 46, 47,
  32, 33, 34, 35, 36, 37, 38, 39,
  24, 25, 26, 27, 28, 29, 30, 31,
  16, 17, 18, 19, 20, 21, 22, 23,
  8, 9, 10, 11, 12, 13, 14, 15,
  0, 1, 2, 3, 4, 5, 6, 7
}
};

double calculate_k(vector<TunerEntry> positions);
vector<TunerEntry> read_txt_file(int positions);
double mean_square_error(float K, vector<TunerEntry> positions); 
double thread_mse(float K, vector<TunerEntry> positions, double* error);
double threaded_mse(float K, vector<TunerEntry> positions);
double tune_parameter(int &arr, vector<TunerEntry> positions);
double tune_parameter(int arr[], int i, vector<TunerEntry> positions);
double tune_parameter(float arr[][64], int i, int j, vector<TunerEntry> positions);
double tune_parameter_gradient(float arr[][64], int i, int j);
void tune(int time);
float evaluation_tune(Board &board);
};

#endif