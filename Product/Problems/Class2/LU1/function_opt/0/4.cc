
#include <immintrin.h>

void function(int n, double* A)
{
    int vec_size = 4;  // number of elements in AVX register
    __m256d vec1, vec2;
    
    for (int k = 0; k < n; k += vec_size)
    {
        for (int i = k + 1; i < n; i += 1)  // Process the matrix in columns of 4 for AVX
        {
            vec1 = _mm256_loadu_pd(&A[k*(n + 1) + k]);
            vec1 = _mm256_div_pd(_mm256_loadu_pd(&A[i*(n + 1) + k]), vec1);
            for (int j = k + 1; j < n; j += vec_size) 
            {
                vec2 = _mm256_loadu_pd(&A[i*(n + 1) + k]);
                vec2 = _mm256_fmadd_pd(vec1, _mm256_loadu_pd(&A[k*(n + 1) + j]), vec2);
                _mm256_storeu_pd(&A[i*(n + 1) + j], vec2);
            }
        }
    }
}
