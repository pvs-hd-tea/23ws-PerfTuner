
#include <immintrin.h>

void function(int n, double a, double *b, double c, double *d, double *e)
{
    int vec_size = 4;  // AVX register can hold 4 double elements
    __m256d vec_a, vec_b, vec_c, vec_d;

    int i = 0;
    for (; i <= n - vec_size; i += vec_size)
    {
        vec_a = _mm256_loadu_pd(&a[i]);
        vec_b = _mm256_loadu_pd(&b[i]);
        vec_c = _mm256_set1_pd(c);
        vec_d = _mm256_loadu_pd(&d[i]);

        vec_a = _mm256_mul_pd(vec_a, vec_b);   // Multiply a by b
        vec_c = _mm256_mul_pd(vec_c, vec_d);   // Multiply c by d
        vec_e = _mm256_add_pd(vec_a, vec_c);   // Add the results

        _mm256_storeu_pd(&e[i], vec_e);
    }

    for (; i < n; i++)
    {
        e[i] = a * b[i] + c * d[i];
    }
}
