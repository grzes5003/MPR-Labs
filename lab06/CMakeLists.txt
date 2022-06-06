cmake_minimum_required(VERSION 3.16)
project(lab06
        VERSION 0.1
        LANGUAGES CXX CUDA
        )
find_package(CUDA)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions("-DHAS_BOOST")
#    add_definitions( -DBOOST_ALL_DYN_LINK )
ENDIF ()

set(CMAKE_CUDA_STANDARD 14)
#set(TIMERS ./timers)

#add_subdirectory(${TIMERS})
include_directories(${TIMERS})

add_executable(lab06
        main.cu
        cpu_vec_add.cpp
        args_parser.cpp
        args.h
        Utility.hpp Utility.cpp
        timers/Timeit.cpp
        timers/Timeit.hpp
        timers/CpuTimer.hpp
        timers/CpuTimer.cpp
        timers/Timer.hpp
        timers/GpuTimer.cuh
        timers/GpuTimer.cu
        )
#target_compile_features(lab06 PUBLIC cxx_std_14)
#target_link_libraries(lab06 -static)
#set_target_properties(Timers PROPERTIES LINKER_LANGUAGE CUDA)
target_link_libraries(lab06 ${Boost_LIBRARIES})

#target_link_libraries(lab06 )
#target_include_directories(lab06 PUBLIC
#        "${PROJECT_BINARY_DIR}"
#        "${PROJECT_SOURCE_DIR}/timers"
#        )

#target_compile_options(lab06 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
#        -rdc=true
#        >)
set_target_properties(lab06 PROPERTIES CUDA_ARCHITECTURES "30")
set_target_properties(lab06 PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

#set_property(TARGET lab06 PROPERTY CUDA_RESOLVE_DEVICE_SYMBOLS ON)

#install(TARGETS lab06 DESTINATION bin)
