
#include <immintrin.h>

void function(int n, int B, double* A)
{
    __m256d vec1, vec2;
    int vec_size = 4;

    for (int K = 0; K < n; K += B) // K-blocks
    {
        // upper left block
        for (int k = K; k < K + B; k += vec_size) // k index within block
        {
            for (int i = k + 1; i < K + B; i += vec_size) // head loop for i
            {
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                vec2 = _mm256_broadcast_sd(&A[k*(n+1)+k]);
                vec1 = _mm256_div_pd(vec1, vec2);
                
                for (int j = k + 1; j < K + B; j += vec_size) // head loop for j
                {
                    vec2 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                    vec2 = _mm256_fnmadd_pd(vec1, _mm256_loadu_pd(&A[k*(n+1)+j]), vec2);
                    _mm256_storeu_pd(&A[i*(n+1)+j], vec2);
                }
            }
        }

        // U blocks in first block row
        for (int J = K + B; J < n; J += B) // block loop in j-direction
        {
            for (int k = K; k < K + B; k += vec_size) // k index within block
            {
                for (int i = k + 1; i < K + B; i += vec_size) // head loop for i
                {
                    for (int j = J; j < J + B; j += vec_size) // j-loop within block
                    {
                        vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                        vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                        vec2 = _mm256_fnmadd_pd(vec1, vec2, _mm256_loadu_pd(&A[i*(n+1)+j]));
                        _mm256_storeu_pd(&A[i*(n+1)+j], vec2);
                    }
                }
            }
        }

        // L blocks in first block column
        for (int I = K + B; I < n; I += B) // block loop in i-direction
        {
            for (int k = K; k < K + B; k += vec_size) // k index within block
            {
                for (int i = I; i < I + B; i += vec_size) // i-loop within block
                {
                    vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                    vec2 = _mm256_broadcast_sd(&A[k*(n+1)+k]);
                    vec1 = _mm256_div_pd(vec1, vec2);
                    
                    for (int j = k + 1; j < K + B; j += vec_size) // head loop for j
                    {
                        vec2 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                        vec2 = _mm256_fnmadd_pd(vec1, _mm256_loadu_pd(&A[k*(n+1)+j]), vec2);
                        _mm256_storeu_pd(&A[i*(n+1)+j], vec2);
                    }
                }
            }
        }

        // the rest matrix; here is the bulk of the work
        for (int I = K + B; I < n; I += B) // block loop in i-direction
        {
            for (int J = K + B; J < n; J += B) // block loop in j-direction
            {
                for (int i = I; i < I + B; i += vec_size) // i-loop within block
                {
                    for (int j = J; j < J + B; j += vec_size) // j-loop within block
                    {
                        for (int k = K; k < K + B; k += vec_size) // k index within block
                        {
                            vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                            vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                            vec2 = _mm256_fnmadd_pd(vec1, vec2, _mm256_loadu_pd(&A[i*(n+1)+j]));
                            _mm256_storeu_pd(&A[i*(n+1)+j], vec2);
                        }
                    }
                }
            }
        }
    }
}
