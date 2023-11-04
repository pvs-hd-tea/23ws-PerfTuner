#include <iostream>
#include <immintrin.h>
#include <chrono>

// function to multiply and add with simple arrays
void multiply_and_add_array(const float *a, const float *b, const float *c, float *d)
{
    for (int i = 0; i < 8; i++)
    {
        d[i] = a[i] * b[i];
        d[i] = d[i] + c[i];
    }
}

// function to multiply and add with AVX2
__m256 multiply_and_add_avx2(__m256 a, __m256 b, __m256 c)
{
    return _mm256_fmadd_ps(a,b,c);
}


int main(int n, char **m)
{

    std::cout << "This is a test programm to compare the multiplication and addition of 8 floats with arrays and vectors." << std::endl;
    
    // create and initilaze arrays and vectors with random numbers
    float a[8], b[8], c[8], d[8];
    __m256 vec_a, vec_b, vec_c, vec_d;
    
    std::srand(time(0));
    for (size_t i = 0; i < 8; i++)
    {
        a[i] = vec_a[i] = (float) (std::rand()) / (float) (std::rand());
        b[i] = vec_b[i] = (float) (std::rand()) / (float) (std::rand());
        c[i] = vec_c[i] = (float) (std::rand()) / (float) (std::rand());
    }

    // perform the operations 10000 times and compare the times needed
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        multiply_and_add_array(a, b, c, d);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 10000; i++) {
        vec_d = multiply_and_add_avx2(vec_a,vec_b,vec_c);
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // output:
    /*
    std::cout << "Array 1:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl
              << "Array 2:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl
              << "Array 3:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << c[i] << " ";
    }
    */
    std::cout << std::endl
              << "Result 1:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << d[i] << " ";
    }
    /*
    std::cout << std::endl << "Second version" << std::endl;

    std::cout << "Array 1:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << vec_a[i] << " ";
    }
    std::cout << std::endl
              << "Array 2:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << vec_b[i] << " ";
    }
    std::cout << std::endl
              << "Array 3:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << vec_c[i] << " ";
    }
    */
    std::cout << std::endl
              << "Result 2:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << vec_d[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Time for the first operation: "  << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for the second operation: "  << duration2.count() << " microseconds." << std::endl;


}
