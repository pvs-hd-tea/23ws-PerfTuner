
#include <immintrin.h>
void function(int n, int B, double* A)
{
    __m256d vec1, vec2;     // Declare variables outside the loop
    int vec_size = 4;       // Number of elements in AVX register (4 for double)
    
    for (int K = 0; K < n; K += B) // K-blocks
    {
        // Upper left block
        for (int k = K; k < K + B; k += 1) // k index within block
        {
            for (int i = k + 1; i < K + B; i += 1) // Head loop for i
            {
                vec1 = _mm256_set1_pd(A[k*(n+1)+k]);  // Load value into AVX register
                for (int j = k + 1; j < K + B; j += vec_size) // Head loop for j (optimized using AVX)
                {
                    vec1 = _mm256_set1_pd(A[k*(n+1)+j]); // Load value into AVX register
                    for (int idx = i; idx < i + vec_size; idx++) // Load values for computation
                    {
                        vec2 = _mm256_loadu_pd(&A[idx*(n+1)+j]);
                        vec2 = _mm256_sub_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&A[idx*(n+1)+k])));
                        _mm256_storeu_pd(&A[idx*(n+1)+j], vec2); // Store back the result
                    }
                }
            }
        }
        
        // U blocks in first block row (Optimized using AVX)
        for (int J = K + B; J < n; J += B) // Block loop in j-direction
        {
            for (int k = K; k < K + B; k += 1)   // k index within block
            {
                for (int i = k + 1; i < K + B; i += 1) // Head loop for i
                {
                    vec1 = _mm256_set1_pd(A[k*(n+1)+k]);  // Load value into AVX register
                    for (int j = J; j < J + B; j += vec_size) // Loop over j within block
                    {
                        vec1 = _mm256_set1_pd(A[k*(n+1)+j]); // Load value into AVX register
                        for (int idx = i; idx < i + vec_size; idx++) // Load values for computation
                        {
                            vec2 = _mm256_loadu_pd(&A[idx*(n+1)+j]);
                            vec2 = _mm256_sub_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&A[idx*(n+1)+k])));
                            _mm256_storeu_pd(&A[idx*(n+1)+j], vec2); // Store back the result
                        }
                    }
                }
            }
        }
        
        // L blocks in first block column (Optimized using AVX)
        for (int I = K + B; I < n; I += B)   // Block loop in i-direction
        {
            for (int k = K; k < K + B; k += 1) // k index within block
            {
                for (int i = I; i < I + B; i += 1) // i-loop within block
                {
                    vec1 = _mm256_set1_pd(A[k*(n+1)+k]);  // Load value into AVX register
                    for (int j = k + 1; j < K + B; j += vec_size) // Head loop for j (optimized using AVX)
                    {
                        vec1 = _mm256_set1_pd(A[k*(n+1)+j]); // Load value into AVX register
                        for (int idx = i; idx < i + vec_size; idx++) // Load values for computation
                        {
                            vec2 = _mm256_loadu_pd(&A[idx*(n+1)+j]);
                            vec2 = _mm256_sub_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&A[idx*(n+1)+k])));
                            _mm256_storeu_pd(&A[idx*(n+1)+j], vec2); // Store back the result
                        }
                    }
                }
            }
        }
        
        // The rest matrix; bulk of the work (Optimized using AVX)
        for (int I = K + B; I < n; I += B)   // Block loop in i-direction
        {
            for (int J = K + B; J < n; J += B)   // Block loop in j-direction
            {
                for (int i = I; i < I + B; i += 1) // i-loop within block
                {
                    for (int j = J; j < J + B; j += vec_size) // j-loop within block (optimized using AVX)
                    {
                        for (int k = K; k < K + B; k += 1) // k index within block
                        {
                            vec1 = _mm256_set1_pd(A[k*(n+1)+k]); // Load value into AVX register
                            vec2 = _mm256_loadu_pd(&A[i*(n+1)+j]); // Load value into AVX register
                            vec2 = _mm256_sub_pd(vec2, _mm256_mul_pd(vec1, _mm256_loadu_pd(&A[i*(n+1)+k]));
                            _mm256_storeu_pd(&A[i*(n+1)+j], vec2);  // Store back the result
                        }
                    }
                }
            }
        }
    }
}
