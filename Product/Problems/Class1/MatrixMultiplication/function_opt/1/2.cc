
#include <immintrin.h>

void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int i = 0;
    __m256d vec1, vec2; // variables for AVX operations
    int vec_size = 4;   // 4 elements in AVX register for double 

    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j += vec_size)
        {
            vec2 = _mm256_setzero_pd(); // initialize to zero
            for (int i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_fmadd_pd(vec1, _mm256_loadu_pd(&b[i * col2 + j]), vec2);
            }
            _mm256_storeu_pd(&c[k * col2 + j], vec2);
        }
    }
}
