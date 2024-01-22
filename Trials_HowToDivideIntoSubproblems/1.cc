#include <iostream>
#include <immintrin.h>
#include <vector>

/* 1 */
const int n = 4;
const int B = 2;
double A[n][n+1];

void f1()
{
  for (int k = 0; k < n; k += 1)             
    for (int i = k + 1; i < n; i += 1)       
    {
      A[i][k] /= A[k][k];                    
      for (int j = k + 1; j < n; j += 1)      
        A[i][j] -= A[i][k] * A[k][j];        
    }
}

void initialize() 
{
  A[0][0] = 1;
  A[0][1] = -3;
  A[0][2] = 3;
  A[0][3] = -3;
  A[1][0] = 5;
  A[1][1] = -3;
  A[1][2] = 1;
  A[1][3] = 5;
  A[2][0] = 2;
  A[2][1] = -2;
  A[2][2] = 2;
  A[2][3] = 2;
  A[3][0] = 0;
  A[3][1] = 0;
  A[3][2] = 0;
  A[3][3] = 0;
}

void print()
{
  for(int i = 0; i < n-1; i++)
  {
    for(int j = 0; j < n; j++)
    {
      std::cout << A[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

/* 2 */
void f3(const std::vector<float> a, const std::vector<float> b, float &c)
{
    c = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        c += a[i] * b[i];
    }
}

void f2(const std::vector<float> a, const std::vector<float> b, float &c)
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
        // c += vec_c[0] + vec_c[4];
        ;
    }

    // final elements
    __m256i mask = _mm256_setr_epi32(last[0], last[1], last[2], last[3], last[4], last[5], last[6], last[7]);
    vec_a = _mm256_maskload_ps(&a[max * 8], mask);
    vec_b = _mm256_maskload_ps(&b[max * 8], mask);
    vec_c = _mm256_dp_ps(vec_a, vec_b, 0xFF);
    // c += vec_c[0] + vec_c[4];
    ;
}




int main()
{  
/* 1 */ 
  initialize();
  print(); 
  std::cout << std::endl;
  f1();
  print(); 
  std::cout << std::endl;

/* 2 */
float c;
std::vector<float> a = {1.,1.,1.};
std::vector<float> b = {1.,1.,1.};
f2(a,b,c);
std::cout << c;

  return 0;
}
