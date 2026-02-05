
#pragma once

#include <pybind11/pybind11.h>
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
    #include "saveGame.h"

    #include "staleMate.h"
    #include "checkMate.h"
}

namespace py = pybind11;
using namespace py::literals;

std::string move_piece(std::string board_string, std::string move_string);
bool check_mate(std::string board_string);
bool check_stalemate(std::string board_string);
