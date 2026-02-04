
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


/**
 * @brief Parses a 2D board array to populate a Player structure with piece coordinates and promotion states.
 */
Player player_parser(char **board, PieceColor color)
{
    Player ply { createPlayer(color) };
    
    int pawn_idx = 0, rook_idx = 0, knight_idx = 0,
        bishop_idx = 0, queen_idx = 0;

    std::function<int(int)> check_piece = (color == COLOR_WHITE) 
        ? [](int c) { return std::islower(c); }
        : [](int c) { return std::isupper(c); };

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            char current_char = board[i][j];

            if (check_piece(current_char))
            {
                char piece_type = std::tolower(current_char);

                switch (piece_type)
                {
                    case 'p':
                        if (pawn_idx < NUM_PAWNS) 
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].promoted = false;
                            ply.pawns[pawn_idx].firstMove = false;
                            pawn_idx++;
                        }
                        break;

                    case 'r':
                        if (rook_idx < NUM_PIECES)
                        {
                            ply.rooks[rook_idx].rowPosition = i;
                            ply.rooks[rook_idx].colPosition = j;

                            rook_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].symbol = current_char; 
                         
                            ply.pawns[pawn_idx].firstMove = false;
                            pawn_idx++;
                        }
                        break;

                    case 'n':
                        if (knight_idx < NUM_PIECES)
                        {
                            ply.knights[knight_idx].rowPosition = i;
                            ply.knights[knight_idx].colPosition = j;

                            knight_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].symbol = current_char;
                            
                            ply.pawns[pawn_idx].firstMove = false;
                            pawn_idx++;
                        }
                        break;

                    case 'b':
                        if (bishop_idx < NUM_PIECES)
                        {
                            ply.bishops[bishop_idx].rowPosition = i;
                            ply.bishops[bishop_idx].colPosition = j;

                            bishop_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].symbol = current_char;

                            ply.pawns[pawn_idx].firstMove = false;
                            pawn_idx++;
                        }
                        break;

                    case 'q':
                        if (queen_idx < 1)
                        {
                            ply.queen[0].rowPosition = i;
                            ply.queen[0].colPosition = j;

                            queen_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].symbol = current_char;

                            ply.pawns[pawn_idx].firstMove = false;
                            pawn_idx++;
                        }
                        break;

                    case 'k':
                        ply.king[0].rowPosition = i;
                        ply.king[0].colPosition = j;

                        ply.king->firstMove = false;
                        break;
                }
            }
        } 
    }

    return ply;
}


Captured captures_parser(char **board, Player ply)
{}


Move move_parser(std::string move_string)
{
    Move move;

    move.symbol = move_string[0];

    move.colPrev = move_string[1] - 'A';
    move.rowPrev = move_string[2];

    move.colNext = move_string[3] - 'A';
    move.rowNext = move_string[4];

    if (move_string.size() == 6) move.promotedPawn = move_string[5];
    else move.promotedPawn = ' ';
    
    return move;
}
