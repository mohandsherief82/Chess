
#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <cctype>

#include "parsers.hpp"

extern "C"
{
    #include "pawnMoves.h"
    #include "rookMoves.h"
    
    #include "knightMoves.h"
    #include "bishopMoves.h"
    
    #include "queenMoves.h"
    #include "kingMoves.h"
    
    #include "check.h"
    #include "staleMate.h"
 
    #include "checkMate.h"
}

#define MOVE_BUFFER 6

namespace py = pybind11;
using namespace py::literals;

std::string move_piece(std::string board_string, std::string move_string);
bool check_mate(std::string board_string, int color_int);
bool check_stalemate(std::string board_string, int color_int);
std::vector<std::string> get_available_moves(std::string board_string, int color_int);
