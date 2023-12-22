cmake_minimum_required(VERSION 3.22)
project(Lab6)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

add_executable(Lab6 main.cpp)
