#include <iostream>
#include <vector>
#include <algorithm>

float scalar_product_array(const std::vector<float>& a, const std::vector<float>& b)
{
    float c = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        c += a[i] * b[i];
    }
    return c;
}

int main()
{
    std::vector<float> a = {1.5, 2.5, 3.5};
    std::vector<float> b = {2.0, 3.0, 4.0};

    float result = scalar_product_array(a, b);
    std::cout << result;

    return 0;
}