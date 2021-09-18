#include <iostream>
#include "propositionalFormulaClass.cpp"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  std::string s;
  std::getline(std::cin, s);
  PropositionalFormula form;
  form.read(s);
  std::cout<< form.show() << '\n';
}