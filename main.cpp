#include "./source/class.h"
#include "./tasks/ramsey.h"
#include "./tasks/chromatic_number.h"

bool True() {
  return 1;
}
int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif
  
  /*
    Fully connected graph with 6 verticies, checking if 3 it's chromatic number (no).
    slow algorithm - 16.5 s
    fast algorithm - 270 ms

    Then checking if 6 it's chromatic number (yes).
  
  */
  std::cout << "Test 1 (Color) fully connected, 6 verticies, check 3. Time: " 
            << fnc::color({{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2},
                          {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, 
                          {3, 4}, {3, 5}, {4, 5}}, 3, true) << '\n';
  

  std::cout << "Test 2 (Color) fully connected, 6 verticies, check 6. Time: " 
            << fnc::color({{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2},
                          {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, 
                          {3, 4}, {3, 5}, {4, 5}}, 6, true) << '\n';

  std::cout << "Test 3 (Ramsey) n = 3, m = 3, R = 6. Time: "
            << fnc::ramsey(6, 3, 3, true) << '\n';
  
}
