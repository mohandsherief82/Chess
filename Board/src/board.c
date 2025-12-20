#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "../../Pieces/include/player.h"
#include "../include/board.h"
#include "../../Game-End/include/saveGame.h"
#include "../../Moves/include/captures.h"

#define MAX_MOVES_TO_DISPLAY 16

char** initializeBoard()
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

    for (int i = 0; i < numPieces; i++)
    {
        Piece* p = (Piece*)(current_piece + i * piece_size);

        int col = p->colPosition;
        int row = p->rowPosition;

        if ((col >= 0 && col < 8) && (row >= 0 && row < 8) && p->isActive) board[row][col] = p->symbol;
    }
}


void updateBoard(char** board, Player player1, Player player2, Captured ply1Captures, Captured ply2Captures, bool PRINT)
{
    clearScreen();
    if (PRINT)
    {
        printf("----------------|-------------------------------|-----------------------------------------------|----------------------"
                "--------------------------------------------|-----------\n");
        printf("\t\t|             Moves             |\t\t\t Board \t\t\t|\t\t\t Captures \t\t\t\t   |\n");
        printf("\t\t|---------------|---------------|-----------------------------------------------|--------------------"
                    "----------------------------------------------|\n");
        printf("\t\t|     White     |     Black     |\t    A   B   C   D   E   F   G   H  \t|\t\t\t Black Captures \t\t\t   |\n");
        printf("\t\t|---------------|---------------|\t  |---|---|---|---|---|---|---|---|\t|"
                "------------------------------------------------------------------|\n");
    }

    addPieces(board, player1.pawns, NUM_PAWNS, sizeof(Pawn));
    addPieces(board, player2.pawns, NUM_PAWNS, sizeof(Pawn));

    addPieces(board, player1.rocks, NUM_PIECES, sizeof(Rock));
    addPieces(board, player2.rocks, NUM_PIECES, sizeof(Rock));

    addPieces(board, player1.knights, NUM_PIECES, sizeof(Knight));
    addPieces(board, player2.knights, NUM_PIECES, sizeof(Knight));

    addPieces(board, player1.bishops, NUM_PIECES, sizeof(Bishop));
    addPieces(board, player2.bishops, NUM_PIECES, sizeof(Bishop));

    addPieces(board, player1.queen, 1, sizeof(Queen));
    addPieces(board, player2.queen, 1, sizeof(Queen));

    addPieces(board, player1.king, 1, sizeof(King));
    addPieces(board, player2.king, 1, sizeof(King));

    FILE *fptr = fopen("./Game-End/testing/game.bin", "rb");
    
    if (fptr == NULL)
    {
        FILE* ftemp = fopen("./Game-End/testing/game.bin", "wb");
        if (ftemp != NULL) fclose(ftemp);
        fptr = fopen("./Game-End/testing/game.bin", "rb");
    }

    long fileSize = getFileSize(fptr);
    int totalMoves = (int)(fileSize / sizeof(Move));
    
    if (totalMoves > 16) 
    {
        int startMove = totalMoves - 16;
        if (startMove % 2 != 0) startMove++;
        fseek(fptr, (long)(startMove * sizeof(Move)), SEEK_SET);
    }

    for(int i = 0; i < BOARD_SIZE; i++)
    {
        Move move;
        if (PRINT) printf("\t\t");
        
        for (int j = 0; j < 2; j++)
        {            
            if (fread(&move, sizeof(Move), 1, fptr) && PRINT)
                printf("|  %c: %c%d -> %c%d  ", ((j == 0) ? move.symbol : toupper(move.symbol)), move.colPrev + 'A', 8 - move.rowPrev, move.colNext + 'A', 8 - move.rowNext);
            else if (PRINT) printf("|               ");
        }
        
        if (PRINT) printf("|\t%d", BOARD_SIZE - i);

        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            if (board[i][j] == EMPTY_SQUARE)
                board[i][j] = ((i + j) % 2 == 0) ? WHITE_SQUARE : BLACK_SQUARE;
            if (PRINT) printf(" | %c", board[i][j]);
        }
        
        if (PRINT) printf(" |%d\t|", BOARD_SIZE - i);
        if (i == 4 && PRINT) printf("\t\t\t White Captures \t\t\t   |");
        else if (i == 1 || i == 2)
        {
            int startIndex = (i % 5 == 1) ? 0 : 8
                , endIndex = startIndex + 8;
            
            if (PRINT) printf("                 ");

            for (int k = startIndex; k < endIndex; k++)
            {
                if (PRINT) printf("| %c ", ply2Captures.capturedSymbols[k]);
                if (ply2Captures.capturedSymbols[k] == 'X' && PRINT) printf("|   ");
            }

            if (i == 1 && PRINT) printf("|                |");
            else if (PRINT) printf("             |");
        }
        else if (i == 6 || i == 7)
        {
            int startIndex = (i % 5 == 1) ? 0 : 8
                , endIndex = startIndex + 8;            
            if (PRINT) printf("                 ");

            for (int k = startIndex; k < endIndex; k++)
            {
                if (PRINT) printf("| %c ", ply1Captures.capturedSymbols[k]);
                if (ply1Captures.capturedSymbols[k] == 'X' && PRINT) printf("|   ");
            }

            if (i == 6 && PRINT) printf("|                |");
            else if (PRINT) printf("             |");
        }
        else if (PRINT) printf("                                                                  |");
        if (PRINT) printf("\n");

        if (PRINT) printf("\t\t|---------------|---------------|\t  |---|---|---|---|---|---|---|---|\t|");
        if ((i == 3 || i == 4) && PRINT) printf("------------------------------------------------------------------|");
        else if (i % 5 >= 0 && i % 5 <= 2 && PRINT) printf("                 |---|---|---|---|---|---|---|---|                |");
        else if (PRINT) printf("                                                                  |");
        if (PRINT) printf("\n");
    }

    if (PRINT)
    {
        printf("\t\t|\t        |\t        |\t    A   B   C   D   E   F   G   H  \t|\t\t\t\t\t\t\t\t   |\n");
        printf("----------------|---------------|---------------|-----------------------------------------------|----------------------"
                "--------------------------------------------|-----------\n");
    }

    fclose(fptr);
}


bool isEmpty(char** board, int r, int c)
{
    return board[r][c] == WHITE_SQUARE || board[r][c] == BLACK_SQUARE || board[r][c] == EMPTY_SQUARE;
}


void clearScreen()
{
    // system("clear");
    printf("\033[2J\033[H");
    fflush(stdout);

    return;
}


void freeBoard(char** board, Player player1, Player player2)
{
    for (int i = 0; i < BOARD_SIZE; i++) free(board[i]);
    free(board);
    freePlayer(player1);
    freePlayer(player2);

    return;
}
