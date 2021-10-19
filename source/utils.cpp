#include "utils.h"

void util::Evaluation::push(std::string var_name, bool var_value) {
  rule[var_name] = var_value;
}

bool util::Evaluation::get(std::string var_name) {
  if (rule.count(var_name) == 0)
    throw std::invalid_argument("Variable is not defined.");
  return rule[var_name];
}

bool util::is_digit(char x) {
  return (x >= '0') and (x <= '9');
}

bool util::is_letter(char x) {
  return (x >= 'a') and (x <= 'z');
}

void util::is_variable(std::string var) {
  bool p_digit = false;
  for (auto sym : var) {
    bool digit;
    if (is_letter(sym))
      digit = false;
    else if (util::is_digit(sym))
      digit = true;
    else
      throw std::invalid_argument("Incorrect variable: invalid symbol in variable name");

    if (p_digit and !digit) 
      throw std::invalid_argument("Incorrect variable: letter symbol after digits");
      
    p_digit = digit;
  }
}

std::string util::get_symbol(std::string x) {
  std::map<std::string, std::string> t = {{"and", "∧"}, {"or", "∨"}, {"implies", "→"}, {"not", "¬"}};
  return t[x];
}

std::string util::get_operator(std::string x) {
  std::map<std::string, std::string> t  ={{"∧", "and"}, {"∨", "or"}, {"→", "implies"}, {"¬", "not"}};
  return t[x];
}

bool util::in_get_operator(std::string x) {
  std::map<std::string, std::string> t  ={{"∧", "and"}, {"∨", "or"}, {"→", "implies"}, {"¬", "not"}};
  return t.count(x);
}

bool util::operation_symbols(std::string x) {
  std::set<std::string> t = {"∧", "∨", "→"};
  return t.count(x);
}
