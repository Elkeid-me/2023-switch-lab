cmake_minimum_required(VERSION 3.11)
project(CompNet-Lab4 LANGUAGES C CXX)
enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_compile_options(-Wall)
set(CMAKE_BUILD_TYPE Release)

find_library(LIB_LAB4RUNTIME lab4runtime build/)

add_library(switch STATIC src/switch.cxx)

add_executable(simulator src/simulator.cc)
target_link_libraries(simulator ${LIB_LAB4RUNTIME} switch)
