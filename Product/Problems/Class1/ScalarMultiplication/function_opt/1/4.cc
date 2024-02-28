
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    for (int i = 0; i < n; i+=4)
    {
        __m256d scalar_a = _mm256_set1_pd(a);

        __m256d vec_b = _mm256_loadu_pd(&b[i]);
        __m256d result = _mm256_mul_pd(scalar_a, vec_b);

        _mm256_storeu_pd(&c[i], result);
    }
}
