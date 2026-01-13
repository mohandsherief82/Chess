#pragma once

#include <vector>
#include <string>

extern "C"
{
    #include "Board/include/board.h"
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
    #include "chessTypes.h"
}

namespace Concrete
{
    class Observer
    {
        public:
            virtual ~Observer() = default;
            virtual void update() = 0;
    };

    class Subject
    {
        public:
            virtual ~Subject() = default;
            virtual void addObserver(Observer* observer);
            virtual void removeObserver(Observer* observer);
            virtual void notifyObservers();
        protected:
            std::vector<Concrete::Observer*> observers_data;
    };
}


namespace Chess
{
    class AIOpponent: public Concrete::Observer
    {
        public:
            void update() override;
    };

    class GInterface: public Concrete::Observer
    {
        public:
            void update() override;
    };

    class Board: public Concrete::Subject
    {
        private:
            char ***board_ptr = nullptr;
            std::string board_str = "";
            Player *ply1 = nullptr, *ply2 = nullptr;
            Captured *ply1_captures, *ply2_captures;
            int ply1EP = -1, ply2EP = -1;
        public:
            Board();
            Board(char ***board_ptr);
            ~Board() { freeBoard(board_ptr, ply1, ply2); }
            
            void update_board();
            std::string get_board_string();
            char **get_board_array() { return *board_ptr; }
    };
}
