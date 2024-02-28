
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int vec_size = 4;   // number of elements in AVX register, 4 for double
    __m256d vec1, vec2;
    
    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j += vec_size)  // Optimize over 'j' using AVX
        {
            vec2 = _mm256_setzero_pd();
            for (int i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);  // Load 256 bits (4 doubles) from 'a'
                __m256d temp = _mm256_loadu_pd(&b[i * col2 + j]);  // Load 256 bits (4 doubles) from 'b'
                vec2 = _mm256_fmadd_pd(vec1, temp, vec2);  // Multiply and accumulate in AVX
            }
            _mm256_storeu_pd(&c[k * row1 + j], vec2);  // Store result in 'c'
        }
    }
}
