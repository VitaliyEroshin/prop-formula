#include "./tasks/ramsey.h"
#include <iostream>

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  //fnc::ramsey(6, 2, 2);
  std::cout << fnc::ramsey(6, 2, 2, false);
}