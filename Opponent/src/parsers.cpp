
#include "parsers.hpp"

char** board_parser(std::string board_string) 
{
    char** board = new char*[BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        board[i] = new char[BOARD_SIZE];
        
        for (int j = 0; j < BOARD_SIZE; j++) 
            board[i][j] = board_string[i * BOARD_SIZE + j];
    }

    return board;
}


Player player_parser(char **board, PieceColor color)
{
    Player ply { createPlayer(color) };

    std::function<int(int)> check_piece = (color == COLOR_WHITE) 
        ? [](int c) { return std::islower(c); }
        : [](int c) { return std::isupper(c); };

    return ply;
}


Captured captures_parser(char **board, Player ply)
{}


Move move_parser(std::string move_string)
{}
