#include <iostream>
#include <immintrin.h>
#include <chrono>
#include <vector>


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

// function to multiply and add with AVX
__m256 multiply_and_add_avx(__m256 a, __m256 b, __m256 c)
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
__m256i xor_avx(const __m256i a, const __m256i b)
{
    return _mm256_xor_si256(a,b);
}


// function to multiply two matricies without AVX

void matrix_mul_array(const std::vector<std::vector<float>> a, const std::vector<std::vector<float>> b, 
std::vector<std::vector<float>>& c)
{
    for (int k=0; k < 8; k++)
    {
        for (int j=0; j < 8; j++)
        {
            c[k][j] = 0;
            for (int i=0; i < 8; i++)
            {
                c[k][j] = c[k][j] + a[k][i] * b[i][j]; 
            }
        }
    }
}

// function to multiply two matricies with AVX

void matrix_mul_avx(const std::vector<std::vector<float>> a, const std::vector<std::vector<float>> b, 
std::vector<std::vector<float>>& c)
{
    __m256 vec_a;
    __m256 vec_b;
    __m256 vec_c;
   
    for (int k=0; k < 8; k++)
    {
        for (int j=0; j < 8; j++)
        {
            vec_a = _mm256_set_ps(a[k][7],a[k][6],a[k][5],a[k][4],a[k][3],a[k][2],a[k][1],a[k][0]);
            vec_b = _mm256_set_ps(b[7][j],b[6][j],b[5][j],b[4][j],b[3][j],b[2][j],b[1][j],b[0][j]);
            vec_c = _mm256_dp_ps(vec_a, vec_b, 0xFF);
            c[k][j] = vec_c[0]+vec_c[4];
        }
    }
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
        vec_df = multiply_and_add_avx(vec_af,vec_bf,vec_cf);
    }
    stop = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // output:
    std::cout << "Test of multiply and add:" << std::endl;
    
    std::cout << std::endl
              << "Result of calculation without AVX:" << std::endl;
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << df[i] << " ";
    }
    std::cout << std::endl
              << "Result of calculation with AVX:" << std::endl;
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << vec_df[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Time for operation without AVX: "  << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for operation with AVX: "  << duration2.count() << " microseconds." << std::endl;


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
        vec_ci = xor_avx(vec_ai,vec_bi);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // output:
    std::cout << std::endl << std::endl << "Test of xor:" << std::endl;
    
    std::cout << std::endl
              << "Result of calculation without AVX:" << std::endl;
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << ci[i] << " ";
    }
    std::cout << std::endl
              << "Result of calculation with AVX:" << std::endl;
    
    
    _mm256_store_si256((__m256i *)ci, vec_ci); //write result to int array
    for (int i = 0; i < size_of_vector; i++)
    {
        std::cout << ci[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Time for operation without AVX: "  << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for operation with AVX: "  << duration2.count() << " microseconds." << std::endl;



    // test multiplication of matricies

    std::vector<std::vector<float>> a(8,std::vector<float>(8));
    std::vector<std::vector<float>> b(8,std::vector<float>(8));
    std::vector<std::vector<float>> c_array(8,std::vector<float>(8));
    std::vector<std::vector<float>> c_avx(8,std::vector<float>(8));

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            a[i][j] = (float) rand() / (float) rand();
            b[i][j] = (float) rand() / (float) rand();
        }
    }


    // perform the operations number_of_runs times and compare the times needed
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++) {
        matrix_mul_array(a,b,c_array);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < number_of_runs; i++) {
        matrix_mul_avx(a,b,c_avx);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);


    // output:
    std::cout << std::endl << std::endl << "Test of matrix multiplication:" << std::endl;
    
    std::cout << std::endl << "Result of calculation without AVX:" << std::endl;    
    for (int i=0;i < 8;i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::cout << c_array[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    
    std::cout << std::endl << "Result of calculation with AVX:" << std::endl;
    for (int i=0;i < 8;i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::cout << c_avx[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Time for operation without AVX: "  << duration1.count() << " microseconds." << std::endl;
    std::cout << "Time for operation with AVX: "  << duration2.count() << " microseconds." << std::endl;

}
