#!/bin/bash
echo "compiling..."
g++ -std=c++17 main.cpp tasks/ramsey.cpp source/class.cpp source/utils.cpp -o release
echo "compiled"
