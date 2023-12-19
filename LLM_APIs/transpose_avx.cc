#include <iostream>
#include <vector>
#include <immintrin.h>
#include <cassert>

#include "vectorclass.h"

#include "time_experiment.hh"

// =================
// Utility functions
// =================
void initialize(const int n, double* a){
    for (int i=0; i<n*n; ++i){
        a[i] = i;
    }
}
void null(const int n, double* a){
    for (int i=0; i<n*n; ++i){
        a[i] = 0;
    }
}
void print(const int n, double* a){
    for (int i=0; i<n; ++i){
        for (int j=0; j<n; ++j){
            std::cout << a[i*n + j] << " ";
        }
        std::cout << std::endl;
    }
}

// ===================================
// Transpose using vectorclass library
// ===================================

// Transpose a 4x4 matrix
void transpose_vcl_4x4(int n, double* a, double* b){
  assert(n == 4);

  Vec4d a0, a1, a2, a3;
  Vec4d t0, t1, t2, t3;
  Vec4d b0, b1, b2, b3;

  a0.load(&a[0]);
  a1.load(&a[4]);
  a2.load(&a[8]);
  a3.load(&a[12]);

  t0 = blend4<0,4,2,6>(a0, a1);
  t1 = blend4<1,5,3,7>(a0, a1);
  t2 = blend4<0,4,2,6>(a2, a3);
  t3 = blend4<1,5,3,7>(a2, a3);

  b0 = blend4<0,1,4,5>(t0, t2);
  b1 = blend4<0,1,4,5>(t1, t3);
  b2 = blend4<2,3,6,7>(t0, t2);
  b3 = blend4<2,3,6,7>(t1, t3);

  b0.store(&b[0]);
  b1.store(&b[4]);
  b2.store(&b[8]);
  b3.store(&b[12]);
}

// Transpose a matrix using vcl. Assume that n%8==0. This performs much better
// if the arrays are 64 byte aligned!
void transpose_vcl(int n, double *a, double *b){
  assert(n%8 == 0);

  // Use a blocking of size 8. This way we have full cache line reuse if the
  // arrays are 64 byte aligned.
  const int m = 8;
  for (int i=0; i<n; i+=m){
    for (int j=0; j<n; j+=m){

      // Within the block of size 8x8 be do four transposes of size 4x4 using
      // the same instructions as in the 4x4 example above.
      for (int ii=0; ii<8; ii+=4){
        for (int jj=0; jj<8; jj+=4){

          // We need to find the correct index in the one dimensional array
          int b_start = i+ii + n*(j+jj);
          int a_start = j+jj + n*(i+ii);

          Vec4d a0, a1, a2, a3;
          Vec4d t0, t1, t2, t3;
          Vec4d b0, b1, b2, b3;

          a0.load(&a[a_start]);
          a1.load(&a[a_start+n]);
          a2.load(&a[a_start+2*n]);
          a3.load(&a[a_start+3*n]);

          t0 = blend4<0,4,2,6>(a0, a1);
          t1 = blend4<1,5,3,7>(a0, a1);
          t2 = blend4<0,4,2,6>(a2, a3);
          t3 = blend4<1,5,3,7>(a2, a3);

          b0 = blend4<0,1,4,5>(t0, t2);
          b1 = blend4<0,1,4,5>(t1, t3);
          b2 = blend4<2,3,6,7>(t0, t2);
          b3 = blend4<2,3,6,7>(t1, t3);

          b0.store(&b[b_start]);
          b1.store(&b[b_start+n]);
          b2.store(&b[b_start+2*n]);
          b3.store(&b[b_start+3*n]);
        }
      }
    }
  }
}
class ExperimentVCL{
  int _n;
  double *_a;
  double *_b;
public:
  ExperimentVCL (int n) : _n(n)
  {
    // std::cout << "Exp vcl " << _n << std::endl;
    _a = new(std::align_val_t{64}) double [_n*_n];
    _b = new(std::align_val_t{64}) double [_n*_n];
    initialize(_n, _a);
    initialize(_n, _b);
  }
  ~ExperimentVCL () {
    delete[] _a;
    delete[] _b;
  }
  // run an experiment; can be called several times
  void run () const
  {
    transpose_vcl(_n, _a, _b);
  }
  // report number of operations for one run
  double operations () const
  {
    return _n*_n;
  }
};

// ================================
// Transpose using intel intrinsics
// ================================

// Transpose a 4x4 matrix. Produces the same assembler code as the vectorclass
// version above.
void transpose_intrinsics_v1_4x4(const int n, double* a, double* b){
    __m256d a0 = _mm256_loadu_pd(&a[0]);
    __m256d a1 = _mm256_loadu_pd(&a[4]);
    __m256d a2 = _mm256_loadu_pd(&a[8]);
    __m256d a3 = _mm256_loadu_pd(&a[12]);

    __m256d t0 = _mm256_unpacklo_pd(a0, a1);
    __m256d t1 = _mm256_unpacklo_pd(a2, a3);
    __m256d t2 = _mm256_unpackhi_pd(a0, a1);
    __m256d t3 = _mm256_unpackhi_pd(a2, a3);

    __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
    __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
    __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
    __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

    _mm256_storeu_pd (&b[0], b0);
    _mm256_storeu_pd (&b[4], b1);
    _mm256_storeu_pd (&b[8], b2);
    _mm256_storeu_pd (&b[12], b3);
}

void transpose_intrinsics_v1(const int n, double* a, double* b){
  const int m = 8;
  for (int i=0; i<n; i+=m){
    for (int j=0; j<n; j+=m){
      for (int ii=0; ii<8; ii+=4){
        for (int jj=0; jj<8; jj+=4){
          int b_start = i+ii + n*(j+jj);
          int a_start = j+jj + n*(i+ii);

          __m256d a0 = _mm256_loadu_pd(&a[a_start]);
          __m256d a1 = _mm256_loadu_pd(&a[a_start+n]);
          __m256d a2 = _mm256_loadu_pd(&a[a_start+2*n]);
          __m256d a3 = _mm256_loadu_pd(&a[a_start+3*n]);

          __m256d t0 = _mm256_unpacklo_pd(a0, a1);
          __m256d t1 = _mm256_unpacklo_pd(a2, a3);
          __m256d t2 = _mm256_unpackhi_pd(a0, a1);
          __m256d t3 = _mm256_unpackhi_pd(a2, a3);

          __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
          __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
          __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
          __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

          _mm256_storeu_pd (&b[b_start], b0);
          _mm256_storeu_pd (&b[b_start+n], b1);
          _mm256_storeu_pd (&b[b_start+2*n], b2);
          _mm256_storeu_pd (&b[b_start+3*n], b3);
        }
      }
    }
  }
}
class ExperimentIntrinsics{
  int _n;
  double *_a;
  double *_b;
public:
  ExperimentIntrinsics (int n) : _n(n)
  {
    // std::cout << "Exp intrinsics " << _n << std::endl;
    _a = new(std::align_val_t{64}) double [_n*_n];
    _b = new(std::align_val_t{64}) double [_n*_n];
    initialize(_n, _a);
    initialize(_n, _b);
  }
  ~ExperimentIntrinsics () {
    delete[] _a;
    delete[] _b;
  }
  // run an experiment; can be called several times
  void run () const
  {
    transpose_intrinsics_v1(_n, _a, _b);
  }
  // report number of operations for one run
  double operations () const
  {
    return _n*_n;
  }
};

// ========================================
// Intel intrinsics with nontemporal stores
// ========================================
//
// This is the real reason for implenting the intel intrinsics version above. In
// the vectorclass it is not possible to use non-temporal stores. Using intel
// intrinsics this can be done with streaming stores. For big matrices we are
// sure that the result b will not be used again. Instead of reading a cache
// line of b, writing the results into the cache and storing the cache in memory
// we can directly store the results in the main memory.
//
// For small matrices this will slow the program down as we might be able to
// store the matrices a and b in the cache and use them multiple times. In this
// case we are not yet bound by the main memory bandwidth. Instead the cache
// memory bandwidth is the deciding factor.

// Matrix transpose with non-temporal stores
void transpose_intrinsics_v2(const int n, double* a, double* b){
  const int m = 8;
  for (int i=0; i<n; i+=m){
    for (int j=0; j<n; j+=m){
      for (int ii=0; ii<8; ii+=4){
        for (int jj=0; jj<8; jj+=4){
          int b_start = i+ii + n*(j+jj);
          int a_start = j+jj + n*(i+ii);

          __m256d a0 = _mm256_loadu_pd(&a[a_start]);
          __m256d a1 = _mm256_loadu_pd(&a[a_start+n]);
          __m256d a2 = _mm256_loadu_pd(&a[a_start+2*n]);
          __m256d a3 = _mm256_loadu_pd(&a[a_start+3*n]);

          __m256d t0 = _mm256_unpacklo_pd(a0, a1);
          __m256d t1 = _mm256_unpacklo_pd(a2, a3);
          __m256d t2 = _mm256_unpackhi_pd(a0, a1);
          __m256d t3 = _mm256_unpackhi_pd(a2, a3);

          __m256d b0 = _mm256_permute2f128_pd(t0, t1, 0x20);
          __m256d b1 = _mm256_permute2f128_pd(t2, t3, 0x20);
          __m256d b2 = _mm256_permute2f128_pd(t0, t1, 0x31);
          __m256d b3 = _mm256_permute2f128_pd(t2, t3, 0x31);

          // Use non-temporal stores
          _mm256_stream_pd (&b[b_start], b0);
          _mm256_stream_pd (&b[b_start+n], b1);
          _mm256_stream_pd (&b[b_start+2*n], b2);
          _mm256_stream_pd (&b[b_start+3*n], b3);
        }
      }
    }
  }
}
class ExperimentIntrinsicsV2{
  double *_a;
  double *_b;
  int _n;
public:
  ExperimentIntrinsicsV2 (int n) : _n(n)
  {
    // std::cout << "Exp non-temporal " << _n << std::endl;
    _a = new(std::align_val_t{64}) double [_n*_n];
    _b = new(std::align_val_t{64}) double [_n*_n];
    initialize(_n, _a);
    initialize(_n, _b);
  }
  ~ExperimentIntrinsicsV2 () {
    delete[] _a;
    delete[] _b;
  }
  // run an experiment; can be called several times
  void run () const
  {
    transpose_intrinsics_v2(_n, _a, _b);
  }
  // report number of operations for one run
  double operations () const
  {
    return _n*_n;
  }
};

// ===================================
// Visual tests for the implementation
// ===================================

void test_matrix_transpose_4x4(){
  const int n(4);
  double* a;
  double* b;
  a = new double [n*n];
  b = new double [n*n];
  initialize(n, a);
  std::cout << "Matrix a:" << std::endl;
  print(n, a);

  null(n, b);
  transpose_vcl_4x4(n, a, b);
  std::cout << std::endl;
  std::cout << "vcl:" << std::endl;
  print(n, b);

  null(n, b);
  transpose_intrinsics_v1_4x4(n, a, b);
  std::cout << std::endl;
  std::cout << "intrinsics:" << std::endl;
  print(n, b);

  delete a;
  delete b;

  std::cout << std::endl << std::endl;
}

void test_matrix_transpose(){
  const int n(16);
  alignas(64) double a[n*n];
  alignas(64) double b[n*n];

  initialize(n, a);
  std::cout << "Matrix a:" << std::endl;
  print(n, a);

  null(n, b);
  transpose_vcl(n, a, b);
  std::cout << std::endl;
  std::cout << "vcl:" << std::endl;
  print(n, b);

  null(n, b);
  transpose_intrinsics_v1(n, a, b);
  std::cout << std::endl;
  std::cout << "intrinsics:" << std::endl;
  print(n, b);

  null(n, b);
  transpose_intrinsics_v2(n, a, b);
  std::cout << std::endl;
  std::cout << "intrinsics_v2" << std::endl;
  print(n, b);

  std::cout << std::endl << std::endl;
}


int main(){
  test_matrix_transpose_4x4();
  test_matrix_transpose();

  // Note: Since we want to compare to transpose_v1 we keep the strange matrix
  // sizes. We only require n%8 == 0
  std::vector<int> sizes;
  for (int i=24; i<=16384; i*=2) sizes.push_back(i);

  std::vector<std::string> expnames; // name of experimenta

  expnames.push_back("expl vec");
  std::cout << expnames.back() << std::endl;
  std::vector<double> bandwidth1;
  for (auto n : sizes){
    ExperimentVCL e(n);
    auto d = time_experiment(e,1000000);
    double result = d.first*e.operations()*2*sizeof(double)/d.second*1e6/1e9;
    bandwidth1.push_back(result);
    std::cout << result << std::endl;
  }

  expnames.push_back("intrinsics");
  std::cout << expnames.back() << std::endl;
  std::vector<double> bandwidth2;
  for (auto n : sizes){
    ExperimentIntrinsics e(n);
    auto d = time_experiment(e,1000000);
    double result = d.first*e.operations()*2*sizeof(double)/d.second*1e6/1e9;
    bandwidth2.push_back(result);
    std::cout << result << std::endl;
  }

  expnames.push_back("non-temporal");
  std::cout << expnames.back() << std::endl;
  std::vector<double> bandwidth3;
  for (auto n : sizes){
    ExperimentIntrinsicsV2 e(n);
    auto d = time_experiment(e,1000000);
    double result = d.first*e.operations()*2*sizeof(double)/d.second*1e6/1e9;
    bandwidth3.push_back(result);
    std::cout << result << std::endl;
  }

  std::cout << "N";
  for (std::string s : expnames)
    std::cout << ", " << s;
  std::cout << std::endl;
  for (std::size_t i=0; i<sizes.size(); i++)
    {
      std::cout << sizes[i];
      std::cout << ", " << bandwidth1[i];
      std::cout << ", " << bandwidth2[i];
      std::cout << ", " << bandwidth3[i];
      std::cout << std::endl;
    }

  return 0;
}
