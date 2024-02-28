
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    __m256d vec1, vec2;  // define AVX variables outside the loop
    int vec_size = 4;     // number of elements in AVX register, 4 for double
    int idx = 0;
    
    for (int k = 0; k < row1; k++)
    {
        int i = 0;
        for (; i <= col1 - vec_size; i += vec_size)  // optimized AVX loop for col1
        {
            vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
            for (int j = 0; j < col2; j++)
            {
                vec2 = _mm256_broadcast_sd(&b[i * col2 + j]);
                vec2 = _mm256_mul_pd(vec1, vec2);
                vec2 = _mm256_add_pd(vec2, _mm256_loadu_pd(&c[k * row1 + j]));
                _mm256_storeu_pd(&c[k * row1 + j], vec2);
            }
        }
        
        // Handle remaining elements in col1
        for (; i < col1; i++)
        {
            for (int j = 0; j < col2; j++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}
