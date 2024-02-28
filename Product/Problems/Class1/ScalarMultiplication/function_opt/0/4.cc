
#include <immintrin.h>

void function(int n, double a, double *b, double *c)
{
    int vec_size = 4; // vec_size = 4 since AVX can process 4 double values
    __m256d vec_a, vec_b, vec_c, scalar_a;
    scalar_a = _mm256_set1_pd(a); // broadcast the scalar value 'a' to all elements in AVX register
    
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)
    {
        vec_a = _mm256_loadu_pd(&b[i]);
        vec_c = _mm256_mul_pd(scalar_a, vec_a);
        _mm256_storeu_pd(&c[i], vec_c);
    }
    
    for (; i < n; i++)
    {
        c[i] = a * b[i]; // handle the remaining elements using scalar operation
    }
}
