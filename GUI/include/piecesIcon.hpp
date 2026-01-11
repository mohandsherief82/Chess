#pragma once
#include <QLabel>
#include <QMouseEvent>

extern "C"
{
    #include "chessTypes.h"
    #include "player.h"
}

class DraggablePiece : public QLabel
{
    Q_OBJECT
    public:
        DraggablePiece(QWidget *parent, int row_pos, int col_pos, PieceColor color, PieceType symbol);
        DraggablePiece(QWidget *parent, Player *ply, int row_pos, int col_pos, PieceColor color, PieceType symbol);
        int getRow() { return row_pos; }
        int getCol() { return col_pos; }
        Player *getPlayer() { return ply; }
        PieceColor color;
        PieceType symbol;
         
    protected:
        void mousePressEvent(QMouseEvent *event) override;
    private:
        int col_pos, row_pos;
        Player *ply;
};
