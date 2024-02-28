
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c)
{
    int vec_size = 4; // AVX can process 4 double values simultaneously
    __m256d vec_a, vec_b, vec_c, factor; // AVX register variables

    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (int i = 0; i < col-vec_size+1; i += vec_size)
        {
            factor = _mm256_set1_pd(a[k * col + i]); // Load a[k * col + i] into AVX register
            vec_a = _mm256_loadu_pd(&a[k * col + i]); // Load 4 elements of 'a' into AVX register
            vec_b = _mm256_loadu_pd(&b[i]); // Load 4 elements of 'b' into AVX register
            vec_c = _mm256_loadu_pd(&c[k]); // Load 4 elements of 'c' into AVX register
            vec_c = _mm256_fmadd_pd(vec_a, vec_b, vec_c); // Fused Multiply-Add operation
            _mm256_storeu_pd(&c[k], vec_c); // Store the result back into 'c'
        }

        // Handle the remaining elements using scalar operations
        for (int i = (col/vec_size) * vec_size; i < col; i++)
        {
            c[k] += a[k * col + i] * b[i];
        }
    }
}
