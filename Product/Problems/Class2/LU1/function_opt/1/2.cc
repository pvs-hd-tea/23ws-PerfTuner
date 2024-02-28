
#include <immintrin.h>
void function(int n, double* A)
{
    int m = 4;  // number of elements processed simultaneously using AVX
    __m256d vec1, vec2;

    for (int k = 0; k < n; k += m)
    {
        for (int i = k + 1; i < n; i += 1)
        {
            A[i*(n+1)+k] /= A[k*(n+1)+k];
            int j = k + 1;
            for (; j <= n - m; j += m)
            {
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                vec2 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                vec2 = _mm256_mul_pd(vec1, vec2);
                _mm256_storeu_pd(&A[i*(n+1)+j], _mm256_sub_pd(vec1, vec2));
            }
            for (; j < n; j += 1)
            {
                A[i*(n+1)+j] -= A[i*(n+1)+k] * A[k*(n+1)+j];
            }
        }
    }
}
