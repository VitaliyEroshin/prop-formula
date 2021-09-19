#include <iostream>
#include "propositionalFormulaClass.cpp"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  std::string s;
  std::getline(std::cin, s);
  prop::Formula form;
  form.read(s);
  prop::varEval v;
  //v.push("a1", 1);
  
  std::cout<< form.eval(v) << '\n';
}