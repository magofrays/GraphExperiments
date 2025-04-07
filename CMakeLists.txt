cmake_minimum_required(VERSION 3.25)
project(GraphExperiments)

set(CMAKE_CXX_STANDARD 20)

add_executable(GraphExperiments)
add_subdirectory(src)
include_directories(GraphExperiments src)

