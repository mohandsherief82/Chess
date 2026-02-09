#include "parsers.hpp"

#include <cstring>
#include <cstdlib>

#include <cctype>


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
                        if (ply.king)
                        {
                            ply.king->rowPosition = i;
                            ply.king->colPosition = j;
                            ply.king->symbol = current_char;
                            ply.king->firstMove = false;
                        }
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
    if (!original_board) return nullptr;

    char** new_board = (char**)malloc(BOARD_SIZE * sizeof(char*));

    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        new_board[i] = (char*)malloc(BOARD_SIZE * sizeof(char));
   
        std::memcpy(new_board[i], original_board[i], BOARD_SIZE * sizeof(char));
    }

    return new_board;
}

/**
 * @brief Internal helper to allocate and copy memory only if source exists.
 */
static void safe_copy_block(void** dest, void* src, size_t size)
{
    if (src != nullptr)
    {
        *dest = std::malloc(size);

        if (*dest != nullptr)
            std::memcpy(*dest, src, size);
    }
    else *dest = nullptr;
}

/**
 * @brief Creates a deep copy of a Player structure, including sub-pointers.
 * @param player Pointer to the player to be copied.
 * @return A new Player struct with duplicated piece data.
 */
Player copy_player(const Player* player) 
{
    Player cpy;
    if (!player) 
    {
        std::memset(&cpy, 0, sizeof(Player));
        return cpy;
    }

    std::memcpy(&cpy, player, sizeof(Player));

    safe_copy_block((void**)&cpy.bishops, player->bishops, NUM_PIECES * sizeof(Bishop));
    safe_copy_block((void**)&cpy.knights, player->knights, NUM_PIECES * sizeof(Knight));
    
    safe_copy_block((void**)&cpy.rooks,   player->rooks,   NUM_PIECES * sizeof(Rook));
    safe_copy_block((void**)&cpy.pawns,   player->pawns,   NUM_PAWNS * sizeof(Pawn));
    
    safe_copy_block((void**)&cpy.queen,   player->queen,   sizeof(Queen));
    safe_copy_block((void**)&cpy.king,    player->king,    sizeof(King));

    return cpy;
}


/**
 * @brief Frees all memory associated with a specific engine state.
 * @param board_ptr Pointer to the 2D board array pointer.
 * @param ply1 Pointer to the first player.
 * @param ply2 Pointer to the second player.
 */
void free_engine_state(char*** board_ptr, Player* ply1, Player* ply2) 
{
    auto clear_player_pointers = [](Player* p) {
        if (!p) return;
        // Only free if they were dynamically allocated (copy_player does this)
        if (p->pawns)   { free(p->pawns);   p->pawns = nullptr;   }
        if (p->rooks)   { free(p->rooks);   p->rooks = nullptr;   }
        if (p->knights) { free(p->knights); p->knights = nullptr; }
        if (p->bishops) { free(p->bishops); p->bishops = nullptr; }
        if (p->queen)   { free(p->queen);   p->queen = nullptr;   }
        if (p->king)    { free(p->king);    p->king = nullptr;    }
    };

    clear_player_pointers(ply1);
    clear_player_pointers(ply2);
    
    if (board_ptr && *board_ptr) 
    {
        char** board = *board_ptr;
    
        for (int i = 0; i < BOARD_SIZE; i++) 
            if (board[i]) free(board[i]);

        free(board);

        *board_ptr = nullptr;
    }
}
