
#include <immintrin.h>

void function(int n, double a, double *b, double *c)
{
    int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
    __m256d vec1, vec2, factor; // define variables only once outside the loop
    
    factor = _mm256_set1_pd(a); // load the constant only once outside the loop
    int i = 0;
    for (; i <= n - vec_size; i += vec_size) // optimized loop using AVX
    {
        vec1 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_mul_pd(factor, vec1);
        _mm256_storeu_pd(&c[i], vec2);
    }
    
    for (; i < n; i += 1) // handle the remaining elements
    {
        c[i] = a * b[i];
    }
}
