#include <iostream>
#include <vector>
#include <immintrin.h>

void scalar_product_array_opt(const std::vector<float> a, const std::vector<float> b, float &c) {
    size_t size = a.size();
    
    float temp[8] = {0};
    __m256 sum = _mm256_setzero_ps();
    
    for (size_t i = 0; i < size; i += 8) {
        __m256 vec_a = _mm256_loadu_ps(&a[i]);
        __m256 vec_b = _mm256_loadu_ps(&b[i]);
        
        __m256 prod = _mm256_mul_ps(vec_a, vec_b);
        
        sum = _mm256_add_ps(sum, prod);
    }
    
    _mm256_storeu_ps(temp, sum);
    
    for (int i = 0; i < 8; i++) {
        c += temp[i];
    }
}

int main() {
    std::vector<float> a = {1.0, 2.0, 3.0};
    std::vector<float> b = {4.0, 5.0, 6.0};
    float c = 0.0;
    
    scalar_product_array_opt(a, b, c);
    
    std::cout << "Result: " << c << std::endl;
    
    return 0;
}