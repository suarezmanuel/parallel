/*Azmon Avraham Zvi 216270827
Manuel Suarez*/
#include <emmintrin.h>
#include <math.h>
#include <stdio.h>

float formula1 (float* p, int n) {

    // create constants' vectors
    __m128d _0 = _mm_set_pd1(0);
    __m128 _1 = _mm_set_ps1(1);
    __m128d _1d = _mm_set_pd1(1);

    __m128d sum_numerator1 = _0;
    __m128d sum_numerator2 = _0;
    __m128d mult_denumerator1 = _1d;
    __m128d mult_denumerator2 = _1d;

    // sum two vectors of length 4
    // shuffle patterns: 4321, 2143 thats it because log2(4)=2
    for (int i=0; i < n/4; i++) {
        // fill x with first 4 ints of p
        __m128 x      = _mm_loadu_ps(p);
        __m128 sqrt_x = _mm_sqrt_ps(x);
     
        // convert first two elements of sum into doubles
        sum_numerator1 = _mm_add_pd(sum_numerator1,  _mm_cvtps_pd(sqrt_x));
        // _mm_movehl_ps will make a new _m128 to be {sum[2],sum[3],sum[2],sum[3]}
        // then we convert the first two to doubles and sum them
        sum_numerator2 = _mm_add_pd(sum_numerator2, _mm_cvtps_pd(_mm_movehl_ps(sqrt_x, sqrt_x)));        

        __m128 x_x   = _mm_mul_ps(x, x);
        __m128 x_x_1 = _mm_add_ps(x_x, _1);
        __m128 mult = x_x_1;

        // holds a fourth in each index
        mult_denumerator1 = _mm_mul_pd(mult_denumerator1, _mm_cvtps_pd(x_x_1));
        mult_denumerator2 = _mm_mul_pd(mult_denumerator2, _mm_cvtps_pd(_mm_movehl_ps(x_x_1, x_x_1)));

        p += 4;
    }

    // _mm_unpackhi_pd gets the top dobule of sum_numerator
    double sum = _mm_cvtsd_f64(sum_numerator1) 
               + _mm_cvtsd_f64(_mm_unpackhi_pd(sum_numerator1,sum_numerator1)) 
               + _mm_cvtsd_f64(sum_numerator2) 
               + _mm_cvtsd_f64(_mm_unpackhi_pd(sum_numerator2,sum_numerator2));

    double mult = _mm_cvtsd_f64(mult_denumerator1) 
                * _mm_cvtsd_f64(_mm_unpackhi_pd(mult_denumerator1,mult_denumerator1)) 
                * _mm_cvtsd_f64(mult_denumerator2) 
                * _mm_cvtsd_f64(_mm_unpackhi_pd(mult_denumerator2,mult_denumerator2));

    return sqrt(1.0+cbrt(sum)/mult);
}

// int main (int argc, char** argv) {
//     float arr [4] = {1,1,1,1};
//     printf("res:  %f\n", function(arr,4));
// }


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