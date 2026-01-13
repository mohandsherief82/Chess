#include "piecesIcon.hpp"

#include <QDrag>
#include <QMimeData>


DraggablePiece::DraggablePiece(QWidget *parent, int row_pos, int col_pos,
     PieceColor color, PieceType symbol) : QLabel (parent)
{
    this->color = color;
    this->symbol = symbol;

    this->row_pos = row_pos;
    this->col_pos = col_pos;
}

DraggablePiece::DraggablePiece(QWidget *parent, Player *ply, Captured *ply_captures, int row_pos, int col_pos
        , PieceColor color, PieceType symbol) : QLabel (parent)
{
    this->color = color;
    this->symbol = symbol;

    this->row_pos = row_pos;
    this->col_pos = col_pos;

    this->ply = ply;
    this->ply_captures = ply_captures;
}


void DraggablePiece::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !pixmap().isNull())
    {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(this->objectName()); 

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap());
        drag->setHotSpot(event->pos()); 

        this->hide();

        if (drag->exec(Qt::MoveAction) != Qt::MoveAction) this->show();
    }
}
