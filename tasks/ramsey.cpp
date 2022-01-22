#include "ramsey.h"

void fnc::cnR(int n, int R, std::vector<int> &subset, std::string &result, std::string prefix,
              std::string inside, std::string outside) {
  /*
    Recursively generates subsets of [R] of size n.
  */

  int size = subset.size();
  if (size == n) {
    std::string t = "";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < i; j++) {
        t += prefix + "x" + std::to_string(subset[j] * R + subset[i]) + " " + inside + " ";
      }
    }

    while (!std::isdigit(t.back())) {
      t.pop_back();
    }
      
    result += "(" + t + ") " + outside  + " ";
  }

  int start = 1;
  if (size) {
    start = subset.back() + 1;
  }

  for (int i = start; i <= R; i++) {
    subset.push_back(i);
    cnR(n, R, subset, result, prefix);
    subset.pop_back();
  }
}

std::string fnc::generate_ramsey_formula(int R, int n, int m) {
  std::string result = "(";
  std::vector<int> subset;
  fnc::cnR(n, R, subset, result);

  while (result.back() != ')')
    result.pop_back();

  result += ") ∨ (";

  fnc::cnR(m, R, subset, result, "¬");

  while (result.back() != ')')
    result.pop_back();
  result += ")";
  return result;
}

bool fnc::ramsey(int R, int n, int m, bool print_time) {
  
  std::string formula = fnc::generate_ramsey_formula(R, n, m);
  
  /*
    There you can uncomment the line to print generated formula.
  */

  // std::cout << formula;
  form::Formula form;
  form.read(formula);
  return form.taut(print_time);
}

int fnc::__Ramsey(const std::vector<int>& arguments) {
  assert(arguments.size() == 4);
  return fnc::ramsey(arguments[0], arguments[1], arguments[2], arguments[3]);
}