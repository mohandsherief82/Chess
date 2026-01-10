#include "piecesIcon.hpp"

#include <QDrag>
#include <QMimeData>

DraggablePiece::DraggablePiece(QWidget *parent) : QLabel(parent) 
{}


DraggablePiece::DraggablePiece(int row_pos, int col_pos)
{
    QLabel(parent);

    this->row_pos = row_pos;
    this->col_pos = col_pos;
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
