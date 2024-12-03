#include <stdio.h>
#include <emmintrin.h>
#include <string.h>

/*

asdf
asbb

xff xff x00 x00

00000001 00000001 00000000 00000000

1 1 0 0

*/
int main (int argc, char** argv) {

    // because we are already going through the hassle of getting the length
    // it may not be very beneficial using simd
    char* str1 = argv[1];
    char* str2 = argv[2];
    int diff = 0;

    int len1 = strlen(str1);
    int len2 = strlen(str2);

    // min (l1,l2) is divisible by 4
    for (int i=0; i < min(len1, len2)/4; i++) {
        // all here are non nulls

        // we specify the bytes themselves
        __m128i a = _mm_setr_epi8(str1[0],str1[1],str1[2],str1[3],0,0,0,0, 0,0,0,0, 0,0,0,0); 
        __m128i b = _mm_setr_epi8(str2[0],str2[1],str2[2],str2[3],0,0,0,0, 0,0,0,0, 0,0,0,0); 

        // get byte2byte comparison of str1, str2
        // result is a mask, each 0xFF byte means theres a difference
        __m128i result = _mm_cmpeq_epi8(a, b);

        // turn each 0xFF into a bit, from vector of 128 bits to mask of 16 bits
        int mask = _mm_movemask_epi8(result);

        // only get the first 4 bits all the other ones are 0
        diff += __builtin_popcount(mask & 0xF);

        str1 += 4;
        str2 += 4;
    }

    // get the difference, is divisible by 4
    diff += abs(len1 - len2);

    printf("%d", diff);
}


// int main (int argc, char** argv) {
//     char* str1 = argv[1];
//     char* str2 = argv[2];
//     int diff = 0;

//     while (*str1 || *str2) {
//         diff += *str1 != *str2;
//         if (*str1) str1++;
//         if (*str2) str2++;
//     }

//     printf("%d", diff);
// }

// int main (int argc, char** argv) {
//     char* str1 = argv[1];
//     char* str2 = argv[2];
//     int firstdone = 0;
//     int seconddone = 0;
//     int diff = 0;

//     firstdone  = str1[0] == '\0';   
//     seconddone = str2[0] == '\0';

//     while (!firstdone && !seconddone) {
//         if (*str1 != *str2) diff ++;
//         str1++;
//         str2++;        
//         firstdone  = *str1 == '\0';
//         seconddone = *str2 == '\0';
//     }

//     char* str = firstdone == 1 ? str2 : str1;
//     int other = *str == '\0';

//     while (!other) {
//         diff++;
//         str++;
//         other = *str == '\0';
//     }

//     printf("%d", diff);
// }