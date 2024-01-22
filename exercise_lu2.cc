#include <iostream>
#include <immintrin.h>


const int n = 4;
const int B = 2;
double A[n][n+1];

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


  /*
  A[0][0] = 2;
  A[0][1] = 1;
  A[0][2] = 7;
  A[0][3] = -2;
  A[0][4] = 5;
  A[1][0] = 2;
  A[1][1] = 2;
  A[1][2] = 1;
  A[1][3] = 0;
  A[1][4] = 1;
  A[2][0] = 0;
  A[2][1] = 0;
  A[2][2] = 2;
  A[2][3] = 1;
  A[2][4] = 3;
  A[3][0] = 0;
  A[3][1] = 0;
  A[3][2] = 1;
  A[3][3] = 1;
  A[3][4] = 2;
*/

  /*for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      A[i][j]=i+j+1;
    }
  }*/
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


int main()
{ 
  std::cout << "----------------------------------";
  std::cout << std::endl;
  std::cout << "initialize()";
  std::cout << std::endl;
  initialize();
  print(); 
  std::cout << std::endl;
  std::cout << "----------------------------------";
  std::cout << std::endl;
  
  std::cout << "----------------------------------";
  std::cout << std::endl;
  std::cout << "lu_1()";
  std::cout << std::endl;
  lu_1();
  print(); 
  std::cout << std::endl;
  std::cout << "----------------------------------";
  std::cout << std::endl;
  
  std::cout << "----------------------------------";
  std::cout << std::endl;
  std::cout << "lu_2()";
  std::cout << std::endl;
  initialize();
  lu_2();
  print(); 
  std::cout << std::endl;
  std::cout << "----------------------------------";
  std::cout << std::endl;
  
  std::cout << "----------------------------------";
  std::cout << std::endl;
  std::cout << "lu_3()";
  std::cout << std::endl;
  initialize();
  lu_3();
  print(); 
  std::cout << std::endl;

    return 0;
}