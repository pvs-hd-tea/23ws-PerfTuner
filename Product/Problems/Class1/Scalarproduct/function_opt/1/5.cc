
#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    c = 0;

    __m256d vec1, vec2;
    int vec_size = 4;
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_mul_pd(vec1, vec2);  // Use AVX multiply instruction
        c += vec2;  // Accumulate the results
    }

    for (; i < n; i++)
    {
        c += a[i] * b[i];
    }
}