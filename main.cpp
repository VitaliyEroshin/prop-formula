#include "./tasks/ramsey.h"
#include "./tasks/chromatic_number.h"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif
  
  std::cout << fnc::color({{0, 1}, {1, 2}}, 2, true) << '\n';
  std::cout << fnc::ramsey(6, 2, 2, true);
}
