
#include <immintrin.h>

void function(int n, double a, double *b, double *c)
{
    int vec_size = 4; // Vector size for AVX operations
    __m256d vec_a, vec_b, vec_c;
    vec_a = _mm256_set1_pd(a);

    // Head loop for vectorized operations using AVX
    for (int i = 0; i < n; i += vec_size)
    {
        vec_b = _mm256_loadu_pd(&b[i]);
        vec_c = _mm256_mul_pd(vec_a, vec_b);
        _mm256_storeu_pd(&c[i], vec_c);
    }

    // Handle remaining elements
    for (int i = n - n % vec_size; i < n; i++)
    {
        c[i] = a * b[i];
    }
}
