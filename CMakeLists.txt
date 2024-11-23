cmake_minimum_required(VERSION 3.10)
project(gameWatch)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

add_executable(app
    src/main.c
    src/main_functions.c
    src/sub/utils.c
)

target_include_directories(app
    PRIVATE src
    PRIVATE src/sub
)

target_link_libraries(app)
