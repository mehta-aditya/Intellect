#include "attacks.hpp"
#include "uci.hpp"
#include "eval.hpp"
#include "tools/misc.hpp"
int main(){
  Attacks::init();
  //calculate_k();
  //Tuner::tune(6000);
  UCI::init();

  return 0;
}

