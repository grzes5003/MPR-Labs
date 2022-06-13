cmake_minimum_required(VERSION 3.16)

project(reduction
        VERSION 0.1
        LANGUAGES CXX CUDA
        )

add_executable(reduction
        "reduction/reduction.h"
        "reduction/reduction_global.cu"
        "reduction/reduction_global_kernel.cu"
        "reduction/reduction_shared.cu"
        "reduction/reduction_shared_kernel.cu"
        exception.h
        helper_timer.h)
set_target_properties(reduction PROPERTIES CUDA_ARCHITECTURES "30")
set_target_properties(reduction PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)


project(atomico_perations
        VERSION 0.1
        LANGUAGES CXX CUDA
        )

add_executable(atomico_perations
        "atomico_perations/reduction.cu"
        "atomico_perations/reduction.h"
        "atomico_perations/reduction_blk_atmc_kernel.cu"
        "atomico_perations/reduction_kernel.cu"
        "atomico_perations/reduction_wrp_atmc_kernel.cu"
        exception.h
        helper_timer.h)
set_target_properties(atomico_perations PROPERTIES CUDA_ARCHITECTURES "30")
set_target_properties(atomico_perations PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)


project(histogram
        VERSION 0.1
        LANGUAGES CXX CUDA
        )

add_executable(histogram
        "histogram/gputimer.h"
        "histogram/histo.cu"
        exception.h
        helper_timer.h)
set_target_properties(histogram PROPERTIES CUDA_ARCHITECTURES "30")
set_target_properties(histogram PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

project(loop_unrolling
        VERSION 0.1
        LANGUAGES CXX CUDA
        )

add_executable(loop_unrolling
        "loop_unrolling/reduction.cu"
        "loop_unrolling/reduction.h"
        "loop_unrolling/reduction_cg_kernel.cu"
        "loop_unrolling/reduction_wp_kernel.cu"
        exception.h
        helper_timer.h)
set_target_properties(loop_unrolling PROPERTIES CUDA_ARCHITECTURES "30")
set_target_properties(loop_unrolling PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

project(reduction
        VERSION 0.1
        LANGUAGES CXX CUDA
        )

add_executable(warp_divergence
        "warp_divergence/reduction.cu"
        "warp_divergence/reduction.h"
        "warp_divergence/reduction_kernel_interleaving.cu"
        "warp_divergence/reduction_kernel_sequential.cu"
        exception.h
        helper_timer.h)
set_target_properties(warp_divergence PROPERTIES CUDA_ARCHITECTURES "30")
set_target_properties(warp_divergence PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)


include_directories("reduction")
include_directories("atomico_perations")
include_directories("histogram")
include_directories("loop_unrolling")
include_directories("warp_divergence")

include_directories(.)
set(CMAKE_CXX_STANDARD 14)
find_package(CUDA)