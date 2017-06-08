#ifndef _TARTHODPLA_H_
#define _TARTHODPLA_H_

#include "TArtDataObject.hh"
#include <TMath.h>
class TArtHODPla : public TArtDataObject {

 public:
  TArtHODPla(){fTURaw=0;fTDRaw=0;fQURaw=0;fQDRaw=0;fTAveCal=-1;fTUCal=-1;fTDCal=-1;fTUSlw=-1;fTDSlw=-1;fTAveSlw=-1;}
  virtual ~TArtHODPla(){}

  // function to set Q data
  void  SetTURaw(Int_t val){fTURaw = val;}
  void  SetTDRaw(Int_t val){fTDRaw = val;}
  void  SetQURaw(Int_t val){fQURaw = val;}
  void  SetQDRaw(Int_t val){fQDRaw = val;}
  void  SetQUPed(Double_t val){fQUPed = val;} 
  void  SetQDPed(Double_t val){fQDPed = val;} 
  void  SetQUCal(Double_t val){fQUCal = val;}
  void  SetQDCal(Double_t val){fQDCal = val;}
  void  SetQAveRaw(Double_t val){fQAveRaw = val;}
  void  SetQAvePed(Double_t val){fQAvePed = val;}
  void  SetQAveCal(Double_t val){fQAveCal = val;}
  
  // function to get Q data
  Int_t GetTURaw(){return fTURaw;}
  Int_t GetTDRaw(){return fTDRaw;}
  Int_t GetQURaw(){return fQURaw;}
  Int_t GetQDRaw(){return fQDRaw;}
  Double_t GetQUPed(){return fQUPed;} 
  Double_t GetQDPed(){return fQDPed;} 
  Double_t GetQUCal(){return fQUCal;}
  Double_t GetQDCal(){return fQDCal;}
  Double_t GetQAveRaw(){return fQAveRaw;}
  Double_t GetQAvePed(){return fQAvePed;}
  Double_t GetQAveCal(){return fQAveCal;}
 
  // get reconstructed values
  virtual void SetTAveCal(Double_t val){fTAveCal=val;}
  virtual void SetTUCal(Double_t val){fTUCal=val;}
  virtual void SetTDCal(Double_t val){fTDCal=val;}
  virtual void SetTUSlw(Double_t val){fTUSlw=val;}
  virtual void SetTDSlw(Double_t val){fTDSlw=val;}
  virtual void SetTAveSlw (Double_t val){fTAveSlw=val;}
  virtual void SetZCoef(Int_t i,Double_t val){zcoef[i]=val;}//i=0-2
  // get reconstructed values
  Double_t GetTAveCal(){return fTAveCal;}
  Double_t GetTUCal(){return fTUCal;}
  Double_t GetTDCal(){return fTDCal;}
  Double_t GetTUSlw(){return fTUSlw;}
  Double_t GetTDSlw(){return fTDSlw;}
  Double_t GetTAveSlw (){return fTAveSlw;}

  virtual Double_t GetZet(double beta){
    Double_t I=6.47e-2;//keV; for plastic
    Double_t me = 511;//keV; mass of electron
    Double_t ionpair = 2.*me/I;
    double de_v = TMath::Log(ionpair*beta*beta) - TMath::Log((1-beta*beta)) - beta*beta;
    //    std::cout << zcoef[0] <<" " << zcoef[1] << " "<<zcoef[2] << std::endl;
    //    Double_t quenting_subt=*beta;
    //   return zcoef[0]*(TMath::Sqrt(fQAveCal/de_v)*beta-quenting_subt) + zcoef[1];
    Double_t dev_corr=TMath::Sqrt(fQAveCal/de_v)*beta - zcoef[3]*beta;
    return zcoef[2]*dev_corr*dev_corr+zcoef[1]*dev_corr + zcoef[0];
  }
 private:

  // Q data
  Int_t fTURaw;  
  Int_t fTDRaw;
  Int_t fQURaw;  
  Int_t fQDRaw;
  Double_t     fQUPed;
  Double_t     fQDPed;
  Double_t     fQUCal;
  Double_t     fQDCal;
  Double_t     fQAveRaw;
  Double_t     fQAvePed;
  Double_t     fQAveCal;

  // reconstructed data
  Double_t   fTAveCal;
  Double_t   fTUCal;
  Double_t   fTDCal;
  Double_t   fTUSlw; // after slewing correction
  Double_t   fTDSlw;
  Double_t   fTAveSlw;
  Double_t   zcoef[4];
 public:
  // overriding functions for sorting based on Q
  Bool_t IsEqual(TObject *obj) const { return fQAveCal == ((TArtHODPla*)obj)->fQAveCal;} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fQAveCal < ((TArtHODPla*)obj)->fQAveCal) return 1; 
    else if (fQAveCal > ((TArtHODPla*)obj)->fQAveCal) return -1; 
    else return 0; 
  }

  ClassDef(TArtHODPla,1);

};
#endif
