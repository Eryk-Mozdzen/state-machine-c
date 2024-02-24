cmake_minimum_required(VERSION 3.16)

project(example-light)

add_executable(${PROJECT_NAME}
    "main.c"
    "../../src/fsm.c"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "../../include"
)

target_compile_options(${PROJECT_NAME} PUBLIC
    -Wall
    -Wextra
    -Wpedantic
)

# mkdir build
# cd build
# cmake ..
# make
# ./Light