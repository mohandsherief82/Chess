cmake_minimum_required(VERSION 3.28)
project(Chess LANGUAGES C)

# Looks in this folder (the root) for headers like chessTypes.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Define the main game
add_executable(Game main.c chessTypes.h)

add_subdirectory(Pieces)
add_subdirectory(Board)
add_subdirectory(Moves)
add_subdirectory(GameEnd)

# Link everything
target_link_libraries(
    Game 
    PRIVATE 
        Pieces 
        Board 
        Moves 
        GameEnd
)