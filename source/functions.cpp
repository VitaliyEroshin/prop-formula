#include "functions.h"

bool form::_conjunction(const std::vector<int>& arguments) {
  int result = 1;
  for (auto argument : arguments) {
    result &= argument;
  }

  return result;
}

bool form::_disjunction(const std::vector<int>& arguments) {
  int result = 0;
  for (auto argument : arguments) {
    result |= argument;
  }

  return result;
}

bool form::_implication(const std::vector<int>& arguments) {
  assert(arguments.size() == 2);
  // implication should take in two arguments.
  return !arguments[0] || arguments[1];
}

bool form::_negation(const std::vector<int>& arguments) {
  assert(arguments.size() == 1);
  // negation is unary operator.
  return !arguments.back();
}

bool form::_exclusive_disjunction(const std::vector<int>& arguments) {
  int result = 0;
  for (auto argument : arguments) {
    result ^= argument;
  }

  return result;
}

bool form::_equality(const std::vector<int>& arguments) {
  int first = arguments.back();
  for (auto& argument : arguments) {
    if (argument != first) {
      return false;
    }
  }
  return true;
}