// -*- mode: c++ -*-
// 
// File TArtMDF_s027_BrhoMDF.cxx generated by TMultiDimFit::MakeRealCode
// on Fri Mar  3 11:29:06 2017
// ROOT version 5.34/30
//
// This file contains the function 
//
//    double  TArtMDF_s027_Brho::MDF(double *x); 
//
// For evaluating the parameterization obtained
// from TMultiDimFit and the point x
// 
// See TMultiDimFit class documentation for more information 
// 
#include "TArtMDF_s027_BrhoMDF.hh"
//
// Static data variables
//
int    TArtMDF_s027_BrhoMDF::gNVariables    = 6;
int    TArtMDF_s027_BrhoMDF::gNCoefficients = 46;
double TArtMDF_s027_BrhoMDF::gDMean         = 7.07123;
// Assignment to mean vector.
double TArtMDF_s027_BrhoMDF::gXMean[] = {
  -0.00844737, -0.00668431, -0.00176135, -0.00376485, 0.032329, 0.115702 };

// Assignment to minimum vector.
double TArtMDF_s027_BrhoMDF::gXMin[] = {
  -109.082, -0.0397993, -84.9463, -0.0399876, -728.227, -0.0312466 };

// Assignment to maximum vector.
double TArtMDF_s027_BrhoMDF::gXMax[] = {
  109.58, 0.0402378, 84.9944, 0.040023, 660.828, 0.333647 };

// Assignment to coefficients vector.
double TArtMDF_s027_BrhoMDF::gCoefficient[] = {
  -0.265048,
  1.57278,
  0.336779,
  0.257598,
  -0.121106,
  0.0747756,
  -0.0772429,
  -0.030605,
  -0.0191806,
  0.0576326,
  0.0298817,
  -0.0104656,
  -0.032129,
  0.0303185,
  0.0114196,
  0.000948196,
  0.00534815,
  0.00856851,
  -0.0211812,
  -0.0099434,
  -0.024545,
  0.00713831,
  -0.00940607,
  0.00251754,
  0.00290393,
  -0.00368658,
  0.0086833,
  0.00123207,
  -0.00787935,
  0.00229905,
  -0.000226103,
  -0.000500708,
  0.000341439,
  0.0012116,
  -0.00178571,
  -5.07358e-05,
  -0.000540943,
  5.53933e-05,
  0.000126502,
  0.000131598,
  -0.000433239,
  -0.000293387,
  -7.83902e-06,
  1.83953e-05,
  2.27336e-05,
  -1.9373e-05
 };

// Assignment to error coefficients vector.
double TArtMDF_s027_BrhoMDF::gCoefficientRMS[] = {
  0.00883659,
  0.0223978,
  0.0353326,
  0.0176452,
  0.0249282,
  0.0895865,
  0.0225037,
  0.0266101,
  0.0165762,
  0.0415831,
  0.0368334,
  0.176709,
  0.0575472,
  0.0547857,
  0.0589455,
  0.165307,
  0.0299419,
  0.0304883,
  0.119526,
  0.100533,
  0.219944,
  0.0869865,
  0.0964154,
  0.0357591,
  0.0562635,
  0.104517,
  0.826089,
  0.096842,
  0.767464,
  0.367744,
  0.02519,
  0.0773886,
  0.0813449,
  0.138806,
  0.284212,
  0.0217566,
  0.0745732,
  0.0474658,
  0.062304,
  0.0435741,
  0.127756,
  0.0635279,
  0.0242137,
  0.052605,
  0.0598466,
  0.0560863
 };

// Assignment to powers vector.
// The powers are stored row-wise, that is
//  p_ij = TArtMDF_s027_BrhoMDF::gPower[i * NVariables + j];
int    TArtMDF_s027_BrhoMDF::gPower[] = {
  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  2,
  2,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  3,
  1,  2,  1,  1,  2,  1,
  1,  1,  2,  2,  1,  1,
  3,  1,  1,  1,  1,  1,
  1,  2,  1,  1,  1,  1,
  1,  3,  1,  1,  1,  1,
  1,  1,  1,  3,  1,  1,
  1,  1,  1,  1,  1,  4,
  1,  2,  2,  2,  1,  1,
  1,  2,  1,  1,  1,  3,
  1,  1,  3,  1,  1,  1,
  2,  3,  1,  1,  1,  1,
  3,  1,  1,  1,  2,  1,
  1,  1,  3,  1,  2,  1,
  1,  1,  1,  3,  1,  2,
  2,  1,  2,  2,  1,  1,
  1,  2,  1,  3,  1,  1,
  2,  2,  1,  1,  2,  1,
  1,  3,  1,  1,  1,  2,
  1,  2,  3,  1,  1,  1,
  2,  1,  1,  1,  1,  2,
  4,  1,  1,  1,  1,  1,
  2,  1,  1,  3,  1,  1,
  1,  1,  1,  2,  1,  3,
  1,  2,  2,  1,  1,  2,
  1,  1,  1,  2,  2,  2,
  2,  1,  1,  2,  1,  2,
  1,  1,  2,  1,  2,  1,
  2,  1,  1,  1,  1,  3,
  3,  1,  2,  1,  1,  1,
  1,  2,  1,  2,  2,  1,
  2,  1,  1,  2,  2,  1,
  1,  1,  1,  2,  1,  1,
  1,  1,  1,  2,  1,  2,
  1,  2,  2,  1,  1,  1,
  2,  1,  1,  2,  1,  1,
  1,  2,  1,  2,  1,  1,
  2,  1,  2,  1,  1,  2,
  2,  1,  2,  1,  1,  1,
  1,  1,  2,  1,  1,  1,
  1,  1,  2,  1,  3,  1,
  1,  3,  1,  2,  1,  1,
  1,  3,  2,  1,  1,  1
};

// 
// The method   double TArtMDF_s027_BrhoMDF::MDF(double *x)
// 
double TArtMDF_s027_BrhoMDF::MDF(double *x) {
  double returnValue = TArtMDF_s027_BrhoMDF::gDMean;
  int    i = 0, j = 0, k = 0;
  for (i = 0; i < TArtMDF_s027_BrhoMDF::gNCoefficients ; i++) {
    // Evaluate the ith term in the expansion
    double term = TArtMDF_s027_BrhoMDF::gCoefficient[i];
    for (j = 0; j < TArtMDF_s027_BrhoMDF::gNVariables; j++) {
      // Evaluate the polynomial in the jth variable.
      int power = TArtMDF_s027_BrhoMDF::gPower[TArtMDF_s027_BrhoMDF::gNVariables * i + j]; 
      double p1 = 1, p2 = 0, p3 = 0, r = 0;
      double v =  1 + 2. / (TArtMDF_s027_BrhoMDF::gXMax[j] - TArtMDF_s027_BrhoMDF::gXMin[j]) * (x[j] - TArtMDF_s027_BrhoMDF::gXMax[j]);
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

// EOF for TArtMDF_s027_BrhoMDF.cxx
