#include <immintrin.h>
#include <cmath>
#include <random>
#include <iostream>

const int n = 48;       // size of matrix
double A[n*(n + 1)];    // matrix to be solved
double x[n];            // final results
int randNum[n*n];      // random numbers for initializing the matrix

void function(int n, double* A);


void solver()  // obtain solution by back substitution method
{
  x[n-2] = A[(n-2)*(n+1)+n-1]/A[(n-2)*(n+1)+n-2];

	 for(int i = n-3; i>=0; i--)
	 {
		  x[i] = A[i*(n+1)+n-1];
		  for(int j = i+1; j<=n-1; j++)
		  {
		  		x[i] = x[i] - A[i*(n+1)+j]*x[j];
		  }
		  x[i] = x[i]/A[i*(n+1)+i];
	 }
}

void setRand()  // set a random array for initialization
{
  srand(0);
  for (int i = 0; i < n;i++)
  {
    for (int j = 0; j < n; j++)
    {
      randNum[i*n+j] = rand() % 2;
    }
  }
}

void initialize()   // set matrix
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n-1; j++)
    {
      if (i==j) A[i*(n+1)+i] = 1;
      else A[i*(n+1)+j] = 0;
    }
  }

  // set x_i = i
  for (int i = 0; i < n; i++)
  {
    A[i*(n+1)+n-1] = i;
  }

  // mix up the matrix
  for (int k = 0; k < n-1; k++)
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        if (k!=i) A[i*(n+1)+j] += randNum[k*n+i]*A[k*(n+1)+j];
      }
    }
  }
}

void display()  // show the final result
{
  for(int i = 0; i<n-1; i++)
  {
    std::cout << "x[" << i << "] = " << std::round(x[i]) << std::endl;
  }
}

int main()
{
    setRand();
    initialize();
    function(n,A);
    solver();
    display();
    return 0;
}