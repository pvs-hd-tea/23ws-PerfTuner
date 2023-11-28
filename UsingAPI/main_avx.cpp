#include <iostream>
#include <vector>
#include <algorithm>
#include <immintrin.h>

float scalar_product_array_opt(const std::vector<float>& a, const std::vector<float>& b)
{
    __m256 sum = _mm256_setzero_ps();
    size_t size = a.size();

    for (size_t i = 0; i < size; i += 8)
    {
        __m256 vec_a = _mm256_loadu_ps(&a[i]);
        __m256 vec_b = _mm256_loadu_ps(&b[i]);
        __m256 result = _mm256_mul_ps(vec_a, vec_b);
        sum = _mm256_add_ps(sum, result);
    }

    float arr[8];
    _mm256_storeu_ps(arr, sum);
    
    float c = 0;
    for (size_t i = 0; i < 8; i++)
    {
        c += arr[i];
    }

    for (size_t i = size - (size % 8); i < size; i++)
    {
        c += a[i] * b[i];
    }
    
    return c;
}

int main()
{
    std::vector<float> a = {1.5, 2.5, 3.5};
    std::vector<float> b = {2.0, 3.0, 4.0};

    float result = scalar_product_array_opt(a, b);
    std::cout << result;

    return 0;
}