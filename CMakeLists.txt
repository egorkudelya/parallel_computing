cmake_minimum_required(VERSION 3.24)
project(parallel_computing1)

set(CMAKE_CXX_STANDARD 17)


add_library(conc STATIC
        queue/queue.h
        pool/thread_pool.cpp
        pool/thread_pool.h
        utils/timer.cpp
        utils/timer.h
        )

set_target_properties(conc PROPERTIES LINKER_LANGUAGE CXX)

find_package(Eigen3 REQUIRED)
link_directories(${Eigen_INCLUDE_DIRS})

target_link_libraries(conc Eigen3::Eigen)

add_executable(parallel_computing1 main.cpp)
target_link_libraries(parallel_computing1 conc)