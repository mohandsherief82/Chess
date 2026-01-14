#pragma once

#include <QLabel>
#include <QMouseEvent>

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
        DraggablePiece(QWidget *parent, Chess::Board *&game_board, int row_pos, int col_pos, PieceColor color, PieceType symbol);
        
        int getRow() const { return row_pos; }
        int getCol() const { return col_pos; }
        
        Chess::Board *get_game_board() { return game_board; }
        
        PieceColor color;
        PieceType symbol;
    protected:
        void mousePressEvent(QMouseEvent *event) override;
    private:
        int col_pos, row_pos;
        Chess::Board *game_board = nullptr;
};
