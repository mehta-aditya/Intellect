#include "attacks.hpp"
#include "uci.hpp"
#include "eval.hpp"

int main(){
  Attacks::init();
  //Attacks::init_magic_numbers();
  UCI::init();

  return 0;
}