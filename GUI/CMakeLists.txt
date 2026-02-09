cmake_minimum_required(VERSION 3.28)
project(GUI LANGUAGES CXX)

# Enable Qt automation tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON) 

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

# Define the executable
add_library(Gui SHARED resources.qrc)

# Manage sources and headers
target_sources(
    Gui
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/piecesIcon.hpp
            include/interfaces.hpp
            include/startMenu.hpp
            include/boardCell.hpp
            include/guiBoard.hpp
            include/helpers.hpp
            include/dialog.hpp
    PRIVATE
        src/piecesIcon.cpp
        src/interfaces.cpp
        src/startMenu.cpp
        src/boardCell.cpp
        src/guiBoard.cpp
        src/helpers.cpp
        src/dialog.cpp
)

# Link the required libraries
target_link_libraries(
    Gui 
    PRIVATE 
        Qt6::Widgets 
        Qt6::Svg
    PUBLIC
        Minimax
)
