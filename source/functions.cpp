#include "functions.h"

int form::__conjunction(std::vector<int> arguments) {
  int result = 1;
  for (auto argument : arguments) {
    result &= argument;
  }

  return result;
}

int form::__disjunction(std::vector<int> arguments) {
  int result = 0;
  for (auto argument : arguments) {
    result |= argument;
  }

  return result;
}

int form::__implication(std::vector<int> arguments) {
  assert(arguments.size() == 2);
  // implication should take in two arguments.
  return !arguments[0] || arguments[1];
}

int form::__negation(std::vector<int> arguments) {
  assert(arguments.size() == 1);
  // negation is unary operator.
  return !arguments.back();
}

int form::__exclusive_disjunction(std::vector<int> arguments) {
  int result = 0;
  for (auto argument : arguments) {
    result ^= argument;
  }

  return result;
}
