/*
This privides event mixing scheme

*/

#ifndef EASY_EVENT_MIXING_HH
#define EASY_EVENT_MIXING_HH

#include <vector>
#include <TObject.h>

class TLorentzVector;
class TH1;
class TObject;

class TArtEasyEventMixing : public TObject{
public:
  TArtEasyEventMixing();
  TArtEasyEventMixing(std::vector<TLorentzVector> part1, std::vector<TLorentzVector> part2,Double_t bins,Double_t Erelmax);
  void LoadData(std::vector<TLorentzVector> part1, std::vector<TLorentzVector> part2);
  void ReconstructData();
  void SetErelMax(Double_t val){fErelmax=val;finit_erelmax=true;}
  void SetNbins(Double_t val){fnbins=val;finit_nbins=true;}
  Double_t Mix();
  void DoIteration();
  Int_t GetNumVirtualPairs();
  Double_t GetRelativeEnergy();
  Double_t GetWeight();
  Int_t GetNumPairs(){return fneve;}
  Bool_t GetNextVirtualPair();
  Bool_t GetNextPair();
  Int_t GetCounterFrag(){return fcounter_frag;}
  Int_t GetCounterNeut(){return fcounter_neut;}
  void MakeCorrelatedSpectrum();
  void MakeUncorrelatedSpectrum(Int_t i);
  void CalcCorrelationFunction(Int_t i);
  void MakeCorrelationMatrix(Int_t i);
  void IterationOfMeanCorrelation();
  void MakeInterpolation(TH1* hist, const Double_t ValueToCheck, Double_t &MeanValue, Double_t& MeanValueError);
  void InitCounter(){
    fcounter_frag=0;
    fcounter_neut=0;
    fcounter=0;
  }
  void ShowStatus(Int_t now, Int_t num,TString str);

private:
  std::vector<TLorentzVector> ffragtable;
  std::vector<TLorentzVector> fneuttable;
  std::vector<Double_t> fCm_frag;
  std::vector<Double_t> fCm_neut;
  std::vector<Double_t> fweightmatrix;
  std::vector<Double_t> ferrorweight;
  std::vector<Int_t> fignored_frag; // ignored neutrons for each fragment
  std::vector<Int_t> fignored_neut; // ignored fragments for each neutron
  std::vector<TH1*> fhcorrfunc;
  TH1* fherel;
  std::vector<TH1*> fhereluncor;
  Double_t fweight;
  Double_t ferror;
  Double_t fnbins;
  Double_t fErelmax;
  Int_t fneve;
  Int_t fcounter_frag;//
  Int_t fcounter_neut;//
  Int_t fcounter;//
  Bool_t finit,finit_erelmax,finit_nbins;//
  ClassDef(TArtEasyEventMixing,1);
};
#endif
