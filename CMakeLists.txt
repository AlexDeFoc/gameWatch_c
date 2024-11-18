cmake_minimum_required(VERSION 3.10)
project(gameWatch)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

add_executable(app src/main.c)

target_link_libraries(app)
