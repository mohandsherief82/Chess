#pragma once
#include <QLabel>
#include <QMouseEvent>

extern "C"
{
    #include "chessTypes.h"
}

class DraggablePiece : public QLabel
{
    Q_OBJECT // Added for Qt Meta-Object support
    public:
        DraggablePiece(int row_pos, int col_pos, PieceColor color, PieceType symbol);
        int col_pos, row_pos;
        PieceColor color;
        PieceType symbol; 
    protected:
        void mousePressEvent(QMouseEvent *event) override;
};
