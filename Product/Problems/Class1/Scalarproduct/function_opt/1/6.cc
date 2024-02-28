
#include <immintrin.h>

void function(int n, double *a, double *b, double &c)
{
    c = 0;
    int vec_size = 4;
    __m256d vec1, vec2;
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size)     
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_mul_pd(vec1, vec2);
        __m256d temp_sum = _mm256_add_pd(vec2, _mm256_permute2f128_pd(vec2, vec2, 1)); // Horizontal add using permute
        c += _mm256_cvtsd_f64(temp_sum); // Get the sum of 4 values and add it to c
    }
    
    for (; i < n; i += 1)  
    {
        c += a[i] * b[i];
    }
}
