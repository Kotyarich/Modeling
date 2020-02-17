cmake_minimum_required(VERSION 3.13)
project(lab1)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O3")

#set(CMAKE_CXX_STANDARD 14)

add_executable(lab1 main.cpp polynomial.cpp polynomial.h)