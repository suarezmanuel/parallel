#include <emmintrin.h>
#include <math.h>
#include <stdio.h>

float function (float* p, int n) {

    // create constants' vectors
    __m128 _0 = _mm_set_ps1(0);
    __m128 _1 = _mm_set_ps1(1);

    __m128 sum_numerator = _0;
    __m128 mult_denumerator = _1;

    // sum two vectors of length 4
    // shuffle patterns: 4321, 2143 thats it because log2(4)=2
    for (int i=0; i < n/4; i++) {
        // fill x with first 4 ints of p
        __m128 x = _mm_loadu_ps(p);
        // sqrt each element in vector
        __m128 sqrt_x = _mm_sqrt_ps(x);

        __m128 sum = sqrt_x;
     
        // holds a fourth in each index
        sum_numerator = _mm_add_ps(sum_numerator, sum);

        __m128 x_x   = _mm_mul_ps(x, x);
        __m128 x_x_1 = _mm_add_ps(x_x, _1);
        __m128 mult = x_x_1;

        // holds a fourth in each index
        mult_denumerator = _mm_mul_ps(mult_denumerator, mult);

        p += 4;
    }

    // creates mask for the shuffle, values are from 0 to 3
    // #define _MM_SHUFFLE(z, y, x, w) (((z) << 6) | ((y) << 4) | ((x) << 2) | (w))
    // holds result in all 4 indices
    sum_numerator = _mm_add_ps(sum_numerator, _mm_shuffle_ps(sum_numerator, sum_numerator, _MM_SHUFFLE(2,3,1,0)));
    sum_numerator = _mm_add_ps(sum_numerator, _mm_shuffle_ps(sum_numerator, sum_numerator, _MM_SHUFFLE(1,0,3,2)));

    mult_denumerator = _mm_mul_ps(mult_denumerator, _mm_shuffle_ps(mult_denumerator, mult_denumerator, _MM_SHUFFLE(2,3,1,0)));
    mult_denumerator = _mm_mul_ps(mult_denumerator, _mm_shuffle_ps(mult_denumerator, mult_denumerator, _MM_SHUFFLE(1,0,3,2)));

    // convert scalar single-precision to float 32 - get first 32 bit float out of vector
    float sum_numerator_f32 = _mm_cvtss_f32(sum_numerator);
    float mult_denumerator_f32 = _mm_cvtss_f32(mult_denumerator);
    // printf("sum:  %f\n", sum_numerator_f32);
    // printf("mult: %f\n", mult_denumerator_f32);
    // printf("res:  %f\n", sqrt(1+cbrt(sum_numerator_f32)/mult_denumerator_f32));
    return sqrt(1+cbrt(sum_numerator_f32)/mult_denumerator_f32);
}

int main (int argc, char** argv) {
    float arr [4] = {1,1,1,1};
    printf("res:  %f\n", function(arr,4));
}


// int main (int argc, char** argv) {
//     // how to sum two vectors of length 4
//     // shuffle patterns: 4321, 2143 thats it because log2(4)=2
    
//     long long sum_numerator = 0;
//     long long mult_denumerator = 1;

//     // __m128i x;
//     // __m128i sqrt_x;
//     // __m128i x_x;
//     // __m128i sum_sqrt_x;

//     float arr [4] = {1,1,1,1}; 
//     int n = 4;

//     float* p = arr;

//     for (int i=0; i < n; i++) {
//         float x = *p;
//         float sqrt_x = sqrt(x);
//         sum_numerator += sqrt_x;

//         float x_x = x*x;
//         float x_x_1 = x_x + 1;

//         mult_denumerator *= x_x_1;
//         p++;
//     }

//     printf("sum:  %lld\n", sum_numerator);
//     printf("mult: %lld\n", mult_denumerator);
//     printf("res:  %f\n", sqrt(1+cbrt(sum_numerator)/mult_denumerator));
// }