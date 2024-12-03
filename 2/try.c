#include<stdio.h>
#include<strings.h>
#include<immintrin.h>

int main(){
    __m128i char_A = _mm_set1_epi8('A');// = 01111111 01111111 ...
	char val[16];		     
	_mm_storeu_si128((__m128i*)val, char_A);
     printf("%c %c %c %c %c %c %c %c %c %c %c %c %c \n", 
           val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8], val[9], val[10]);
    return 1;
}
