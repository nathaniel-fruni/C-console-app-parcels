cmake_minimum_required(VERSION 3.26)
project(C-console-app-parcels C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES "src/*.c")

add_executable(C-console-app-parcels ${SOURCES})
