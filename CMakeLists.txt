cmake_minimum_required(VERSION 3.20)
project(MLTA_HW)

set(CMAKE_CXX_STANDARD 14)

include_directories(etc)
include_directories(source)
include_directories(tasks)

add_executable(MLTA_HW
        etc/test.cpp
        etc/test.h
        source/class.cpp
        source/class.h
        source/functions.cpp
        source/functions.h
        source/headers.h
        source/utils.cpp
        source/utils.h
        tasks/chromatic_number.cpp
        tasks/chromatic_number.h
        tasks/ramsey.cpp
        tasks/ramsey.h
        main.cpp)
