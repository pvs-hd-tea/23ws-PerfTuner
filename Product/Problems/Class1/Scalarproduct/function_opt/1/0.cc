
#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    c = 0;
    __m256d vec1, vec2, vecSum = _mm256_setzero_pd();
    int vec_size = 4;
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size)
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_mul_pd(vec1, vec2);
        vecSum = _mm256_add_pd(vecSum, vec2);
    }
    
    double tmp[4] __attribute__((aligned(32)));
    _mm256_store_pd(tmp, vecSum);
    c = tmp[0] + tmp[1] + tmp[2] + tmp[3];
    
    for (; i < n; i++)
    {
        c += a[i] * b[i];
    }
}
