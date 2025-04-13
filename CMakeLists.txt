cmake_minimum_required(VERSION 3.25)
project(GraphExperiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(GraphExperiments
    src/main.cpp
    src/graph.cpp
    src/visualizer.cpp
    src/graph_algorithms.cpp
    src/graph_tester.cpp
)
target_include_directories(GraphExperiments PRIVATE 
    src/headers
)


target_link_libraries(GraphExperiments PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
