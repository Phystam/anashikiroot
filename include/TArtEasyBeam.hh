
// Easy beam tracking class
// by T.Tomai
// modified on 3 May.
// FDC1,2 をEasySAMURAIPIDに移動
// totalmodeを廃止

#ifndef _TARTEASYBEAM_H_
#define _TARTEASYBEAM_H_

#include <TNamed.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "TArtShikiParameters.hh"
#include "TArtReconstruction.hh"
class TArtSAMURAIParameters;
class TArtCalibDC;
class TArtDCHit;
class TArtDCTrack;
class TArtCalibBDC1Hit;
class TArtCalibBDC1Track;
class TArtCalibBDC2Hit;
class TArtCalibBDC2Track;
class TF1;
class TCanvas;
class TLorentzVector;
class TVector3;
class TArtStoreManager;
class TString;
class TArtShikiParameters;
class TArtEasyTarget;
class TArtEasyBeam : public TArtReconstruction{


private:
  TArtEasyBeam();
public:
  virtual ~TArtEasyBeam();
  static TArtEasyBeam* Instance();
  //  void Initialize();
  void ClearData();
  void ReconstructData();
  void SetTDCDistribution(TString filename);
  void SelectTarget(TString targetname);
  void TDCmode(Bool_t val=true){fTDCmode=val;fBeammode=false;}
  void Beammode(Bool_t val=true){fBeammode=val;}

  Bool_t IsTDCmode(){return fTDCmode;}
  Bool_t IsBeammode(){return fBeammode;}
  Bool_t IsInTgt(){return fintgt;}
  Double_t GetBDC1X(){return fBDC1X;}// unit: mm
  Double_t GetBDC1Y(){return fBDC1Y;}// unit: mm
  Double_t GetBDC1Z(){return fBDC1Z;}// unit: mm
  Double_t GetBDC2X(){return fBDC2X;}// unit: mm
  Double_t GetBDC2Y(){return fBDC2Y;}// unit: mm
  Double_t GetBDC2Z(){return fBDC2Z;}// unit: mm
  Double_t GetTgtX(){return fTgtX;}  // unit: mm
  Double_t GetTgtY(){return fTgtY;}  // unit: mm
  Double_t GetTgtZ(){return fTgtZ;}  // unit: mm
  TVector3 GetTgtPos(){return fTgtPos;}  // unit: mm
  Double_t GetBDC1A(){return fBDC1A;}// unit: mrad
  Double_t GetBDC1B(){return fBDC1B;}// unit: mrad
  Double_t GetBDC2A(){return fBDC2A;}// unit: mrad
  Double_t GetBDC2B(){return fBDC2B;}// unit: mrad
  Double_t GetTgtA(){return fTgtA;}  // unit: mrad
  Double_t GetTgtB(){return fTgtB;}  // unit: mrad
  Double_t GetBDC1XChi2(){return fBDC1XChi2;}
  Double_t GetBDC2XChi2(){return fBDC2XChi2;}
  Double_t GetBDC1YChi2(){return fBDC1YChi2;}
  Double_t GetBDC2YChi2(){return fBDC2YChi2;}
  // Double_t GetBDC1TDC(Int_t id){return fBDC1TDC[id];}
  // Double_t GetBDC2TDC(Int_t id){return fBDC2TDC[id];}
  // Double_t GetFDC1TDC(Int_t id){return fFDC1TDC[id];}
  TF1* GetTrackX(){return ftrackX;}
  TF1* GetTrackY(){return ftrackY;}

  Double_t GetBDC1PosLayer   (Int_t ilayer){return fBDC1PosLayer[ilayer];}
  Double_t GetBDC2PosLayer   (Int_t ilayer){return fBDC2PosLayer[ilayer];}
  Double_t GetBDC1DriftLength(Int_t ilayer){return fBDC1DriftLength[ilayer];}
  Double_t GetBDC2DriftLength(Int_t ilayer){return fBDC2DriftLength[ilayer];}
  Double_t GetBDC1Residual   (Int_t ilayer){return fBDC1Residual[ilayer];}
  Double_t GetBDC2Residual   (Int_t ilayer){return fBDC2Residual[ilayer];}
  Int_t    GetBDC1TDCLayer   (Int_t ilayer){return fBDC1TDCLayer[ilayer];}
  Int_t    GetBDC2TDCLayer   (Int_t ilayer){return fBDC2TDCLayer[ilayer];}



  void SetBDC1X(Double_t val){fBDC1X=val;}
  void SetBDC1Y(Double_t val){fBDC1Y=val;}
  void SetBDC1Z(Double_t val){fBDC1Z=val;}
  void SetBDC2X(Double_t val){fBDC2X=val;}
  void SetBDC2Y(Double_t val){fBDC2Y=val;}
  void SetBDC2Z(Double_t val){fBDC2Z=val;}
  void SetTgtX (Double_t val){fTgtX =val;}
  void SetTgtY (Double_t val){fTgtY =val;}
  void SetTgtZ (Double_t val){fTgtZ =val;}
  void SetBDC1A(Double_t val){fBDC1A=val;}
  void SetBDC1B(Double_t val){fBDC1B=val;}
  void SetBDC2A(Double_t val){fBDC2A=val;}
  void SetBDC2B(Double_t val){fBDC2B=val;}
  void SetTgtA (Double_t val){fTgtA =val;}
  void SetTgtB (Double_t val){fTgtB =val;}
  void SetBDC1XChi2(Double_t val){fBDC1XChi2=val;}
  void SetBDC2XChi2(Double_t val){fBDC2XChi2=val;}
  void SetBDC1YChi2(Double_t val){fBDC1YChi2=val;}
  void SetBDC2YChi2(Double_t val){fBDC2YChi2=val;}

  void SetBDC1PosLayer(Int_t ilayer, Double_t val){fBDC1PosLayer[ilayer]=val;}
  void SetBDC2PosLayer(Int_t ilayer, Double_t val){fBDC2PosLayer[ilayer]=val;}

  void SetBDC1DriftLength(Int_t ilayer, Double_t val){fBDC1DriftLength[ilayer]=val;}
  void SetBDC2DriftLength(Int_t ilayer, Double_t val){fBDC2DriftLength[ilayer]=val;}

  void SetBDC1Residual(Int_t ilayer, Double_t val){fBDC1Residual[ilayer]=val;}
  void SetBDC2Residual(Int_t ilayer, Double_t val){fBDC2Residual[ilayer]=val;}

  // function to fill data container
  // void SetBDC1Hit();
  // void SetBDC2Hit();
  // void SetFDC1Hit();
  // void SetFDC2Hit();
  // void SetBDC1Track();
  // void SetBDC2Track();
  // void SetFDC1Track();
  // void SetFDC2Track();
  // function to access data container
  std::vector<TArtDCHit*>   GetBDC1Hits  (){return fbdc1hits;}
  std::vector<TArtDCHit*>   GetBDC2Hits  (){return fbdc2hits;}
  std::vector<TArtDCTrack*> GetBDC1Tracks(){return fbdc1tracks;}
  std::vector<TArtDCTrack*> GetBDC2Tracks(){return fbdc2tracks;}
  Int_t GetNumBDC1Hits  (){return (Int_t)fbdc1hits.size();}
  Int_t GetNumBDC2Hits  (){return (Int_t)fbdc2hits.size();}
  Int_t GetNumBDC1Tracks(){return (Int_t)fbdc1tracks.size();}
  Int_t GetNumBDC2Tracks(){return (Int_t)fbdc2tracks.size();}

  TLorentzVector GetMomentumVector(Double_t totalmomentum, Double_t totalenergy);
  TVector3 GetTgtPosition();

  std::vector<TArtDCHit*>   fbdc1hits;
  std::vector<TArtDCHit*>   fbdc2hits;
  std::vector<TArtDCTrack*> fbdc1tracks;
  std::vector<TArtDCTrack*> fbdc2tracks;

 private:
  TCanvas* fc1;
  TCanvas* fc2;
  // pointer to subsystem objects used for definition of beam
  TArtCalibBDC1Hit* fcalibbdc1hit;
  TArtCalibBDC2Hit* fcalibbdc2hit;
  TArtCalibBDC1Track* fcalibbdc1track;
  TArtCalibBDC2Track* fcalibbdc2track;

  //variables
  Double_t fBDC1X;
  Double_t fBDC1Y;
  Double_t fBDC1Z;
  Double_t fBDC2X;
  Double_t fBDC2Y;
  Double_t fBDC2Z;
  Double_t fTgtX;
  Double_t fTgtY;
  Double_t fTgtZ;
  TVector3 fTgtPos;
  Double_t fBDC1A;
  Double_t fBDC1B;
  Double_t fBDC2A;
  Double_t fBDC2B;
  Double_t fTgtA;
  Double_t fTgtB;
  Double_t fBDC1XChi2;
  Double_t fBDC2XChi2;
  Double_t fBDC1YChi2;
  Double_t fBDC2YChi2;
  
  Double_t fBDC1Xoffset;
  Double_t fBDC1Yoffset;
  Double_t fBDC2Xoffset;
  Double_t fBDC2Yoffset;

  Double_t fBDC1PosLayer[8] ;
  Double_t fBDC2PosLayer[8] ;

  Double_t fBDC1Residual[8] ;
  Double_t fBDC2Residual[8] ;
  Double_t fBDC1DriftLength[8] ;
  Double_t fBDC2DriftLength[8] ;

  Int_t fBDC1TDCLayer[8] ;
  Int_t fBDC2TDCLayer[8] ;


  TF1* ftrackX;
  TF1* ftrackY;
  // vector<Int_t> fBDC1TDC;
  // vector<Int_t> fBDC2TDC;
  // vector<Int_t> fFDC1TDC;
  Bool_t fBDC1_reconstructed;
  Bool_t fBDC2_reconstructed;
  Bool_t fTDCmode;
  Bool_t fBeammode;
  Bool_t fintgt;
  TArtStoreManager* sman;
  const TArtShikiParameters* fshikipara;
  TArtEasyTarget* feasytarget;
  // Double_t fclight=2.99792458e8;// m/s
  // Double_t fe=1.6021766208e-19;// C
  // Double_t famu=1.660538921e-27;// kg

  ClassDef(TArtEasyBeam,1);


};

#endif
