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

add_executable(LAB01 lab01/diff_variants.c lab01/lab01.c)

if(MPI_COMPILE_FLAGS)
    set_target_properties(LAB01 PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(LAB01 PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()