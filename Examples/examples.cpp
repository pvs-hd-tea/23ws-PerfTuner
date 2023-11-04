#include <iostream>
#include <immintrin.h>
#include <chrono>


int size_of_vector = 8;
int number_of_runs = 10000;

// function to multiply and add with simple arrays
void multiply_and_add_array(const float *a, const float *b, const float *c, float *d)
{
    for (int i = 0; i < size_of_vector; i++)
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


// function to perform bitwise logical XOR operation on signed integer arrays
void xor_array(const int32_t* a, const int32_t* b, int32_t* c)
{
    for (int i = 0; i < size_of_vector; i++)
    {
        c[i] = a[i] ^ b[i];
    }
}

// function to perform bitwise logical XOR operation on signed integer AVX vectors
__m256i xor_vector(const __m256i a, const __m256i b)
{
    return _mm256_xor_si256(a,b);
}


int main(int n, char **m)
{

    // test multiply and add:
    
    // create and initilaze arrays and vectors with random numbers
    
    alignas(32) float af[size_of_vector], bf[size_of_vector], cf[size_of_vector], df[size_of_vector];
    __m256 vec_af, vec_bf, vec_cf, vec_df;
    
    std::srand(time(0));
    for (size_t i = 0; i < size_of_vector; i++)
    {
        af[i] = (float) (std::rand()) / (float) (std::rand());
        bf[i] = (float) (std::rand()) / (float) (std::rand());
        cf[i] = (float) (std::rand()) / (float) (std::rand());
    }
    vec_af = _mm256_load_ps(af);
    vec_bf = _mm256_load_ps(bf);
    vec_cf = _mm256_load_ps(cf);

    // perform the operations number_of_runs times and compare the times needed
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++) {
        multiply_and_add_array(af, bf, cf, df);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++) {
        vec_df = multiply_and_add_avx2(vec_af,vec_bf,vec_cf);
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // output:
    std::cout << "Test of multiply and add:" << std::endl;
    
    std::cout << std::endl
              << "Result of calculation without vectors:" << std::endl;
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << df[i] << " ";
    }
    std::cout << std::endl
              << "Result of calculation with vectors:" << std::endl;
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << vec_df[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Time for the first operation: "  << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for the second operation: "  << duration2.count() << " microseconds." << std::endl;


    // test XOR 

    int32_t ai[size_of_vector], bi[size_of_vector], ci[size_of_vector];
    __m256i vec_ai, vec_bi, vec_ci;

    
    for (size_t i = 0; i < size_of_vector; i++)
    {
        ai[i] = rand();
        bi[i] = rand();
    }

    vec_ai = _mm256_set_epi32(ai[7],ai[6],ai[5],ai[4],ai[3],ai[2],ai[1],ai[0]);
    vec_bi = _mm256_set_epi32(bi[7],bi[6],bi[5],bi[4],bi[3],bi[2],bi[1],bi[0]);
    

     // perform the operations number_of_runs times and compare the times needed
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++) {
        xor_array(ai, bi, ci);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++) {
        vec_ci = xor_vector(vec_ai,vec_bi);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // output:
    std::cout << std::endl << std::endl << "Test of xor:" << std::endl;
    
    std::cout << std::endl
              << "Result of calculation without vectors:" << std::endl;
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << ci[i] << " ";
    }
    std::cout << std::endl
              << "Result of calculation with vectors:" << std::endl;
    
    
    _mm256_store_si256((__m256i *)ci, vec_ci); //write result to int array
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << ci[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Time for the first operation: "  << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for the second operation: "  << duration2.count() << " microseconds." << std::endl;

}
