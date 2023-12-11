vector<float> dotProduct_opt(const vector<float>& a, const vector<float>& b) {
    int n = a.size();
    vector<float> result(n / 8 * 8, 0.0f);

    int i = 0;
    for (; i < n / 8 * 8; i += 8) {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);

        __m256 dp = _mm256_dp_ps(va, vb, 0xFF);

        float* dp_result = (float*)&dp;

        result[i] = dp_result[0];
        result[i + 1] = dp_result[4];
        result[i + 2] = dp_result[8];
        result[i + 3] = dp_result[12];
        result[i + 4] = dp_result[1];
        result[i + 5] = dp_result[5];
        result[i + 6] = dp_result[9];
        result[i + 7] = dp_result[13];
    }

    for (; i < n; ++i) {
        result[i] = a[i] * b[i];
    }

    return result;
}

int main() {
    const int size = 1000000;
    vector<float> a(size, 1.0f);
    vector<float> b(size, 2.0f);

    vector<float> result = dotProduct_opt(a, b);

    for (int i = 0; i < size; ++i) {
        cout << result[i] << " ";
    }

    return 0;
}