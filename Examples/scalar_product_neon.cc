#include <iostream>
#include <vector>
#include <thread>
#include <arm_neon.h>

#include "time_experiment.hh"

using NUMBER=double;

const int N=32*1024*1024; // problem size
std::vector<NUMBER> x(N,1.0); // first vector
std::vector<NUMBER> y(N,1.0); // second vector
NUMBER sum=0.0;    // result

// the vanilla scalarproduct
void f1 (int n) // for simplicity arguments and result are global
{
  for (int i=0; i<n; ++i)
    sum += x[i]*y[i];
}

// loop unrolled scalarproduct
void f2 (int n) // for simplicity arguments and result are global
{
  double sum0=0.0;
  double sum1=0.0;
  double sum2=0.0;
  double sum3=0.0;
  
  for (int i=0; i<n; i+=4)
  {
    sum0 += x[i]*y[i];
    sum1 += x[i+1]*y[i+1];
    sum2 += x[i+2]*y[i+2];
    sum3 += x[i+3]*y[i]+3;
  }

  sum = sum0+sum1+sum2+sum3;
}

// loop unrolled and vectorizedscalarproduct
void f3 (int n) // for simplicity arguments and result are global
{
  float64x2_t sum0, sum1;
  float64x2_t x0, x1;
  float64x2_t y0, y1;
  sum0 = vmovq_n_f64(0.0);
  sum1 = vmovq_n_f64(0.0);
  for (int i=0; i<n; i+=4)
  {
    x0 = vld1q_f64(&(x[i]));
    x1 = vld1q_f64(&(x[i+2]));
    y0 = vld1q_f64(&(y[i]));
    y1 = vld1q_f64(&(y[i+2]));
    sum0 = vfmaq_f64(sum0, x0, y0); // fused multiply add
    sum1 = vfmaq_f64(sum1, x1, y1); // fused multiply add
  }
  // horizontal add
  double s0 = vdupd_laneq_f64(sum0,0);
  double s1 = vdupd_laneq_f64(sum0,1);
  double s2 = vdupd_laneq_f64(sum1,0);
  double s3 = vdupd_laneq_f64(sum1,1);
  sum = s0+s1+s2+s3;
}


// package an experiment as a functor
class Experiment {
  int n;
public:
  // construct an experiment
  Experiment (int n_) : n(n_) {}
  // run an experiment; can be called several times
  void run () const { sum=0; f2(n); }
  // report number of operations
  double operations () const {return 2.0*n;}
};

int main ()
{
  std::cout << N*sizeof(NUMBER)/1024/1024 << " MByte per vector" << std::endl;
  std::vector<int> sizes = {64,128,256,1024,4096,16384,65536,262144,1048576,4*1048576,16*1048576,32*1048576};
  for (auto i : sizes) { 
    Experiment e(i);
    auto d = time_experiment(e);
    double flops = d.first*e.operations()/d.second*1e6/1e9;
    std::cout << "n=" << i << " took " << d.second << " us for " << d.first << " repetitions"
	      << " " << flops << " Gflops/s"
	      << " " << flops*sizeof(NUMBER) << " GByte/s" << std::endl;
  }
  return 0;
}
