#include <emmintrin.h>
#include <math.h>
#include <stdio.h>

extern float function (float *px, float *py, int n);
// float function (float *px, float *py, int n) {
//     // calc mul of ((x-y)^2+1)
//     // calc sum xy
//     __m128 sum = _mm_set_ps1(0);
//     __m128 _1 = _mm_set_ps1(1);
//     __m128 mult = _1;

//     for (int i=0; i < n/4; i++) {
//         __m128 x = _mm_loadu_ps(px);
//         __m128 y = _mm_loadu_ps(py);

//         __m128 xy = _mm_mul_ps(x,y);

//         __m128 x_minus_y = _mm_sub_ps(x,y);
//         __m128 pow_x_minus_y = _mm_mul_ps(x_minus_y, x_minus_y);
//         __m128 pow_x_minus_y_plus_1 = _mm_add_ps(pow_x_minus_y, _1);

//         sum = _mm_add_ps(sum, xy);
//         mult = _mm_mul_ps(mult, pow_x_minus_y_plus_1);

//         px += 4;
//         py += 4;
//     }

//     sum = _mm_add_ps(sum, _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(2,3,0,1)));
//     sum = _mm_add_ps(sum, _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(1,0,3,2)));

//     mult = _mm_mul_ps(mult, _mm_shuffle_ps(mult, mult, _MM_SHUFFLE(2,3,0,1)));
//     mult = _mm_mul_ps(mult, _mm_shuffle_ps(mult, mult, _MM_SHUFFLE(1,0,3,2)));

//     return _mm_cvtss_f32(sum) / _mm_cvtss_f32(mult);
// }

int main (int argc, char** argv) {
    float arr [4] = {1,2,3,4};
    printf("res:  %f\n", function(arr, arr,4));
}