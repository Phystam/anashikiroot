#ifndef _TARTMDF_BEAM_ELOSS_C_H_
#define _TARTMDF_BEAM_ELOSS_C_H_
#include <TObject.h>
class TArtMDF_Beam_Eloss_C : public TObject{
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
  ClassDef(TArtMDF_Beam_Eloss_C,1)
};
#endif
