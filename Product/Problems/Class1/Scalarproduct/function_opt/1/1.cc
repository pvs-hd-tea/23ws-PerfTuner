
#include <immintrin.h>

void function(int n, double *a, double *b, double &c)
{
    __m256d vec1, vec2;  // define AVX variables outside the loop
    int vec_size = 4;     // number of elements in AVX register (4 for double)
    int i = 0;
    
    for (; i <= n - vec_size; i += vec_size)  // optimized AVX loop
    {
        vec1 = _mm256_loadu_pd(&a[i]);  // load 4 elements from 'a'
        vec2 = _mm256_loadu_pd(&b[i]);  // load 4 elements from 'b'
        vec1 = _mm256_sub_pd(vec1, vec2);  // subtract and store in 'vec1'
        _mm256_storeu_pd(&c[i], vec1);     // store result back to 'c'
    }
    
    for (; i < n; i++)  // handle remaining elements
    {
        c[i] = a[i] - b[i];  // scalar subtraction
    }
}
