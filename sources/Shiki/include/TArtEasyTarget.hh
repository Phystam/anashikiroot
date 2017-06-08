#ifndef _TARTEASYTARGET_HH_
#define _TARTEASYTARGET_HH_

#include "TArtDataObject.hh"
#include "TArtStoreManager.hh"
#include "TArtCore.hh"
class TArtDataObject;
class TArtStoreManager;
class TArtCore;

class TArtEasyTarget : public TArtDataObject {
private:

public:
  TArtEasyTarget()
    : TArtDataObject(), fPosZ(-1)
  {
    for(Int_t i=0;i<4;i++){
      ftof[i]=-1;
    }
    TArtCore::Info(__FILE__,"Creating target information setup...");
    sman=TArtStoreManager::Instance();
  }

  virtual ~TArtEasyTarget(){;}

  //singleton
  //  static TArtEasyTarget* Instance();

  void SetPosZ(Double_t val){ fPosZ = val;}
  void SetTOF(Int_t i,Double_t val){ ftof[i] = val;}//i=0-4,polynomial coefficients
  void SetBeamE(Int_t i,Double_t val){ fbeamE[i] = val;}//i=0-4,polynomial coefficients
  void SetFragE(Int_t i,Double_t val){ ffragE[i] = val;}//i=0-4,polynomial coefficients
  Double_t GetPosZ(){ return fPosZ;}
  Double_t GetTOF(Int_t i){return ftof[i];}
  Double_t* GetTOF(){return ftof;}//overloaded function
  Double_t GetBeamE(Int_t i){return fbeamE[i];}
  Double_t* GetBeamE(){return fbeamE;}//overloaded function
  Double_t GetFragE(Int_t i){return ffragE[i];}
  Double_t* GetFragE(){return ffragE;}//overloaded function
private:
  Double_t fPosZ;
  Double_t ftof[4];
  Double_t fbeamE[4];
  Double_t ffragE[4];
  TArtStoreManager* sman;
  ClassDef(TArtEasyTarget,1);
};

#endif
