#include "boardCell.hpp"
#include "piecesIcon.hpp"

#include <QDragMoveEvent> 
#include <iostream>

BoardCell::BoardCell(int r, int c, char ***board_ptr, QWidget *parent) 
    : QWidget(parent), row_pos(r), col_pos(c), board_ptr(board_ptr)
{
    setAcceptDrops(true);
    setAttribute(Qt::WA_StyledBackground, true);

    this->setFixedSize(70, 70);
}


void BoardCell::dragEnterEvent(QDragEnterEvent *event) 
{
    event->acceptProposedAction();
}


void BoardCell::dragMoveEvent(QDragMoveEvent *event) 
{
    event->acceptProposedAction();
}


void BoardCell::dropEvent(QDropEvent *event)
{
    DraggablePiece *piece = qobject_cast<DraggablePiece*>(event->source());
    Chess::Board *game_board = piece->get_game_board();
    int player_turn = game_board->get_player_turn();
    
    if (piece)
    {
        Move move {
            .colPrev = piece->getCol(),
            .rowPrev = piece->getRow(),

            .colNext = this->getCol(),
            .rowNext = this->getRow()
        };


        std::cout << "MOVE: [" << move.rowPrev << "," << move.colPrev << "] -> [" 
                  << move.rowNext << "," << move.colNext << "]" << std::endl;

        MoveValidation move_state {};
        
        Player *ply = game_board->get_player(player_turn);
        
        Captured *ply_captures = game_board->get_player_captures(player_turn);

        char **board = this->getBoard();
        int *whiteEP, *blackEP;

        switch (piece->symbol)
        {
            case PAWN: 
                move_state = movePawn(board, ply, move, ply_captures, whiteEP, blackEP, false, false);
                break;
            case ROOK: 
                move_state = moveRook(board, ply, move, ply_captures, false); 
                break;
            case KNIGHT: 
                move_state = moveKnight(board, ply, move, ply_captures, false); 
                break;
            case BISHOP: 
                move_state = moveBishop(board, ply, move, ply_captures, false); 
                break;
            case QUEEN: 
                move_state = moveQueen(board, ply, move, ply_captures, false); 
                break;
            case KING: 
                move_state = moveKing(board, ply, move, ply_captures, false); 
                break;
        }

        if (move_state == VALID_MOVE) 
        {
            piece->hide(); 
            event->acceptProposedAction();

            // Add functionality to update the board
        }

        // Handling other valid move cases

        else std::cout << "Invalid Move" << move_state << std::endl; // pop up saying invalid move
    }
}
