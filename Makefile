CC=gcc
FLAGS=-Wall -g

board: 
	$(CC) $(FLAGS) ./Pieces/src/*.c ./Board/src/board.c ./Board/main.c -o ./Board/bin/main.o

test_pieces:
	./Pieces/bin/pieces.o

pieces:
	$(CC) $(FLAGS) ./Pieces/test.c ./Pieces/src/*.c -o ./Pieces/bin/pieces.o

run: board
	./Board/bin/main.o

clean:
	rm -f ./Board/bin/*.o
	rm -f ./Pieces/bin/*.o
