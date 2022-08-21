
#include "../engine.hpp"
#include "../eval.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

#ifndef MISC_HPP
#define MISC_HPP

//class used for updating eval parameters
class Tuner{
public:
    //int K;
    //int positions;
    bool improving;
    float best_e;
    json weights;
    // void update_weights(string json_name, int arr[], int piece = NO_PIECE, int square = NO_PIECE); //update the evaluation weights in eval.hpp
    // void write_weights(); //change the json file
    float mean_square_error(int K, int positions); //update the json file
    float tune_parameter(int *param);
    void tune(int time);

    // Tuner(int k, int pos) {
    //     K = k;
    //     positions = pos;
    //     ifstream weight_json("weights.json");
    //     weights = json::parse(weight_json);
    // }
};


#endif