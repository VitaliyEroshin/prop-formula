#include "test.h"

int even(const std::vector<int>& arguments) {
  int sum = 0;
  for (auto x : arguments) {
    sum += x;
  }
  return !(sum % 2);
}

void test() {
  /*
    Fully connected graph with 6 verticies, checking if 3 it's chromatic number (no).
    slow algorithm - 16.5 s
    fast algorithm - 270 ms

    Then checking if 6 it's chromatic number (yes).
  
  */

  form::Formula f;

  std::cout << "Test 1 (Color) fully connected, 6 verticies, check 3. Time: " 
            << fnc::color({{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2},
                          {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, 
                          {3, 4}, {3, 5}, {4, 5}}, 3, true) << '\n';
  

  std::cout << "Test 2 (Color) fully connected, 6 verticies, check 6. Time: " 
            << fnc::color({{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2},
                          {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, 
                          {3, 4}, {3, 5}, {4, 5}}, 6, true) << '\n';

  std::cout << "Test 3 (Ramsey) n = 3, m = 3, R = 6. Time: "
            << fnc::ramsey(6, 3, 3, true) << '\n';
  
  
  f.read("x or (y and not(z))");
  std::cout << "Test 4 (SAT): x or (y and not(z)). Time: " << f.sat(true) << '\n' << f.show() << '\n';

  f.read("x and not(x)");
  std::cout << "Test 5 (SAT): x and not(x). Time: " << f.sat(true) << '\n' << f.show() << '\n';

  f.addFunction(fnc::__Ramsey, "Ramsey");
  f.read("Ramsey(6, 4, 4, 0)");
  
  std::cout << "Test 6 (Ramsey function): Ramsey(6, 4, 4, 0). Time: " << f.sat(true) << '\n' << f.show() << '\n';

  f.read("Ramsey(6, 3, 3, 0)");
  std::cout << "Test 7 (Ramsey function): Ramsey(6, 3, 3, 1). Time: " << f.sat(true) << '\n' << f.show() << '\n';

  f.read("x and y");
  std::cout << "Test 8 (Truth table): x and y. \n";
  f.build_truth_table(true);

  f.addFunction(even, "Even");

  f.read("x or Even(x, y, z)");
  std::cout << "Test 9 (Truth table): x or Even(x, y, z). \n";
  
  f.build_truth_table(true);
  
}
