#include <iostream>
#include <immintrin.h>
#include <chrono>
#include <vector>
#include "example.cpp"
#include "exampleOpt.cpp"

int number_of_runs = 1000;


int main(int n, char **m)
{

    int length_of_vector = 1000;
    std::vector<float> vec_a;
    std::vector<float> vec_b;
    float c;

    std::srand(std::time(0));
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
        scalar_product_array_opt(vec_a, vec_b, c);
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Result of scalar product with AVX: " << c << "." << std::endl;

    std::cout << "Time for operation without AVX: " << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for operation with AVX: " << duration2.count() << " microseconds." << std::endl;

}

