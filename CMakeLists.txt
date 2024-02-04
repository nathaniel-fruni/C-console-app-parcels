cmake_minimum_required(VERSION 3.26)
project(untitled1 C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES "src/*.c")

add_executable(untitled1 ${SOURCES})