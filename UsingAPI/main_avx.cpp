#include <iostream>
#include <vector>
#include <immintrin.h>

void scalar_product_array_opt(const std::vector<float>& a, const std::vector<float>& b, float& c) {
    size_t size = a.size();
    size_t size_rounded = size / 8 * 8;
    
    // Initialize accumulators
    __m256 sum = _mm256_setzero_ps();
    __m256 temp;
    
    // Compute scalar product using AVX intrinsics
    for (size_t i = 0; i < size_rounded; i += 8) {
        __m256 vec_a = _mm256_loadu_ps(&a[i]);
        __m256 vec_b = _mm256_loadu_ps(&b[i]);
        
        temp = _mm256_mul_ps(vec_a, vec_b);
        sum = _mm256_add_ps(sum, temp);
    }
    
    // Horizontal sum of the accumulators
    __m256 hsum = _mm256_hadd_ps(sum, sum);
    __m128 low = _mm256_extractf128_ps(hsum, 0);
    __m128 high = _mm256_extractf128_ps(hsum, 1);
    __m128 res = _mm_add_ps(low, high);
    
    // Store the result
    float result;
    _mm_store_ss(&result, res);
    
    // Compute the remaining elements
    for (size_t i = size_rounded; i < size; i++) {
        result += a[i] * b[i];
    }
    
    c = result;
}

int main() {
    std::vector<float> a = {1.0, 2.0, 3.0};
    std::vector<float> b = {4.0, 5.0, 6.0};
    float c;
    
    scalar_product_array_opt(a, b, c);
    
    std::cout << c << std::endl;
    
    return 0;
}