#include "source/class.h"

int f(std::vector<int> a) {
  for (int i = 1; i < 3; i++) {
    if (a[i] != a[i - 1])
      return 0;
  }
  return 1;
}
int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  form::Formula formula;
  formula.add_function(f, "F");
  formula.read("F(p, q, F(p, q, r))");
  formula.build_truth_table(false);
}
