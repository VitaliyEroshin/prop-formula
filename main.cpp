#include "./source/class.h"
#include "./tasks/ramsey.h"
#include "./tasks/chromatic_number.h"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  
  std::cout << fnc::color({{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2},
                          {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, 
                          {3, 4}, {3, 5}, {4, 5}}, 3, true) << '\n';
  /*
    Fully connected graph with 5 verticies, checking if 3 it's chromatic number.
  */

  // slow - 16.5 sec
  
}
