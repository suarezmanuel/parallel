gcc -c 4.s -g -o 4_1
gcc -c 3.c -g -o 4_2
gcc 4_1 4_2 main.c -lm -g -o 4