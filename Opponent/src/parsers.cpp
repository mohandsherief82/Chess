#include "parsers.hpp"

#include <cstring>
#include <cstdlib>


/**
 * @brief Converts a string representation of the board into a 2D C-style array.
 * @param board_string The 64-character string representing the board.
 * @return A dynamically allocated 2D char array.
 */
char** board_parser(std::string board_string) 
{
    char** board = (char**)malloc(BOARD_SIZE * sizeof(char*));

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        board[i] = (char*)malloc(BOARD_SIZE * sizeof(char));

        for (int j = 0; j < BOARD_SIZE; j++) board[i][j] = board_string[i * BOARD_SIZE + j];
    }

    return board;
}


/**
 * @brief Parses a 2D board array to populate a Player structure with piece positions and states.
 * @param board The 8x8 character array representing the current board state.
 * @param color The color of the player to be parsed (COLOR_WHITE or COLOR_BLACK).
 * @return A populated Player structure containing all found pieces and their coordinates.
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
                            rook_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].symbol = current_char; 
                            ply.pawns[pawn_idx].promoted = true;

                            ply.pawns[pawn_idx].isActive = true; 
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
                            knight_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].promoted = true;

                            ply.pawns[pawn_idx].isActive = true; 
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
                            bishop_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].promoted = true;

                            ply.pawns[pawn_idx].isActive = true; 
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
                            queen_idx++;
                        }
                        else if (pawn_idx < NUM_PAWNS)
                        {
                            ply.pawns[pawn_idx].rowPosition = i;
                            ply.pawns[pawn_idx].colPosition = j;

                            ply.pawns[pawn_idx].symbol = current_char;
                            ply.pawns[pawn_idx].promoted = true;
                        
                            ply.pawns[pawn_idx].isActive = true; 
                            ply.pawns[pawn_idx].firstMove = false;
                        
                            pawn_idx++;
                        }

                        break;

                    case 'k':
                        ply.king->rowPosition = i;
                        ply.king->colPosition = j;

                        ply.king->symbol = current_char;
                        ply.king->firstMove = false;

                        break;
                }
            }
        } 
    }

    return ply;
}


/**
 * @brief Creates a deep copy of the current board state.
 * @param original_board The pointer to the current 2D board.
 * @return A new 2D array with identical contents.
 */
char** copy_board(char** original_board) 
{
    char** new_board = (char**)malloc(BOARD_SIZE * sizeof(char*));

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        new_board[i] = (char*)malloc(BOARD_SIZE * sizeof(char));
        std::memcpy(new_board[i], original_board[i], BOARD_SIZE * sizeof(char));
    }

    return new_board;
}


/**
 * @brief Creates a deep copy of a Player structure, including sub-pointers.
 * @param original_player Pointer to the player to be copied.
 * @return A new Player struct with duplicated piece data.
 */
Player copy_player(const Player* original_player) 
{
    Player new_player = *original_player;

    new_player.queen = (Queen*)malloc(sizeof(Queen));
    *new_player.queen = *original_player->queen;
    
    new_player.king = (King*)malloc(sizeof(King));
    *new_player.king = *original_player->king;
    
    return new_player;
}


/**
 * @brief Frees all memory associated with a specific engine state.
 * @param board The 2D board array.
 * @param ply1 Pointer to the first player.
 * @param ply2 Pointer to the second player.
 */
void free_engine_state(char** board, Player* ply1, Player* ply2) 
{
    if (board) 
    {
        for (int i = 0; i < BOARD_SIZE; i++) free(board[i]);
    
        free(board);
    }

    if (ply1) { free(ply1->queen); free(ply1->king); }
    
    if (ply2) { free(ply2->queen); free(ply2->king); }
}