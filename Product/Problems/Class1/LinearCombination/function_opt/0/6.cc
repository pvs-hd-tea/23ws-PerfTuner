
#include <immintrin.h>
void function(int n, double a, double *b, double c, double *d, double *e)
{
    __m256d vec1, vec2;  // define variables once outside the loop
    int vec_size = 4;     // number of elements in AVX register
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)  // optimized loop using AVX
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_mul_pd(vec1,vec2);
        vec1 = _mm256_loadu_pd(&c[i]);
        vec2 = _mm256_fmadd_pd(vec1,_mm256_loadu_pd(&d[i]),vec2);
        _mm256_storeu_pd(&e[i],vec2);
    }
    for (; i < n; i += 1)  // handle the remaining elements
    {
        e[i] = a * b[i] + c * d[i];
    }
}
