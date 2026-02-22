cmake_minimum_required(VERSION 4.0)
project(programming_labs)

set(CMAKE_CXX_STANDARD 26)

add_executable(programming_labs
        main.cpp
        stack.h
        unrolled_linked_list.h
        circular_notification_buffer.h)