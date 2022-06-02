cmake_minimum_required(VERSION 3.16)
project(lab06 CUDA)
find_package(CUDA)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions( "-DHAS_BOOST" )
ENDIF()
set(CMAKE_CUDA_STANDARD 14)


add_executable(lab06 main.cu cpu_vec_add.cpp GpuTimer.cu GpuTimer.h args_parser.cpp args.h Utility.cpp Utility.h Timer.h)
target_compile_features(lab06 PUBLIC cxx_std_14)
target_link_libraries(lab06 -static)
target_link_libraries(lab06 ${Boost_LIBRARIES})

target_compile_options(lab06 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
        -rdc=true
        >)
set_target_properties(lab06 PROPERTIES CUDA_ARCHITECTURES "30")
set_target_properties(lab06 PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
