#include "minimax.hpp"


const std::unordered_map<char, int> Minimax::piece_values = {
    {'k', 1000}, {'q', 90}, {'r', 50}, {'b', 30}, {'n', 30}, {'p', 10}
};


Minimax::Minimax(int depth, PieceColor ai_color) : max_depth(depth), computer_color(ai_color) {}


/**
 * @brief Evaluates the board material relative to the AI's color.
 * @param board The 2D array representing the chess board.
 * @return Integer score where positive favors the computer.
 */
int Minimax::evaluation_function(char** board) 
{
    int score = 0;
    for (int r = 0; r < BOARD_SIZE; r++) 
    {
        for (int c = 0; c < BOARD_SIZE; c++) 
        {
            char piece = board[r][c];

            char lower_p = static_cast<char>(std::tolower(static_cast<unsigned char>(piece)));

            auto it = piece_values.find(lower_p);
            
            if (it != piece_values.end()) 
            {
                PieceColor p_color = (std::islower(static_cast<unsigned char>(piece))) ? COLOR_WHITE : COLOR_BLACK;
         
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
    PieceColor current_color = is_maximizing ? computer_color : (computer_color == COLOR_WHITE ? COLOR_BLACK : COLOR_WHITE);

    if (depth == max_depth || is_terminal(board, white, black, current_color)) 
        return (float)evaluation_function(board);

    Player* current_player = (current_color == COLOR_WHITE) ? white : black;
    std::vector<Move> moves = get_legal_moves(board, current_player);

    if (moves.empty()) return (float)evaluation_function(board);

    if (is_maximizing) 
    {
        float max_eval = -std::numeric_limits<float>::infinity();

        for (const auto& m : moves) 
        {
            char** next_board = copy_board(board);
        
            Player next_white = copy_player(white);
            Player next_black = copy_player(black);
        
            Player* m_ply = (current_color == COLOR_WHITE) ? &next_white : &next_black;
            Player* o_ply = (current_color == COLOR_WHITE) ? &next_black : &next_white;
            
            apply_move_to_state(next_board, m_ply, o_ply, m);
        
            float eval = minimax_core(next_board, &next_white, &next_black, false, alpha, beta, depth + 1);
        
            free_engine_state(next_board, &next_white, &next_black);
            
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
        
            Player next_white = copy_player(white);
            Player next_black = copy_player(black);
        
            Player* m_ply = (current_color == COLOR_WHITE) ? &next_white : &next_black;
            Player* o_ply = (current_color == COLOR_WHITE) ? &next_black : &next_white;

            apply_move_to_state(next_board, m_ply, o_ply, m);
        
            float eval = minimax_core(next_board, &next_white, &next_black, true, alpha, beta, depth + 1);
        
            free_engine_state(next_board, &next_white, &next_black);
            
            min_eval = std::min(min_eval, eval);
        
            beta = std::min(beta, eval);
        
            if (beta <= alpha) break;
        }

        return min_eval;
    }
}

/**
 * @brief Evaluates the board string and returns the Move struct for the best calculated move.
 * @param board_string The current game board as a string.
 * @return A Move structure representing the AI's choice.
 */
Move Minimax::get_best_move(std::string board_string) 
{
    char** board = board_parser(board_string);

    Player white = player_parser(board, COLOR_WHITE);
    Player black = player_parser(board, COLOR_BLACK);

    Move best_move_found;
    
    float max_val = -std::numeric_limits<float>::infinity();
    
    Player* ai_ply = (computer_color == COLOR_WHITE) ? &white : &black;
    
    std::vector<Move> moves = get_legal_moves(board, ai_ply);

    for (const auto& m : moves) 
    {
        char** next_board = copy_board(board);
    
        Player next_white = copy_player(&white);
        Player next_black = copy_player(&black);
    
        Player* m_ply = (computer_color == COLOR_WHITE) ? &next_white : &next_black;
        Player* o_ply = (computer_color == COLOR_WHITE) ? &next_black : &next_white;

        apply_move_to_state(next_board, m_ply, o_ply, m);
    
        float eval = minimax_core(next_board, &next_white, &next_black, false, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), 1);
    
        free_engine_state(next_board, &next_white, &next_black);

        if (eval > max_val) 
        {
            max_val = eval;
            best_move_found = m;
        }
    }

    free_engine_state(board, &white, &black);
    
    return best_move_found;
}
