
#include <immintrin.h>
void function(int n, double* A)
{
    int vec_size = 4;   // Number of elements processed in each AVX iteration
    __m256d vec1, vec2; // AVX variables for vectorized operations

    for (int k = 0; k < n; k += vec_size)
    {
        for (int i = k + 1; i < n; i++)
        {
            A[i*(n+1)+k] /= A[k*(n+1)+k];
            for (int j = k + 1; j < n; j++)
                A[i*(n+1)+j] -= A[i*(n+1)+k] * A[k*(n+1)+j];
        }
    }
}
