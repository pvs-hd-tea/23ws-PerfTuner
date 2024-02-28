
#include <immintrin.h>
void function(int n, double* A)
{
    int vec_size = 4;
    __m256d vec1, vec2;

    for (int k = 0; k < n; k += 1)
    {
        for (int i = k + 1; i < n; i += 1)
        {
            A[i*(n+1)+k] /= A[k*(n+1)+k];
            int j = k + 1;
            for (; j <= n - vec_size; j += vec_size)
            {
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                vec1 = _mm256_div_pd(vec1, _mm256_broadcast_sd(&A[k*(n+1)+k]));
                vec2 = _mm256_sub_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&A[i*(n+1)+j]));
                _mm256_storeu_pd(&A[i*(n+1)+j], vec2);
            }
            for (; j < n; j++)
            {
                A[i*(n+1)+j] -= A[i*(n+1)+k] * A[k*(n+1)+j];
            }
        }
    }
}
