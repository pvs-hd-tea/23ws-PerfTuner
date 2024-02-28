
#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    c = 0;

    // Optimized using AVX intrinsics
    __m256d vec1, vec2;  
    int vec_size = 4;   
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)     
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_sub_pd(vec1, vec2);
        _mm256_storeu_pd(&c[i], vec2);
    }
    
    // Handle remaining elements
    for (; i < n; i += 1)  
        c[i] = a[i] - b[i];
}
