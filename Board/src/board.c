#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "chessTypes.h"
#include "player.h"
#include "board.h"


extern char* path;


char** initializeBoard()
{
    char **board = (char**)malloc(BOARD_SIZE * sizeof(char*));
    if (board == NULL) exit(1);

    for(int i = 0; i < BOARD_SIZE; i++)
    {
        board[i] = (char*)malloc(BOARD_SIZE * sizeof(char));
        if (board[i] == NULL) exit(1);
        for (int j = 0; j < BOARD_SIZE; j++) board[i][j]= EMPTY_SQUARE;
    }

    return board;
}


long getFileSize(FILE *fp)
{
    long current_pos = ftell(fp);
    fseek(fp, 0, SEEK_END);

    long size = ftell(fp);
    fseek(fp, current_pos, SEEK_SET);
    
    return size;
}


void addPieces(char** board, void* piecesArray, int numPieces, size_t piece_size)
{
    char* current_piece = (char*)piecesArray;

    if (piecesArray == NULL) return;

    for (int i = 0; i < numPieces; i++)
    {
        Piece* p = (Piece*)(current_piece + i * piece_size);

        int col = p->colPosition;
        int row = p->rowPosition;

        if ((col >= 0 && col < 8) && (row >= 0 && row < 8) && p->isActive) board[row][col] = p->symbol;
    }
}

void updateBoard(char** board, Player *player1, Player *player2)
{
    for(int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = EMPTY_SQUARE;

    addPieces(board, player1->pawns, NUM_PAWNS, sizeof(Pawn));
    addPieces(board, player2->pawns, NUM_PAWNS, sizeof(Pawn));

    addPieces(board, player1->rooks, NUM_PIECES, sizeof(Rook));
    addPieces(board, player2->rooks, NUM_PIECES, sizeof(Rook));

    addPieces(board, player1->knights, NUM_PIECES, sizeof(Knight));
    addPieces(board, player2->knights, NUM_PIECES, sizeof(Knight));

    addPieces(board, player1->bishops, NUM_PIECES, sizeof(Bishop));
    addPieces(board, player2->bishops, NUM_PIECES, sizeof(Bishop));

    addPieces(board, player1->queen, 1, sizeof(Queen));
    addPieces(board, player2->queen, 1, sizeof(Queen));

    addPieces(board, player1->king, 1, sizeof(King));
    addPieces(board, player2->king, 1, sizeof(King));

    for(int i = 0; i < BOARD_SIZE; i++)
    {   
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            if (board[i][j] == EMPTY_SQUARE)
                board[i][j] = ((i + j) % 2 == 0) ? WHITE_SQUARE : BLACK_SQUARE;
        }
    }
}

void freeBoard(char*** board, Player *player1, Player *player2)
{
    for (int i = 0; i < BOARD_SIZE; i++) free(board[0][i]);
    free(board[0]);

    freePlayer(player1);
    freePlayer(player2);

    board[0] = NULL;

    return;
}
