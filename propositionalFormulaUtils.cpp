#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace proputil {
  struct Evaluation {
    std::map<std::string, bool> rule;

    void push(std::string var_name, bool var_value) {
      rule[var_name] = var_value;
    }

    bool get(std::string var_name) {
      if (rule.count(var_name) == 0)
        throw std::invalid_argument("Variable is not defined.");
      return rule[var_name];
    }
  };


  std::map<std::string, std::string> get_symbol = 
    {{"and", "∧"}, {"or", "∨"}, {"implies", "→"}, {"not", "¬"}};

  std::map<std::string, std::string> get_operator =
    {{"∧", "and"}, {"∨", "or"}, {"→", "implies"}, {"¬", "not"}};

  std::set<std::string> operation_symbols = {"∧", "∨", "→"}; 

  void is_variable(std::string var) {
    bool p_digit = false;
    for (auto sym : var) {
      bool digit;
      if (sym >= 'a' and sym <= 'z')
        digit = false;
      else if (sym >= '0' and sym <= '9')
        digit = true;
      else
        throw std::invalid_argument("Incorrect variable: invalid symbol in variable name");

      if (p_digit and !digit) 
        throw std::invalid_argument("Incorrect variable: letter symbol after digits");
      
      p_digit = digit;
    }
  }

}