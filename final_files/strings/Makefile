CC = gcc

all: hamming b64

hamming: hamming.s
	$(CC) -o hamming hamming.s

b64: b64.c
	$(CC) -o b64 b64.c -msse4.2

clean:
	rm -f hamming b64