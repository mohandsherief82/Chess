#include "pawnMoves.h"
#include "rookMoves.h"

#include "knightMoves.h"
#include "bishopMoves.h"

#include "queenMoves.h"
#include "kingMoves.h"

#include "captures.h"
#include "board.h"

#include "chessTypes.h"
#include "player.h"

#include "check.h"

#include <stdio.h>
#include <stdbool.h>

#include <ctype.h>
#include <stdlib.h>

#include <string.h>


char** copyBoard(char** board)
{
    char** cpy_board = malloc(sizeof(char*) * BOARD_SIZE);
    
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cpy_board[i] = malloc(sizeof(char) * BOARD_SIZE);
        memcpy(cpy_board[i], board[i], BOARD_SIZE * sizeof(char)); 
    }
    
    return cpy_board;
}


Player copyPlayer(Player* player)
{
    Player cpy;
    memcpy(&cpy, player, sizeof(Player));

    cpy.bishops = malloc(NUM_PIECES * sizeof(Bishop));
    memcpy(cpy.bishops, player->bishops, NUM_PIECES * sizeof(Bishop)); 

    cpy.knights = malloc(NUM_PIECES * sizeof(Knight));
    memcpy(cpy.knights, player->knights, NUM_PIECES * sizeof(Knight));

    cpy.rooks = malloc(NUM_PIECES * sizeof(Rook));
    memcpy(cpy.rooks, player->rooks, NUM_PIECES * sizeof(Rook));

    cpy.pawns = malloc(NUM_PAWNS * sizeof(Pawn));
    memcpy(cpy.pawns, player->pawns, NUM_PAWNS * sizeof(Pawn));

    cpy.queen = malloc(sizeof(Queen));
    memcpy(cpy.queen, player->queen, sizeof(Queen));

    cpy.king = malloc(sizeof(King));
    memcpy(cpy.king, player->king, sizeof(King));

    return cpy;
}


void freeCopy(Player cpy_player, char** cpy_board)
{
    free(cpy_player.pawns);
    free(cpy_player.rooks);

    free(cpy_player.knights);
    free(cpy_player.bishops);

    free(cpy_player.queen);
    free(cpy_player.king);
    
    for (int i = 0; i < BOARD_SIZE; i++) 
        free(cpy_board[i]);
    
    free(cpy_board);
}


static bool can_piece_move(char** board, Player* player, Piece* p, char type)
{
    if (!p->isActive) return false;

    Move test_move;
    
    int ply_ep_col = -1, opp_ep_col = -1;

    test_move.rowPrev = p->rowPosition;
    test_move.colPrev = p->colPosition;

    for (int r = 0; r < BOARD_SIZE; r++)
    {
        for (int c = 0; c < BOARD_SIZE; c++)
        {
            if (r == test_move.rowPrev && c == test_move.colPrev) continue;
            
            if (!isEmpty(board, r, c) && pieceColorAt(board, r, c) == player->color) continue;

            test_move.rowNext = r;
            test_move.colNext = c;

            char** cpy_b = copyBoard(board);
            
            Player cpy_p = copyPlayer(player);
            
            Captured temp_cap = {0};
            
            MoveValidation state = INVALID_MOVE;

            if (type == 'p') state = movePawn(cpy_b, &cpy_p, test_move, &temp_cap, &ply_ep_col, &opp_ep_col, true, false);
            else if (type == 'r') state = moveRook(cpy_b, &cpy_p, test_move, &temp_cap, true);
            else if (type == 'n') state = moveKnight(cpy_b, &cpy_p, test_move, &temp_cap, true);
            else if (type == 'b') state = moveBishop(cpy_b, &cpy_p, test_move, &temp_cap, true);
            else if (type == 'q') state = moveQueen(cpy_b, &cpy_p, test_move, &temp_cap, true);
            else if (type == 'k') state = moveKing(cpy_b, &cpy_p, test_move, &temp_cap, true);

            if (state != INVALID_MOVE && !isChecked(cpy_b, &cpy_p, true))
            {
                freeCopy(cpy_p, cpy_b);
                return true;
            }
            
            freeCopy(cpy_p, cpy_b);
        }
    }
    
    return false;
}


bool legalMove(char** board, Player* player)
{
    for (int i = 0; i < NUM_PAWNS; i++) 
        if (can_piece_move(board, player, (Piece*)&player->pawns[i], 'p')) return true;

    for (int i = 0; i < NUM_PIECES; i++) 
    {
        if (can_piece_move(board, player, (Piece*)&player->rooks[i], 'r')) return true;
        if (can_piece_move(board, player, (Piece*)&player->knights[i], 'n')) return true;
        if (can_piece_move(board, player, (Piece*)&player->bishops[i], 'b')) return true;
    }

    if (can_piece_move(board, player, (Piece*)player->queen, 'q')) return true;
    if (can_piece_move(board, player, (Piece*)player->king, 'k')) return true;

    return false;
}


bool inSufficientMaterial(Player* player1, Player* player2)
{
    if (player1->queen->isActive || player2->queen->isActive) return false;
    
    for (int i = 0; i < NUM_PAWNS; i++) 
    {
        if (player1->pawns[i].isActive || player2->pawns[i].isActive) return false;
    }

    for (int i = 0; i < NUM_PIECES; i++) 
    {
        if (player1->rooks[i].isActive || player2->rooks[i].isActive) return false;
    }

    int p1_minor = 0;
    int p2_minor = 0;

    for (int i = 0; i < NUM_PIECES; i++) 
    {
        if (player1->bishops[i].isActive) p1_minor++;
        if (player1->knights[i].isActive) p1_minor++;
        if (player2->bishops[i].isActive) p2_minor++;
        if (player2->knights[i].isActive) p2_minor++;
    }

    if (p1_minor == 0 && p2_minor == 0) return true;
    if ((p1_minor == 1 && p2_minor == 0) || (p1_minor == 0 && p2_minor == 1)) return true;
    if (p1_minor == 1 && p2_minor == 1) return true;

    return false;
}


bool checkStalemate(char** board, Player* player, Player* opponent)
{
    if (inSufficientMaterial(player, opponent)) return true;
    
    return !legalMove(board, player) && !isChecked(board, player, true);
}
