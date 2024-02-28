
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    int vec_size = 4;
    __m256d vec1, vec2, factor;
    factor = _mm256_set1_pd(a);

    for (int i = 0; i < n; i += vec_size)
    {
        vec1 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_mul_pd(factor, vec1);
        _mm256_storeu_pd(&c[i], vec2);
    }

    for (int i = n - (n % vec_size); i < n; i++)
    {
        c[i] = a * b[i];
    }
}
