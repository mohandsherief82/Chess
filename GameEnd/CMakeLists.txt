cmake_minimum_required(VERSION 3.28)

add_library(GameEnd)
target_sources(
    GameEnd
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include/
        FILES
            include/staleMate.h
            include/saveGame.h
            include/checkMate.h
)

target_sources(
    GameEnd
    PRIVATE
        src/staleMate.c
        src/saveGame.c
        src/checkMate.c
)

target_link_libraries(GameEnd PUBLIC Moves)
