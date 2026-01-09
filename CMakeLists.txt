cmake_minimum_required(VERSION 3.28)
project(Chess LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# AUTOMOC: Handles Qt's meta-object system (signals/slots)
# AUTOUIC: Handles .ui files from Qt Designer
# AUTORCC: Handles .qrc resource files (your icons)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

# Looks in this folder (the root) for headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Define the main game executable
add_executable(Game main.cpp)

# Define subdirectories
add_subdirectory(Pieces)
add_subdirectory(Board)
add_subdirectory(Moves)
add_subdirectory(GameEnd)
add_subdirectory(GUI)

# Link all libraries
target_link_libraries(
    Game 
    PRIVATE 
        Pieces 
        Board 
        Moves 
        GameEnd
        Gui
        Qt6::Widgets
        Qt6::Svg
)

# Add game binary directories to the build
add_custom_command(
    TARGET 
        Game 
    POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/GameBin/Load"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/GameBin/Redo"
)
