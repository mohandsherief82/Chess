cmake_minimum_required(VERSION 3.28)
project(GUI LANGUAGES CXX)

# Enable Qt automation tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON) 

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

# Define the executable
add_executable(Gui)

# Manage sources and headers
target_sources(
    Gui
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/guiBoard.hpp
    PRIVATE
        src/guiBoard.cpp
        test.cpp
        resources.qrc
)

# Ensure the compiler can find your headers
target_include_directories(Gui PRIVATE include ..)

# Link the required libraries
target_link_libraries(Gui PRIVATE Qt6::Widgets Qt6::Svg)