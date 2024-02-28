
#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    c = 0;
    __m256d vec1, vec2;  // declare AVX variables
    int vec_size = 4;     // number of elements in AVX register
    
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)    // AVX head loop optimization
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_mul_pd(vec1, vec2);
        _mm256_storeu_pd(&c[i], vec2);
    }
    
    for (; i < n; i += 1)  // handle remaining elements
    {
        c += a[i] * b[i];
    }
}
