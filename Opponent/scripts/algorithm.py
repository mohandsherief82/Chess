class Minimax:
    __piece_value = {'k':13, 'q': 9, 'r': 5, 'b': 3, 'n': 3, 'p': 1}
    
    def __init__(self, max_depth: int):
        self.max_depth = max_depth


    def __actions(self):
        pass

    
    def minimax(self, current_state: str, depth: int = 0):
        if self.__terminal():
            return self.__eval(current_state)
        
        else:
            pass


    def __terminal(self, current_state: str):
        pass

    
    def __eval(self, current_state: str):
        score = 0

        for c in self.current_state:
            if c in self.__piece_value.keys():
                if c.islower():
                    score = score + self.piece_value[c.lower()]
                else:
                    score = score - self.piece_value[c.lower()]

        return score
