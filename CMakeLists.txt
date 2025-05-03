cmake_minimum_required(VERSION 3.25)
project(GraphExperiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)


add_executable(LDZ2
    LDZ2/main.cpp
    LDZ2/src/graph.cpp
    LDZ2/src/visualizer.cpp
    LDZ2/src/graph_algorithms.cpp
    LDZ2/src/graph_tester.cpp
)
add_executable(LDZ3
    LDZ3/main.cpp
)
target_include_directories(LDZ3 PRIVATE 
    LDZ3/src/headers
)
target_include_directories(LDZ2 PRIVATE 
    LDZ2/src/headers
)


target_link_libraries(LDZ2 PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)
