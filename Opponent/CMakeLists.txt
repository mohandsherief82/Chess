cmake_minimum_required(VERSION 3.15)
project(example )

find_package(pybind11 REQUIRED)

add_executable(example)

target_sources(
    example
    PRIVATE
        src/main.cpp
)

target_link_libraries(example PRIVATE pybind11::embed)
