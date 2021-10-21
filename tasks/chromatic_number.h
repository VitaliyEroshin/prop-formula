#pragma once
#include "../source/class.h"
#include "../source/utils.h"
#include <utility>

namespace fnc {
  std::string generate_chromatic_number_fast(std::vector<std::pair<int, int>> graph, int verticies_count, int colors_count);
  std::string generate_chromatic_number_slow(std::vector<std::pair<int, int>> graph, int verticies_count, int colors_count);
  bool color(std::vector<std::pair<int, int>> graph, int n, bool calc_time = false);
}

