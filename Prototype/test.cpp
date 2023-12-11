#include <iostream>
#include <immintrin.h>
#include <vector>

void scalar_product_array(const std::vector<float>& a, const std::vector<float>& b, float& c)
{
    c = 0;
    size_t i = 0;
    __m256 sum = _mm256_setzero_ps();

    for (; i <= a.size() - 8; i += 8)
    {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        sum = _mm256_add_ps(sum, _mm256_mul_ps(va, vb));
    }

    float result[8];
    _mm256_storeu_ps(result, sum);
    for (int j = 0; j < 8; ++j)
    {
        c += result[j];
    }

    for (; i < a.size(); ++i)
    {
        c += a[i] * b[i];
    }
}

int main()
{
    std::vector<float> a = {1., 1., 1., 1., 1., 1., 1., 1.,1.};
    std::vector<float> b = {1., 1., 1., 1., 1., 1., 1., 1.,1.};
    float c;
    scalar_product_array(a,b,c);
    std::cout << c;

    return 0;
}
