cmake_minimum_required(VERSION 3.10)

project(whereami C)
set(CMAKE_C_STANDARD 99)

add_library(whereami STATIC
    src/whereami.c
)

target_include_directories(whereami PUBLIC src)