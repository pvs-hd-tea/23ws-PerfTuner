
#include <immintrin.h>

void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int vec_size = 4;
    __m256d vec1, vec2, factor;
    int k, j, i;

    for (k = 0; k < row1; k++)
    {
        for (j = 0; j < col2; j++)
        {
            c[k * row1 + j] = 0;
            for (i = 0; i <= col1 - vec_size; i += vec_size)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_loadu_pd(&b[i * col2 + j]);
                vec2 = _mm256_mul_pd(vec1, vec2);
                c[k * row1 + j] += vec2[0] + vec2[1] + vec2[2] + vec2[3];
            }
            for (; i < col1; i++)
            {
                c[k * row1 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}
