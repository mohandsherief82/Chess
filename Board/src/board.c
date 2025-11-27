#include <stdio.h>
#include <stdlib.h>

#include "../../Pieces/include/player.h"
#include "../include/board.h"

char** initialize_board()
{
    char **board = (char**)malloc(BOARD_SIZE * sizeof(char*));

    // Initialize board with empty cells,
    // will be overwritten with the first display of the board
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        board[i] = (char*)malloc(BOARD_SIZE * sizeof(char));
        for (int j = 0; j < BOARD_SIZE; j++) board[i][j]= EMPTY_SQUARE;
    }

    return board;
}


void addPieces(char** board, void* piecesArray, int numPieces, size_t piece_size)
{
    // Pointer to the current piece (treated as raw bytes)
    char* current_piece = (char*)piecesArray;

    for (int i = 0; i < numPieces; i++)
    {
        // Get the current piece's address by moving 'i' steps of size 'piece_size'
        // This is done using pointer arithmetic on the char* pointer.
        Piece* p = (Piece*)(current_piece + i * piece_size);

        int col = p->colPosition;
        int row = p->rowPosition;

        if ((col >= 0 && col < 8) && (row >= 0 && row < 8)) board[row][col] = p->symbol;
    }
}


void display_board(char** board, Player player1, Player player2)
{ 
    printf("\t\t\t    A   B   C   D   E   F   G   H \n");
    printf("\t\t\t  |---|---|---|---|---|---|---|---|\n");

    addPieces(board, player1.pawns, NUM_PAWNS, sizeof(Pawn));
    addPieces(board, player2.pawns, NUM_PAWNS, sizeof(Pawn));

    addPieces(board, player1.rocks, NUM_PIECES, sizeof(Rock));
    addPieces(board, player2.rocks, NUM_PIECES, sizeof(Rock));

    addPieces(board, player1.knights, NUM_PIECES, sizeof(Knight));
    addPieces(board, player2.knights, NUM_PIECES, sizeof(Knight));

    addPieces(board, player1.bishops, NUM_PIECES, sizeof(Bishop));
    addPieces(board, player2.bishops, NUM_PIECES, sizeof(Bishop));

    addPieces(board, &player1.queen, 1, sizeof(Queen));
    addPieces(board, &player2.queen, 1, sizeof(Queen));

    addPieces(board, &player1.king, 1, sizeof(King));
    addPieces(board, &player2.king, 1, sizeof(King));


    for(int i = 0; i < BOARD_SIZE; i++)
    {
        printf("\t\t\t%d", BOARD_SIZE -i);

        for (int j = 0; j < BOARD_SIZE; j++) 
            {
                if (board[i][j] == EMPTY_SQUARE)
                    board[i][j] = ((i + j) % 2 == 0) ? WHITE_SQUARE : BLACK_SQUARE;
                printf(" | %c", board[i][j]);
            }
        
        printf(" |%d\n", BOARD_SIZE -i);
        printf("\t\t\t  |---|---|---|---|---|---|---|---|\n");
    }

    printf("\t\t\t    A   B   C   D   E   F   G   H \n");
}


void clearScreen()
{
    printf("\033[2J\033[H");

    return;
}


void freeBoard(char** board, Player player1, Player player2)
{
    free(board);
    freePlayer(player1);
    freePlayer(player2);

    return;
}

