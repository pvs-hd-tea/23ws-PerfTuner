
#include <immintrin.h>

void function(int n, double* A)
{
    __m256d vec1, vec2, vec3;
    int vec_size = 4;

    for (int k = 0; k < n; k += vec_size) {
        for (int i = k + 1; i < n; i += vec_size) {
            vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]);
            vec2 = _mm256_loadu_pd(&A[k*(n+1)+k]);
            vec1 = _mm256_div_pd(vec1, vec2);
            _mm256_storeu_pd(&A[i*(n+1)+k], vec1);

            for (int j = k + 1; j < n; j += vec_size) {
                vec1 = _mm256_loadu_pd(&A[i*(n+1)+k]); // a copy of vec1 for multiplication
                vec2 = _mm256_loadu_pd(&A[k*(n+1)+j]);
                vec3 = _mm256_mul_pd(vec1, vec2);

                vec2 = _mm256_loadu_pd(&A[i*(n+1)+j]);
                vec2 = _mm256_sub_pd(vec2, vec3);
                _mm256_storeu_pd(&A[i*(n+1)+j], vec2); 
            }
        }
    }
}
