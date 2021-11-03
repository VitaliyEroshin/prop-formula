#include "utils.h"

void util::Evaluation::push(std::string var_name, int var_value) {
  rule[var_name] = var_value;
}

int util::Evaluation::get(std::string var_name) {
  if (std::isdigit(var_name[0])) {
    return std::stoi(var_name);
  }

  assert(rule.count(var_name));
  // Variable is not defined.

  return rule[var_name];
}

void util::is_variable(std::string var) {
  bool p_digit = false;
  for (auto sym : var) {
    bool digit;
    if (std::islower(sym))
      digit = false;
    else if (std::isdigit(sym))
      digit = true;
    else
      throw std::invalid_argument("Incorrect variable: invalid symbol in variable name");

    if (p_digit and !digit) 
      throw std::invalid_argument("Incorrect variable: letter symbol after digits");
      
    p_digit = digit;
  }
}

std::string util::get_symbol(std::string x) {
  std::map<std::string, std::string> t = {{"and", "∧"}, {"or", "∨"}, {"implies", "→"}, {"not", "¬"}, {"^", "xor"}};
  return t[x];
}

std::string util::get_operator(std::string x) {
  std::map<std::string, std::string> t  ={{"∧", "and"}, {"∨", "or"}, {"→", "implies"}, {"¬", "not"}, {"⊕", "xor"}};
  return t[x];
}

bool util::in_get_operator(std::string x) {
  std::map<std::string, std::string> t  ={{"∧", "and"}, {"∨", "or"}, {"→", "implies"}, {"¬", "not"}, {"⊕", "xor"}};
  return t.count(x);
}

bool util::operation_symbols(std::string x) {
  std::set<std::string> t = {"∧", "∨", "→", "⊕"};
  return t.count(x);
}

std::vector<bool> util::get_binary_representation(int x) {
  std::vector<bool> result;
  while (x != 0) {
    result.push_back(x % 2);
    x /= 2;
  }
  if (result.empty()) {
    result.push_back(0);
  }
  return result;
}
