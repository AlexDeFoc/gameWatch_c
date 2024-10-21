cmake_minimum_required(VERSION 3.30.3)

project("gameWatch_c")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE release)

add_executable("app" src/main.c)
