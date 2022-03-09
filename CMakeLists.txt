cmake_minimum_required(VERSION 3.16.3)
project(MPR_proj C)

set(CMAKE_C_STANDARD 99)
find_package(MPI)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(MPR_proj main.c)

target_link_libraries(MPR_proj ${MPI_C_LIBRARIES})

# LAB01
project(LAB01 C)

add_executable(LAB01 lab01/diff_variants.c util/helpers.h util/helpers.c)