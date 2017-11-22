
// BigRIPS/SAMURAI reconstruction class
// by T.Tomai
// this is a singleton class

#ifndef _TARTEASYPID_H_
#define _TARTEASYPID_H_

#include <TNamed.h>
#include <TClonesArray.h>
#include <TMath.h>

#include "TArtReconstruction.hh"

class TArtBigRIPSParameters;
class TArtCalibPlastic;
class TArtPlastic;
class TArtCalibIC;
class TArtIC;
class TArtStoreManager;
class TArtShikiParameters;
class TArtEasyTarget;
class TArtEasyMassExcess;
class TArtEasyEnergyLossFunc;
class TArtEasyPID : public TArtReconstruction{

private:

  TArtEasyPID(Double_t Brho0 = 7.6478);
  //TArtEasyPID();

public:
  virtual ~TArtEasyPID();
  //シングルトンアクセス用
  static TArtEasyPID* Instance(Double_t Brho0 = 7.6478);
  //  void Initialize();
  void ClearData();
  void ReconstructData();
  Bool_t IsRejected(){return frejected_f3||frejected_f5||frejected_f7||frejected_tof;}
  Bool_t IsRejected_F3(){return frejected_f3;}
  Bool_t IsRejected_F5(){return frejected_f5;}
  Bool_t IsRejected_F7(){return frejected_f7;}
  Bool_t IsRejected_TOF(){return frejected_tof;}
  Double_t GetBrho0(){return fBrho0;} 
  Double_t GetBrho(){return fBrho;} 
  Double_t GetTOF713(){return fTOF713;} 
  Double_t GetTOF37(){return fTOF37;} 
  Double_t GetDeltaE(){return fDeltaE;} 
  Double_t GetBeta(){return fBeta;}
  Double_t GetGamma(){return fGamma;}
  Double_t GetZet(){return fZ;}
  Double_t GetAoZ(){return fAoZ;}
  Int_t GetZetInt(){return TMath::Nint(fZ);}
  Int_t GetAInt(){return TMath::Nint(fAoZ*GetZetInt());}
  Double_t GetMass();
  Double_t GetSBTTime(){return ff13Time;}
  //calculation of energy or momentum
  //From TOF
  //TotalEnergy: MeV
  //TotalKineticEnergy: MeV
  //KineticEnergy: MeV/u (not AMeV!!!)
  //TotalMomentum: MeV/c
  //Momentum: MeV/c/u
  Double_t GetTotalEnergyFromTOF();//calc from mass
  Double_t GetTotalKineticEnergyFromTOF();//calc from mass
  Double_t GetKineticEnergyFromTOF();//calc from mass
  Double_t GetKineticEnergyAtTgtFromTOF();//calc from mass
  Double_t GetTotalKineticEnergyAtTgtFromTOF();//calc from mass
  Double_t GetTotalEnergyAtTgtFromTOF();//calc from mass

  Double_t GetTotalMomentumFromTOF();//calc from mass
  Double_t GetMomentumFromTOF();//calc from mass
  Double_t GetMomentumAtTgtFromTOF();//calc from mass
  Double_t GetTotalMomentumAtTgtFromTOF();//calc from mass

  //  Double_t GetTotalEnergy(Int_t A=fA_for_calc,Int_t Z=fZ_for_calc);//calc from beta
  Double_t GetTotalMomentumFromBrho();
  Double_t GetMomentumFromBrho();
  Double_t GetMomentumAtTgtFromBrho();
  Double_t GetTotalMomentumAtTgtFromBrho();

  Double_t GetTotalEnergyFromBrho();
  Double_t GetTotalKineticEnergyFromBrho();
  Double_t GetKineticEnergyFromBrho();
  Double_t GetTotalKineticEnergyAtTgtFromBrho();
  Double_t GetKineticEnergyAtTgtFromBrho();
  Double_t GetTotalEnergyAtTgtFromBrho();
  //alias
  Double_t GetTotalEnergy            (){return GetTotalEnergyFromTOF            ();}
  Double_t GetTotalKineticEnergy     (){return GetTotalKineticEnergyFromTOF     ();}
  Double_t GetKineticEnergy          (){return GetKineticEnergyFromTOF          ();}
  Double_t GetKineticEnergyAtTgt     (){return GetKineticEnergyAtTgtFromTOF     ();}
  Double_t GetTotalKineticEnergyAtTgt(){return GetTotalKineticEnergyAtTgtFromTOF();}
  Double_t GetTotalEnergyAtTgt       (){return GetTotalEnergyAtTgtFromTOF       ();}

  Double_t GetTotalMomentum          (){return GetTotalMomentumFromTOF          ();}
  Double_t GetMomentum               (){return GetMomentumFromTOF               ();}
  Double_t GetMomentumAtTgt          (){return GetMomentumAtTgtFromTOF          ();}
  Double_t GetTotalMomentumAtTgt     (){return GetTotalMomentumAtTgtFromTOF     ();}

  Double_t E2Tgt(Double_t E);
  Double_t GetTOFSBTTgt();
  void SetNuclide(Int_t a,Int_t z, Double_t mass_ex){
    fA_for_calc = a;
    fZ_for_calc = z;
    fEx_for_calc = mass_ex;
  }
  void SetBrho0(Double_t val){fBrho0=val;} 
  void SetBrho(Double_t val){fBrho=val;} 
  void SetTOF713(Double_t val){fTOF713=val;} 
  void SetTOF37(Double_t val){fTOF37=val;} 
  void SetDeltaE(Double_t val){fDeltaE=val;} 
  void SetBeta(Double_t val){fBeta=val;}
  void SetGamma(Double_t val){fGamma=val;}
  void SetZet(Double_t val){fZ=val;}
  void SetAoZ(Double_t val){fAoZ=val;}

  void SetPlastic(Int_t index,TArtPlastic* pl){fpla[index]=pl;}
  void SetICB(TArtIC* ic){ficb=ic;}
  TArtPlastic* GetPlastic(Int_t id){return fpla[id-1];}
  TArtIC* GetICB(){return ficb;}
  // function to access data container

  void SelectTarget(TString name);
  void SelectTarget(Int_t ID);
 private:

  // pointer to subsystem objects used for PID
  TArtCalibPlastic* fcalibpla;
  TArtCalibIC* fcalibic;

  TArtPlastic* fpla[7];  
  TArtIC* ficb;

  Double_t fBrho0;
  Double_t fBrho;
  Double_t fTOF37;
  Double_t fTOF713;
  Double_t fDeltaE;
  Double_t fBeta;
  Double_t fGamma;
  Double_t fZ;
  Double_t fAoZ;
  Double_t ff13Time;
  Bool_t frejected_f3,frejected_f5,frejected_f7,frejected_tof;
  Int_t fA_for_calc;
  Int_t fZ_for_calc;
  Double_t fEx_for_calc;
  Bool_t freconstructed;
  TArtStoreManager* sman;
  const TArtShikiParameters* fshikipara;
  TArtEasyTarget* feasytarget;
  TArtEasyMassExcess* feasyex;
  TArtEasyEnergyLossFunc* feasyeloss;
  static const Double_t fclight=2.99792458e8;// m/s
  static const Double_t fe=1.6021766208e-19;// C
  static const Double_t famu=1.660538921e-27;// kg
  static const Double_t famu_MeV=931.494061;//MeV

  ClassDef(TArtEasyPID,1);

};

#endif
