
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    for (int i = 0; i < n; i+=4)
    {
        __m256d scalar = _mm256_set1_pd(a);
        __m256d vec_b = _mm256_loadu_pd(&b[i]);
        
        // Perform AVX intrinsics multiplication
        __m256d result = _mm256_mul_pd(scalar, vec_b);
        
        // Store the results in the corresponding index of array 'c'
        _mm256_storeu_pd(&c[i], result);
    }
}
