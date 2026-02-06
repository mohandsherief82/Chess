#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include "parsers.hpp"

extern "C" {
    #include "pawnMoves.h"
    #include "rookMoves.h"

    #include "knightMoves.h"
    #include "bishopMoves.h"

    #include "queenMoves.h"
    #include "kingMoves.h"
    
    #include "chessTypes.h"
    #include "board.h"

    #include "checkMate.h"
    #include "staleMate.h"
    
    #include "check.h"
}

class Minimax 
{
    private:
        int max_depth;
        PieceColor computer_color;
        static const std::unordered_map<char, int> piece_values;

        float minimax_core(char** board, Player* ply_white, Player* ply_black, bool is_maximizing, float alpha, float beta, int depth);
        int evaluation_function(char** board);
        
        bool is_terminal(char** board, Player* white, Player* black, PieceColor current_color);
        std::vector<Move> get_legal_moves(char** board, Player* current_player);
        
        void apply_move_to_state(char** board, Player* moving_player, Player* opponent, Move move);
    public:
        Minimax(int depth, PieceColor ai_color);
        Move get_best_move(std::string board_string);
};
