import math

class Minimax:
    __piece_value = {'k': 13, 'q': 9, 'r': 5, 'b': 3, 'n': 3, 'p': 1}
    
    def __init__(self, max_depth: int = 5):
        self.max_depth = max_depth

    def __get_available_moves(self, current_state: str):
        pass

    def __is_terminal(self, current_state: str):
        pass

    def minimax(self, current_state: str, is_maximizing_turn: bool, depth: int = 0):
        """
            Recursive minimax function. 
            Note: is_maximizing_turn toggles each layer.
        """
        best_score = 0

        # Base case: Check terminal state or max depth
        if depth == self.max_depth or self.__is_terminal(current_state):
            return self.__evaluation_function(current_state)

        if is_maximizing_turn:
            best_score = -math.inf
            
            for move in self.__get_available_moves(current_state):
                # Recurse: increment depth and flip turn to False
                evaluation = self.minimax(move, False, depth + 1)

                best_score = max(best_score, evaluation)
            
        else:
            best_score = math.inf
            
            for move in self.__get_available_moves(current_state):
                # Recurse: increment depth and flip turn to True
                evaluation = self.minimax(move, True, depth + 1)

                best_score = min(best_score, evaluation)
            
        return best_score

    def __evaluation_function(self, current_state: str):
        """
            Calculates score based on piece values. 
            Lowercase = Positive (AI), Uppercase = Negative (Opponent).
        """
        score = 0

        for char in current_state:
            lower_char = char.lower()
        
            if lower_char in self.__piece_value:
                # Add value for lowercase (AI pieces), subtract for uppercase (Opponent)
                if char.islower():
                    score += self.__piece_value[lower_char]
                else:
                    score -= self.__piece_value[lower_char]

        return score

    def get_best_move(self, current_state: str):
        """
            Helper to find the best move string from the starting state 
        """
        best_move = None
        max_eval = -math.inf
        
        for move in self.__get_available_moves(current_state):
            # Start recursion at depth 1, opponent's turn (False)
            eval_score = self.minimax(move, 1, False)
            if eval_score > max_eval:
                max_eval = eval_score
                best_move = move
                
        return best_move
