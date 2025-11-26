compile:
	gcc -Wall -g ./Pieces/src/*.c ./Board/src/board.c -o ./Board/bin/main.o

pieces:
	gcc -Wall -g ./Pieces/src/*.c ./Pieces/test.c -o ./Pieces/bin/pieces.o

run:
	./Board/bin/main.o
	echo

clean:
	rm -f ./Board/bin/main.o