cmake_minimum_required(VERSION 3.23)
project(hw3)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-pthread")

add_executable(homework3 main.cpp hw3.cpp hw3.h readData.cpp LinkedList.cpp)
