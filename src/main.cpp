#include "attacks.hpp"
#include "uci.hpp"
#include "eval.hpp"
#include "tools/misc.hpp"

int main(){
  Attacks::init();

  // Tuner::tune(3600);
  
  UCI::init();

  return 0;
}

