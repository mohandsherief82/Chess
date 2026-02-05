
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
                
                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].isActive = true;
                
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
                
                            ply.rooks[rook_idx].symbol = current_char;
                            ply.rooks[rook_idx].isActive = true;
                
                            rook_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;
                
                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].isActive = true;
                
                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].firstMove = false;
                
                            pawn_idx++;
                        }
                        break;

                    case 'n':
                        if (knight_idx < NUM_PIECES)
                        {
                            ply.knights[knight_idx].rowPosition = i;
                            ply.knights[knight_idx].colPosition = j;
                
                            ply.knights[knight_idx].symbol = current_char;
                            ply.knights[knight_idx].isActive = true;
                
                            knight_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;
                
                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].isActive = true;
                
                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].firstMove = false;
                
                            pawn_idx++;
                        }
                        break;

                    case 'b':
                        if (bishop_idx < NUM_PIECES)
                        {
                            ply.bishops[bishop_idx].rowPosition = i;
                            ply.bishops[bishop_idx].colPosition = j;

                            ply.bishops[bishop_idx].symbol = current_char;
                            ply.bishops[bishop_idx].isActive = true;

                            bishop_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].isActive = true;

                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].firstMove = false;

                            pawn_idx++;
                        }
                        break;

                    case 'q':
                        if (queen_idx < 1)
                        {
                            ply.queen[0].rowPosition = i;
                            ply.queen[0].colPosition = j;

                            ply.queen[0].symbol = current_char;
                            ply.queen[0].isActive = true;

                            queen_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].isActive = true;

                            ply.pawns[pawn_idx].promoted = true;
                            ply.pawns[pawn_idx].firstMove = false;

                            pawn_idx++;
                        }
                        break;

                    case 'k':
                        ply.king[0].rowPosition = i;
                        ply.king[0].colPosition = j;

                        ply.king[0].symbol = current_char;
                        ply.king[0].isActive = true;
                        
                        ply.king->firstMove = false;
                        break;
                }
            }
        } 
    }

    return ply;
}


/**
 * @brief Template to handle different piece types (Pawn, Rook, etc.) safely.
*/
template <typename T>
void process_missing_generic(Player *ply, Captured *captures, int current_count, int max_count, T *piece_array, char symbol)
{
    if (current_count < max_count) 
    {
        for (int i = current_count; i < max_count; i++) 
        {
            if (piece_array[i].isActive) 
            {
                captures->newCapture = true;
                captures->captureCount++;
                
                captures->capturedPiece.rowPosition = piece_array[i].rowPosition;
                captures->capturedPiece.colPosition = piece_array[i].colPosition;
                captures->capturedPiece.symbol = symbol;
                
                capturePiece(ply, captures);
            }
        }
    }
}


/**
 * @brief Parses the board to identify which pieces of the opponent have been captured.
 */
Captured captures_parser(char **board, Player *ply, PieceColor color)
{
    Captured captures { initializeCapture(color) }; 

    int pawn_count = 0, rook_count = 0, knight_count = 0, bishop_count = 0, queen_count = 0;

    auto is_player_piece = (ply->color == COLOR_WHITE) 
        ? [](char c) { return std::islower(c); }
        : [](char c) { return std::isupper(c); };
    
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            if (is_player_piece(board[i][j])) 
            {
                switch (std::tolower(board[i][j])) 
                {
                    case 'p': pawn_count++;   break;
                    case 'r': rook_count++;   break;
                   
                    case 'n': knight_count++; break;
                    case 'b': bishop_count++; break;
                   
                    case 'q': queen_count++;  break;
                }
            }
        }
    }
    
    process_missing_generic(ply, &captures, queen_count, 1, ply->queen, 'q');
    process_missing_generic(ply, &captures, bishop_count, NUM_PIECES, ply->bishops, 'b');

    process_missing_generic(ply, &captures, knight_count, NUM_PIECES, ply->knights, 'n');
    process_missing_generic(ply, &captures, rook_count, NUM_PIECES, ply->rooks, 'r');
    
    if (pawn_count < NUM_PAWNS) 
        process_missing_generic(ply, &captures, pawn_count, NUM_PAWNS, ply->pawns, 'p');

    return captures;
}


/**
 * @brief Parses a move string into a Move structure.
 */
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
