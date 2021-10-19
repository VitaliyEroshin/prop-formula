#pragma once
#include <string>
#include <map>
#include <set>

namespace util {
  struct Evaluation{
    std::map<std::string, bool> rule;
    void push(std::string var_name, bool var_value);
    bool get(std::string var_name);
  };

  bool is_digit(char x);
  bool is_letter(char x);
  void is_variable(std::string var);

  std::map<std::string, std::string> get_symbol;
  std::map<std::string, std::string> get_operator;
  std::set<std::string> operation_symbols;

}