
#include <immintrin.h>
void function(int n, double a, double *b, double c, double *d, double *e)
{
    int vec_size = 4;
    __m256d vec1, vec2;
    
    int i = 0;
    for (; i <= n - vec_size; i += vec_size)
    {
        vec1 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_loadu_pd(&d[i]);
        vec1 = _mm256_mul_pd(_mm256_set1_pd(a), vec1); // multiply a with elements in vec1
        vec2 = _mm256_mul_pd(_mm256_set1_pd(c), vec2); // multiply c with elements in vec2
        vec2 = _mm256_add_pd(vec1, vec2); // add the results together
        _mm256_storeu_pd(&e[i], vec2);
    }
    
    for (; i < n; i++)
    {
        e[i] = a * b[i] + c * d[i];
    }
}
