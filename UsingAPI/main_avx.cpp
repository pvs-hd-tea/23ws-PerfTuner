#include <iostream>
#include <vector>
#include <immintrin.h>

void scalar_product_array_opt(const std::vector<float>& a, const std::vector<float>& b, float& c) {
    c = 0;
    __m256 sum = _mm256_setzero_ps();
    for (size_t i = 0; i < a.size(); i+=8) {
        __m256 a_vec = _mm256_loadu_ps(&a[i]);
        __m256 b_vec = _mm256_loadu_ps(&b[i]);
        __m256 mul = _mm256_mul_ps(a_vec, b_vec);
        sum = _mm256_add_ps(sum, mul);
    }
    float temp[8];
    _mm256_storeu_ps(temp, sum);
    for (int i = 0; i < 8; i++) {
        c += temp[i];
    }
}

int main() {
    std::vector<float> a {1.0, 2.0, 3.0};
    std::vector<float> b {4.0, 5.0, 6.0};
    float result = 0;

    scalar_product_array_opt(a, b, result);

    std::cout << "Result: " << result << std::endl;

    return 0;
}