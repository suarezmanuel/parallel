CC = gcc

all: hamming b64 main

hamming: hamming.s
	$(CC) -c hamming.s -o hamming

b64: b64.c
	$(CC) -c b64.c -msse4.2 -o b64

main: main.c
	$(CC) hamming b64 main.c -msse4.2 -o main
	rm -f hamming b64
run: 
	./main

clean:
	rm -f main