cmake_minimum_required(VERSION 3.16.3)
project(LAB04 C)

set(CMAKE_C_STANDARD 99)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(LAB04 sorts.c params.c params.h algs.h main.c rand_arr.c)

target_link_libraries(LAB04 PRIVATE "${OpenMP_C_FLAGS}")
target_compile_options(LAB04 PRIVATE "${OpenMP_C_FLAGS}")