cmake_minimum_required(VERSION 3.23)
project(hw4)
set(CMAKE_CXX_STANDARD 14)
#find_package(MPI REQUIRED)
#hw4.cpp
add_executable(homework4  main.cpp readData.cpp)


#include_directories(MPI_INCLUDE_PATH)
#target_link_libraries(homework4 PUBLIC MPI::MPI_CXX)


