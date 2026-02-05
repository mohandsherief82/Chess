#include "move_wrappers.hpp"

/**
 * @brief Logic for performing a move on a board string and returning the resulting string.
 */
std::string move_piece(std::string board_string, std::string move_string)
{
    if (move_string.size() < 5) return board_string;

    char **board { board_parser(board_string) };

    Move move { move_parser(move_string) };
    PieceColor current_color = (std::islower(move.symbol)) ? COLOR_WHITE : COLOR_BLACK;

    Player ply1 { player_parser(board, COLOR_WHITE) };
    Player ply2 { player_parser(board, COLOR_BLACK) };

    Player *moving_player = (current_color == COLOR_WHITE) ? &ply1 : &ply2;
    Player *opponent = (current_color == COLOR_WHITE) ? &ply2 : &ply1;

    Captured current_captures { captures_parser(board, opponent, current_color) };
    
    int ply_ep { -1 }, opp_ep { -1 };
    MoveValidation valid = INVALID_MOVE;

    switch (std::tolower(move.symbol))
    {
        case 'p': 
            valid = movePawn(board, moving_player, move, &current_captures, &ply_ep, &opp_ep, false, false);
            break;
        case 'r': 
            valid = moveRook(board, moving_player, move, &current_captures, false);
            break;
        case 'n': 
            valid = moveKnight(board, moving_player, move, &current_captures, false);
            break;
        case 'b': 
            valid = moveBishop(board, moving_player, move, &current_captures, false);
            break;
        case 'q': 
            valid = moveQueen(board, moving_player, move, &current_captures, false);
            break;
        case 'k': 
            valid = moveKing(board, moving_player, move, &current_captures, false);
            break;
    }

    std::string new_board_str = "";

    if (valid != INVALID_MOVE)
    {
        new_board_str.reserve(BOARD_SIZE * BOARD_SIZE);

        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                new_board_str.push_back(board[i][j]);
    }
    else new_board_str = board_string;

    freeBoard(&board, &ply1, &ply2);
    return new_board_str;
}

/**
 * @brief Checks checkmate for a specific color.
 */
bool check_mate(std::string board_string, int color_int)
{
    char **board { board_parser(board_string) };
    PieceColor color = static_cast<PieceColor>(color_int);

    Player ply { player_parser(board, color) };
    bool state = checkMate(board, &ply);

    freeBoard(&board, &ply, NULL);
    return state;
}

/**
 * @brief Checks stalemate for a specific color, including insufficient material.
 */
bool check_stalemate(std::string board_string, int color_int)
{
    char **board { board_parser(board_string) };
    PieceColor color = static_cast<PieceColor>(color_int);
    PieceColor opponent_color = (color == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;

    Player ply { player_parser(board, color) };
    Player opp { player_parser(board, opponent_color) };

    bool state = checkStalemate(board, &ply, &opp);

    freeBoard(&board, &ply, &opp);
    return state;
}


/**
 * @brief Formats a Move struct into the string format expected by move_piece.
 */
static std::string format_move_to_string(Move m)
{
    std::string move_str;
    move_str.reserve(MOVE_BUFFER);
    
    move_str += m.symbol;
    move_str += (char)(m.colPrev + 'A');

    move_str += (char)(m.rowPrev + '0');
    move_str += (char)(m.colNext + 'A');
    
    move_str += (char)(m.rowNext + '0');
    move_str += (m.promotedPawn != ' ') ? m.promotedPawn : ' ';
    
    return move_str;
}


/**
 * @brief Generates all legal moves for the given board and color.
 */
std::vector<std::string> get_available_moves(std::string board_string, int color_int)
{
    std::vector<std::string> move_list;

    char** board = board_parser(board_string);
    
    PieceColor color = static_cast<PieceColor>(color_int);
    
    Player player = player_parser(board, color);

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
                
                if (!isEmpty(board, r, c) && pieceColorAt(board, r, c) == color) continue;

                test_move.rowNext = r;
                test_move.colNext = c;

                char** cpy_b = board_parser(board_string);
                
                Player cpy_p = player_parser(cpy_b, color);

                Captured tmp_cap { initializeCapture(color) };
                
                int p_ep = -1, o_ep = -1;
                
                MoveValidation valid = INVALID_MOVE;

                if (type == 'p') valid = movePawn(cpy_b, &cpy_p, test_move, &tmp_cap, &p_ep, &o_ep, true, false);
                else if (type == 'r') valid = moveRook(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'n') valid = moveKnight(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'b') valid = moveBishop(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'q') valid = moveQueen(cpy_b, &cpy_p, test_move, &tmp_cap, true);
                else if (type == 'k') valid = moveKing(cpy_b, &cpy_p, test_move, &tmp_cap, true);

                if (valid != INVALID_MOVE && !isChecked(cpy_b, &cpy_p, true))
                {
                    move_list.push_back(format_move_to_string(test_move));
                }
                freeBoard(&cpy_b, &cpy_p, nullptr);
            }
        }
    };

    for (int i = 0; i < NUM_PAWNS; i++) collect_for_piece((Piece*)&player.pawns[i], 'p');
    for (int i = 0; i < NUM_PIECES; i++) 
    {
        collect_for_piece((Piece*)&player.rooks[i], 'r');
        collect_for_piece((Piece*)&player.knights[i], 'n');
        
        collect_for_piece((Piece*)&player.bishops[i], 'b');
    }

    collect_for_piece((Piece*)player.queen, 'q');
    collect_for_piece((Piece*)player.king, 'k');

    freeBoard(&board, &player, nullptr);
    return move_list;
}
