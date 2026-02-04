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


bool move_piece(std::string board_string, std::string move_string)
{
    if (move_string.size() < 4) return false;

    char **board { board_parser(board_string) };

    Player ply1 { player_parser(board, COLOR_WHITE) };
    Player ply2 { player_parser(board, COLOR_BLACK) };

    Captured ply1_captures { captures_parser(board, ply1) };
    Captured ply2_captures { captures_parser(board, ply2) };

    int ply1_EP { -1 }, ply2_EP { -1 };

    Move move { move_parser(move_string) };

    MoveValidation valid;

    switch (std::tolower(move.symbol))
    {
        case 'p': 
            valid = movePawn(board, &ply1, move, &ply1_captures, &ply1_EP, &ply2_EP, false, false);
            break;
        case 'r': 
            valid = moveRook(board, &ply1, move, &ply1_captures, false);
            break;
        case 'n': 
            valid = moveKnight(board, &ply1, move, &ply1_captures, false);
            break;
        case 'b': 
            valid = moveBishop(board, &ply1, move, &ply1_captures, false);
            break;
        case 'q': 
            valid = moveQueen(board, &ply1, move, &ply1_captures, false);
            break;
        case 'k': 
            valid = moveKing(board, &ply1, move, &ply1_captures, false);
            break;
        default: return false;
    }

    freeBoard(&board, &ply1, &ply2);

    if (valid == INVALID_MOVE) return false;
    else return true;
}


bool check_mate(std::string board_string)
{
    char **board { board_parser(board_string) };

    Player ply1 { player_parser(board, COLOR_WHITE) };
    Player ply2 { player_parser(board, COLOR_BLACK) };

    bool state = checkMate(board, &ply1);

    freeBoard(&board, &ply1, &ply2);

    return state;
}


bool check_stalemate(std::string board_string)
{
    char **board { board_parser(board_string) };

    Player ply1 { player_parser(board, COLOR_WHITE) };
    Player ply2 { player_parser(board, COLOR_BLACK) };

    bool state = checkStalemate(board, &ply1);

    freeBoard(&board, &ply1, &ply2);

    return state;
}


PYBIND11_MODULE(Move_Wrappers, m)
{
    m.doc() = "Wrappers for all moving function implemented in C and moddified by C++";

    m.def("move_piece", &move_piece, "A Wrapper Function that wraps the pieces move functions all at once",
        "board_string"_a, "move_string"_a);

    m.def("check_mate", &check_mate, "Checks whether the game reached a state of checkmate."
        , "board_string"_a);
    m.def("check_stalemate", &check_stalemate, "Checks whether the game reached a state of checkmate."
        , "board_string"_a);
}
