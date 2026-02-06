cmake_minimum_required(VERSION 3.28)

add_library(Minimax)

target_sources(
    Minimax
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/parsers.hpp
            include/minimax.hpp
    PRIVATE
        src/parsers.cpp
        src/minimax.cpp
)

target_link_libraries(
    Minimax
    PUBLIC
        GameEnd
)
