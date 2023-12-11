#include <iostream>
#include <immintrin.h>

int main()
{
    double A[10][10];

    for (int i = 0; i < 10; i += 1)
    {
        for (int j = 0; j < 10; j += 1)
        {
            A[i][j] = i+j;
        }
    }

    int lower = 0;
    int upper = 10;
    int c = 1;
    int k = 1;
    for (int i = lower; i < upper; i += 1)
      for (int j = lower; j < upper; j += 1)
        A[i][j] -= k * A[c][j];


    for (int i = 0; i < 10; i += 1)
    {
        for (int j = 0; j < 10; j += 1)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }


    for (int i = 0; i < 10; i += 1)
    {
        for (int j = 0; j < 10; j += 1)
        {
            A[i][j] = i+j;
        }
    }

    std::cout << std::endl;

    int vec_size = 4; // vec_sice = 4 since the AVX can hold 4 double values
    __m256d vec1,vec2,factor;
    for (int i = lower; i < upper; i += 1)
    {
      factor = _mm256_set1_pd(k);
      int j = lower;
      for (; j <= upper - vec_size; j += vec_size)
      {
        vec1 = _mm256_loadu_pd(&A[c][j]);
        vec2 = _mm256_loadu_pd(&A[i][j]);
        vec2 = _mm256_fnmadd_pd(factor,vec1,vec2);
        _mm256_storeu_pd(&A[i][j],vec2);
      }
      for (; j < upper; j += 1)
        A[i][j] -= k * A[c][j];
    }

    for (int i = 0; i < 10; i += 1)
    {
        for (int j = 0; j < 10; j += 1)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
