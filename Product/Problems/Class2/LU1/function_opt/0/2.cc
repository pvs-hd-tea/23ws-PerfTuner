
#include <immintrin.h>
void function(int n, double* A)
{
    __m256d vec1, vec2;
    int vec_size = 4;
    for (int k = 0; k < n; k += vec_size)  // optimization using AVX
    {
        for (int i = k + 1; i < n; i += 1)
        {
            // Divide A[i*(n+1)+k] by A[k*(n+1)+k]
            vec1 = _mm256_loadu_pd(&A[k*(n+1) + k]);
            vec2 = _mm256_set1_pd(1.0 / A[k*(n+1) + k]);
            vec2 = _mm256_div_pd(vec1, vec2);
            _mm256_storeu_pd(&A[i*(n+1) + k], vec2);
    
            for (int j = k + 1; j < n; j += 1)
            {
                // Subtract A[i*(n+1)+k] * A[k*(n+1)+j] from A[i*(n+1)+j]
                vec1 = _mm256_loadu_pd(&A[i*(n+1) + k]);
                vec2 = _mm256_loadu_pd(&A[k*(n+1) + j]);
                vec2 = _mm256_mul_pd(vec1, vec2);
                vec1 = _mm256_loadu_pd(&A[i*(n+1) + j]);
                vec1 = _mm256_sub_pd(vec1, vec2);
                _mm256_storeu_pd(&A[i*(n+1) + j], vec1);
            }
        }
    }
}
