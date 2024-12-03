CC = gcc
FLAGS = -c

all: formula1 formula2 main

formula1: formula1.c
	$(CC) $(FLAGS) formula1.c -o formula1 

formula2: formula2.s
	$(CC) $(FLAGS) formula2.s -o formula2

main: main.c
	$(CC) formula1 formula2 main.c -lm -o main
	rm -f formula1 formula2

run: 
	./main
clean:
	rm -f main