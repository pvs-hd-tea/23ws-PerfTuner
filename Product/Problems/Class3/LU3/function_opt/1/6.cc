
#include <immintrin.h>
void function(int n, int B, double* A)
{
    __m256d vec1, vec2;
    int vec_size = 4;
    int i, j, k;

    // upper left block
    for (k = 0; k < n; k += vec_size) // k index within block
    {
        for (i = k + 1; i < n; i += vec_size) // head loop for i
        {
            vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
            vec2 = _mm256_loadu_pd(&A[k*(n+1)+k]);
            vec1 = _mm256_div_pd(vec1, vec2);
            _mm256_storeu_pd(&A[i*(n+1)+k], vec1);

            for (j = k + 1; j < n; j += vec_size) // head loop for j
            {
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                vec2 = _mm256_mul_pd(vec1, vec2);
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                vec1 = _mm256_sub_pd(vec1, vec2);
                _mm256_storeu_pd(&A[i*(n+1)+j], vec1);
            }
        }
    }

    // U blocks in first block row
    for (int J = B; J < n; J += B)         
    {
        for (k = 0; k < n; k += vec_size)       
        {
            for (i = k + 1; i < n; i += vec_size)
            {
                for (j = J; j < J + B; j += vec_size)
                {
                    vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                    vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                    vec2 = _mm256_mul_pd(vec1, vec2);
                    vec1 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                    vec1 = _mm256_sub_pd(vec1, vec2);
                    _mm256_storeu_pd(&A[i*(n+1)+j], vec1);
                }
            }
        }
    }

    // L blocks in first block column
    for (int I = B; I < n; I += B)     
    {
        for (k = 0; k < n; k += vec_size)   
        {
            for (i = I; i < I + B; i += vec_size) 
            {
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                vec2 = _mm256_loadu_pd(&A[k*(n+1)+k]);
                vec1 = _mm256_div_pd(vec1, vec2);
                _mm256_storeu_pd(&A[i*(n+1)+k], vec1);

                for (j = k + 1; j < n; j += vec_size)
                {
                    vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                    vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                    vec2 = _mm256_mul_pd(vec1, vec2);
                    vec1 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                    vec1 = _mm256_sub_pd(vec1, vec2);
                    _mm256_storeu_pd(&A[i*(n+1)+j], vec1);
                }
            }
        }
    }

    // the rest matrix; here is the bulk of the work
    for (int I = B; I < n; I += B)
    {
        for (int J = B; J < n; J += B)
        {
            for (i = I; i < I + B; i += vec_size)
            {
                for (j = J; j < J + B; j += vec_size)
                {
                    for (k = 0; k < n; k += vec_size)
                    {
                        vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
                        vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                        vec2 = _mm256_mul_pd(vec1, vec2);
                        vec1 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                        vec1 = _mm256_sub_pd(vec1, vec2);
                        _mm256_storeu_pd(&A[i*(n+1)+j], vec1);
                    }
                }
            }
        }
    }
}
