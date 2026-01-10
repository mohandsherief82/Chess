#pragma once
#include <QLabel>
#include <QMouseEvent>

extern "C"
{
    #include "chessTypes.h"
}

class DraggablePiece : public QLabel
{
    Q_OBJECT
    public:
        DraggablePiece(QWidget *parent, int row_pos, int col_pos, PieceColor color, PieceType symbol);
        int col_pos, row_pos;
        PieceColor color;
        PieceType symbol; 
    protected:
        void mousePressEvent(QMouseEvent *event) override;
    private:
        void movePiece();
};
