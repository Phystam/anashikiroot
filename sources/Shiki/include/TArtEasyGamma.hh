
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
  TVector3 GetPos(){return fpos;}
  Double_t GetEnergy(){return fEnergy;}
  Double_t GetQRaw(){return fQRaw;}
  Double_t GetTRaw(){return fQRaw;}
  Double_t GetTCal(){return fTCal;}
  Double_t GetTSlw(){return fTSlw;}
  void SetQRaw(Double_t val){fQRaw=val;}
  void SetTRaw(Double_t val){fTRaw=val;}
  void SetTCal(Double_t val){fTCal=val;}
  void SEtTSlw(Double_t val){fTSlw=val;}
  void SetEnergy(Double_t val){fEnergy=val;}
  void SetPos(TVector3 val){fpos=val;}

private:
  TVector3 fpos;
  Double_t fEnergy;
  Double_t fQRaw;
  Double_t fTRaw;
  Double_t fTCal;
  Double_t fTSlw;
  ClassDef(TArtEasyGamma,1);
};

#endif
