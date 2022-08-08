#include "attacks.hpp"
#include "uci.hpp"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int captures = 0;
int castles = 0;
int checks = 0;
int promotions = 0;

unsigned int random_state = 1804289383;
unsigned int get_random_U32_number(){
    // get current state
    unsigned int number = random_state;
    
    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    
    // update random number state
    random_state = number;
    
    // return random number
    return number;
}

// generate 64-bit pseudo legal numbers
U64 get_random_U64_number(){
    // define 4 random numbers
    U64 n1, n2, n3, n4;
    
    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;
    
    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 generate_magic_number(){
  return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}

U64 Attacks::find_magic_number(int square, int bits, vector<int> deltas) {
    U64 magic_number;
    // bitboards
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];
    //generate attack mask
    U64 attack_mask = Attacks::sliding_attacks(square, EMPTY_BB, deltas);
    if (deltas == Attacks::LINE_DELTAS) {
      BITMASK_CLEAR(attack_mask, Attacks::get_edges(square));
      BITMASK_CLEAR(attack_mask, CORNERS);
    }
    else {
      BITMASK_CLEAR(attack_mask, EDGES);
    }

    int occupancy_indicies = (1 << bits);
    // loop over occupancy indicies
    for (int i = 0; i < occupancy_indicies; i++) {
        // init occupancies
        occupancies[i] = Attacks::get_blockers(i, bits, attack_mask);
        
        // init attacks
        attacks[i] = Attacks::sliding_attacks(square, occupancies[i], deltas);
    }
    // test magic numbers
    for (int random_count = 0; random_count < 100000000; random_count++){
        // generate magic number candidate
        magic_number = generate_magic_number();
        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) {continue;}
        
        // init used attacks
        memset(used_attacks, 0ULL, sizeof(used_attacks));
        
        // init index & fail flag
        int index, fail;
        
        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancy_indicies; index++){
            // init magic index
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - bits));
            
            // if magic index works
            if (used_attacks[magic_index] == 0ULL){
                // init used attacks
                used_attacks[magic_index] = attacks[index];
            }
            // otherwise
            else if (used_attacks[magic_index] != attacks[index]) {
                // magic index doesn't work
                fail = 1;
            }
        }
        
        // if magic number works
        if (!fail) {return magic_number;}
            
    }
    
    // if magic number doesn't work
    printf("  Magic number fails!\n");
    return 0ULL;
}

void Attacks::init_magic_numbers() {
      // loop over 64 board squares
      cout << "BISHOP" << endl;
      for (int square = 0; square < 64; square++){
          printf("%llu", find_magic_number(square, diag_relevant_bits[square], Attacks::DIAG_DELTAS));
          cout << "ULL," << endl;
          
          // init bishop magic numbers
          //diag_magics[square] = find_magic_number(square, diag_relevant_bits[square], Attacks::DIAG_DELTAS);
  }
        cout << "ROOK" << endl;
        // loop over 64 board squares
      for (int square = 0; square < 64; square++){
          printf("%llu", find_magic_number(square, line_relevant_bits[square], Attacks::LINE_DELTAS));
          cout << "ULL," << endl;
          // init rook magic numbers
          //line_magics[square] = find_magic_number(square, line_relevant_bits[square], Attacks::LINE_DELTAS);
      }

}

int main(){
  Attacks::init();
  //Attacks::init_magic_numbers();
  UCI::init();
  
  return 0;
}