#include "TArtEasyEventMixing.hh"
#include <TH1.h>
#include <vector>
#include <TLorentzVector.h>
#include "TArtCore.hh"

TArtEasyEventMixing::TArtEasyEventMixing(){
  InitCounter();
  fhcorrfunc = new TH1D("fhcorrfunc","Correlation function",fnbins,0,fErelmax);
  fherel = new TH1D("fhcorrfunc","Correlated relative energy spectrum",fnbins,0,fErelmax);
  fhereluncor = new TH1D("fhereluncor","Uncorrelated relative energy spectrum",fnbins,0,fErelmax);
}

TArtEasyEventMixing::TArtEasyEventMixing(std::vector<TLorentzVector> frags, std::vector<TLorentzVector> neuts,Double_t bins=100,Double_t Erelmax=5){
  TArtCore::Info(__FILE__,"Let's do Event Mixing! Now creating data set from inputs.");
  LoadData(frags, neuts);
  fnbins=bins;
  fErelmax=Erelmax;
  TArtEasyEventMixing();
}


void TArtEasyEventMixing::LoadData(std::vector<TLorentzVector> frags, std::vector<TLorentzVector> neuts){
  if((int)frags.size()!=(int)neuts.size()){
    TArtCore::Warning(__FILE__,"Vector size doesn't match! Aborting loading data...");
    return;
  }
  ffragtable=frags;
  fneuttable=frags;
  fneve=(Int_t)ffragtable.size();
  //initializing <Correlation function>
  for(int i=0;i<fneve;i++){
    fCm_frag.push_back(1);
    fCm_feut.push_back(1);
  }

}

Int_t TArtEasyEventMixing::GetNumVirtualPairs(){
  Int_t num = (Int_t)ffragtable.size();
  return num*(num-1);
}

Bool_t TArtEasyEventMixing::GetNextVirtualPair(){ 
  //もしイベント数が0だったらfalseを返す
  if(fneve==0){
    return false;
  }
  //もしfcounter_fragもfcounter_neutもイベント数と同じだったら終了なのでfalseを返す
  if(fcounter_neut==fneve-1 && fcounter_frag==fneve-1){
    return false;
  }
  
  //もしfcounter_neutがイベント数と同じだったらneutをリセットしてfcounter_fragを1増やす
  if(fcounter_neut==fneve-1 && fcounter_frag!=fneve-1){
    fcounter_neut=0;
    fcounter_frag++;
  }

  //それ以外の場合はneutを1増やす
  if(fcounter_neut!=fneve-1 && fcounter_frag!=fneve-1){
    fcounter_neut++;
    //さらに、同じイベントの組を取ってこようとしてたらスキップする
    if(fcounter_neut==fcounter_frag){
      fcounter_neut++;
    }
  }
  return true;
}

Double_t TArtEasyEventMixing::GetRelativeEnergy(){
  TLorentzVector frag = ffragtable[fcounter_frag];
  TLorentzVector neut = fneuttable[fcounter_neut];
  TLorentzVector added = frag+neut;
  return added.M()-frag.M()-neut.M();
}
