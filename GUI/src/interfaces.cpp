#include "interfaces.hpp"

#include <algorithm>
#include <iostream>

namespace Concrete
{
    /**
     * @brief adds given observer to the observers list of a subject.
     * @param Observer.
     * @return none.
    */
    void Subject::addObserver(Concrete::Observer* observer)
    {
        if (observer != nullptr) observers_data.push_back(observer);
    }


    /**
     * @brief removes the given observer from the observers list.
     * @param Observer.
     * @return none.
    */
    void Subject::removeObserver(Concrete::Observer* observer)
    {
        auto it = std::find(observers_data.begin(), observers_data.end(), observer);
        if (it != observers_data.end()) observers_data.erase(it);
    }


    /**
     * @brief notifies all observers when called with an update to the board.
     * @param none.
     * @return none.
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

        this->player_turn = player_turn;
    }

    
    Board::~Board()
    {
        delete ply1EP;
        delete ply2EP;
        
        delete ply1_captures;
        delete ply2_captures;
        
        freeBoard(board_ptr, ply1, ply2);
        
        delete board_ptr;
    }
    

    void Board::update_board()
    {
        updateBoard(this->get_board_array(), this->ply1, this->ply2);
        
        this->update_turn((this->player_turn == PLAYER1) ? PLAYER2 : PLAYER1);
 
        this->notifyObservers();
        return;
    }
}
