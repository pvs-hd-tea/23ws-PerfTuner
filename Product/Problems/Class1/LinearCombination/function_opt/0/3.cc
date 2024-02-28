
#include <immintrin.h>

void function(int n, double* a, double* b, double* c)
{
    __m256d vec_a, vec_b, vec_result;
    int vec_size = 4;
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size)
    {
        vec_a = _mm256_loadu_pd(&a[i]);
        vec_b = _mm256_loadu_pd(&b[i]);
        vec_result = _mm256_add_pd(vec_a, vec_b);
        _mm256_storeu_pd(&c[i], vec_result);
    }
    
    for (; i < n; i++)
    {
        c[i] = a[i] + b[i];
    }
}
