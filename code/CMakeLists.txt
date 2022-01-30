cmake_minimum_required(VERSION 3.20)
project(git C)

set(CMAKE_C_STANDARD 11)
add_executable(fighter fighter.c)
add_executable(test test.c)