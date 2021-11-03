#pragma once
#include "headers.h"

namespace util {
  struct Evaluation{
    std::map<std::string, bool> rule;

    void push(std::string var_name, int var_value);

    int get(std::string var_name);
  };
  
  void is_variable(std::string var);
  
  std::string get_symbol(std::string x);

  std::string get_operator(std::string x);

  bool in_get_operator(std::string x);
  
  bool operation_symbols(std::string x);
  
  std::vector<bool> get_binary_representation(int x);
}