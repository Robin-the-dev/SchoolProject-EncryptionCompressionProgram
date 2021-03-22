#makefile

CC = gcc
CFLAGS = -Wall -Werror -ansi

normal : main.c Encryption.o Functionality.o Huffman.o
	$(CC) $(CFLAGS) -o PM.exe main.c Encryption.o Functionality.o Huffman.o -D DEBUG=0 -lm
	rm *.o

debug : main.c Encryption.o Functionality.o Huffman.o
	$(CC) $(CFLAGS) -o PM_DEBUG.exe main.c Encryption.o Functionality.o Huffman.o -D DEBUG=1 -lm
	rm *.o
	
Encryption.o : Encryption.c Core.h
	$(CC) $(CFLAGS) -c -o Encryption.o Encryption.c

Functionality.o : Functionality.c Core.h
	$(CC) $(CFLAGS) -c -o Functionality.o Functionality.c

Huffman.o : Huffman.c Huffman.h Core.h
	$(CC) $(CFLAGS) -c -o Huffman.o Huffman.c

clean :
	rm *.txt *.bin PM.exe PM_DEBUG.exe
