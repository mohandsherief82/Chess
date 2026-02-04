
#include "parsers.hpp"

char **board_parser(std::string board_string)
{
    char **board { initializeBoard() };

    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = board_string[i * (BOARD_SIZE - 1) + j];

    return board;
}


Player player_parser(char **board, PieceColor color)
{}


Captured captures_parser(char **board, Player ply)
{}


Move move_parser(std::string move_string)
{}
