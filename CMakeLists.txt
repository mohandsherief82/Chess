cmake_minimum_required(VERSION 3.28)
project(Chess LANGUAGES C CXX)

# Looks in this folder (the root) for headers like chessTypes.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Define the main game
add_executable(Game main.c)

add_subdirectory(Pieces)
add_subdirectory(Board)
add_subdirectory(Moves)
add_subdirectory(GameEnd)
add_subdirectory(GUI)

# Link everything
target_link_libraries(
    Game 
    PRIVATE 
        Pieces 
        Board 
        Moves 
        GameEnd
        Gui
)