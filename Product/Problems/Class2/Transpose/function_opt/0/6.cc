
#include <immintrin.h>
void function(int n, int M, double *A, double *B)
{
    int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
    __m256d vec1, vec2; // define variables only once outside the loop
    int i, ii, j, jj;
    double tmp;

    for (i = 0; i < n; i += M)   // split i loop in blocks of size M
    {
        for (j = 0; j < n; j += M) // split j loop in blocks of size M
        {
            vec1 = _mm256_loadu_pd(&A[j * n + i]); // load elements from A into vec1
            for (ii = i; ii < i + M; ii += vec_size) // process i block in AVX registers
            {
                vec2 = _mm256_loadu_pd(&A[ii * n + j]); // load elements from A into vec2
                vec2 = _mm256_permute4x64_pd(vec2, 0b11011000); // transpose vec2
                _mm256_storeu_pd(&B[jj * n + ii], vec2); // store transposed elements to B
            }
        }
    }
}
