cmake_minimum_required(VERSION 3.12)
project(TP1 C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

set(CMAKE_C_STANDARD 11)

add_executable(TP1 LAB1.h main.c)