cmake_minimum_required(VERSION 3.15)
project(Minimax )

find_package(pybind11 REQUIRED)

add_library(Minimax)

target_sources(
    Minimax
    PRIVATE
        src/algorithm.cpp
        src/move_wrappers.cpp
)

target_link_libraries(
    Minimax 
    PRIVATE 
        pybind11::embed
    PUBLIC
        GameEnd
)
