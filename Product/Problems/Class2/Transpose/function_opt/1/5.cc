
#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
    int vec_size = 4;  // number of elements in AVX register, 4 for double
    __m256d vec1, vec2;  // define variables only once outside the loop

    for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    {
        for (int j = 0; j < n; j += M) // split j loop in blocks of size M
        {
            for (int ii = i; ii < i + M; ii += vec_size)  // AVX-optimized loop for ii iteration
            {
                for (int jj = j; jj < j + M; jj++)  // sequential loop for jj iteration (not AVX-optimized)
                {
                    vec1 = _mm256_loadu_pd(&A[ii * n + jj]);  // load elements from A using AVX
                    vec2 = _mm256_loadu_pd(&B[jj * n + ii]);  // load elements from B using AVX
                    vec2 = _mm256_add_pd(vec1, vec2);         // perform vectorized addition using AVX
                    _mm256_storeu_pd(&B[jj * n + ii], vec2);  // store the result back to B using AVX
                }
            }
        }
    }
}
