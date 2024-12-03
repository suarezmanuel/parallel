gcc -c main.c  -g -o 2_1
gcc -c 1.s -g -o 2_2
gcc -c 2.c -msse4.2 -g -o 2_3
gcc 2_1 2_2 2_3 -g -o 2