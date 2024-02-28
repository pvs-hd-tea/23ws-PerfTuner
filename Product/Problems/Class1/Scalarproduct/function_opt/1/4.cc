
#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    __m256d vec1, vec2, result;
    int vec_size = 4;
    int i = 0;
    
    c = 0;
    
    // Main computation loop with AVX intrinsics
    for (; i <= n - vec_size; i += vec_size)
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        result = _mm256_mul_pd(vec1, vec2);
        c += result[0] + result[1] + result[2] + result[3];  // Accumulate the 4 results
    }
    
    // Handle the remaining elements
    for (; i < n; i += 1)
    {
        c += a[i] * b[i];
    }
}
