cmake_minimum_required(VERSION 3.10)
project(lab_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

add_library(my_proc_lib lib/process_lib.cpp)

add_executable(test_util main.cpp)

target_link_libraries(test_util my_proc_lib)