
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int vec_size = 4; // number of elements in AVX register, 4 for double
    __m256d vec1, vec2, sum;
    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j <= col2 - vec_size; j += vec_size)
        {
            sum = _mm256_setzero_pd();
            for (int i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_loadu_pd(&b[i * col2 + j]);
                sum = _mm256_fmadd_pd(vec1, vec2, sum);
            }
            _mm256_storeu_pd(&c[k * row1 + j], sum);
        }
        for (int j = col2 - vec_size + 1; j < col2; j++)
        {
            c[k * row1 + j] = 0;
            for (int i = 0; i < col1; i++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}
