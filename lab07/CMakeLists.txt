cmake_minimum_required(VERSION 3.16)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions("-DHAS_BOOST")
ENDIF ()

set(CMAKE_CUDA_STANDARD 14)

project(matrix_transpose
        VERSION 0.1
        LANGUAGES CXX CUDA
        )

project(matrix_transpose_2
        VERSION 0.1
        LANGUAGES CXX CUDA
        )


project(image_scaling
        VERSION 0.1
        LANGUAGES CXX CUDA
        )

find_package(CUDA)

add_executable(image_scaling
        image_scaling.cu
        scrImagePgmPpmPackage.h
        scrImagePgmPpmPackage.cpp
        arg_parser.cpp args.h GpuTimer.cu GpuTimer.h BLOCK_SIZE.h)
set_target_properties(image_scaling PROPERTIES CUDA_ARCHITECTURES "30")

add_executable(matrix_transpose matrix_transpose.cu arg_parser.cpp args.h GpuTimer.cu GpuTimer.h)
set_target_properties(matrix_transpose PROPERTIES CUDA_ARCHITECTURES "30")

add_executable(matrix_transpose_2 matrix_transpose_2.cu arg_parser.cpp args.h GpuTimer.cu GpuTimer.h)
set_target_properties(matrix_transpose_2 PROPERTIES CUDA_ARCHITECTURES "30")