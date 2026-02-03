
piece_value = {'k':13, 'q': 9, 'r': 5, 'b': 3, 'n': 3, 'p': 1}

class Minimax:
    def __init__(self, current_state: str):
        self.current_state = current_state


    def __actions(self):
        pass

    
    def minimax(self):
        pass


    def __terminal(self):
        pass

    
    def __eval(self):
        score = 0

        for c in self.current_state:
            if c.islower():
                score = score + piece_value[c.lower()]
            else:
                score = score - piece_value[c.lower()]

        return score
