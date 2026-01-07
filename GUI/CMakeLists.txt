cmake_minimum_required(VERSION 3.28)
project(GUI LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Make sure this filename matches your file on disk!
add_executable(Gui)
target_sources(
    Gui
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/guiBoard.hpp
)
target_sources(
    Gui
    PRIVATE
        src/guiBoard.cpp
        test.cpp
)

target_link_libraries(Gui PRIVATE Qt6::Widgets)
