
#include <immintrin.h>
void function(int n, double* A)
{
    __m256d vec1, vec2;
    int vec_size = 4;
    int i, j, k;
    
    for (k = 0; k < n; k += vec_size)
    {
        for (i = k + 1; i < n; i++)
        {
            vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
            vec1 = _mm256_div_pd(vec1, _mm256_loadu_pd(&A[k*(n+1)+k]));
            for (j = k + 1; j < n; j++)
            {
                vec2 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                vec2 = _mm256_sub_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&A[k*(n+1)+j]));
                _mm256_storeu_pd(&A[i*(n+1)+j], vec2);
            }
            _mm256_storeu_pd(&A[i*(n+1)+k], vec1);
        }
    }
    
    // Handling any remaining elements
    for ( ; k < n; k++)
    {
        for (i = k + 1; i < n; i++)
        {
            A[i*(n+1)+k] /= A[k*(n+1)+k];
            for (j = k + 1; j < n; j++)
                A[i*(n+1)+j] -= A[i*(n+1)+k] * A[k*(n+1)+j];
        }
    }
}
