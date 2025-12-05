#include <stdio.h>
#include <ctype.h>

#include "./include/pawnMoves.h"
#include "./include/rockMoves.h"
#include "./include/knightMoves.h"
#include "./include/bishopMoves.h"
#include "./include/queenMoves.h"
#include "./include/kingMoves.h"
#include "./include/captures.h"
#include "./include/saveGame.h"
#include "../Board/include/board.h"


bool playerTurn(char** board, Player* player, Captured* capture)
{
    Move move;
    while (true)
    {
        move = getMove();

        if (move.symbol == 's') return true;

        bool pieceMoveValid = false;
        
        if (move.symbol == 'p') pieceMoveValid = movePawn(board, player, move, capture);
        else if (move.symbol == 'r') pieceMoveValid = moveRock(board, player, move, capture);
        else if (move.symbol == 'n') pieceMoveValid = moveKnight(board, player, move, capture);
        else if (move.symbol == 'b') pieceMoveValid = moveBishop(board, player, move, capture);
        else if (move.symbol == 'q') pieceMoveValid = moveQueen(board, player, move, capture);
        else if (move.symbol == 'k') pieceMoveValid = moveKing(board, player, move, capture);
        
        if (!pieceMoveValid)
        {
            printf("Invalid piece movement rules or target, Try Again!!!\n");
            continue;
        }
        
        if (isChecked(board, player))
        {
            printf("Illegal move: King remains in check or moved into check. Reverting move.\n");
            
            // undoLastMove(board, player, move, capture); 
            continue; 
        }

        break; 
    }
    
    saveMove(move);

    return false;
}

// Prototype for game logic
int main ()
{
    Player ply1 = createPlayer(COLOR_WHITE)
            , ply2 = createPlayer(COLOR_BLACK);
    Captured whiteCaptures = initializeCapture(COLOR_WHITE)
            , blackCaptures = initializeCapture(COLOR_BLACK);
    char** board = initializeBoard();
    bool saveGame = false;

    

    displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

    while (true)
    {
        printf("Player 1's turn: ");

        saveGame = playerTurn(board, &ply1, &whiteCaptures);
        if (whiteCaptures.newCapture == true) capturePiece(ply2, &whiteCaptures);
        if (saveGame)
        {
            clearScreen();
            printf("Done, Game Saved!!!\n");
            break;
        }

        clearScreen();
        displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

        printf("Player 2's turn: ");
        
        saveGame = playerTurn(board, &ply2, &blackCaptures);
        if (blackCaptures.newCapture == true) capturePiece(ply1, &blackCaptures);

        clearScreen();
        displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

        if (saveGame)
        {
            clearScreen();
            printf("Done, Game Saved!!!\n");
            break;
        }
    }

    freeBoard(board, ply1, ply2);

    return 0;
}
