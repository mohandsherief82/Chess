cmake_minimum_required(VERSION 3.28)

add_library(Pieces STATIC)
target_sources(
    Pieces
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include/
        FILES
            include/knight.h
            include/pawn.h
            include/bishop.h
            include/king.h
            include/queen.h
            include/captures.h
            include/player.h
            include/rook.h
)

target_sources(
    Pieces
    PRIVATE
        src/knight.c
        src/pawn.c
        src/bishop.c
        src/king.c
        src/queen.c
        src/captures.c
        src/player.c
        src/rook.c
)
