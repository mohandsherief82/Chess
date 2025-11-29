CC=gcc
FLAGS=-Wall -g

.PHONY: board pieces moves test_pieces test_moves test_moves_io test_board

board: 
	$(CC) $(FLAGS) ./Pieces/src/*.c ./Board/src/board.c ./Board/main.c -o ./Board/bin/main.o

pieces:
	$(CC) $(FLAGS) ./Pieces/test.c ./Pieces/src/*.c -o ./Pieces/bin/pieces.o

moves:
	$(CC) $(FLAGS) ./Pieces/src/*.c ./Board/src/board.c ./Moves/src/*.c ./Moves/test.c -o ./Moves/bin/test.o

test_pieces: pieces
	./Pieces/bin/pieces.o

test_moves: moves
	./Moves/bin/test.o

test_moves_io: moves
	./Moves/bin/test.o < ./Moves/testing/input.txt > ./Moves/testing/output.txt

test_board: board
	./Board/bin/main.o

clean:
	rm -f ./Board/bin/*
	rm -f ./Pieces/bin/*
	rm -f ./Moves/bin/*
	clear
