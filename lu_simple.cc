
/*
Program: Gauss Elimination Method
All array indexes are assumed to start from 1
*/

#include<iostream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>

#define   SIZE   10

using namespace std;

int main()
{
	 float a[SIZE][SIZE], x[SIZE], ratio;
	 int i,j,k,n;

     // Setting precision and writing floating point values in fixed-point notation.
     cout<< setprecision(3)<< fixed;

	 // Inputs

    a[0][0] = 2;
    a[0][1] = 1;
    a[0][2] = 7;
    a[0][3] = -2;
    a[0][4] = 5;
    a[1][0] = 2;
    a[1][1] = 2;
    a[1][2] = 1;
    a[1][3] = 0;
    a[1][4] = 1;
    a[2][0] = 0;
    a[2][1] = 0;
    a[2][2] = 2;
    a[2][3] = 1;
    a[2][4] = 3;
    a[3][0] = 0;
    a[3][1] = 0;
    a[3][2] = 1;
    a[3][3] = 1;
    a[3][4] = 2;
    n=4;

     /*
	 // 1. Reading number of unknowns
	 cout<<"Enter number of unknowns: ";
	 cin>>n;

	 // 2. Reading Augmented Matrix
	 cout<<"Enter Coefficients of Augmented Matrix: "<< endl;
	 for(i=0;i<=n-1;i++)
	 {
		  for(j=0;j<=n;j++)
		  {
			   cout<<"a["<< i<<"]"<< "[" << j<<"]= ";
			   cin>>a[i][j];
		  }
	 } */
	// Applying Gauss Elimination
	 for(k=0;k<n;k++)
	 {
		  if(a[k][k] == 0.0)
		  {
			   cout<<"Mathematical Error!";
			   exit(0);
		  }
		  for(i=k+1;i<n;i++)
		  {
			   a[i][k] /= a[k][k];

			   for(j=k+1;j<=n;j++)
			   {
			  		a[i][j] -= a[i][k]*a[k][j];
			   }
		  }
	 }
	 /* Obtaining Solution by Back Substitution Method */
	 x[n-1] = a[n-1][n]/a[n-1][n-1];

	 for(i=n-2;i>=0;i--)
	 {
		  x[i] = a[i][n];
		  for(j=i+1;j<=n-1;j++)
		  {
		  		x[i] = x[i] - a[i][j]*x[j];
		  }
		  x[i] = x[i]/a[i][i];
	 }

	 /* Displaying Solution */
	 cout<< endl<<"Solution: "<< endl;
	 for(i=0;i<n;i++)
	 {
	  	cout<<"x["<< i<<"] = "<< x[i]<< endl;
	 }

	 return(0);
}

