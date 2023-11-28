#include <iostream>
#include <immintrin.h>
#include <cmath>
#include <random>


const int n = 48;  // n should be a multiple of B
const int B = 12;  // B should be a multiple of 4
double A[n][n+1];  // matrix to be solved
double x[n];       // final results
int randNum[n][n]; // random numbers for initializing the matrix

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

// Version 2: Split each loop into two loops with head loops
// because i and j loops do not start on block boundaries
void lu_2()
{
  for (int K = 0; K < n; K += B)       // K-blocks
    for (int k = K; k < K + B; k += 1) // k index within block
    {
      for (int i = k + 1; i < K + B; i += 1) // head loop for i
      {
        A[i][k] /= A[k][k];
        for (int j = k + 1; j < K + B; j += 1) // head loop for j
          A[i][j] -= A[i][k] * A[k][j];
        for (int J = K + B; J < n; J += B)   // block loop in j-direction
          for (int j = J; j < J + B; j += 1) // j-loop within block
            A[i][j] -= A[i][k] * A[k][j];
      }
      for (int I = K + B; I < n; I += B)   // block loop in i-direction
        for (int i = I; i < I + B; i += 1) // i-loop within block
        {
          A[i][k] /= A[k][k];
          for (int j = k + 1; j < K + B; j += 1) // head loop for j
            A[i][j] -= A[i][k] * A[k][j];
          for (int J = K + B; J < n; J += B)   // block loop in j-direction
            for (int j = J; j < J + B; j += 1) // j-loop within block
              A[i][j] -= A[i][k] * A[k][j];
        }
    }
}

/* Version 3: Now reorder loops for cache reuse
 * 1) use the following transformation
 *    loop A
 *      loop B
 *      {
 *        loop C1
 *        loop C2 (may depend on results in C1)
 *      }
 *  is the same as
 *    loop A
 *      loop B
 *        loop C1
 *    loop A
 *      loop B
 *        loop C2 (may depend on results in C1)
 * 2) Move loops over blocks outside
 */
void lu_3()
{
  for (int K = 0; K < n; K += B) // K-blocks
  {
    // upper left block
    for (int k = K; k < K + B; k += 1)       // k index within block
      for (int i = k + 1; i < K + B; i += 1) // head loop for i
      {
        A[i][k] /= A[k][k];
        for (int j = k + 1; j < K + B; j += 1) // head loop for j
          A[i][j] -= A[i][k] * A[k][j];
      }

    // U blocks in first block row
    for (int J = K + B; J < n; J += B)         // block loop in j-direction
      for (int k = K; k < K + B; k += 1)       // k index within block
        for (int i = k + 1; i < K + B; i += 1) // head loop for i
          for (int j = J; j < J + B; j += 1)   // j-loop within block
            A[i][j] -= A[i][k] * A[k][j];      // consecutive access

    // L blocks in first block column
    for (int I = K + B; I < n; I += B)     // block loop in i-direction
      for (int k = K; k < K + B; k += 1)   // k index within block
        for (int i = I; i < I + B; i += 1) // i-loop within block
        {
          A[i][k] /= A[k][k];
          for (int j = k + 1; j < K + B; j += 1) // head loop for j
            A[i][j] -= A[i][k] * A[k][j];
        }

    // the rest matrix; here is the bulk of the work
    for (int I = K + B; I < n; I += B)         // block loop in i-direction
      for (int J = K + B; J < n; J += B)       // block loop in j-direction
        for (int i = I; i < I + B; i += 1)     // i-loop within block
          for (int j = J; j < J + B; j += 1)   // j-loop within block
            for (int k = K; k < K + B; k += 1) // k index within block
              A[i][j] -= A[i][k] * A[k][j];
  }
}


void lu_3_self()  // by hand optimized version of lu_3
{

  __m256d vec1,vec2,res,factor;

  for (int K = 0; K < n; K += B) // K-blocks
  {
    // upper left block
    for (int k = K; k < K + B; k += 1)         // k index within block
      for (int i = k + 1; i < K + B; i += 1)   // head loop for i
      {
        A[i][k] /= A[k][k];
        for (int j = k + 1; j < K + B; j += 1) // head loop for j
          A[i][j] -= A[i][k] * A[k][j];
      }

    // U blocks in first block row
    for (int J = K + B; J < n; J += B)         // block loop in j-direction
      for (int k = K; k < K + B; k += 1)       // k index within block
        for (int i = k + 1; i < K + B; i += 1) // head loop for i
          for (int j = J; j < J + B; j += 1)   // j-loop within block
            A[i][j] -= A[i][k] * A[k][j];      // consecutive access

    // L blocks in first block column
    for (int I = K + B; I < n; I += B)         // block loop in i-direction
      for (int k = K; k < K + B; k += 1)       // k index within block
        for (int i = I; i < I + B; i += 1)     // i-loop within block
        {
          A[i][k] /= A[k][k];
          for (int j = k + 1; j < K + B; j += 1) // head loop for j
            A[i][j] -= A[i][k] * A[k][j];
        }

    // the rest matrix; here is the bulk of the work
    for (int I = K + B; I < n; I += B)         // block loop in i-direction
      for (int J = K + B; J < n; J += B)       // block loop in j-direction
        for (int i = I; i < I + B; i += 1)     // i-loop within block
          for (int j = J; j < J + B; j += 4)   // j-loop within block  <- changed stepsize
            for (int k = K; k < K + B; k += 1) // k index within block
            {
              factor = _mm256_set1_pd(A[i][k]);        // changed to AVX intrinsics
              vec1 = _mm256_loadu_pd(&A[k][j]);
              vec2 = _mm256_loadu_pd(&A[i][j]);
              vec2 = _mm256_fnmadd_pd(factor,vec1,vec2);
              _mm256_storeu_pd(&A[i][j],vec2);
            }
  }
}

void solver()  // obtain solution by back substitution method
{
  x[n-2] = A[n-2][n-1]/A[n-2][n-2];

	 for(int i = n-3; i>=0; i--)
	 {
		  x[i] = A[i][n-1];
		  for(int j = i+1; j<=n-1; j++)
		  {
		  		x[i] = x[i] - A[i][j]*x[j];
		  }
		  x[i] = x[i]/A[i][i];
	 }
}

void display()  // show the final result
{
  std::cout << std::endl << "Solution: " << std::endl;
  for(int i = 0; i<n-1; i++)
  {
    std::cout << "x[" << i << "] = " << x[i] << std::endl;
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

  lu_3();
  solver();
  display();

  initialize();

  lu_3_self();
  solver();
  display();

  return 0;
}
