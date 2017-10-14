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
  TArtEasyEventMixing(std::vector<TLorentzVector> part1, std::vector<TLorentzVector> part2);
  void LoadData(std::vector<TLorentzVector> part1, std::vector<TLorentzVector> part2);
  void SetErelMax(Double_t val){fErelmax=val;}
  Double_t Mix();
  void DoIteration();
  Int_t GetNumVirtualPairs();
  Double_t GetRelativeEnergy();
  Int_t GetNumPairs(){return fneve;}
  Bool_t GetNextVirtualPair();
  void CalcCorrelationFunction();

private:
  void InitCounter(){
    fcounter_frag=0;
    fcounter_neut=0;
  }
  std::vector<TLorentzVector> ffragtable;
  std::vector<TLorentzVector> fneuttable;
  std::vector<Double_t> fCm_frag;
  std::vector<Double_t> fCm_feut;
  TH1* fhcorrfunc;
  Double_t fweight;
  Double_t ferror;
  Double_t fErelmax;
  Int_t fneve;
  Int_t fcounter_frag;//
  Int_t fcounter_neut;//
  ClassDef(TArtEasyEventMixing,1);
};
#endif
