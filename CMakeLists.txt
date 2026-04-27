cmake_minimum_required(VERSION 3.14)
project(programming_labs)

set(CMAKE_CXX_STANDARD 26)

add_executable(programming_labs
        main.cpp
        graph_algorithms.cpp
        graph_utils.cpp
        graph_representations.cpp  # ОЦЕЙ РЯДОК ОБОВ'ЯЗКОВИЙ
)