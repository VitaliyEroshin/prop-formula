#include "headers.h"

namespace form {
  bool _conjunction(const std::vector<int>& arguments);

  bool _disjunction(const std::vector<int>& arguments);

  bool _implication(const std::vector<int>& arguments);

  bool _negation(const std::vector<int>& arguments);

  bool _exclusive_disjunction(const std::vector<int>& arguments);

  bool _equality(const std::vector<int>& arguments);
}