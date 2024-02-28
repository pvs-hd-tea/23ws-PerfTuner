
#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
    __m256d vec1, vec2;
    int vec_size = 4;
    for (int i = 0; i < n; i += vec_size)
    {
        for (int j = 0; j < n; j += vec_size)
        {
            for (int ii = i; ii < i + vec_size; ii++)
            {
                for (int jj = j; jj < j + vec_size; jj++)
                {
                    vec1 = _mm256_loadu_pd(&A[ii * n + jj]);
                    vec2 = _mm256_loadu_pd(&B[jj * n + ii]);
                    _mm256_storeu_pd(&B[jj * n + ii], vec1);
                    _mm256_storeu_pd(&A[ii * n + jj], vec2);
                }
            }
        }
    }
}
