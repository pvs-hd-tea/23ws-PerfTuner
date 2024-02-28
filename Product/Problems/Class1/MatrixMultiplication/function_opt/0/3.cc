
void function(int row1, int col1, int col2, double *a, double *b, double *c)
{
    for (int k = 0; k < row1; k++)
    {
        for (int j = 0; j < col2; j++)
        {
            c[k * row1 + j] = 0;
            __m256d result = _mm256_setzero_pd();  // Initialize result vector with zeros
            for (int i = 0; i < col1; i += 4)
            {
                __m256d vec1 = _mm256_loadu_pd(&a[k * col1 + i]);  // Load 4 values from array a
                __m256d vec2 = _mm256_loadu_pd(&b[i * col2 + j]);  // Load 4 values from array b
                result = _mm256_add_pd(result, _mm256_mul_pd(vec1, vec2));  // Vectorized addition and multiplication
            }
            double temp[4];
            _mm256_storeu_pd(temp, result);  // Store the result vector into temp array
            c[k * row1 + j] = temp[0] + temp[1] + temp[2] + temp[3];  // Sum the values in temp array
        }
    }
}
