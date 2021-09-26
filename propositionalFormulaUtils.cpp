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

  bool is_digit(char x) {
    return (x >= '0') and (x <= '9');
  }

  bool is_letter(char x) {
    return (x >= 'a') and (x <= 'z');
  }

  void is_variable(std::string var) {
    bool p_digit = false;
    for (auto sym : var) {
      bool digit;
      if (is_letter(sym))
        digit = false;
      else if (is_digit(sym))
        digit = true;
      else
        throw std::invalid_argument("Incorrect variable: invalid symbol in variable name");

      if (p_digit and !digit) 
        throw std::invalid_argument("Incorrect variable: letter symbol after digits");
      
      p_digit = digit;
    }
  }

}