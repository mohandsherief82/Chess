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