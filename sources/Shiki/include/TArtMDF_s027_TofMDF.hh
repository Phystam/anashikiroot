#ifndef _TARTMDF_S027_TOFMDF_H_
#define _TARTMDF_S027_TOFMDF_H_
#include <TObject.h>
class TArtMDF_s027_TofMDF : public TObject {
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
  ClassDef(TArtMDF_s027_TofMDF,1);
};
#endif
