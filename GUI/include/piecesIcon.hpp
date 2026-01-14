#pragma once

#include <QLabel>
#include <QMouseEvent>

#include <memory>

#include "interfaces.hpp"

extern "C"
{
    #include "chessTypes.h"
    #include "player.h"
    #include "captures.h"
}

class DraggablePiece : public QLabel
{
    Q_OBJECT
    public:
        DraggablePiece(QWidget *parent, int row_pos, int col_pos, PieceColor color, PieceType symbol);
        
        int getRow() const { return row_pos; }
        int getCol() const { return col_pos; }
        
        PieceColor color;
        PieceType symbol;
    protected:
        void mousePressEvent(QMouseEvent *event) override;
    private:
        int col_pos, row_pos;
};
