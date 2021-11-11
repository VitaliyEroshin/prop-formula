#!/bin/bash
echo "compiling..."
clang++ -std=c++17 -g main.cpp etc/test.cpp tasks/ramsey.cpp tasks/chromatic_number.cpp source/class.cpp source/utils.cpp source/functions.cpp -o bin/debug
echo "compiled"