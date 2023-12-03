#include <iostream>
#include <vector>

int add(int a, int b);

void scalar_product_array(const std::vector<float>& a, const std::vector<float>& b, float& c);

int main() {
    std::vector<float> a = {1.0, 2.0, 3.0};
    std::vector<float> b = {4.0, 5.0, 6.0};
    float c;
    
    scalar_product_array(a, b, c);
    
    std::cout << c << std::endl;
    
    return 0;
}

void scalar_product_array(const std::vector<float>& a, const std::vector<float>& b, float& c) {
    c = 0;
    for (size_t i = 0; i < a.size(); i++) {
        c += a[i] * b[i];
    }
}