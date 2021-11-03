#!/bin/bash
echo "compiling..."
clang++ -std=c++17 -O3 main.cpp tasks/ramsey.cpp tasks/chromatic_number.cpp source/class.cpp source/utils.cpp source/functions.cpp -o release
echo "compiled"
