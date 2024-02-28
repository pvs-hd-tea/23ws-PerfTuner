
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    __m256d vec1, vec2;
    int vec_size = 4;
    int i = 0;

    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j += vec_size)
        {
            vec2 = _mm256_setzero_pd();
            for (; i <= col1 - vec_size; i += vec_size)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_add_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&b[i * col2 + j])));
            }
            i -= vec_size; // Adjust i for next iteration
            for (; i < col1; i++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
            _mm256_storeu_pd(&c[k * row1 + j], vec2);
        }
    }
}
