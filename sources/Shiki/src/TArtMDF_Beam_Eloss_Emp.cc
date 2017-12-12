// -*- mode: c++ -*-
// 
// File TArtMDF_Beam_Eloss_EmpMDF.cxx generated by TMultiDimFit::MakeRealCode
// on Mon Dec 11 20:56:44 2017
// ROOT version 5.34/30
//
// This file contains the function 
//
//    double  TArtMDF_Beam_Eloss_Emp::MDF(double *x); 
//
// For evaluating the parameterization obtained
// from TMultiDimFit and the point x
// 
// See TMultiDimFit class documentation for more information 
// 
#include "TArtMDF_Beam_Eloss_Emp.hh"
//
// Static data variables
//
int    TArtMDF_Beam_Eloss_Emp::gNVariables    = 3;
int    TArtMDF_Beam_Eloss_Emp::gNCoefficients = 32;
double TArtMDF_Beam_Eloss_Emp::gDMean         = 266.996;
// Assignment to mean vector.
double TArtMDF_Beam_Eloss_Emp::gXMean[] = {
  0.0714286, 0.166667, 0.00240821 };

// Assignment to minimum vector.
double TArtMDF_Beam_Eloss_Emp::gXMin[] = {
  27, 9, 194.648 };

// Assignment to maximum vector.
double TArtMDF_Beam_Eloss_Emp::gXMax[] = {
  34, 11, 347.017 };

// Assignment to coefficients vector.
double TArtMDF_Beam_Eloss_Emp::gCoefficient[] = {
  -0.0031734,
  76.8126,
  -0.778539,
  0.329283,
  -0.177409,
  0.128962,
  -0.0527558,
  0.178225,
  0.140489,
  -0.335449,
  0.0540764,
  0.185641,
  -0.0241025,
  -0.0391622,
  0.00298288,
  -0.020975,
  -0.0241534,
  -0.030901,
  0.0557301,
  -0.015229,
  0.0111581,
  0.0275456,
  -0.0297909,
  0.00439379,
  -0.00620564,
  0.0090462,
  -0.00809847,
  0.00203027,
  -0.00254364,
  -0.000306568,
  -0.000558668,
  -0.000166721
 };

// Assignment to error coefficients vector.
double TArtMDF_Beam_Eloss_Emp::gCoefficientRMS[] = {
  0.948429,
  2.24215,
  0.809245,
  2.14444,
  3.61218,
  2.00869,
  3.79201,
  3.16335,
  2.17591,
  5.93763,
  3.21855,
  3.95236,
  1.07392,
  4.18119,
  4.10113,
  2.21758,
  3.64723,
  3.91549,
  9.96174,
  3.72877,
  3.07439,
  5.02366,
  6.80291,
  2.79275,
  4.63011,
  6.77875,
  7.30418,
  3.16026,
  4.56408,
  2.39336,
  5.62942,
  4.20497
 };

// Assignment to powers vector.
// The powers are stored row-wise, that is
//  p_ij = TArtMDF_Beam_Eloss_Emp::gPower[i * NVariables + j];
int    TArtMDF_Beam_Eloss_Emp::gPower[] = {
  1,  1,  1,
  1,  1,  2,
  1,  2,  1,
  2,  1,  1,
  1,  1,  3,
  1,  2,  2,
  2,  1,  2,
  2,  2,  1,
  2,  3,  1,
  3,  1,  1,
  1,  1,  4,
  5,  1,  1,
  1,  3,  1,
  1,  2,  3,
  2,  1,  3,
  4,  1,  1,
  2,  3,  2,
  2,  2,  2,
  3,  1,  2,
  1,  1,  5,
  1,  2,  4,
  3,  2,  3,
  5,  1,  2,
  1,  3,  2,
  2,  1,  4,
  2,  2,  3,
  3,  1,  3,
  4,  1,  2,
  1,  2,  5,
  1,  3,  3,
  3,  1,  4,
  1,  3,  4
};

// 
// The method   double TArtMDF_Beam_Eloss_Emp::MDF(double *x)
// 
double TArtMDF_Beam_Eloss_Emp::MDF(double *x) {
  double returnValue = TArtMDF_Beam_Eloss_Emp::gDMean;
  int    i = 0, j = 0, k = 0;
  for (i = 0; i < TArtMDF_Beam_Eloss_Emp::gNCoefficients ; i++) {
    // Evaluate the ith term in the expansion
    double term = TArtMDF_Beam_Eloss_Emp::gCoefficient[i];
    for (j = 0; j < TArtMDF_Beam_Eloss_Emp::gNVariables; j++) {
      // Evaluate the polynomial in the jth variable.
      int power = TArtMDF_Beam_Eloss_Emp::gPower[TArtMDF_Beam_Eloss_Emp::gNVariables * i + j]; 
      double p1 = 1, p2 = 0, p3 = 0, r = 0;
      double v =  1 + 2. / (TArtMDF_Beam_Eloss_Emp::gXMax[j] - TArtMDF_Beam_Eloss_Emp::gXMin[j]) * (x[j] - TArtMDF_Beam_Eloss_Emp::gXMax[j]);
      // what is the power to use!
      switch(power) {
      case 1: r = 1; break; 
      case 2: r = v; break; 
      default: 
        p2 = v; 
        for (k = 3; k <= power; k++) { 
          p3 = p2 * v;
          p1 = p2; p2 = p3; 
        }
        r = p3;
      }
      // multiply this term by the poly in the jth var
      term *= r; 
    }
    // Add this term to the final result
    returnValue += term;
  }
  return returnValue;
}

// EOF for TArtMDF_Beam_Eloss_EmpMDF.cxx
