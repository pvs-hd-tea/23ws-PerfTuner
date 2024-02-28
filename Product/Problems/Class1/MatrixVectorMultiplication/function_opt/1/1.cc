
#include <immintrin.h>
#include <iostream>

void function(int row, int col, double *a, double *b, double *c)
{
    // AVX optimization starts here
    int vec_size = 4;  // vec_size = 4 since AVX can hold 4 double values
    __m256d vec1, vec2, factor;
    int i;
    
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (i = 0; i < col - vec_size; i += vec_size) // head loop (optimized using AVX)
        {
            factor = _mm256_set1_pd(a[k * col + i]);
            vec1 = _mm256_loadu_pd(&b[i]); 
            vec2 = _mm256_loadu_pd(&c[k]);
            vec2 = _mm256_fmadd_pd(factor, vec1, vec2); 
            _mm256_storeu_pd(&c[k], vec2);
        }
        for (; i < col; i++) // handle the remaining elements
        {
            c[k] += a[k * col + i] * b[i];
        }
    }
}
