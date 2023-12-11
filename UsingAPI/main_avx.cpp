#include <iostream>
#include <vector>
#include <immintrin.h>

void scalar_product_array_opt(const std::vector<float> a, const std::vector<float> b, float &c)
{
    c = 0;
    size_t size = a.size();

    __m256 acc = _mm256_setzero_ps();

    for (size_t i = 0; i < size; i += 8)
    {
        __m256 vec_a = _mm256_loadu_ps(&a[i]);
        __m256 vec_b = _mm256_loadu_ps(&b[i]);
        __m256 mul = _mm256_mul_ps(vec_a, vec_b);
        acc = _mm256_add_ps(acc, mul);
    }

    float temp[8];
    _mm256_storeu_ps(temp, acc);

    for (int i = 0; i < 8; i++)
    {
        c += temp[i];
    }
}

int main()
{
    std::vector<float> a = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    std::vector<float> b = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    float result;
    scalar_product_array_opt(a, b, result);
    std::cout << "Result: " << result << std::endl;

    return 0;
}