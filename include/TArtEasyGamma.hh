
// CATANA reconstruction class
// by T.Tomai

#ifndef _TARTEASYGAMMA_H_
#define _TARTEASYGAMMA_H_

#include <TNamed.h>
#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtCATANAParameters;
class TArtCalibCATANA;
class TArtCATANACsI;
class TArtStoreManager;

class TArtEasyGamma : public TArtReconstruction{

public:

  TArtEasyGamma();
  //TArtEasyGamma();
  virtual ~TArtEasyGamma();

  //  void Initialize();
  void ClearData();
  void ReconstructData();
  // void SetBeta(Double_t val){fBeta=val;}
  // Double_t GetQRaw(Double_t val){fQraw=val;}
  // TArtCalibCATANA* GetCalibCATANA(){return fcalibcatana;}
  // TArtCATANACsI* GetCATANACsI(Int_t i){return fcsi[i];}//by crystal
  // Double_t GetCsIX(Int_t i){ return fcsix[i];}
  // Double_t GetCsIY(Int_t i){ return fcsiy[i];}
  // Double_t GetCsIZ(Int_t i){ return fcsiz[i];}
  // void SetTgtX(Double_t val){ftgtx=val;}
  // void SetTgtY(Double_t val){ftgty=val;}
  // void SetTgtZ(Double_t val){ftgtz=val;}
private:

  // pointer to subsystem objects used for Gamma
  TArtCalibCATANA* fcalibcatana;
  // TArtCATANACsI* fcsi[100];
  // Double_t fcsix[100];
  // Double_t fcsiy[100];
  // Double_t fcsiz[100];
  // Double_t ftgtx,ftgty,ftgtz;
  // Double_t ftheta[100];
  // Double_t fphi[100];
  // Double_t fr[100];

  // const Double_t fclight=2.99792458e8;// m/s
  // const Double_t fe=1.6021766208e-19;// C
  // const Double_t famu=1.660538921e-27;// kg
  // const Double_t famu_MeV=931.494061;//MeV

  // Double_t fBeta;
  // Double_t fQraw;
  ClassDef(TArtEasyGamma,1);


};

#endif
