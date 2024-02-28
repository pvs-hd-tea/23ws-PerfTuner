
#include <immintrin.h>
void function(int n, double* A)
{
    __m256d vec1, vec2;
    const int vec_size = 4;
    
    for (int k = 0; k < n; k++)
    {
        for (int i = k + 1; i < n; i++)
        {
            A[i*(n+1)+k] /= A[k*(n+1)+k];
            
            int j = k + 1;
            for (; j <= n - vec_size; j += vec_size)
            {
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                vec2 = _mm256_mul_pd(vec1, _mm256_broadcast_sd(&A[i*(n+1)+k]));
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                vec1 = _mm256_sub_pd(vec1, vec2);
                _mm256_storeu_pd(&A[i*(n+1)+j], vec1);
            }
            
            for (; j < n; j += 1)
            {
                A[i*(n+1)+j] -= A[i*(n+1)+k] * A[k*(n+1)+j];
            }
        }
    }
}
