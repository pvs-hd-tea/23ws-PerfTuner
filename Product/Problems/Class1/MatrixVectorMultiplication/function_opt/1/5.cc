
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    int vec_size = 4; // AVX can handle 4 double values
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        double factor = a[k * col] / a[col]; // calculate the factor outside the loop

        __m256d vec_factor = _mm256_set1_pd(factor); // broadcast the factor
        int i = 0;
        for (; i < col - vec_size; i += vec_size)
        {
            __m256d vec_a = _mm256_loadu_pd(&a[k * col + i]);
            __m256d vec_b = _mm256_loadu_pd(&b[i]);
            __m256d vec_c = _mm256_loadu_pd(&c[k]);
            
            vec_c = _mm256_fmadd_pd(vec_a, vec_b, vec_c);
            _mm256_storeu_pd(&c[k], vec_c);
        }

        for (; i < col; i += 1)
        {
            c[k] += a[k * col + i] * b[i]; // handle the remaining elements individually
        }
    }
}
