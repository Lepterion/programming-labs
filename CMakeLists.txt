cmake_minimum_required(VERSION 4.0)
project(programming_labs)

set(CMAKE_CXX_STANDARD 26)

add_executable(programming_labs main.cpp
        common_tree.cpp
        common_tree.h
        binary_tree.cpp
        binary_tree.h
        expression_tree.cpp
        expression_tree.h
        tree_visualizer.hpp
)