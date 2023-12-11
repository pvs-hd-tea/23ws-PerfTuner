#include <iostream>
#include <vector>

void scalar_product_array(const std::vector<float> a, const std::vector<float> b, float &c)
{
    c = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        c += a[i] * b[i];
    }
}

int main()
{
    std::vector<float> a = {1.0, 2.0, 3.0};
    std::vector<float> b = {4.0, 5.0, 6.0};
    float result;
    scalar_product_array(a, b, result);
    std::cout << "Result: " << result << std::endl;

    return 0;
}