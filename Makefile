CC=gcc
FLAGS=-Wall

.PHONY: board pieces moves test_pieces test_moves test_moves_io test_board

board: 
	$(CC) $(FLAGS) ./Pieces/src/*.c ./Moves/src/captures.c ./Board/src/board.c ./Board/test.c -o ./Board/bin/test.o

pieces:
	$(CC) $(FLAGS) ./Pieces/test.c ./Pieces/src/*.c -o ./Pieces/bin/pieces.o

moves:
	$(CC) $(FLAGS) ./Pieces/src/*.c ./Board/src/board.c ./Moves/src/*.c ./Moves/test.c -o ./Moves/bin/test.o

game_end:
	$(CC) $(FLAGS) ./Pieces/src/*.c ./Board/src/board.c ./Moves/src/*.c ./Game-End/src/*.c ./Game-End/test.c -o ./Game-End/bin/test.o

compile:
	$(CC) $(FLAGS) ./Pieces/src/*.c ./Board/src/board.c ./Moves/src/*.c ./Game-End/src/*.c ./Game-End/test.c -o ./run.o

test_board: board
	./Board/bin/test.o

test_pieces: pieces
	./Pieces/bin/pieces.o

test_moves: moves
	./Moves/bin/test.o

test_game: game_end
	./Game-End/bin/test.o

run: compile
	./run.o

clean:
	rm -f ./Board/bin/*
	rm -f ./Pieces/bin/*
	rm -f ./Moves/bin/*
	rm -f ./*.o
	clear
