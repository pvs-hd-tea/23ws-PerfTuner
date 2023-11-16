void scalar_product_array(const std::vector<float> a, const std::vector<float> b, float &c)
{
    size_t size = a.size();
    
    if (size % 8 == 0)
    {
        __m256 sum_avx = _mm256_setzero_ps();
        
        for (size_t i = 0; i < size; i += 8)
        {
            __m256 a_avx = _mm256_load_ps(&a[i]);
            __m256 b_avx = _mm256_load_ps(&b[i]);
            
            __m256 mul_avx = _mm256_mul_ps(a_avx, b_avx);
            
            sum_avx = _mm256_add_ps(sum_avx, mul_avx);
        }
        
        float temp[8] = {0};
        _mm256_storeu_ps(temp, sum_avx);
        
        c = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7];
    }
    else
    {
        c = 0;
        for (size_t i = 0; i < size; i++)
        {
            c += a[i] * b[i];
        }
    }
}