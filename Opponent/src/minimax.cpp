#include "minimax.hpp"

#include <iostream>
#include <cctype>

const std::unordered_map<char, int> Minimax::piece_values = {
    {'k', 1000}, {'q', 90}, {'r', 50}, {'b', 30}, {'n', 30}, {'p', 10}
};


Minimax::Minimax(int depth, PieceColor ai_color) : max_depth(depth), computer_color(ai_color) {}


/**
 * @brief Iterates through all pieces of a player and simulates every possible move to find legal ones.
 * @param board The current board state.
 * @param player Pointer to the player whose legal moves are being calculated.
 * @return A vector of valid Move structures.
 */
std::vector<Move> Minimax::get_legal_moves(char** board, Player* player)
{
    std::vector<Move> move_list;
    
    if (!board || !player) return move_list;

    PieceColor color = player->color;

    auto collect_for_piece = [&](Piece* p, char type) 
    {
        if (!p || !p->isActive) return;

        Move test_move;

        test_move.symbol = p->symbol;
        test_move.rowPrev = p->rowPosition;

        test_move.colPrev = p->colPosition;
        test_move.promotedPawn = ' ';

        for (int r = 0; r < BOARD_SIZE; r++)
        {
            for (int c = 0; c < BOARD_SIZE; c++)
            {
                if (r == p->rowPosition && c == p->colPosition) continue;
                
                if (!isEmpty(board, r, c))
                {
                    PieceColor target_color = (std::islower((unsigned char)board[r][c])) ? COLOR_WHITE : COLOR_BLACK;

                    if (target_color == color) continue;
                }

                test_move.rowNext = r;
                test_move.colNext = c;

                char** cpy_b = copy_board(board);
                Player cpy_p = copy_player(player); 
                
                Captured tmp_cap = initializeCapture(color);

                int p_ep = -1, o_ep = -1;

                MoveValidation valid = INVALID_MOVE;

                if (type == 'p') valid = movePawn(cpy_b, &cpy_p, test_move, &tmp_cap, &p_ep, &o_ep, true, false);
                else if (type == 'r') valid = moveRook(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'n') valid = moveKnight(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'b') valid = moveBishop(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'q') valid = moveQueen(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'k') valid = moveKing(cpy_b, &cpy_p, test_move, &tmp_cap, true);

                if (valid != INVALID_MOVE && !isChecked(cpy_b, &cpy_p, true))
                    move_list.push_back(test_move);

                free_engine_state(&cpy_b, &cpy_p, nullptr);
            }
        }
    };

    for (int i = 0; i < 8; i++) collect_for_piece((Piece*)&player->pawns[i], 'p');

    for (int i = 0; i < 2; i++) 
    {
        collect_for_piece((Piece*)&player->rooks[i], 'r');
        collect_for_piece((Piece*)&player->knights[i], 'n');
        collect_for_piece((Piece*)&player->bishops[i], 'b');
    }

    collect_for_piece((Piece*)player->queen, 'q');

    collect_for_piece((Piece*)player->king, 'k');

    return move_list;
}

/**
 * @brief Evaluates the board material relative to the AI's color.
 * @param board The 2D array representing the chess board.
 * @return Integer score where positive favors the computer.
 */
int Minimax::evaluation_function(char** board) 
{
    if (!board) return 0;

    int score = 0;

    for (int r = 0; r < BOARD_SIZE; r++) 
    {
        for (int c = 0; c < BOARD_SIZE; c++) 
        {
            char piece = board[r][c];

            if (piece == ' ') continue;

            char lower_p = (char)std::tolower((unsigned char)piece);

            auto it = piece_values.find(lower_p);
            
            if (it != piece_values.end()) 
            {
                PieceColor p_color = (std::islower((unsigned char)piece)) ? COLOR_WHITE : COLOR_BLACK;

                score += (p_color == computer_color) ? it->second : -it->second;
            }
        }
    }

    return score;
}


/**
 * @brief Checks if the current game state is a terminal node (Checkmate or Stalemate).
 * @param board The 2D array representing the board.
 * @param white Pointer to the white player.
 * @param black Pointer to the black player.
 * @param current_color The color of the player whose turn it is.
 * @return True if the game is over.
 */
bool Minimax::is_terminal(char** board, Player* white, Player* black, PieceColor current_color) 
{
    if (!board || !white || !black) return true;

    Player* current = (current_color == COLOR_WHITE) ? white : black;

    Player* opponent = (current_color == COLOR_WHITE) ? black : white;

    return checkMate(board, current) || checkStalemate(board, current, opponent);
}


/**
 * @brief Recursive engine for finding the best score using Alpha-Beta pruning.
 * @param board Current board state.
 * @param white Pointer to White player state.
 * @param black Pointer to Black player state.
 * @param is_maximizing Boolean indicating if the current layer is a Maximizer.
 * @param alpha Current best score for the Maximizer.
 * @param beta Current best score for the Minimizer.
 * @param depth Current recursion depth.
 * @return The best score found for this branch.
 */
float Minimax::minimax_core(char** board, Player* white, Player* black, bool is_maximizing, float alpha, float beta, int depth) 
{
    if (!board || !white || !black) return 0.0f;

    PieceColor current_color = is_maximizing ? computer_color : (computer_color == COLOR_WHITE ? COLOR_BLACK : COLOR_WHITE);

    if (depth > this->max_depth || is_terminal(board, white, black, current_color)) 
        return (float)this->evaluation_function(board);

    std::vector<Move> moves = get_legal_moves(board, (current_color == COLOR_WHITE) ? white : black);

    if (moves.empty()) return (float)this->evaluation_function(board);

    if (is_maximizing) 
    {
        float max_eval = -std::numeric_limits<float>::infinity();

        for (const Move &m : moves) 
        {
            char** next_board = copy_board(board);
            if (!next_board) continue;

            Player next_white = copy_player(white);
            Player next_black = copy_player(black);

            std::cout << "Depth: " << depth << std::endl;
            
            apply_move(next_board, &next_white, &next_black, m, current_color);

            float eval = minimax_core(next_board, &next_white, &next_black, false, alpha, beta, depth + 1);
            
            free_engine_state(&next_board, &next_white, &next_black);

            max_eval = std::max(max_eval, eval);

            alpha = std::max(alpha, eval);

            if (beta <= alpha) break;
        }

        return max_eval;
    } 
    else 
    {
        float min_eval = std::numeric_limits<float>::infinity();
        for (const auto& m : moves) 
        {
            char** next_board = copy_board(board);
            if (!next_board) continue;

            Player next_white = copy_player(white);
            Player next_black = copy_player(black);

            apply_move(next_board, &next_white, &next_black, m, current_color);

            float eval = minimax_core(next_board, &next_white, &next_black, true, alpha, beta, depth + 1);
            
            free_engine_state(&next_board, &next_white, &next_black);

            min_eval = std::min(min_eval, eval);

            beta = std::min(beta, eval);

            if (beta <= alpha) break;
        }

        return min_eval;
    }
}


/**
 * @brief Updates the board and player structures to reflect the result of a move.
 * @param board The 2D array representing the chess board to be modified.
 * @param white Pointer to the white player structure.
 * @param black Pointer to the black player structure.
 * @param move The move structure containing the source and destination coordinates.
 * @param moving_color The color of the player making the move.
 */
void Minimax::apply_move(char** board, Player* white, Player* black, Move move, PieceColor moving_color)
{
    if (!board || !white || !black) return;

    Player* moving_player = (moving_color == COLOR_WHITE) ? white : black;
    Player* opponent = (moving_color == COLOR_WHITE) ? black : white;

    Captured ply_captures = initializeCapture(moving_player->color);

    int ply_ep = -1, opp_ep = -1;

    char sym = (char)std::tolower((unsigned char)move.symbol);

    if (sym == 'p') movePawn(board, moving_player, move, &ply_captures, &ply_ep, &opp_ep, false, false);
    else if (sym == 'r') moveRook(board, moving_player, move, &ply_captures, false);
    else if (sym == 'n') moveKnight(board, moving_player, move, &ply_captures, false);
    else if (sym == 'b') moveBishop(board, moving_player, move, &ply_captures, false);
    else if (sym == 'q') moveQueen(board, moving_player, move, &ply_captures, false);
    else if (sym == 'k') moveKing(board, moving_player, move, &ply_captures, false);

    updateBoard(board, moving_player, opponent);
}


/**
 * @brief Evaluates the board string and returns the Move struct for the best calculated move.
 * @param board The current game board as a 2D array.
 * @param white The white player state.
 * @param black The black player state.
 * @return A Move structure representing the AI's choice.
 */
Move Minimax::get_best_move(char** board, Player white, Player black)
{
    Move best_move_found;

    best_move_found.symbol = ' ';
    best_move_found.rowPrev = -1;
    
    best_move_found.colPrev = -1;
    best_move_found.rowNext = -1;
    
    best_move_found.colNext = -1;
    best_move_found.promotedPawn = ' ';

    if (!board) return best_move_found;

    float max_val = -std::numeric_limits<float>::infinity();

    Player* ai_ply = (computer_color == COLOR_WHITE) ? &white : &black;

    std::vector<Move> moves = get_legal_moves(board, ai_ply);

    for (const auto& m : moves) 
    {
        char** next_board = copy_board(board);
        
        if (!next_board) continue;

        Player next_white = copy_player(&white);
        Player next_black = copy_player(&black);

        apply_move(next_board, &next_white, &next_black, m, computer_color);

        std::cout << "Run" << std::endl; 

        float eval = minimax_core(next_board, &next_white, &next_black, false, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), 1);
        
        if (eval > max_val) 
        {
            max_val = eval;

            best_move_found = m;
        }

        free_engine_state(&next_board, &next_white, &next_black);
    }

    return best_move_found;
}
