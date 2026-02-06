cmake_minimum_required(VERSION 3.15)

find_package(pybind11 REQUIRED)

pybind11_add_module(Move_Wrappers)

target_sources(
    Move_Wrappers
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include
        FILES
            include/parsers.hpp
            include/minimax.hpp
    PRIVATE
        src/parsers.cpp
        src/minimax.cpp
)

target_link_libraries(
    Move_Wrappers 
    PRIVATE 
        pybind11::module
        pybind11::embed
    PUBLIC
        GameEnd
)

set_target_properties(Move_Wrappers PROPERTIES PREFIX "" SUFFIX "${PYTHON_MODULE_EXTENSION}")

add_custom_command(
    TARGET Move_Wrappers POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Move_Wrappers> ${CMAKE_BINARY_DIR}
    COMMENT "Copying Minimax Python module to main build directory..."
)
