
#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
    int vec_size = 4; // vec_size = 4 since the AVX can hold 4 double values
    __m256d vec1, vec2, factor; // define variables only once outside the loop.

    for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    {
        for (int j = 0; j < n; j += M) // split j loop in blocks of size M
        {
            factor = _mm256_set1_pd(A[n + i]); // load constant only once outside the loop
            for (int ii = i; ii < i + M; ii++)
            {
                int iii = ii * n;
                int mii = (ii - i) * n;
                for (int jj = j; jj < j + M; jj += vec_size) // optimize loop with AVX
                {
                    vec1 = _mm256_loadu_pd(&A[ii + jj]);
                    vec2 = _mm256_loadu_pd(&A[jj * n + ii]);
                    vec2 = _mm256_fnmadd_pd(factor, vec1, vec2);
                    _mm256_storeu_pd(&B[jj * n + ii], vec2);
                }
                for (int jj = j + M; jj < j; jj++)  // remaining elements
                {
                    B[jj * n + ii] = A[ii * n + jj];
                    for (int k = 0; k < vec_size; k++)
                    {
                        A[mii + jj] -= A[ii * n + jj] * A[n + ii + k];
                    }
                }
            }
        }
    }
}
