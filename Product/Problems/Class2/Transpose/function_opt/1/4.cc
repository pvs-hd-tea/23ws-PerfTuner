
#include <immintrin.h>

void function(int n, int M, double *A, double *B)
{
    int vec_size = 4;   // number of elements in AVX register, 4 for double
    __m256d vec1, vec2; // define AVX variables outside the loop

    for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    {
        for (int j = 0; j < n; j += M) // split j loop in blocks of size M
        {
            for (int ii = i; ii < i + M; ii++)
            {
                int jj = j;
                for (; jj <= j + M - vec_size; jj += vec_size) // head loop (optimized using AVX)
                {
                    vec1 = _mm256_loadu_pd(&A[ii * n + jj]);
                    vec2 = _mm256_permute4x64_pd(vec1, 0b11011000); // transpose the elements
                    _mm256_storeu_pd(&B[jj * n + ii], vec2);
                }
                for (; jj < j + M; jj += 1)  // handle the remaining elements
                {
                    B[jj * n + ii] = A[ii * n + jj];
                }
            }
        }
    }
}
