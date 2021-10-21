#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>

namespace util {
  struct Evaluation{
    std::map<std::string, bool> rule;
    void push(std::string var_name, bool var_value);
    bool get(std::string var_name);

  };

  bool is_digit(char x);
  bool is_letter(char x);
  void is_variable(std::string var);
  
  std::string get_symbol(std::string x);
  std::string get_operator(std::string x);
  bool in_get_operator(std::string x);
  bool operation_symbols(std::string x);
  
  std::vector<bool> get_binary_representation(int x);
}