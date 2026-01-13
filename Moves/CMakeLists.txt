cmake_minimum_required(VERSION 3.28)

add_library(Moves STATIC)
target_sources(
    Moves
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include/
        FILES
            include/knightMoves.h
            include/pawnMoves.h
            include/bishopMoves.h
            include/kingMoves.h
            include/queenMoves.h
            include/check.h
            include/rookMoves.h
)

target_sources(
    Moves
    PRIVATE
        src/knightMoves.c
        src/pawnMoves.c
        src/bishopMoves.c
        src/kingMoves.c
        src/queenMoves.c
        src/check.c
        src/rookMoves.c
)

target_link_libraries(Moves PUBLIC Board)
