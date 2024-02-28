
#include <immintrin.h>
void function(int n, double a, double *b, double c, double *d, double *e)
{
    __m256d vec1, vec2;
    int vec_size = 4; // number of elements in AVX register

    int i = 0;
    for (; i <= n - vec_size; i += vec_size) // AVX-optimized loop
    {
        vec1 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_loadu_pd(&d[i]);
        vec2 = _mm256_mul_pd(_mm256_set1_pd(a), vec1);

        __m256d vec3 = _mm256_mul_pd(_mm256_set1_pd(c), vec2);
        vec2 = _mm256_add_pd(vec2, vec3);

        _mm256_storeu_pd(&e[i], vec2);
    }

    for (; i < n; i++) // handle remaining elements
    {
        e[i] = a * b[i] + c * d[i];
    }
}
