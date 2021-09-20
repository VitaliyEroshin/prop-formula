#include "propositionalFormulaClass.cpp"

/*
void cnr(int n, int r, std::vector<int> &element) {
  int cur_size = element.size();
  if (cur_size == n) {

  }

  int start = 1;
  if (cur_size)
    start = element.back();

  for (int i = start; i <= r; i++) {
    element.push_back(i);
    cnr(n, r, element);
    element.pop_back();
  }
}

bool fi(int R, int n, int m) {

}
*/

int main() {
  #ifdef local
    freopen("./io/input.txt", "r", stdin);
    freopen("./io/output.txt", "w", stdout);
  #endif

  std::string s;
  std::getline(std::cin, s);
  prop::Formula form;
  form.read(s);
  std::cout<< form.show() << '\n';
}