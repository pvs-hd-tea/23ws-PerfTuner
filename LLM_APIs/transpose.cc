#include <iostream>
#include <vector>
#include <string>
#include "time_experiment.hh"

// B = A^T,
// A,B are nxn matrices stored row-major in a 1d array,
// assume A and B are NOT the same matrix


// consecutive write, strided read
void transpose1(int n, double *A, double *B)
{
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      B[i * n + j] = A[j * n + i];
}

// strided write, consecutive read
void transpose2(int n, double *A, double *B)
{
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      B[j * n + i] = A[i * n + j];
}


// blocked consecutive write, strided read
template <int M> // MxM blocks
void transpose3(int n, double *A, double *B)
{
  for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    for (int j = 0; j < n; j += M) // split j loop in blocks of size M
      for (int ii = i; ii < i + M; ii++)
        for (int jj = j; jj < j + M; jj++)
          B[ii * n + jj] = A[jj * n + ii];
}

// blocked strided write, consecutive read
template <int M> // MxM blocks
void transpose4(int n, double *A, double *B)
{
  for (int i = 0; i < n; i += M)   // split i loop in blocks of size M
    for (int j = 0; j < n; j += M) // split j loop in blocks of size M
      for (int ii = i; ii < i + M; ii++)
        for (int jj = j; jj < j + M; jj++)
          B[jj * n + ii] = A[ii * n + jj];
}

// initialize square matrix
void initialize(int n, double *A)
{
  for (int i = 0; i < n * n; i++)
    A[i] = i;
}

class Experiment1
{
  int n;
  double *A, *B;

public:
  // construct an experiment
  Experiment1(int n_) : n(n_)
  {
    std::cout << "Exp1: " << n << std::endl;
    A = new (std::align_val_t{64}) double[n * n];
    B = new (std::align_val_t{64}) double[n * n];
    initialize(n, A);
    initialize(n, B);
    if (((size_t)A) % 64 != 0)
    {
      std::cout << "Exp1: A not aligned to 64 " << std::endl;
    }
    if (((size_t)B) % 64 != 0)
    {
      std::cout << "Exp1: B not aligned to 64 " << std::endl;
    }
  }
  ~Experiment1()
  {
    delete[] A;
    delete[] B;
  }
  // run an experiment; can be called several times
  void run() const
  {
    transpose1(n, A, B);
  }
  // report number of operations for one run
  double operations() const
  {
    return n * n;
  }
};

class Experiment2
{
  int n;
  double *A, *B;

public:
  // construct an experiment
  Experiment2(int n_) : n(n_)
  {
    std::cout << "Exp2: " << n << std::endl;
    A = new (std::align_val_t{64}) double[n * n];
    B = new (std::align_val_t{64}) double[n * n];
    initialize(n, A);
    initialize(n, B);
    if (((size_t)A) % 64 != 0)
    {
      std::cout << "Exp2: A not aligned to 64 " << std::endl;
    }
    if (((size_t)B) % 64 != 0)
    {
      std::cout << "Exp2: B not aligned to 64 " << std::endl;
    }
  }
  ~Experiment2()
  {
    delete[] A;
    delete[] B;
  }
  // run an experiment; can be called several times
  void run() const
  {
    transpose2(n, A, B);
  }
  // report number of operations for one run
  double operations() const
  {
    return n * n;
  }
};

template <int M>
class Experiment3
{
  int n, b;
  double *A, *B;

public:
  // construct an experiment
  Experiment3(int n_) : n(n_)
  {
    std::cout << "Exp5: " << n << std::endl;
    A = new (std::align_val_t{64}) double[n * n];
    B = new (std::align_val_t{64}) double[n * n];
    initialize(n, A);
    initialize(n, B);
    if (((size_t)A) % 64 != 0)
    {
      std::cout << "Exp5: A not aligned to 64 " << std::endl;
    }
    if (((size_t)B) % 64 != 0)
    {
      std::cout << "Exp5: B not aligned to 64 " << std::endl;
    }
  }
  ~Experiment3()
  {
    delete[] A;
    delete[] B;
  }
  // run an experiment; can be called several times
  void run() const
  {
    transpose3<M>(n, A, B);
  }
  // report number of operations for one run
  double operations() const
  {
    return n * n;
  }
};

// conduct experiment X with transposeX
template <int M>
class Experiment4
{
  int n, b;
  double *A, *B;

public:
  // construct an experiment
  Experiment4(int n_) : n(n_)
  {
    std::cout << "Exp6: " << n << std::endl;
    A = new (std::align_val_t{64}) double[n * n];
    B = new (std::align_val_t{64}) double[n * n];
    initialize(n, A);
    initialize(n, B);
    if (((size_t)A) % 64 != 0)
    {
      std::cout << "Exp6: A not aligned to 64 " << std::endl;
    }
    if (((size_t)B) % 64 != 0)
    {
      std::cout << "Exp6: B not aligned to 64 " << std::endl;
    }
  }
  ~Experiment4()
  {
    delete[] A;
    delete[] B;
  }
  // run an experiment; can be called several times
  void run() const
  {
    transpose4<M>(n, A, B);
  }
  // report number of operations for one run
  double operations() const
  {
    return n * n;
  }
};

// main function runs the experiments and outputs results as csv
int main(int argc, char **argv)
{
  std::vector<int> sizes; // vector with problem sizes to try
  // for (int i=16; i<=16384; i*=2) sizes.push_back(i);
  for (int i = 24; i <= 25000; i *= 2)
    sizes.push_back(i);

  std::vector<std::string> expnames; // name of experiment

  // experiment 1
  expnames.push_back("vanilla-consecutive-write");
  std::cout << expnames.back() << std::endl;
  std::vector<double> bandwidth1;
  for (auto n : sizes)
  {
    Experiment1 e(n);
    auto d = time_experiment(e, 1000000);
    double result = d.first * e.operations() * 2 * sizeof(double) / d.second * 1e6 / 1e9;
    bandwidth1.push_back(result);
    std::cout << result << std::endl;
  }
  // experiment 2
  expnames.push_back("vanilla-strided-write");
  std::cout << expnames.back() << std::endl;
  std::vector<double> bandwidth2;
  for (auto n : sizes)
  {
    Experiment2 e(n);
    auto d = time_experiment(e, 1000000);
    double result = d.first * e.operations() * 2 * sizeof(double) / d.second * 1e6 / 1e9;
    bandwidth2.push_back(result);
    std::cout << result << std::endl;
  }
  // experiment 3
  expnames.push_back("blocked-consecutive-write-M4");
  std::cout << expnames.back() << std::endl;
  std::vector<double> bandwidth3;
  for (auto n : sizes)
  {
    Experiment3<4> e(n);
    auto d = time_experiment(e, 1000000);
    double result = d.first * e.operations() * 2 * sizeof(double) / d.second * 1e6 / 1e9;
    bandwidth3.push_back(result);
    std::cout << result << std::endl;
  }
  // experiment 4
  std::cout << expnames.back() << std::endl;
  expnames.push_back("blocked-strided-writed-M4");
  std::vector<double> bandwidth4;
  for (auto n : sizes)
  {
    Experiment4<4> e(n);
    auto d = time_experiment(e, 1000000);
    double result = d.first * e.operations() * 2 * sizeof(double) / d.second * 1e6 / 1e9;
    bandwidth4.push_back(result);
    std::cout << result << std::endl;
  }
  // experiment 5
  expnames.push_back("blocked-consecutive-write-M8");
  std::cout << expnames.back() << std::endl;
  std::vector<double> bandwidth5;
  for (auto n : sizes)
  {
    Experiment3<8> e(n);
    auto d = time_experiment(e, 1000000);
    double result = d.first * e.operations() * 2 * sizeof(double) / d.second * 1e6 / 1e9;
    bandwidth5.push_back(result);
    std::cout << result << std::endl;
  }
  // experiment 6
  std::cout << expnames.back() << std::endl;
  expnames.push_back("blocked-strided-writed-M8");
  std::vector<double> bandwidth6;
  for (auto n : sizes)
  {
    Experiment4<8> e(n);
    auto d = time_experiment(e, 1000000);
    double result = d.first * e.operations() * 2 * sizeof(double) / d.second * 1e6 / 1e9;
    bandwidth6.push_back(result);
    std::cout << result << std::endl;
  }

  // output results
  // Note: size of TLB mentioned in https://www.realworldtech.com/haswell-cpu/5/
  std::cout << "N";
  for (std::string s : expnames)
    std::cout << ", " << s;
  std::cout << std::endl;
  for (int i = 0; i < sizes.size(); i++)
  {
    std::cout << sizes[i];
    std::cout << ", " << bandwidth1[i];
    std::cout << ", " << bandwidth2[i];
    std::cout << ", " << bandwidth3[i];
    std::cout << ", " << bandwidth4[i];
    std::cout << ", " << bandwidth5[i];
    std::cout << ", " << bandwidth6[i];
    std::cout << std::endl;
  }

  return 0;
}
