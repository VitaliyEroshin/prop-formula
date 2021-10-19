#include "../source/class.h"

void cnR(int n, int R, std::string prefix, std::vector<int> &element, std::string &s,
              std::string inside = "∧", std::string outside = "∨") {
  /*
    Recursively generates subsets of [R] of size n.
  */

  int cur_size = element.size();
  if (cur_size == n) {
    std::string t = "";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < i; j++) {
        t += prefix + "x" + std::to_string(element[j] * 10 + element[i]) + " " + inside + " ";
      }
    }
    while (!util::is_digit(t.back()))
      t.pop_back();
    s += "(" + t + ") " + outside  + " ";
  }

  int start = 1;
  if (cur_size)
    start = element.back() + 1;

  for (int i = start; i <= R; i++) {
    element.push_back(i);
    cnR(n, R, prefix, element, s);
    element.pop_back();
  }
}

bool ramsey(int R, int n, int m) {
  std::string s = "(";
  std::vector<int> el;
  cnR(n, R, "", el, s);
  while (s.back() != ')')
    s.pop_back();
  s += ") ∨ (";
  cnR(m, R, "¬", el, s);
  while (s.back() != ')')
    s.pop_back();
  s += ")";
  form::Formula form;
  form.read(s);
  return form.taut(true);
}