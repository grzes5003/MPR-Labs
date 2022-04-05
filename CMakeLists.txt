cmake_minimum_required(VERSION 3.16.3)
project(MPR_proj C)

set(CMAKE_C_STANDARD 99)
find_package(MPI)

#project(util C VERSION 1.0.1 DESCRIPTION "util library")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/util)
add_library(util SHARED util/helpers.h util/helpers.c)
set_target_properties(util PROPERTIES LINKER_LANGUAGE C)
#set_target_properties(util PROPERTIES VERSION ${PROJECT_VERSION})
#set_target_properties(util PROPERTIES SOVERSION 1)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(MPR_proj main.c)

target_link_libraries(MPR_proj ${MPI_C_LIBRARIES})

# LAB01
project(LAB01 C)

add_executable(LAB01 lab01/lab01.c)

target_link_libraries(LAB01 m ${MPI_C_LIBRARIES})

# LAB02
project(LAB02 C)

add_executable(LAB02 lab02/montecarlo_square.c)

target_link_libraries(LAB02 m ${MPI_C_LIBRARIES})

if(MPI_COMPILE_FLAGS)
    set_target_properties(LAB01 PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(LAB01 PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()


project(LAB03 C)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(LAB03 lab03/paraller_for.c lab03/lehmer64.h)

target_link_libraries(LAB03 PRIVATE "${OpenMP_C_FLAGS}")
target_compile_options(LAB03 PRIVATE "${OpenMP_C_FLAGS}")

project(LAB03_2 C)

add_executable(LAB03_2 lab03/paraller_for_static.c)

target_link_libraries(LAB03_2 PRIVATE "${OpenMP_C_FLAGS}")
target_compile_options(LAB03_2 PRIVATE "${OpenMP_C_FLAGS}")

project(LAB04_seq C)

add_executable(LAB04_seq lab03/sort_alg_seq.c)