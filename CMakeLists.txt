cmake_minimum_required(VERSION 3.1)
project(birds)

set(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Threads)
SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
set(SOURCE_FILES main.cpp)
add_executable(birds ${SOURCE_FILES} birds.hpp birds.cpp)
