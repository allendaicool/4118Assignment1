play: shell.o
	gcc -Wall -pedantic -g -o play shell.o

shell.o: shell.c   Header.h
	gcc -c -Wall -pedantic -g shell.c