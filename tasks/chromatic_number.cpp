#include "chromatic_number.h"

std::string fnc::generate_chromatic_number_formula(std::vector<std::pair<int, int>> graph, int verticies_count, int colors_count) {
  std::string result = "";
  for (int vertex = 0; vertex < verticies_count; vertex++) {
    std::string any_color = "";
    for (int color = 0; color < colors_count; color++) {
      any_color += "x" + std::to_string(vertex + color * verticies_count) + " ∨ ";
    }
    
    while (!util::is_digit(any_color.back())) {
      any_color.pop_back();
    }
    
    result += "(" + any_color + ") ∧ ";
  }
  for (auto edge : graph) {
    int x = edge.first;
    int y = edge.second;
    if (y < x) {
      std::swap(x, y);
    }
    for (int color = 0; color < colors_count; color++) {
      result += "¬(x" + std::to_string(x + color * verticies_count)
             + " ∧ x" + std::to_string(y + color * verticies_count) + ") ∧ ";
    }
  }
  while (result.back() != ')') {
    result.pop_back();
  }

  return result;
}

bool fnc::color(std::vector<std::pair<int, int>> graph, int n, bool calc_time) {
  int verticies_count = 0;
  for (auto edge : graph) {
    verticies_count = std::max(verticies_count, edge.first);
    verticies_count = std::max(verticies_count, edge.second);
  }
  std::string formula = fnc::generate_chromatic_number_formula(graph, verticies_count, n);

  form::Formula form;
  form.read(formula);
  return form.sat();
}