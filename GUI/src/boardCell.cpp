#include "boardCell.hpp"
#include "piecesIcon.hpp"

#include <QDragMoveEvent> 
#include <iostream>

BoardCell::BoardCell(int r, int c, char ***board_ptr, QWidget *parent) 
    : QWidget(parent), row_pos(r), col_pos(c), board_ptr(board_ptr)
{
    setAcceptDrops(true);
    setAttribute(Qt::WA_StyledBackground, true);
}

void BoardCell::dragEnterEvent(QDragEnterEvent *event) 
{
    // Debug: This should print as soon as you drag over a square
    std::cout << "Dragging over square: " << row_pos << "," << col_pos << std::endl;
    event->acceptProposedAction();
}

void BoardCell::dragMoveEvent(QDragMoveEvent *event) 
{
    event->acceptProposedAction();
}

void BoardCell::dropEvent(QDropEvent *event)
{
    std::cout << "Drop event triggered!" << std::endl;

    DraggablePiece *piece = qobject_cast<DraggablePiece*>(event->source());
    
    if (piece)
    {        
        std::cout << "MOVE: [" << piece->getRow() << "," << piece->getCol() << "] -> [" 
                  << this->row_pos << "," << this->col_pos << "]" << std::endl;

        Move move {
            .colPrev = piece->getCol(),
            .rowPrev = piece->getRow(),
            .colNext = this->getCol(),
            .rowNext = this->getRow()
        };
        
        Player *ply = piece->getPlayer();
        
        Captured *ply_captures = piece->getCaptures();

        char **board = this->getBoard();
        int *whiteEP, *blackEP;

        switch (piece->symbol)
        {
            case PAWN: 
                movePawn(board, ply, &move, ply_captures, whiteEP, blackEP, false, false); 
                break;
            case ROOK: 
                ; 
                break;
            case KNIGHT: 
                ; 
                break;
            case BISHOP: 
                ; 
                break;
            case QUEEN: 
                ; 
                break;
            case KING: 
                ; 
                break;
        }

        piece->hide(); 
        event->acceptProposedAction();
    }
    else std::cout << "Drop error: Source is not a DraggablePiece" << std::endl;
}
