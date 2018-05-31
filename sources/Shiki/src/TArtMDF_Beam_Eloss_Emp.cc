// -*- mode: c++ -*-
// 
// File TArtMDF_Beam_Eloss_EmpMDF.cxx generated by TMultiDimFit::MakeRealCode
// on Fri Jan  5 18:59:07 2018
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
double TArtMDF_Beam_Eloss_Emp::gDMean         = 231.343;
// Assignment to mean vector.
double TArtMDF_Beam_Eloss_Emp::gXMean[] = {
  0.0714286, 0.166667, 0.00511609 };

// Assignment to minimum vector.
double TArtMDF_Beam_Eloss_Emp::gXMin[] = {
  27, 9, 170.01 };

// Assignment to maximum vector.
double TArtMDF_Beam_Eloss_Emp::gXMax[] = {
  34, 11, 294.83 };

// Assignment to coefficients vector.
double TArtMDF_Beam_Eloss_Emp::gCoefficient[] = {
  -0.25892,
  62.6295,
  -0.270245,
  0.11407,
  -0.0578824,
  0.0444918,
  0.0615622,
  0.0493284,
  -0.0182007,
  -0.119005,
  0.0670121,
  -0.00834103,
  0.0166355,
  -0.0129198,
  -0.00191894,
  -0.00805768,
  -0.0103444,
  -0.000421103,
  -0.00738403,
  0.0128332,
  0.0181804,
  -0.00447905,
  0.00328802,
  -0.00945301,
  -0.000328754,
  0.00143456,
  -0.00173689,
  6.30891e-05,
  0.00340392,
  -0.0033338,
  -0.000683873,
  -0.000119669
 };

// Assignment to error coefficients vector.
double TArtMDF_Beam_Eloss_Emp::gCoefficientRMS[] = {
  0.990755,
  2.36423,
  0.84259,
  2.09201,
  3.86977,
  2.1167,
  3.29225,
  2.25174,
  3.9626,
  6.26406,
  4.2314,
  1.11797,
  3.48816,
  4.46994,
  4.35825,
  1.899,
  4.13669,
  4.30761,
  4.80834,
  5.33119,
  10.462,
  4.10636,
  3.34851,
  7.08955,
  4.58036,
  2.95233,
  5.0289,
  2.56355,
  7.26692,
  7.81726,
  5.02277,
  6.11508
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
  2,  2,  1,
  2,  3,  1,
  2,  1,  2,
  3,  1,  1,
  5,  1,  1,
  1,  3,  1,
  1,  1,  4,
  1,  2,  3,
  2,  1,  3,
  6,  1,  1,
  2,  2,  2,
  3,  2,  2,
  2,  3,  2,
  3,  2,  3,
  3,  1,  2,
  1,  1,  5,
  1,  2,  4,
  5,  1,  2,
  1,  3,  4,
  1,  3,  2,
  2,  1,  4,
  1,  3,  3,
  2,  2,  3,
  3,  1,  3,
  1,  2,  5,
  3,  1,  4
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