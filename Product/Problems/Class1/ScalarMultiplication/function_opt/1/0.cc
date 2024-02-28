
#include <immintrin.h>
void function(int n, double a, double *b, double *c)
{
    int block_size = 4;
    
    for (int i = 0; i < n; i += block_size)
    {
        for (int j = 0; j < n; j += block_size)
        {
            for (int ii = 0; ii < block_size; ii += 4)
            {
                for (int jj = 0; jj < block_size; jj += 4)
                {
                    int c_start = i + ii + n*(j + jj); // Start position in matrix C
                    int b_start = j + jj + n*(i + ii); // Start position in matrix B
                    __m256d a_val = _mm256_set1_pd(a); // Broadcast scalar a to a vector
                    __m256d b_val = _mm256_loadu_pd(&b[b_start]); // Load 4 double values from matrix B
                    __m256d result = _mm256_mul_pd(a_val, b_val); // Multiply a by b
                    
                    // Non-temporal store for efficient writing 
                    _mm256_stream_pd(&c[c_start], result);
                }
            }
        }
    }
}
