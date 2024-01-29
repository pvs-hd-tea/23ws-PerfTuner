
#include <immintrin.h>

void function(int n, double *a, double *b, double *c)
{
    int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
    __m256d vec1, vec2, factor; // define variables only once outside the loop
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size) // optimized head loop using AVX
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_add_pd(vec1, vec2);
        _mm256_storeu_pd(&c[i], vec2);
    }
    
    for (; i < n; i += 1) // handle the remaining elements
    {
        c[i] = a[i] + b[i];
    }
}
