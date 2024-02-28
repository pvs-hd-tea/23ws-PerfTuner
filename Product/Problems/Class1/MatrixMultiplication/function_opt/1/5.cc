
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int vec_size = 4; // number of elements processed in AVX register
    __m256d vec1, vec2;

    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j += vec_size)
        {
            vec2 = _mm256_setzero_pd();
            for (int i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_add_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&b[i * col2 + j])));
            }
            _mm256_storeu_pd(&c[k * row1 + j], vec2);
        }
        for (int j = (col2 / vec_size) * vec_size; j < col2; j++) // handle remaining elements
        {
            c[k * row1 + j] = 0;
            for (int i = 0; i < col1; i++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}
