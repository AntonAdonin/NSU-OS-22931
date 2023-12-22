cmake_minimum_required(VERSION 3.22)
project(Lab30 C)

set(CMAKE_C_STANDARD 17)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

add_executable(Lab30_main main.c
        connection.h)

add_executable(Lab30_server server.c
        connection.h)
