cmake_minimum_required(VERSION 3.10)

project(LPUCampusNavigator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(campus_navigator
    src/main.cpp
    src/Graph.cpp
    src/Dijkstra.cpp
    src/CSVReader.cpp
    src/Navigator.cpp
)

add_executable(campus_navigator_gui
    src/gui_main.cpp
    src/Graph.cpp
    src/Dijkstra.cpp
    src/CSVReader.cpp
    src/Navigator.cpp
)

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})