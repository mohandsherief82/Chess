
import sys
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
build_path = os.path.join(script_dir, "../..", "build")

if build_path not in sys.path:
    sys.path.append(build_path)


import math
from Move_Wrappers import get_available_moves, move_piece, check_mate, check_stalemate


class Minimax:
    __piece_value = {'k': 1000, 'q': 90, 'r': 50, 'b': 30, 'n': 30, 'p': 10}
    
    def __init__(self, max_depth: int = 3):
        self.max_depth = max_depth

    def __get_available_moves(self, current_state: str, color: int) -> list:
        """ 
            Fetches all legal moves for the current player color. 
        """
        return get_available_moves(current_state, color)

    def __is_terminal(self, current_state: str, color: int) -> bool:
        """ 
            Checks if the current state is mate or stalemate for the player. 
        """
        return check_mate(current_state, color) or check_stalemate(current_state, color)

    def __minimax(self, current_state: str, is_maximizing_turn: bool, alpha: float, beta: float, depth: int = 0) -> int:
        current_color = 1 if is_maximizing_turn else 0

        if depth == self.max_depth or self.__is_terminal(current_state, current_color):
            return self.__evaluation_function(current_state)

        moves = self.__get_available_moves(current_state, current_color)
        
        if not moves:
            return self.__evaluation_function(current_state)

        if is_maximizing_turn:
            max_eval = -math.inf

            for move in moves:
                new_state = move_piece(current_state, move)
                eval_score = self.__minimax(new_state, False, alpha, beta, depth + 1)

                max_eval = max(max_eval, eval_score)
                alpha = max(alpha, eval_score)

                if beta <= alpha:
                    break

            return max_eval
        else:
            min_eval = math.inf

            for move in moves:
                new_state = move_piece(current_state, move)
                eval_score = self.__minimax(new_state, True, alpha, beta, depth + 1)
                
                min_eval = min(min_eval, eval_score)
                beta = min(beta, eval_score)
                
                if beta <= alpha:
                    break

            return min_eval

    def __evaluation_function(self, current_state: str) -> int:
        """ 
            Simple Material-based evaluation. 
        """
        score = 0

        for char in current_state:
            lower_char = char.lower()
        
            if lower_char in self.__piece_value:
                if char.islower():
                    score += self.__piece_value[lower_char]
                else:
                    score -= self.__piece_value[lower_char]

        return score


    def get_best_move(self, current_state: str) -> str:
        """ 
            Returns the best move string found by the AI. 
        """
        best_move = None
        max_eval = -math.inf
        
        moves = self.__get_available_moves(current_state, 1)
        
        for move in moves:
            new_state = move_piece(current_state, move)
            eval_score = self.__minimax(new_state, False, -math.inf, math.inf, 1)
            
            if eval_score > max_eval:
                max_eval = eval_score
                best_move = move
                
        return best_move
