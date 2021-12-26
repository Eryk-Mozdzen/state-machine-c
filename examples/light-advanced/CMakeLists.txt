cmake_minimum_required(VERSION 3.16)

project(LightAdvanced)

# compiler settings
add_compile_options(-Wall -pedantic)

# code source
file(GLOB SOURCE "src/*.c" "../../lib/src/*.c")
add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_include_directories(${PROJECT_NAME} PUBLIC ../../lib/inc)

# linking
target_link_libraries(${PROJECT_NAME} m)

# mkdir build
# cd build
# cmake ..
# make
# ./Light