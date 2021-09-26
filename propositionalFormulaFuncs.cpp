#include "propositionalFormulaClass.cpp"

namespace propfunc{
void cnr(int n, int r, std::string prefix, std::vector<int> &element, std::string &s, std::string inside = "∧", std::string outside = "∨") {
  int cur_size = element.size();
  if (cur_size == n) {
    std::string t = "";
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < i; j++) {
        t += prefix + "x" + std::to_string(element[j] * 10 + element[i]) + " " + inside + " ";
      }
    }
    while (!proputil::is_digit(t.back()))
      t.pop_back();
    s += "(" + t + ") " + outside  + " ";
  }

  int start = 1;
  if (cur_size)
    start = element.back() + 1;

  for (int i = start; i <= r; i++) {
    element.push_back(i);
    cnr(n, r, prefix, element, s);
    element.pop_back();
  }
}

bool phi(int R, int n, int m) {
  // Asymptotic goes brrrr
  
  std::string s = "(";
  std::vector<int> el;
  cnr(n, R, "", el, s);
  while (s.back() != ')')
    s.pop_back();
  s += ") ∨ (";
  cnr(m, R, "¬", el, s);
  while (s.back() != ')')
    s.pop_back();
  s += ")";
  prop::Formula form;
  form.read(s);
  return form.taut(true);
}
}