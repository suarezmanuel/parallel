/* 216270827 Azmon Avraham Zvi
340858935 Manuel Ignacio Suarez Del Solar */
#include<stdio.h>
#include<string.h>
#include<immintrin.h>
#include<stdlib.h>
#define MAX_STR 255

// This function is the C version of the strlen
// function using SIMD we saw at the practical session,
// but here with intrinsics.
int sse_strlen(char *str) {
    __m128i zero = _mm_setzero_si128(); // Zero vector
    int length = 0;
    char *ptr = str;
    while (1) {
        __m128i chunk = _mm_loadu_si128((const __m128i*)ptr); // load 16 bytes from the string
        int index = _mm_cmpistri(zero, chunk, _SIDD_CMP_EQUAL_EACH); // find the first zero byte

        if (index != 16) { // if a zero byte is found
            length += index;
            break;
        }

        length += 16;
        ptr += 16;
    }

    return length;
}

// This function turns a character to it's b64 value
// if c isn't a b64 char then the func return -1.
int b64_char_to_value(char c) {
    switch (c) {
        case 'A' ... 'Z':
            return c - 'A';
        case 'a' ... 'z':
            return c - 'a' + 26;
        case '0' ... '9':
            return c - '0' + 52;
        case '+':
            return 62;
        case '/':
            return 63;
        default:
            return -1;
    }
}

// This function calculates the decimal value of
// a b64 string, invalid characters will be ignored.
int sse_b64_to_decimal(char *str){
    int len = sse_strlen(str);
    int decimal_value = 0;
    
    // Defining all the essentials XMMs
    
    // To check if the characters are at the wanted ranges
    __m128i char_A = _mm_set1_epi8('A'); // = A A A A ...
    __m128i char_Z = _mm_set1_epi8('Z'); // = Z Z Z Z ...
    __m128i char_a = _mm_set1_epi8('a'); // = a a a a ...
    __m128i char_z = _mm_set1_epi8('z'); // = z z z z ...
    __m128i char_0 = _mm_set1_epi8('0'); // = 0 0 0 0 ...
    __m128i char_9 = _mm_set1_epi8('9'); // = 9 9 9 9 ...
    __m128i char_plus = _mm_set1_epi8('+'); // = + + + + ...
    __m128i char_slash = _mm_set1_epi8('/'); // = / / / / ...
    int i;
    // Since we don't now if 16|len we ought to fill the rightmost bits
    // so it will be 16-devisable
    int _16_div_len = len - len % 16 + 16;
    char *nstr = (char*)calloc(_16_div_len, sizeof(char));
    strcpy(nstr, str);

    for(i = 0; i < _16_div_len; i += 16){ // Loop through the string 16 characters at a time
        __m128i chunk = _mm_loadu_si128((__m128i*)&nstr[i]); // Load the 16 characters to an XMM

        __m128i cmp_A2Z = _mm_and_si128(_mm_cmpgt_epi8(chunk, _mm_add_epi8(char_A, _mm_set1_epi8(-1))),
         _mm_cmplt_epi8(chunk, _mm_add_epi8(char_Z, _mm_set1_epi8(1)))); // Check if the characters are between A and Z

        __m128i cmp_a2z = _mm_and_si128(_mm_cmpgt_epi8(chunk, _mm_add_epi8(char_a, _mm_set1_epi8(-1))),
         _mm_cmplt_epi8(chunk, _mm_add_epi8(char_z, _mm_set1_epi8(1)))); // Check if the characters are between a and z

        __m128i cmp_029 = _mm_and_si128(_mm_cmpgt_epi8(chunk, _mm_add_epi8(char_0, _mm_set1_epi8(-1)))
        , _mm_cmplt_epi8(chunk, _mm_add_epi8(char_9, _mm_set1_epi8(1)))); // Check if the characters are between 0 and 9
        
        __m128i cmp_plus = _mm_cmpeq_epi8(chunk, char_plus); // Check if the characters are +
        __m128i cmp_slash = _mm_cmpeq_epi8(chunk, char_slash); // Check if the characters are /

        __m128i cmp = _mm_or_si128(_mm_or_si128(_mm_or_si128(cmp_A2Z, cmp_a2z), _mm_or_si128(cmp_029, cmp_plus)), cmp_slash); // Combine all the comparisons

        int mask = _mm_movemask_epi8(cmp); // Convert the comparison to a mask
        for(int j = 0; j < 16; j++){ // Loop through the mask
            if(mask & (1 << j)){ // If the character is valid
                char c = ((char*)&chunk)[j];
                decimal_value = decimal_value * 64 + b64_char_to_value(c);
            }
        }
    }
    free(nstr);
    return decimal_value;
}

int b64_distance(char str1[MAX_STR], char str2[MAX_STR]){
    int val1 = sse_b64_to_decimal(str1);
    int val2 = sse_b64_to_decimal(str2);
    return val1 - val2;
}


int main() {
    // Example strings with smaller values
    char str1[MAX_STR] = "123123*$#^@%@H";
    char str2[MAX_STR] = "123123*$#^@%@i";
    int distance = b64_distance(str1, str2);
    printf("Base64 Distance: %d\n", distance);  // Expected output should be within int range

    return 0;
}