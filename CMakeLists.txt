cmake_minimum_required(VERSION 2.8)
project(Assign5)


if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -O3 ${CMAKE_CXX_FLAGS}")
endif()

add_executable(Assign5 main.cpp)
TARGET_LINK_LIBRARIES( Assign5 pthread)
