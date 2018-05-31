#ifndef _TARTMDF_FRAG_ELOSS_EMP_H_
#define _TARTMDF_FRAG_ELOSS_EMP_H_
#include <TObject.h>
class TArtMDF_Frag_Eloss_Emp : public TObject{
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
  ClassDef(TArtMDF_Frag_Eloss_Emp,1)
};
#endif