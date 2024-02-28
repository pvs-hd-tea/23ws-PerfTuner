
#include <immintrin.h>
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    int i, j, k;
    __m256d vec1, vec2, vec3;

    for (k = 0; k < row1; k++)
    {
        for (j = 0; j <= col2 - 4; j += 4)
        {
            vec3 = _mm256_setzero_pd();
            for (i = 0; i < col1; i++)
            {
                vec1 = _mm256_loadu_pd(&a[k * col1 + i]);
                vec2 = _mm256_loadu_pd(&b[i * col2 + j]);
                vec2 = _mm256_mul_pd(vec1, vec2);
                vec3 = _mm256_add_pd(vec3, vec2);
            }
            _mm256_storeu_pd(&c[k * col2 + j], vec3);
        }

        for (; j < col2; j++)
        {
            c[k * col2 + j] = 0;
            for (i = 0; i < col1; i++)
            {
                c[k * col2 + j] += a[k * col1 + i] * b[i * col2 + j];
            }
        }
    }
}
