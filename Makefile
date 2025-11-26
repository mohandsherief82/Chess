compile:
	gcc -Wall ./Pieces/src/*.c ./Board/src/board.c -o ./Board/bin/main.o

run:
	./Board/bin/main.o
	echo

clean:
	rm -f ./Board/bin/main.o