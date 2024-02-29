
#include <immintrin.h>
void function(int n, double *a, double *b, double *c)
{
    __m256d vec1, vec2;   // define variables once outside the loop
    int vec_size = 4;     // number of elements in AVX register, 4 for double
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size)  // head loop
    {
        vec1 = _mm256_loadu_pd(&a[i]);
        vec2 = _mm256_loadu_pd(&b[i]);
        vec2 = _mm256_add_pd(vec1, vec2);
        _mm256_storeu_pd(&c[i], vec2);
    }
  
    for (; i < n; i += 1)   // tail end loop
    {
        c[i] = a[i] + b[i];
    }
}