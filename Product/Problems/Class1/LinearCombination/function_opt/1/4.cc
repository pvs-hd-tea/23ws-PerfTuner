
#include <immintrin.h>

void function(int n, double a, double *b, double c, double *d, double *e)
{
    int vec_size = 4;
    __m256d vec1, vec2;
    
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)
    {
        vec1 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_loadu_pd(&d[i]);
        vec2 = _mm256_mul_pd(vec2, _mm256_set1_pd(c));
        vec1 = _mm256_mul_pd(vec1, _mm256_set1_pd(a));
        vec2 = _mm256_add_pd(vec1, vec2);
        _mm256_storeu_pd(&e[i], vec2);
    }
    
    for (; i < n; i++)
    {
        e[i] = a * b[i] + c * d[i];
    }
}
