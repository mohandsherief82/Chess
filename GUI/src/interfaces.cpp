#include "interfaces.hpp"

#include <algorithm>
#include <iostream>

namespace Concrete
{
    /*
        @brief adds given observer to the observers list of a subject.
        @param Observer.
        @return none.
    */
    void Subject::addObserver(Concrete::Observer* observer)
    {
        if (observer != nullptr) observers_data.push_back(observer);
    }


    /*
        @brief removes the given observer from the observers list.
        @param Observer.
        @return none.
    */
    void Subject::removeObserver(Concrete::Observer* observer)
    {
        auto it = std::find(observers_data.begin(), observers_data.end(), observer);
        if (it != observers_data.end()) observers_data.erase(it);
    }


    /*
        @brief notifies all observers when called with an update to the board.
        @param none.
        @return none.
    */
    void Subject::notifyObservers()
    {
        for (auto* observer : observers_data)
            if (observer != nullptr) observer->update();
    }
}

namespace Chess
{
    Board::Board()
    {
        this->board_ptr = new char**;
        *this->board_ptr = initializeBoard();

        this->ply1 = new Player(createPlayer(COLOR_WHITE));
        this->ply2 = new Player(createPlayer(COLOR_BLACK));

        this->ply1_captures = new Captured(initializeCapture(COLOR_WHITE));
        this->ply2_captures = new Captured(initializeCapture(COLOR_BLACK));

        this->board_str = this->get_board_string();
    }


    Board::Board(char ***board_ptr, int player_turn)
    {
        if (board_ptr == nullptr || *board_ptr == nullptr)
        {
            this->board_ptr = new char**;
            *this->board_ptr = initializeBoard();
        }
        
        else this->board_ptr = board_ptr;

        this->ply1 = new Player(createPlayer(COLOR_WHITE));
        this->ply2 = new Player(createPlayer(COLOR_BLACK));

        this->ply1_captures = new Captured(initializeCapture(COLOR_WHITE));
        this->ply2_captures = new Captured(initializeCapture(COLOR_BLACK));

        this->board_str = this->get_board_string();

        this->player_turn = player_turn;
    }


    /*
        @brief parses the main board into a string.
        @param none.
        @return returns the string to the function caller.
    */
    std::string Board::get_board_string()
    {
        board_str.clear();

        if (board_ptr == nullptr || *board_ptr == nullptr)
        {
            return "";
        }

        char **actual_board = *board_ptr;

        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                board_str.push_back(actual_board[i][j]);

        return board_str;
    }

    
    Board::~Board()
    {
        // Clean up EP pointers
        delete ply1EP;
        delete ply2EP;
        
        // Clean up captures
        delete ply1_captures;
        delete ply2_captures;
        
        // Clean up board and players (freeBoard handles ply1 and ply2)
        freeBoard(board_ptr, ply1, ply2);
        
        // Clean up the outer pointer
        delete board_ptr;
    }
    

    void Board::update_board()
    {
        updateBoard(*this->board_ptr, this->ply1, this->ply2);

        Player *ply { this->get_player((this->player_turn == PLAYER1) ? PLAYER2 : PLAYER1) };
        char **board { this->get_board_array() };
        
        this->update_turn((this->player_turn == PLAYER1) ? PLAYER2 : PLAYER1);
 
        this->notifyObservers();
        return;
    }


    void AIOpponent::update()
    {
        // Implementation
    }
}
