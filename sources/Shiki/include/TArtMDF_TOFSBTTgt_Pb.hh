#ifndef _TARTMDF_TOFSBTTGT_PB_H_
#define _TARTMDF_TOFSBTTGT_PB_H_
#include <TObject.h>
class TArtMDF_TOFSBTTgt_Pb : public TObject{
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
  ClassDef(TArtMDF_TOFSBTTgt_Pb,1)
};
#endif
