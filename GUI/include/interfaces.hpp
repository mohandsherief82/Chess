#pragma once

#include <vector>
#include <memory>
#include <string>

extern "C"
{
    #include "Board/include/board.h"
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
    #include "chessTypes.h"
}

#define PLAYER1 1
#define PLAYER2 2

namespace Concrete
{
    class Subject;

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

    class Board: public Concrete::Subject, public std::enable_shared_from_this<Board>
    {
        private:
            char ***board_ptr = nullptr;
            std::string board_str = "";
            Player *ply1 = nullptr, *ply2 = nullptr;
            Captured *ply1_captures = nullptr, *ply2_captures = nullptr;
            int *ply1EP = new int(-1), *ply2EP = new int(-1);
            int player_turn = 1;
            std::string game_path {}, redo_path {};
        public:
            Board();
            Board(char ***board_ptr, int player_turn);
            ~Board();
            
            // Prevent copying to avoid double-free issues
            Board(const Board&) = delete;
            Board& operator=(const Board&) = delete;
            
            void update_board();
            
            std::string get_board_string();
            
            char **get_board_array() const { return *board_ptr; }
            char ***get_board_ptr() const { return board_ptr; }
            
            Player *get_player(int ply_num) const { return (ply_num == 1) ? ply1: ply2; }
            Captured *get_player_captures(int ply_num) const { return (ply_num == 1) ? ply1_captures: ply2_captures; }
            
            int *get_player_EP(int ply_num) const { return (ply_num == 1) ? ply1EP: ply2EP; }
            int get_player_turn() const { return player_turn; }

            std::string get_game_path() { return this->game_path; }
            std::string get_redo_path() { return this->redo_path; }

            void update_turn(int turn) { this->player_turn = turn; }
            void udpate_game_path(std::string path) { this->game_path = path; }
            void udpate_redo_path(std::string path) { this->redo_path = path; }
    };
}
