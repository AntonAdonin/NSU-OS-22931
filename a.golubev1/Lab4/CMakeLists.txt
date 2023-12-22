cmake_minimum_required(VERSION 3.27)
project(Lab4)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

include_directories(include)

add_executable(Lab4 src/main.cpp
        include/String.h
        include/StringList.h
        src/String.cpp
        src/StringList.cpp)
