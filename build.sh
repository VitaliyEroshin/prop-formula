#!/bin/bash
echo "compiling..."
clang++ -std=c++17 main.cpp tasks/ramsey.cpp tasks/chromatic_number.cpp source/class.cpp source/utils.cpp -g -o release
echo "compiled"
