#include "source/class.h"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif
 
  form::Formula formula;
  formula.read("(x implies y) and ((not(y) implies z) implies not(x))");
  std::cout << formula.show() << '\n';
  formula.to_cnf();
  std::cout << formula.show() << '\n';
}
