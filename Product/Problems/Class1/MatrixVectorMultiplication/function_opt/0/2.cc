
#include <immintrin.h>
void function(int row, int col, double *a, double *b, double *c) 
{
    for (int k = 0; k < row; k++)
    {
        c[k] = 0;
        for (int i = 0; i < col; i+=4) // Optimized for AVX intrinsics
        {
            __m256d sum = _mm256_setzero_pd();
            for (int ii = 0; ii < 4; ii++)
            {
                __m256d aVec = _mm256_loadu_pd(&a[k * col + i + ii]); //* Load 4 double precision values
                __m256d bVec = _mm256_broadcast_sd(&b[i + ii]);       //* Broadcast scalar to all elements
                sum = _mm256_fmadd_pd(aVec, bVec, sum);               //* Fused multiply-add operation
            }
            c[k] += _mm256_reduce_add_pd(sum);                       //* Reduce the sum of 4 elements to scalar
        }
    }
}
