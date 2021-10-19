#include "./tasks/functions.cpp"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  std::cout << ramsey(6, 2, 2);
}