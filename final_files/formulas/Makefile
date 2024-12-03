CC = gcc

all: formula1 formula2

formula1: formula1.c
	$(CC) -o formula1 formula1.c -msse4.2 -lm

b64: b64.c
	$(CC) -o formula2 formula2.s
clean:
	rm -f formula1 formula2