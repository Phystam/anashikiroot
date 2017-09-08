
// SAMURAI PID reconstruction class
// by T. tomai

#ifndef _TARTEASYSAMURAIPID_H_
#define _TARTEASYSAMURAIPID_H_

#include <TNamed.h>
#include <TClonesArray.h>
#include <vector>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TMath.h>
//#include "TArtMDF_s027_BrhoMDF.hh"
//#include "TArtMDF_s027_BrhoMDF.hh"
//#include "TArtMDF_s027_BrhoMDF.hh"

#include "TArtReconstruction.hh"
class TArtStoreManager;
class TArtCalibHODPla;
class TArtMDF_s027_BrhoMDF;
class TArtMDF_s027_TofMDF;
class TArtMDF_s027_FlMDF;
class TArtDCHit;
class TArtDCTrack;
class TArtCalibFDC1Hit;
class TArtCalibFDC1Track;
class TArtCalibFDC2Hit;
class TArtCalibFDC2Track;
class TArtEasyPID;
class TArtEasyBeam;
class TArtEasyTarget;
class TArtShikiParameters;
class TLorentzVector;
class TVector3;
class TArtEasyMassExcess;
class TArtEasyEnergyLossFunc;

class TArtEasySAMURAIPID : public TArtReconstruction{

public:

  TArtEasySAMURAIPID();
  virtual ~TArtEasySAMURAIPID();

  //Drift chamber reconstruction

  void SetTDCDistribution(TString filename);
  void TDCmode(Bool_t val=true){fTDCmode=val;}
  void FDCmode(){fTDCmode=false;}

  void ClearData();
  void LoadData();
  void ReconstructData();
  void FDCReconstructData();
  void CalcXYCorrectionOfHODF();
  void RunCorrections();

  void SelectTarget(TString name);
  void SelectTarget(Int_t ID);
  Double_t GetBrho(){return fBrho;}
  //  Double_t GetTOFTgtHODSimulated(){return fTOFSim;}
  Double_t GetTOFSBTTgt(){return fTOFSBTTgt;}
  Double_t GetTOFTgtHOD(){return fTOFTgtHOD;}
  Double_t GetTOFSBTHOD(){return fTOFSBTHOD;}
  Double_t GetTOFSimTotal();
  Double_t GetTOFTgtHODSim();
  Double_t GetFlightLength(){return fFl;}
  Double_t GetDeltaE(){return fDeltaE;}
  Double_t GetBeta(){return fBeta;}
  Double_t GetGamma(){return fGamma;}
  Double_t GetZet(){return fZet;}
  Double_t GetAoZ(){return fAoZ;}
  Int_t    GetZetInt(){return TMath::Nint(fZet);}
  Int_t    GetAInt(){return TMath::Nint((Double_t)TMath::Nint(fZet)*fAoZ);}
  Double_t GetAoZInt(){return (Double_t)GetAInt()/GetZetInt();}
  Double_t GetTgtTime(){return ftgttime;}
  Double_t GetHODTime(){return fhodtime;}
  Double_t GetHODID(){return fhodid;}
  Double_t GetHODQ(){return fhodq;}
  Double_t GetHODX(){
    return (4954.17-4124.51)*tan(0.001*fFDC2A)+fFDC2X;
  }
  Double_t GetHODY(){
    return (4954.17-4124.51)*tan(0.001*fFDC2B)+fFDC2Y;
  }
  Double_t GetHODQUPed(){return fhodquped;}
  Double_t GetHODQDPed(){return fhodqdped;}
  Double_t GetHODTUSlw(){return fhodtu;}
  Double_t GetHODTDSlw(){return fhodtd;}
  Double_t GetKineticEnergy(){return fKE;}
  Double_t GetEnergy(){return fEnergy;}
  Double_t GetMomentum(){return fMomentum;}
  TLorentzVector GetMomentum4D(){return fMomentum4D;}
  TLorentzVector GetMomentum4DAtTgt();
  Double_t GetMass(){return fMomentum4D.M();}
  Double_t GetHODQPed(){return fhodqped;}
  Double_t GetNumHODPla(){return fnumhodpla;}
  Int_t SetRunNumber(Int_t val){frunnum=val;}
  Int_t GetRunNumber(){return frunnum;}


  void SetBrho  (Double_t val){fBrho = val;}
  void SetDeltaE(Double_t val){fDeltaE = val;}
  void SetBeta  (Double_t val){fBeta = val;}
  void SetGamma (Double_t val){fGamma = val;}
  void SetZet   (Double_t val){fZet = val;}
  void SetAoZ   (Double_t val){fAoZ = val;}

  //FDC
  Double_t GetFDC1X(){return fFDC1X;}// unit: mm
  Double_t GetFDC1Y(){return fFDC1Y;}// unit: mm
  Double_t GetFDC1Z(){return fFDC1Z;}// unit: mm
  TVector3 GetFDC1Pos(){return fFDC1Pos;}// unit: mm
  Double_t GetFDC2X(){return fFDC2X;}// unit: mm
  Double_t GetFDC2Y(){return fFDC2Y;}// unit: mm
  Double_t GetFDC2Z(){return fFDC2Z;}// unit: mm

  Double_t GetFDC1A(){return fFDC1A;}// unit: mrad
  Double_t GetFDC1B(){return fFDC1B;}// unit: mrad
  Double_t GetFDC2A(){return fFDC2A;}// unit: mrad
  Double_t GetFDC2B(){return fFDC2B;}// unit: mrad
  Double_t GetFDC1XChi2(){return fFDC1XChi2;}
  Double_t GetFDC2XChi2(){return fFDC2XChi2;}
  Double_t GetFDC1YChi2(){return fFDC1YChi2;}
  Double_t GetFDC2YChi2(){return fFDC2YChi2;}
  Double_t GetFDC1PosLayer   (Int_t ilayer){return fFDC1PosLayer[ilayer];}
  Double_t GetFDC2PosLayer   (Int_t ilayer){return fFDC2PosLayer[ilayer];}
  Double_t GetFDC1DriftLength(Int_t ilayer){return fFDC1DriftLength[ilayer];}
  Double_t GetFDC2DriftLength(Int_t ilayer){return fFDC2DriftLength[ilayer];}
  Double_t GetFDC1Residual   (Int_t ilayer){return fFDC1Residual[ilayer];}
  Double_t GetFDC2Residual   (Int_t ilayer){return fFDC2Residual[ilayer];}
  Int_t    GetFDC1TDCLayer   (Int_t ilayer){return fFDC1TDCLayer[ilayer];}
  Int_t    GetFDC2TDCLayer   (Int_t ilayer){return fFDC2TDCLayer[ilayer];}
  Double_t GetFDC2Residual   (Int_t ilayer,Int_t iwire){return fFDC2ResidualWire[ilayer][iwire];}
  void SetFDC1X(Double_t val){fFDC1X=val;}
  void SetFDC1Y(Double_t val){fFDC1Y=val;}
  void SetFDC1Z(Double_t val){fFDC1Z=val;}
  void SetFDC2X(Double_t val){fFDC2X=val;}
  void SetFDC2Y(Double_t val){fFDC2Y=val;}
  void SetFDC2Z(Double_t val){fFDC2Z=val;}
  void SetFDC1A(Double_t val){fFDC1A=val;}
  void SetFDC1B(Double_t val){fFDC1B=val;}
  void SetFDC2A(Double_t val){fFDC2A=val;}
  void SetFDC2B(Double_t val){fFDC2B=val;}
  void SetFDC1XChi2(Double_t val){fFDC1XChi2=val;}
  void SetFDC2XChi2(Double_t val){fFDC2XChi2=val;}
  void SetFDC1YChi2(Double_t val){fFDC1YChi2=val;}
  void SetFDC2YChi2(Double_t val){fFDC2YChi2=val;}
  void SetFDC1PosLayer(Int_t ilayer, Double_t val){fFDC1PosLayer[ilayer]=val;}
  void SetFDC2PosLayer(Int_t ilayer, Double_t val){fFDC2PosLayer[ilayer]=val;}
  void SetFDC1DriftLength(Int_t ilayer, Double_t val){fFDC1DriftLength[ilayer]=val;}
  void SetFDC2DriftLength(Int_t ilayer, Double_t val){fFDC2DriftLength[ilayer]=val;}
  void SetFDC1Residual(Int_t ilayer, Double_t val){fFDC1Residual[ilayer]=val;}
  void SetFDC2Residual(Int_t ilayer, Double_t val){fFDC2Residual[ilayer]=val;}
  


  std::vector<TArtDCHit*>   GetFDC1Hits  (){return ffdc1hits;}
  std::vector<TArtDCHit*>   GetFDC2Hits  (){return ffdc2hits;}
  std::vector<TArtDCTrack*> GetFDC1Tracks(){return ffdc1tracks;}
  std::vector<TArtDCTrack*> GetFDC2Tracks(){return ffdc2tracks;}
  Int_t GetNumFDC1Hits  (){return (Int_t)ffdc1hits.size();}
  Int_t GetNumFDC2Hits  (){return (Int_t)ffdc2hits.size();}
  Int_t GetNumFDC1Tracks(){return (Int_t)ffdc1tracks.size();}
  Int_t GetNumFDC2Tracks(){return (Int_t)ffdc2tracks.size();}

  TArtEasyPID* GetEasyPID(){return feasypid;}
  TArtEasyBeam* GetEasyBeam(){return feasybeam;}
private:
  Int_t frunnum;
  Double_t fBrho;
  Double_t fTOFTgtHOD;
  Double_t fTOFSim;
  Double_t fTOFSBTTgt;
  Double_t fTOFSBTHOD;
  Double_t fFl;
  Double_t fDeltaE;
  Double_t fBeta;
  Double_t fGamma;
  Double_t fZet;
  Double_t fAoZ;
  Double_t fKE;
  Double_t fMomentum;
  Double_t fEnergy;
  TLorentzVector fMomentum4D;
  Double_t fhodtime;
  Double_t ftgttime;
  Int_t fhodid;
  Double_t fhodq;
  Double_t fhodqped;
  Double_t fhodtu;
  Double_t fhodtd;
  Double_t fhodquped;
  Double_t fhodqdped;
  Int_t fnumhodpla;
  Double_t fsbttime;
  TArtEasyPID* feasypid;
  TArtEasyBeam* feasybeam;
  TArtEasyTarget* feasytarget;
  TArtShikiParameters* fshikipara;
  TArtStoreManager* sman;
  TArtCalibHODPla* fcalibhod;
  std::vector<TArtDCHit*>   ffdc1hits;
  std::vector<TArtDCHit*>   ffdc2hits;
  std::vector<TArtDCTrack*> ffdc1tracks;
  std::vector<TArtDCTrack*> ffdc2tracks;
  TArtCalibFDC1Hit* fcalibfdc1hit;
  TArtCalibFDC2Hit* fcalibfdc2hit;
  TArtCalibFDC1Track* fcalibfdc1track;
  TArtCalibFDC2Track* fcalibfdc2track;
  Double_t fFDC1X;
  Double_t fFDC1Y;
  Double_t fFDC1Z;
  TVector3 fFDC1Pos;
  Double_t fFDC2X;
  Double_t fFDC2Y;
  Double_t fFDC2Z;
  Double_t fFDC1A;
  Double_t fFDC1B;
  Double_t fFDC2A;
  Double_t fFDC2B;
  Double_t fFDC1XChi2;
  Double_t fFDC2XChi2;
  Double_t fFDC1YChi2;
  Double_t fFDC2YChi2;
  Double_t fFDC1Xoffset;
  Double_t fFDC1Yoffset;
  Double_t fFDC1PosLayer[14];
  Double_t fFDC2PosLayer[14];
  Double_t fFDC1Residual[14];
  Double_t fFDC2Residual[14];
  Double_t fFDC2ResidualWire[14][112];
  Double_t fFDC1DriftLength[14];
  Double_t fFDC2DriftLength[14];
  Int_t fFDC1TDCLayer[14];
  Int_t fFDC2TDCLayer[14];
  Bool_t fFDC1_reconstructed;
  Bool_t fFDC2_reconstructed;
  Bool_t fTDCmode;
  Bool_t fselectedtarget;
  TArtEasyMassExcess* feasyex;
  TArtEasyEnergyLossFunc* feasyeloss;
  static const Double_t fclight=299.792458;//mm/ns
  static const Double_t famu_MeV=931.494061;//MeV
  TArtMDF_s027_BrhoMDF* fbrhomdf;
  TArtMDF_s027_TofMDF* ftofmdf;
  TArtMDF_s027_FlMDF* fflmdf;
  ClassDef(TArtEasySAMURAIPID,1);
};
#endif
