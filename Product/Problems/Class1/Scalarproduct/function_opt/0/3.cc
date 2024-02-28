
#include <immintrin.h>
void function(int n, double *a, double *b, double &c)
{
    c = 0;
    __m256d sum = _mm256_setzero_pd(); // Initialize the sum variable using AVX intrinsic

    int vec_size = 4; // Number of elements processed in each AVX operation
    int i = 0;

    for (; i <= n - vec_size; i += vec_size) // Loop for AVX optimization
    {
        __m256d vec1 = _mm256_loadu_pd(&a[i]); // Load a vector of 'a' values using AVX
        __m256d vec2 = _mm256_loadu_pd(&b[i]); // Load a vector of 'b' values using AVX
        vec1 = _mm256_mul_pd(vec1, vec2);      // Perform element-wise multiplication using AVX
        sum = _mm256_add_pd(sum, vec1);        // Accumulate the results in the sum variable
    }

    double temp[4]; // Temporary array to store the sum of 4 elements
    _mm256_storeu_pd(temp, sum); // Store the sum from AVX register to an array

    for (int j = 0; j < vec_size; j++) // Sum the elements in the temporary array
    {
        c += temp[j];
    }

    for (; i < n; i++)  // Handle the remaining elements
    {
        c += a[i] * b[i];
    }
}
