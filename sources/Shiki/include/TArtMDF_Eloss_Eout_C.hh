#ifndef _TARTMDF_ELOSS_EOUT_C_H_
#define _TARTMDF_ELOSS_EOUT_C_H_
#include <TObject.h>
class TArtMDF_Eloss_Eout_C : public TObject{
public:
 static double MDF(double* x);
private:
  static int gNVariables;
  static int gNCoefficients;
  static double gDMean;
  static double gXMean[];
  static double gXMin[];
  static double gXMax[];
  static double gCoefficient[];
  static double gCoefficientRMS[];
  static int gPower[];
  ClassDef(TArtMDF_Eloss_Eout_C,1)
};
#endif
