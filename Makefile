CC = gcc
CFLAGS = -std=c17 -Wall
OBJ = main.o parser.o executor.o

shell: $(OBJ)
	$(CC) $(CFLAGS) -o shell $(OBJ)

main.o: main.c parser.h executor.h
parser.o: parser.c parser.h
executor.o: executor.c executor.h

clean:
	rm -f shell $(OBJ)
