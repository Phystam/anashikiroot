#ifndef _TARTEASYNEUTRON_HH_
#define _TARTEASYNEUTRON_HH_

#include "TArtDataObject.hh"
#include "TArtCore.hh"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <iostream>
class TObject;
class TVector3;
class TLorentzVector;
class TArtEasyNeutron : public TArtDataObject {
public:
  TArtEasyNeutron()
    : TArtDataObject(),
      fLayer(-1), fSubLayer(-1), fID(-1),
      fT1Raw(-1),fT2Raw(-1),fT1Cal(-1),fT2Cal(-1),fT1Slw(-1),fT2Slw(-1),
      fTAveCal(-1),fTAveSlw(-1),
      fQ1Raw(-1),fQ2Raw(-1),fQ1Ped(-1),fQ2Ped(-1),fQ1Cal(-1),fQ2Cal(-1),
      fQAvePed(-1),fQAveCal(-1),
      fDTCal(-9999),fDTSlw(-9999), fTOF(-9999), fTOFRaw(-9999), fFL(-9999),
      fBeta(-1),fGamma(-1),fEnergy(-1),fMomentum(-1)
  {
    fPos.SetX(-1);
    fPos.SetY(-1);
    fPos.SetZ(-1);
    fMomentum4D.SetPx(-1);
    fMomentum4D.SetPy(-1);
    fMomentum4D.SetPz(-1);
    fMomentum4D.SetE(-1);
    fMass = 939.565;//MeV/c^2
    fclight = 299.792458;//mm/ns
  }
  virtual ~TArtEasyNeutron(){;}

  //set variables
  void SetIsVeto (Bool_t val)  { fIsVeto  = val;}
  void SetLayer  (Int_t val)   { fLayer   = val;}
  void SetSubLayer(Int_t val)  { fSubLayer= val;}
  virtual void SetID(Int_t val)  { fID      = val;}
  void SetT1Raw  (Double_t val){ fT1Raw   = val;}
  void SetT2Raw  (Double_t val){ fT2Raw   = val;}
  void SetT1Cal  (Double_t val){ fT1Cal   = val;}//ns
  void SetT2Cal  (Double_t val){ fT2Cal   = val;}//ns
  void SetT1Slw  (Double_t val){ fT1Slw   = val;}//ns
  void SetT2Slw  (Double_t val){ fT2Slw   = val;}//ns
  void SetTAveCal(Double_t val){ fTAveCal = val;}//ns
  void SetTAveSlw(Double_t val){ fTAveSlw = val;}//ns
  void SetQ1Raw  (Double_t val){ fQ1Raw   = val;}
  void SetQ2Raw  (Double_t val){ fQ2Raw   = val;}
  void SetQ1Ped  (Double_t val){ fQ1Ped   = val;}
  void SetQ2Ped  (Double_t val){ fQ2Ped   = val;}
  void SetQ1Cal  (Double_t val){ fQ1Cal   = val;}//MeVee
  void SetQ2Cal  (Double_t val){ fQ2Cal   = val;}//MeVee
  void SetQAvePed(Double_t val){ fQAvePed = val;}
  void SetQAveCal(Double_t val){ fQAveCal = val;}//MeVee
  void SetDTCal  (Double_t val){ fDTCal   = val;}//ns
  void SetDTSlw  (Double_t val){ fDTSlw   = val;}//ns
  void SetTOF    (Double_t val){ fTOF     = val;}
  void SetTOFRaw (Double_t val){ fTOFRaw  = val;}
  void SetFlightLength(Double_t val){ fFL     = val;}
  void SetBeta   (Double_t val){ fBeta    = val;}
  void SetGamma  (Double_t val){ fGamma   = val;}
  void SetEnergy (Double_t val){ fEnergy  = val;}//MeV
  void SetMomentum(Double_t val){ fMomentum  = val;}//MeV/c
  void SetPos    (Double_t x, Double_t y, Double_t z){
    fPos.SetX(x);
    fPos.SetY(y);
    fPos.SetZ(z);
  }
  void SetMomentum4D(Double_t px, Double_t py, Double_t pz, Double_t E){
    fMomentum4D.SetPx(px);
    fMomentum4D.SetPy(py);
    fMomentum4D.SetPz(pz);
    fMomentum4D.SetE(E);
  }
  void SetMomentum4D(TLorentzVector momentum){
    fMomentum4D=momentum;
  }

  //get variables
  Bool_t IsVeto    (){ return fIsVeto ;}
  Int_t    GetLayer  (){ return fLayer;}
  virtual Int_t    GetID     (){ return fID;}
  Int_t    GetSubLayer(){return fSubLayer;}
  Double_t GetT1Raw  () const { return fT1Raw  ;}
  Double_t GetT2Raw  () const { return fT2Raw  ;}
  Double_t GetT1Cal  () const { return fT1Cal  ;}//ns
  Double_t GetT2Cal  () const { return fT2Cal  ;}//ns
  Double_t GetT1Slw  () const { return fT1Slw  ;}//ns
  Double_t GetT2Slw  () const { return fT2Slw  ;}//ns
  Double_t GetTAveCal() const { return fTAveCal;}//ns
  Double_t GetTAveSlw() const { return fTAveSlw;}//ns
  Double_t GetQ1Raw  () const { return fQ1Raw  ;}
  Double_t GetQ2Raw  () const { return fQ2Raw  ;}
  Double_t GetQ1Ped  () const { return fQ1Ped  ;}
  Double_t GetQ2Ped  () const { return fQ2Ped  ;}
  Double_t GetQ1Cal  () const { return fQ1Cal  ;}//MeVee
  Double_t GetQ2Cal  () const { return fQ2Cal  ;}//MeVee
  Double_t GetQAvePed() const { return fQAvePed;}
  Double_t GetQAveCal() const { return fQAveCal;}//MeVee
  Double_t GetDTCal  () const { return fDTCal  ;}//ns
  Double_t GetDTSlw  () const { return fDTSlw  ;}//ns
  Double_t GetBeta   () const { return fBeta   ;}
  Double_t GetTOF    () const { return fTOF    ;}
  Double_t GetTOFRaw () const { return fTOFRaw ;}
  Double_t GetFlightLength    () const { return fFL    ;}
  Double_t GetTOFGamma()const { return fTOF - fFL/fclight;}//ns, for gamma-ray calibration
  Double_t GetTOFRawGamma()const { return fTOFRaw - fFL/fclight;}//ns, for gamma-ray calibration
  Double_t GetGamma  () const { return fGamma  ;}
  Double_t GetEnergy () const { return fEnergy ;}//MeV
  Double_t GetKineticEnergy (){ return fEnergy-fMass ;}//MeV
  Double_t GetMomentum(){ return fMomentum;}//MeV/c
  TVector3 GetPos(){return fPos;}
  TLorentzVector GetMomentum4D(){return fMomentum4D;}
private:
  Bool_t   fIsVeto ;
  Int_t fLayer;
  Int_t fSubLayer;
  Int_t fID;
  Double_t fT1Raw  ;
  Double_t fT2Raw  ;
  Double_t fT1Cal  ;//ns
  Double_t fT2Cal  ;//ns
  Double_t fT1Slw  ;//ns
  Double_t fT2Slw  ;//ns
  Double_t fTAveCal;//ns
  Double_t fTAveSlw;//ns
  Double_t fQ1Raw  ;
  Double_t fQ2Raw  ;
  Double_t fQ1Ped  ;
  Double_t fQ2Ped  ;
  Double_t fQ1Cal  ;//MeVee
  Double_t fQ2Cal  ;//MeVee
  Double_t fQAvePed;
  Double_t fQAveCal;//MeVee

  Double_t fDTCal  ;//ns
  Double_t fDTSlw  ;//ns
  Double_t fTOF    ;//ns
  Double_t fTOFRaw ;//ns
  Double_t fFL    ;//mm
  Double_t fBeta   ;
  Double_t fGamma  ;
  Double_t fEnergy ;//MeV
  Double_t fKineticEnergy;//MeV
  Double_t fMomentum;//MeV/c
  TVector3 fPos;//mm
  TLorentzVector fMomentum4D;
  Double_t fMass;//MeV/c^2
  Double_t fclight;//mm/ns
public:
  // overriding functions for sorting based on TOF
  Bool_t IsEqual(TObject *obj) const { return fTOF == ((TArtEasyNeutron*)obj)->GetTOF();} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fTOF < ((TArtEasyNeutron*)obj)->GetTOF()) return 1; 
    else if (fTOF > ((TArtEasyNeutron*)obj)->GetTOF()) return -1; 
    else return 0; 
  }

  //比較演算子のオーバーロード、ソート用
  static bool Comparing(TArtEasyNeutron left,TArtEasyNeutron right) {
    if(isnan(right.fTOF)){
      return true;
    }
    if(isnan(left.fTOF)){
      return false;
    }
    std::cout <<"comparing< "<<left.fTOF<<" and "<<right.fTOF <<std::endl;
    std::cout <<" result-> "<< (left.fTOF < right.fTOF) <<std::endl;
    return left.fTOF < right.fTOF;
  }

  ClassDef(TArtEasyNeutron,1);

};

#endif
