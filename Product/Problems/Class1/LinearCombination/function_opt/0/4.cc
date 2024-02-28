
#include <immintrin.h>
void function(int n, double a, double *b, double c, double *d, double *e)
{
    __m256d vec1, vec2;  // Define __m256d variables outside the loop
    int vec_size = 4;     // Number of elements in AVX register (4 for double)
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size)  // AVX optimized loop
    {
        vec1 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_loadu_pd(&d[i]);
        vec2 = _mm256_mul_pd(_mm256_set1_pd(a), vec1);
        vec1 = _mm256_mul_pd(_mm256_set1_pd(c), vec2);
        _mm256_storeu_pd(&e[i], vec1);
    }
    
    for (; i < n; i += 1)  // Remaining elements processing
    {
        e[i] = a * b[i] + c * d[i];
    }
}
