#include "propositionalFormulaClass.cpp"
#include "propositionalFormulaFuncs.cpp"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  std::cout << propfunc::phi(6, 2, 2);
}