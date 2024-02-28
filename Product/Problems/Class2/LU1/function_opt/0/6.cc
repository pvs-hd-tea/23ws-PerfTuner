
#include <immintrin.h>
void function(int n, double* A)
{
    __m256d vec1, vec2; // define variables only once outside the loop
    int vec_size = 4; // number of elements in AVX register, 4 for double
    for (int k = 0; k < n; k += vec_size) // optimization for AVX vectorization
    {
        for (int i = k + 1; i < n; i += 1)
        {
            vec1 = _mm256_broadcast_sd(&A[k*(n+1)+k]); // broadcast A[k*(n+1)+k] to all elements of vec1
            vec2 = _mm256_loadu_pd(&A[i*(n+1)+k]); // load values from A[i*(n+1)+k] to vec2
            vec2 = _mm256_div_pd(vec2, vec1); // divide vec2 by vec1
            for (int j = k + 1; j < n; j += 1)
            {
                __m256d sub_vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]); // load values from A[i*(n+1)+k] to sub_vec1
                __m256d mul_vec1 = _mm256_loadu_pd(&A[k*(n+1)+j]); // load values from A[k*(n+1)+j] to mul_vec1
                mul_vec1 = _mm256_mul_pd(vec2, mul_vec1); // multiply vec2 by mul_vec1
                sub_vec1 = _mm256_sub_pd(sub_vec1, mul_vec1); // subtract mul_vec1 from sub_vec1
                _mm256_storeu_pd(&A[i*(n+1)+j], sub_vec1); // store the result back into A[i*(n+1)+j]
            }
        }
    }
}
