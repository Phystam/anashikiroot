// -*- mode: c++ -*-
// 
// File TArtMDF_Eloss_Eout_PbMDF.cxx generated by TMultiDimFit::MakeRealCode
// on Mon Dec 18 17:59:57 2017
// ROOT version 5.34/30
//
// This file contains the function 
//
//    double  TArtMDF_Eloss_Eout_Pb::MDF(double *x); 
//
// For evaluating the parameterization obtained
// from TMultiDimFit and the point x
// 
// See TMultiDimFit class documentation for more information 
// 
#include "TArtMDF_Eloss_Eout_Pb.hh"
//
// Static data variables
//
int    TArtMDF_Eloss_Eout_Pb::gNVariables    = 3;
int    TArtMDF_Eloss_Eout_Pb::gNCoefficients = 31;
double TArtMDF_Eloss_Eout_Pb::gDMean         = 247.973;
// Assignment to mean vector.
double TArtMDF_Eloss_Eout_Pb::gXMean[] = {
  0.0714286, 0.166667, 0.00240821 };

// Assignment to minimum vector.
double TArtMDF_Eloss_Eout_Pb::gXMin[] = {
  27, 9, 194.648 };

// Assignment to maximum vector.
double TArtMDF_Eloss_Eout_Pb::gXMax[] = {
  34, 11, 347.017 };

// Assignment to coefficients vector.
double TArtMDF_Eloss_Eout_Pb::gCoefficient[] = {
  0.779617,
  79.8123,
  -4.53591,
  1.87385,
  -1.07601,
  0.807997,
  0.682423,
  -0.327952,
  -0.605515,
  0.802124,
  -0.104229,
  0.361884,
  -0.267369,
  0.130219,
  -0.165422,
  -0.116292,
  0.096606,
  -0.0111266,
  -0.129878,
  0.0330872,
  0.120385,
  0.0493072,
  -0.0523495,
  0.0555296,
  0.0833883,
  -0.0179018,
  -0.032922,
  -0.0541889,
  -0.0278314,
  -0.0116556,
  0.00546837
 };

// Assignment to error coefficients vector.
double TArtMDF_Eloss_Eout_Pb::gCoefficientRMS[] = {
  0.810524,
  2.00518,
  0.791519,
  2.12383,
  3.47959,
  1.92723,
  3.71259,
  3.62092,
  3.12757,
  2.91064,
  1.0199,
  3.0476,
  4.03947,
  4.88682,
  4.52207,
  3.58936,
  2.95243,
  3.93982,
  4.39218,
  2.82887,
  3.65896,
  3.30382,
  4.45876,
  6.58808,
  6.6768,
  2.30841,
  7.0511,
  5.69761,
  4.39096,
  5.03934,
  4.34416
 };

// Assignment to powers vector.
// The powers are stored row-wise, that is
//  p_ij = TArtMDF_Eloss_Eout_Pb::gPower[i * NVariables + j];
int    TArtMDF_Eloss_Eout_Pb::gPower[] = {
  1,  1,  1,
  1,  1,  2,
  1,  2,  1,
  2,  1,  1,
  1,  1,  3,
  1,  2,  2,
  2,  3,  1,
  2,  1,  2,
  3,  1,  1,
  2,  2,  1,
  1,  3,  1,
  1,  1,  4,
  1,  2,  3,
  2,  1,  3,
  2,  2,  2,
  1,  1,  5,
  1,  2,  4,
  3,  2,  2,
  2,  3,  2,
  1,  3,  2,
  3,  1,  2,
  3,  2,  1,
  2,  1,  4,
  2,  2,  3,
  2,  3,  3,
  1,  3,  3,
  3,  1,  3,
  4,  1,  3,
  1,  2,  5,
  2,  2,  4,
  1,  3,  4
};

// 
// The method   double TArtMDF_Eloss_Eout_Pb::MDF(double *x)
// 
double TArtMDF_Eloss_Eout_Pb::MDF(double *x) {
  double returnValue = TArtMDF_Eloss_Eout_Pb::gDMean;
  int    i = 0, j = 0, k = 0;
  for (i = 0; i < TArtMDF_Eloss_Eout_Pb::gNCoefficients ; i++) {
    // Evaluate the ith term in the expansion
    double term = TArtMDF_Eloss_Eout_Pb::gCoefficient[i];
    for (j = 0; j < TArtMDF_Eloss_Eout_Pb::gNVariables; j++) {
      // Evaluate the polynomial in the jth variable.
      int power = TArtMDF_Eloss_Eout_Pb::gPower[TArtMDF_Eloss_Eout_Pb::gNVariables * i + j]; 
      double p1 = 1, p2 = 0, p3 = 0, r = 0;
      double v =  1 + 2. / (TArtMDF_Eloss_Eout_Pb::gXMax[j] - TArtMDF_Eloss_Eout_Pb::gXMin[j]) * (x[j] - TArtMDF_Eloss_Eout_Pb::gXMax[j]);
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

// EOF for TArtMDF_Eloss_Eout_PbMDF.cxx