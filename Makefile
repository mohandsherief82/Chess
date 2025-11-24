compile:
	gcc ./Board/main.c ./Pieces/pawn.c -o main.o -Wall

run:
	./main.o
	echo

clean:
	rm -f main 