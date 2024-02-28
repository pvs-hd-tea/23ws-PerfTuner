
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    __m256d vec1, vec2, factor;
    int vec_size = 4;
    factor = _mm256_set1_pd(a[0]);
    
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (int i = 0; i < col - vec_size; i += vec_size)
        {
            vec1 = _mm256_loadu_pd(&a[k * col + i]);
            vec2 = _mm256_loadu_pd(&b[i]);
            vec2 = _mm256_fmadd_pd(vec1, vec2, _mm256_loadu_pd(&c[k]));
            _mm256_storeu_pd(&c[k], vec2);
        }
        
        // Handle remaining elements
        for (int i = col - vec_size; i < col; i++)
        {
            c[k] += a[k * col + i] * b[i];
        }
        
        c[k] /= factor[0];
    }
}
