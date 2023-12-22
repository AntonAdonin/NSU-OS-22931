cmake_minimum_required(VERSION 3.22)
project(GolubevLab1 C)

set(CMAKE_C_STANDARD 11)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

add_executable(GolubevLab1 main.c)
