cmake_minimum_required(VERSION 3.28)

add_library(Board STATIC)
target_sources(
    Board
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include/
        FILES
            include/board.h
)

target_sources(
    Board
    PRIVATE
        src/board.c
)

target_link_libraries(Board PUBLIC Pieces)
