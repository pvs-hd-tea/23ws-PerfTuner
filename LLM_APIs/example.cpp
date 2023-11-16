void scalar_product_array(const std::vector<float> a, const std::vector<float> b, float &c)
{
    c = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        c += a[i] * b[i];
    }
}
