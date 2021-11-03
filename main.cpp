#include "./etc/test.h"

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif
  
  auto start = std::chrono::steady_clock::now();
  test();
  auto end = std::chrono::steady_clock::now();
  
  std::cout << "\n\n\nfinal time: ";
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms\n";
}
