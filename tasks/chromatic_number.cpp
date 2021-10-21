#include "chromatic_number.h"

std::string fnc::generate_chromatic_number_fast(std::vector<std::pair<int, int>> graph, int verticies_count, int colors_count) {
  std::string result = "";
  colors_count--;
  std::vector<bool> colors_count_bits = util::get_binary_representation(colors_count);
  
  for (int i = 0; i < verticies_count; i++) {
    std::string any_color;
    for (int j = 0; j < colors_count_bits.size(); j++) {
      /*
        let y(j) be i-th bit of max color's number.
        let x(i, j) be true, if j-th bit of color of i-th vertex is 1.

        x(i, j) < y(j) is equal to ¬(y(j) → x(i, j))
        x(i, j) = y(j) is equal to ¬(x(i, j) ⊕ y(i))
        
        here we check if x'th color number is less or equal to max possible color.
        if last bit of x equals last bit of max possible color - thats okay, check previous bit.
        if last bit of x greated then last bit of MPC - thats bad - false.
        if less, thats good - true.
      */

      std::string x_i_j = "x" + std::to_string(i + verticies_count * j); 
      std::string y_j = to_string(colors_count_bits[j]);
      std::string previous_any_color = any_color;
      any_color = "(¬(" + y_j + " → " + x_i_j + ") ∨ (¬(" + x_i_j + " ⊕ " + y_j + ")";
      if (!previous_any_color.empty()) {
        any_color += " ∧ " + previous_any_color;
      }
      any_color += "))";
    }
    result += any_color + " ∧ ";
  }

  for (auto edge : graph) {
    int a = edge.first;
    int b = edge.second;
    if (b < a) {
      std::swap(a, b);
    }
    std::string edge_condition = "(";
    /*
      if there is an edge between x and y, and bits of their colors are equal
      (x(a, 0) ⊕ x(b, 0)) - is false.
      So if all the bits are equal (x and y have the same color), 
      disjunction of compares gives false, and function returns false. 
    */
    
    for (int j = 0; j < colors_count_bits.size(); j++) {
      std::string x_a_j = "x" + std::to_string(a + verticies_count * j);
      std::string x_b_j = "x" + std::to_string(b + verticies_count * j);
      std::string condition = "(" + x_a_j + " ⊕ " + x_b_j + ")";
      edge_condition += condition + " ∨ ";
    }
    while (edge_condition.back() != ')') {
      edge_condition.pop_back();
    }
    result += edge_condition + " ∧ ";
  }
  while (result.back() != ')') {
    result.pop_back();
  }

  return result;  
}


std::string fnc::generate_chromatic_number_slow(std::vector<std::pair<int, int>> graph, int verticies_count, int colors_count) {
  /*
    This function generates the formula.
  */

  std::string result = "";

  for (int vertex = 0; vertex < verticies_count; vertex++) {
    /*
      For each vertex we are making condition, that it is colored in atleast one color
      
      If vertex X have color Y, variable index would be (X + Y * verticies_count).
    */

    std::string any_color = "";
    for (int color = 0; color < colors_count; color++) {
      any_color += "x" + std::to_string(vertex + color * verticies_count) + " ∨ ";
    }
    
    while (!util::is_digit(any_color.back())) {
      any_color.pop_back();
    }
    
    result += "(" + any_color + ") ∧ ";
    
    /*
      Now we added disjunct to the formula, which is true when vertex has the color.
    */
  }

  /*
    Now we add the condition, that verticies connected with edges cannot be the same color.
  */

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

  std::string formula = fnc::generate_chromatic_number_fast(graph, verticies_count, n);

  /*
    There you can uncomment the line to print generated formula.
  */

  // std::cout << formula << '\n';
  
  form::Formula form;
  form.read(formula);
  return form.sat(calc_time);
}