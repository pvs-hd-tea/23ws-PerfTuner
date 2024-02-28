
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int vec_size = 4;  // number of elements in AVX register
    __m256d vec1, vec2;
    
    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j += vec_size)  // optimize the loop using AVX
        {
            vec2 = _mm256_setzero_pd();  // Initialize vector for result
            for (int i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_fmadd_pd(vec1, _mm256_loadu_pd(&b[i * col2 + j]), vec2);  // Fused multiply-add operation
            }
            _mm256_storeu_pd(&c[k * row1 + j], vec2);
        }
        
        // Handle the remaining elements
        for (int j = col2 - (col2 % vec_size); j < col2; j++)
        {
            c[k * row1 + j] = 0;
            for (int i = 0; i < col1; i++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}
