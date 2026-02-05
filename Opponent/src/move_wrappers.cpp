
#include "move_wrappers.hpp"


std::string move_piece(std::string board_string, std::string move_string)
{
    if (move_string.size() < 5) return board_string;

    char **board { board_parser(board_string) };

    Player ply1 { player_parser(board, COLOR_WHITE) };
    Player ply2 { player_parser(board, COLOR_BLACK) };

    Captured ply1_captures { captures_parser(board, &ply2, ply1.color) };
    Captured ply2_captures { captures_parser(board, &ply1, ply2.color) };
    int ply1_EP { -1 }, ply2_EP { -1 };

    Move move { move_parser(move_string) };

    MoveValidation valid;

    std::string new_board_str;
    new_board_str.reserve(BOARD_SIZE * BOARD_SIZE);

    switch (std::tolower(move.symbol))
    {
        case 'p': 
            valid = movePawn(board, &ply2, move, &ply1_captures, &ply1_EP, &ply2_EP, false, false);
            break;
        case 'r': 
            valid = moveRook(board, &ply2, move, &ply1_captures, false);
            break;
        case 'n': 
            valid = moveKnight(board, &ply2, move, &ply1_captures, false);
            break;
        case 'b': 
            valid = moveBishop(board, &ply2, move, &ply1_captures, false);
            break;
        case 'q': 
            valid = moveQueen(board, &ply2, move, &ply1_captures, false);
            break;
        case 'k': 
            valid = moveKing(board, &ply2, move, &ply1_captures, false);
            break;
        default: return board_string;
    }

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
            new_board_str.push_back(board[i][j]);
    }

    freeBoard(&board, &ply1, &ply2);

    return new_board_str;
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
