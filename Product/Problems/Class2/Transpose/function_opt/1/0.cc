
#include <immintrin.h>

void function(int n, int M, double *A, double *B)
{
    int vec_size = 4;
    __m256d vec1, vec2;

    for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    {
        for (int j = 0; j < n; j += M) // split j loop in blocks of size M
        {
            for (int ii = i; ii < i + M; ii++)
            {
                int start_idx = j * n + ii;
                for (int jj = j; jj <= n - vec_size; jj += vec_size) // head loop (optimized using AVX)
                {
                    vec1 = _mm256_loadu_pd(&A[ii * n + jj]);
                    vec2 = _mm256_loadu_pd(&B[jj * n + ii]);
                    _mm256_storeu_pd(&B[jj * n + ii], vec1);
                    _mm256_storeu_pd(&B[jj * n + ii], _mm256_loadu_pd(&A[ii * n + jj]));
                }

                for (int jj = n - (n % vec_size); jj < n; jj++) // handle the remaining elements
                {
                    B[jj * n + ii] = A[ii * n + jj];
                }
            }
        }
    }
}
