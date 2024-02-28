
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    __m256d vec1, vec2, result;
    int vec_size = 4;
    
    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j+=vec_size)
        {
            result = _mm256_setzero_pd();  // Initialize result vector to zero
            for (int i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_loadu_pd(&b[i * col2 + j]);
                result = _mm256_fmadd_pd(vec1, vec2, result);  // Multiply and accumulate
            }
            _mm256_storeu_pd(&c[k * row1 + j], result);  // Store the result vector in the c array
        }
    }
}
