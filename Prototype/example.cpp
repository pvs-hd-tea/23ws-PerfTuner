#include <iostream>
#include <immintrin.h>
#include <cmath>
#include <random>

void scalar_product_array(const std::vector<float> a, const std::vector<float> b, float &c)
{
    c = 0;
    for (size_t i = 0; i < a.size(); i++)
    {
        c += a[i] * b[i];
    }
}

int add(int a, int b) {
    return a + b;
}

const int n = 48;  // n should be a multiple of B
const int B = 12;  // B should be a multiple of 4
double A[n][n+1];  // matrix to be solved
double x[n];       // final results
int randNum[n][n]; // random numbers for initializing the matrix

void display()  // show the final result
{
  std::cout << std::endl << "Solution: " << std::endl;
  for(int i = 0; i<n-1; i++)
  {
    std::cout << "x[" << i << "] = " << x[i] << std::endl;
  }
}

// Version 1: standard kij version
void lu_1()
{
  for (int k = 0; k < n; k += 1)             // row k is (normalized) subtracted from row i
    for (int i = k + 1; i < n; i += 1)       // columns are denoted by j
    {
      A[i][k] /= A[k][k];                    // (normalization) factor
      for (int j = k + 1; j < n; j += 1)     // only elements with j>k are needed later on
        A[i][j] -= A[i][k] * A[k][j];        // actual calculation
    }
}

void setRand()  // set a random array for initialization
{
  std::srand(time(0));
  for (int i = 0; i < n;i++)
  {
    for (int j = 0; j < n; j++)
    {
      randNum[i][j] = std::rand() % 2;
    }
  }
}


void initialize()   // set matrix
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n-1; j++)
    {
      if (i==j) A[i][i] = 1;
      else A[i][j] = 0;
    }
  }

  // set x_i = i
  for (int i = 0; i < n; i++)
  {
    A[i][n-1] = i;
  }

  // mix up the matrix
  for (int k = 0; k < n-1; k++)
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        if (k!=i) A[i][j] += randNum[k][i]*A[k][j];
      }
    }
  }
}

void print()  // print the status of the matrix
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


int main()
{
  setRand();
  initialize();
  lu_1();

  return 0;
}


