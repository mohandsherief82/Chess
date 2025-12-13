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


void displayBoard(char** board, Player player1, Player player2, Captured ply1Captures, Captured ply2Captures)
{
    clearScreen();
    printf("----------------|-------------------------------|-----------------------------------------------|----------------------"
            "--------------------------------------------|-----------\n");
    printf("\t\t|             Moves             |\t\t\t Board \t\t\t|\t\t\t Captures \t\t\t\t   |\n");
    printf("\t\t|---------------|---------------|-----------------------------------------------|--------------------"
                "----------------------------------------------|\n");
    printf("\t\t|     White     |     Black     |\t    A   B   C   D   E   F   G   H  \t|\t\t\t Black Captures \t\t\t   |\n");
    printf("\t\t|---------------|---------------|\t  |---|---|---|---|---|---|---|---|\t|"
            "------------------------------------------------------------------|\n");

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
    Move movesToDisplay[MAX_MOVES_TO_DISPLAY];
    int actualMovesRead = 0;
    long file_size = 0;
    size_t move_size = sizeof(Move);

    if (fptr != NULL)
    {
        long total_moves_in_file = 0;
        
        file_size = getFileSize(fptr);
        total_moves_in_file = file_size / move_size;

        long moves_to_skip = 0;
        
        if (total_moves_in_file > MAX_MOVES_TO_DISPLAY) 
        {
            moves_to_skip = total_moves_in_file - MAX_MOVES_TO_DISPLAY;
            actualMovesRead = MAX_MOVES_TO_DISPLAY;
        } 
        else 
        {
            moves_to_skip = 0;
            actualMovesRead = (int)total_moves_in_file;
        }

        fseek(fptr, moves_to_skip * move_size, SEEK_SET);

        if (actualMovesRead > 0) fread(movesToDisplay, move_size, actualMovesRead, fptr);
        
        fclose(fptr);
        fptr = NULL;
    }
    
    if (fptr == NULL)
    {
        FILE* ftemp = fopen("./Game-End/testing/game.bin", "wb");
        if (ftemp != NULL) fclose(ftemp);
    }

    for(int i = 0; i < BOARD_SIZE; i++)
    {
        printf("\t\t");
        
        for (int j = 0; j < 2; j++)
        {
            int move_index = actualMovesRead - 16 + (i * 2) + j;
            
            if (move_index >= 0 && move_index < actualMovesRead)
            {
                Move move = movesToDisplay[move_index];
                printf("|  %c: %c%d -> %c%d  ", 
                       ((j == 0) ? move.symbol : toupper(move.symbol)), 
                       move.colPrev + 'A', 8 - move.rowPrev, 
                       move.colNext + 'A', 8 - move.rowNext);
            } 
            else printf("|               ");
        }
        
        printf("|\t%d", BOARD_SIZE - i);

        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            if (board[i][j] == EMPTY_SQUARE)
                board[i][j] = ((i + j) % 2 == 0) ? WHITE_SQUARE : BLACK_SQUARE;
            printf(" | %c", board[i][j]);
        }
        
        printf(" |%d\t|", BOARD_SIZE - i);
        if (i == 4) printf("\t\t\t White Captures \t\t\t   |");
        else if (i == 1 || i == 2)
        {
            int startIndex = (i % 5 == 1) ? 0 : 8
                , endIndex = startIndex + 8;
            
            printf("                 ");

            for (int k = startIndex; k < endIndex; k++)
            {
                printf("| %c ", ply2Captures.capturedSymbols[k]);
                if (ply2Captures.capturedSymbols[k] == 'X') printf("|   ");
            }

            if (i == 1) printf("|                |");
            else printf("             |");
        }
        else if (i == 6 || i == 7)
        {
            int startIndex = (i % 5 == 1) ? 0 : 8
                , endIndex = startIndex + 8;            
            printf("                 ");

            for (int k = startIndex; k < endIndex; k++)
            {
                printf("| %c ", ply1Captures.capturedSymbols[k]);
                if (ply1Captures.capturedSymbols[k] == 'X') printf("|   ");
            }

            if (i == 6) printf("|                |");
            else printf("             |");
        }
        else printf("                                                                  |");
        printf("\n");

        printf("\t\t|---------------|---------------|\t  |---|---|---|---|---|---|---|---|\t|");
        if (i == 3 || i == 4) printf("------------------------------------------------------------------|");
        else if (i % 5 >= 0 && i % 5 <= 2) printf("                 |---|---|---|---|---|---|---|---|                |");
        else printf("                                                                  |");
        printf("\n");
    }

    printf("\t\t|\t        |\t        |\t    A   B   C   D   E   F   G   H  \t|\t\t\t\t\t\t\t\t   |\n");
    printf("----------------|---------------|---------------|-----------------------------------------------|----------------------"
            "--------------------------------------------|-----------\n");
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
    free(board);
    freePlayer(player1);
    freePlayer(player2);

    return;
}
