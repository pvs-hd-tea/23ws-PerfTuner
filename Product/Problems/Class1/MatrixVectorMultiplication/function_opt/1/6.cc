
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    __m256d vec1, vec2, factor; 
    factor = _mm256_set1_pd(b[0]);
    
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        int i = 0;
        for (; i <= col - 4; i += 4)
        {
            vec1 = _mm256_loadu_pd(&a[k * col + i]);
            vec2 = _mm256_loadu_pd(&b[i]);
            vec1 = _mm256_mul_pd(vec1, vec2);
            vec1 = _mm256_hadd_pd(vec1, vec1);
            vec1 = _mm256_hadd_pd(vec1, vec1);
            c[k] += vec1[0];
        }
        for (; i < col; i++)
        {
            c[k] += a[k * col + i] * b[i];
        }
    }
}
