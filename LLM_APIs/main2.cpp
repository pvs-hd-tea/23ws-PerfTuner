#include <iostream>
#include <immintrin.h>
#include <chrono>
#include <vector>
#include "example2.cpp"
#include "exampleOpt.cpp"

int number_of_runs = 1000;


int main(int n, char **m)
{

    int length_of_vector = 1000;
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
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++)
    {
        matrix_mul_array(a, b, c_array);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++)
    {
        matrix_mul_array_opt(a, b, c_avx);
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

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

