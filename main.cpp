#include "source/class.h"
#include "etc/test.h"

void interpret() {
  form::Formula formula;
  while (true) {
    std::string type;
    std::cin >> type;
    if (type == "exit") {
      return;
    } else if (type == "read") {
      std::string formula_str = "";
      while (std::cin.peek() != '"') {
        std::cin.get();
      }
      std::cin.get();
      while (std::cin.peek() != '"') {
        formula_str.push_back(std::cin.get());
      }
      std::cout << "You've entered formula: " << formula_str << '\n';
      formula.read(formula_str);
    } else if (type == "simplify") {
      formula.to_cnf();
    } else if (type == "show") {
      std::cout << formula.show() << '\n';
    } else if (type == "buildtt") {
      formula.build_truth_table();
    } else if (type == "taut") {
      std::cout << formula.taut(true) << '\n';
    } else if (type == "sat") {
      std::cout << formula.sat(true) << '\n';
    } else if (type == "deMorgan") {
      formula.de_morgan();
    } else if (type == "distribute") {
      formula.distribute();
    }
  }
}

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  interpret();
}
