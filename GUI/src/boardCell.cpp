#include "boardCell.hpp"
#include "piecesIcon.hpp"
#include "guiBoard.hpp"

#include <QDragMoveEvent> 
#include <iostream>

BoardCell::BoardCell(int r, int c, std::shared_ptr<Chess::Board> game_board, QWidget *parent) 
    : QWidget(parent), row_pos(r), col_pos(c), game_board(game_board)
{
    setAcceptDrops(true);
    setAttribute(Qt::WA_StyledBackground, true);

    this->setFixedSize(CELL_SIZE, CELL_SIZE);
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
    int player_turn = this->game_board->get_player_turn();

    if (piece)
    {
        Move move 
        {
            .symbol = 0,

            .colPrev = piece->getCol(),
            .rowPrev = piece->getRow(),

            .colNext = this->getCol(),
            .rowNext = this->getRow(),
 
            .promotedPawn = 0
        };

        MoveValidation move_state {};
        
        Player *ply = this->game_board->get_player(player_turn);
        
        Captured *ply_captures = this->game_board->get_player_captures(player_turn);

        char **board = this->game_board->get_board_array();
        int *plyEP = this->game_board->get_player_EP(player_turn), 
            *oppEP = this->game_board->get_player_EP((player_turn == PLAYER1) ? PLAYER2 : PLAYER1);

        switch (piece->symbol)
        {
            case PAWN: 
                move_state = movePawn(board, ply, move, ply_captures, plyEP, oppEP, false, false);
                move.symbol = (ply->color == COLOR_WHITE) ? 'p' : 'P';
                break;
            case ROOK: 
                move_state = moveRook(board, ply, move, ply_captures, false); 
                move.symbol = (ply->color == COLOR_WHITE) ? 'r' : 'R';
                break;
            case KNIGHT: 
                move_state = moveKnight(board, ply, move, ply_captures, false); 
                move.symbol = (ply->color == COLOR_WHITE) ? 'n' : 'N';
                break;
            case BISHOP: 
                move_state = moveBishop(board, ply, move, ply_captures, false); 
                move.symbol = (ply->color == COLOR_WHITE) ? 'b' : 'B';
                break;
            case QUEEN: 
                move_state = moveQueen(board, ply, move, ply_captures, false); 
                move.symbol = (ply->color == COLOR_WHITE) ? 'q' : 'Q';
                break;
            case KING: 
                move_state = moveKing(board, ply, move, ply_captures, false); 
                move.symbol = (ply->color == COLOR_WHITE) ? 'k' : 'K';
                break;
        }

        if (move_state != INVALID_MOVE) 
        {
            if (move_state == PROMOTION)
            {
                char chosen_piece = helpers::promotion_menu(this, ply->color);
                               
                promotePawn(move, ply, chosen_piece);

                move.promotedPawn = chosen_piece;
                board[move.rowNext][move.colNext] = chosen_piece;
            }

            piece->hide(); 
            event->acceptProposedAction();

            saveMove(move, (this->game_board->get_game_path()).c_str());
            clearRedo((this->game_board->get_redo_path()).c_str());

            if (ply_captures->newCapture) 
                capturePiece(game_board->get_player((player_turn == PLAYER1) ? PLAYER2 : PLAYER1), ply_captures);
            
            QTimer::singleShot(0, [this](){ this->game_board->update_board(); });
        }
    }
}
