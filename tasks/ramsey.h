#pragma once
#include "../source/class.h"

namespace fnc {
  void cnR(int n, int R, std::vector<int> &subset, std::string &result, std::string prefix = "",
                std::string inside = "∧", std::string outside = "∨");

  std::string generate_ramsey_formula(int R, int n, int m);

  bool ramsey(int R, int n, int m, bool print_time = false);
}