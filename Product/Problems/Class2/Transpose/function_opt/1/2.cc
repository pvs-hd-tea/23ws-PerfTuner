
#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
    __m256d vec1, vec2;  // declare variables only once outside the loop
    int vec_size = 4;     // number of elements in AVX register, 4 for double

    for (int i = 0; i <= n - vec_size; i += vec_size)   // AVX optimized loop
    {
        for (int j = 0; j < n; j += M) 
        {
            for (int ii = i; ii < i + M; ii++)
            {
                vec1 = _mm256_loadu_pd(&A[ii * n + j]);
                vec2 = _mm256_loadu_pd(&B[j * n + ii]);
                vec2 = _mm256_permute4x64_pd(vec2, 0b01001110);  // AVX intrinsic for transpose
                _mm256_storeu_pd(&B[j * n + ii], vec1);
                _mm256_storeu_pd(&A[ii * n + j], vec2);
            }
        }
    }

    for (int i = n - (n % vec_size); i < n; i += 1)    // remaining elements handling
    {
        for (int j = 0; j < n; j += M) 
        {
            for (int ii = i; ii < i + 1; ii++)
            {
                B[j * n + ii] = A[ii * n + j];
            }
        }
    }
}
