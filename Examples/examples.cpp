#include <iostream>
#include <immintrin.h>
#include <chrono>
#include <vector>

int number_of_runs = 1000;

// function to do scalar multiplication of two vectors without AVX
void scalar_product_array(const std::vector<float> a, const std::vector<float> b, float &c)
{
    c = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        c += a[i] * b[i];
    }
}

// function to do scalar multiplication of two vectors with AVX
void scalar_product_avx(const std::vector<float> a, const std::vector<float> b, float &c)
{
    // initialize runtime parameters
    c = 0;
    int max = a.size() / 8;                // number of runs
    int last[] = {0, 0, 0, 0, 0, 0, 0, 0}; // mask for the last elements
    for (size_t i = 0; i < a.size() % 8; i++)
    {
        last[i] = -1;
    }

    __m256 vec_a;
    __m256 vec_b;
    __m256 vec_c;

    // perform the calculation
    for (size_t i = 0; i < max * 8; i = i + 8)
    {
        vec_a = _mm256_loadu_ps(&a[i]);
        vec_b = _mm256_loadu_ps(&b[i]);
        vec_c = _mm256_dp_ps(vec_a, vec_b, 0xFF);
        c += vec_c[0] + vec_c[4];
        ;
    }

    // final elements
    __m256i mask = _mm256_setr_epi32(last[0], last[1], last[2], last[3], last[4], last[5], last[6], last[7]);
    vec_a = _mm256_maskload_ps(&a[max * 8], mask);
    vec_b = _mm256_maskload_ps(&b[max * 8], mask);
    vec_c = _mm256_dp_ps(vec_a, vec_b, 0xFF);
    c += vec_c[0] + vec_c[4];
    ;
}

// function to multiply two matricies without AVX
void matrix_mul_array(const std::vector<std::vector<float>> a, const std::vector<std::vector<float>> b,
                      std::vector<std::vector<float>> &c)
{

    for (int k = 0; k < a.size(); k++)
    {
        for (int j = 0; j < b[0].size(); j++)
        {
            c[k][j] = 0;
            for (int i = 0; i < b.size(); i++)
            {
                c[k][j] = c[k][j] + a[k][i] * b[i][j];
            }
        }
    }
}

// help function to transpose a matrix

std::vector<std::vector<float>> transpose(const std::vector<std::vector<float>> &b)
{
    std::vector<std::vector<float>> trans_vec(b[0].size(), std::vector<float>());
    for (int i = 0; i < b.size(); i++)
    {
        for (int j = 0; j < b[i].size(); j++)
        {
            trans_vec[j].push_back(b[i][j]);
        }
    }
    return trans_vec;
}

// function to multiply two matricies with AVX

void matrix_mul_avx(const std::vector<std::vector<float>> a, const std::vector<std::vector<float>> b,
                    std::vector<std::vector<float>> &c)
{
    __m256 vec_a;
    __m256 vec_b;
    __m256 vec_c;

    std::vector<std::vector<float>> b_new = transpose(b);

    int max = b.size() / 8;                // number of runs
    int last[] = {0, 0, 0, 0, 0, 0, 0, 0}; // mask for the last elements
    for (size_t i = 0; i < b.size() % 8; i++)
    {
        last[i] = -1;
    }
    __m256i mask = _mm256_setr_epi32(last[0], last[1], last[2], last[3], last[4], last[5], last[6], last[7]);

    for (int k = 0; k < a.size(); k++)
    {
        for (int j = 0; j < b[0].size(); j++)
        {
            c[k][j] = 0;
            for (size_t i = 0; i < max * 8; i = i + 8)
            {
                vec_a = _mm256_loadu_ps(&a[k][i]);
                vec_b = _mm256_loadu_ps(&b_new[j][i]);
                vec_c = _mm256_dp_ps(vec_a, vec_b, 0xFF);
                c[k][j] += vec_c[0] + vec_c[4];
            }
            // final elements
            vec_a = _mm256_maskload_ps(&a[k][max * 8], mask);
            vec_b = _mm256_maskload_ps(&b_new[j][max * 8], mask);
            vec_c = _mm256_dp_ps(vec_a, vec_b, 0xFF);
            c[k][j] += vec_c[0] + vec_c[4];
        }
    }
}

int main(int n, char **m)
{

    // test scalar product of two vectors

    int length_of_vector = 1000;
    std::vector<float> vec_a;
    std::vector<float> vec_b;
    float c;

    for (int i = 0; i < length_of_vector; i++)
    {
        vec_a.push_back((float)rand() / (float)rand());
        vec_b.push_back((float)rand() / (float)rand());
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++)
    {
        scalar_product_array(vec_a, vec_b, c);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << std::endl
              << std::endl
              << "Test of scalar product:" << std::endl
              << std::endl;
    std::cout << "Result of scalar product without AVX: " << c << "." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++)
    {
        scalar_product_avx(vec_a, vec_b, c);
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Result of scalar product with AVX: " << c << "." << std::endl;

    std::cout << "Time for operation without AVX: " << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for operation with AVX: " << duration2.count() << " microseconds." << std::endl;

    // test multiplication of matricies

    int a_rows = 9;
    int a_columns = 7;
    int b_rows = a_columns;
    int b_columns = 10;
    int c_rows = a_rows;
    int c_columns = b_columns;

    std::vector<std::vector<float>> a(a_rows, std::vector<float>(a_columns));
    std::vector<std::vector<float>> b(b_rows, std::vector<float>(b_columns));
    std::vector<std::vector<float>> c_array(a_rows, std::vector<float>(b_columns));
    std::vector<std::vector<float>> c_avx(a_rows, std::vector<float>(b_columns));

    for (int i = 0; i < a_rows; i++)
    {
        for (int j = 0; j < a_columns; j++)
        {
            a[i][j] = (float)rand() / (float)rand();
        }
    }
    for (int i = 0; i < b_rows; i++)
    {
        for (int j = 0; j < b_columns; j++)
        {
            b[i][j] = (float)rand() / (float)rand();
        }
    }

    // perform the operations number_of_runs times and compare the times needed
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++)
    {
        matrix_mul_array(a, b, c_array);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++)
    {
        matrix_mul_avx(a, b, c_avx);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // output:
    std::cout << std::endl
              << std::endl
              << "Test of matrix multiplication:" << std::endl;

    std::cout << std::endl
              << "Result of calculation without AVX:" << std::endl;
    for (int i = 0; i < c_rows; i++)
    {
        for (int j = 0; j < c_columns; j++)
        {
            std::cout << c_array[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl
              << std::endl
              << "Result of calculation with AVX:" << std::endl;
    for (int i = 0; i < c_rows; i++)
    {
        for (int j = 0; j < c_columns; j++)
        {
            std::cout << c_avx[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Time for operation without AVX: " << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for operation with AVX: " << duration2.count() << " microseconds." << std::endl;
}
