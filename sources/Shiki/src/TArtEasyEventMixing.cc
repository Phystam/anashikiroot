#include "TArtEasyEventMixing.hh"
#include <TH1.h>
#include <vector>
#include <TLorentzVector.h>
#include "TArtCore.hh"
#include <iostream>
#include <TString.h>
#include <algorithm>
#include "../../Nadeko/include/TArtAlias.hh"
TArtEasyEventMixing::TArtEasyEventMixing(){
  InitCounter();
  finit=false;
}

TArtEasyEventMixing::TArtEasyEventMixing(std::vector<TLorentzVector> frags, std::vector<TLorentzVector> neuts,Double_t bins=100,Double_t Erelmax=5){
  TArtCore::Info(__FILE__,"Let's do Event Mixing! Now creating data set from inputs.");
  LoadData(frags, neuts);
  SetErelMax(Erelmax);
  SetNbins(bins);
}


void TArtEasyEventMixing::LoadData(std::vector<TLorentzVector> frags, std::vector<TLorentzVector> neuts){
  if((int)frags.size()!=(int)neuts.size()){
    TArtCore::Warning(__FILE__,"Vector size doesn't match! Aborting loading data...");
    return;
  }
  ffragtable=frags;
  fneuttable=neuts;
  fneve=(Int_t)ffragtable.size();
  //initializing <Correlation function>
  fweightmatrix.resize(GetNumVirtualPairs());
  ferrorweight.resize(GetNumVirtualPairs());
  fignored_frag.resize(GetNumVirtualPairs());
  fignored_neut.resize(GetNumVirtualPairs());
  fCm_frag.resize(GetNumVirtualPairs());
  fCm_neut.resize(GetNumVirtualPairs());
  std::fill(fCm_frag.begin(),fCm_frag.end(),1);
  std::fill(fCm_neut.begin(),fCm_neut.end(),1);
  finit=true;
}

Int_t TArtEasyEventMixing::GetNumVirtualPairs(){
  Int_t num = (Int_t)ffragtable.size();
  return num*(num-1);
}
Double_t TArtEasyEventMixing::Mix(){
  return 1;
}
void TArtEasyEventMixing::DoIteration(){
}

Bool_t TArtEasyEventMixing::GetNextVirtualPair(){ 
  //もしイベント数が0だったらfalseを返す
  if(fneve==0){
    return false;
  }
  //もしfcounter_fragもfcounter_neutもイベント数と同じだったら終了なのでfalseを返す
  //カウンターも戻す
  if(fcounter==GetNumVirtualPairs()-1){
    InitCounter();
    std::cout<<" end of the loop."<<std::endl;
    
    return false;
  }
  
  //もしfcounter_neutがイベント数と同じだったらneutをリセットしてfcounter_fragを1増やす
  if(fcounter_neut==fneve-1 && fcounter_frag<fneve){
    fcounter_neut=0;
    fcounter_frag++;
    fcounter++;
    return true;
  }

  //それ以外の場合はneutを1増やす
  if(fcounter_neut<fneve && fcounter_frag<fneve){
    fcounter_neut++;
    //さらに、同じイベントの組を取ってこようとしてたらスキップする
    if(fcounter_neut==fcounter_frag){
      fcounter_neut++;
    }
    fcounter++;
    return true;
  }
  //nenio
  return false;
}

Bool_t TArtEasyEventMixing::GetNextPair(){
  //もしイベント数が0だったらfalseを返す
  if(fneve==0){
    return false;
  }
  //もしfcounter_fragもfcounter_neutもイベント数と同じだったら終了なのでfalseを返す
  if(fcounter_neut==fneve-1 && fcounter_frag==fneve-1){
    InitCounter();
    return false;
  }

  fcounter_frag++;
  fcounter_neut++;
  return true;
}

Double_t TArtEasyEventMixing::GetRelativeEnergy(){
  TLorentzVector frag = ffragtable[fcounter_frag];
  TLorentzVector neut = fneuttable[fcounter_neut];
  TLorentzVector added = frag+neut;
  return added.M()-frag.M()-neut.M();
}

Double_t TArtEasyEventMixing::GetWeight(){
  Double_t frag = fCm_frag[fcounter_frag];
  Double_t neut = fCm_neut[fcounter_neut];
  return 1./(frag*neut);
}

void TArtEasyEventMixing::MakeCorrelatedSpectrum(){
  fherel->Sumw2();
  while(GetNextPair()){
    if(fcounter%100==0){
      ShowStatus(fcounter,fneve, "MakeCorrelatedSpectrum()");
    }
    fherel->Fill(GetRelativeEnergy());
  }
  std::cout<<"\n done."<<std::endl;
}

void TArtEasyEventMixing::MakeUncorrelatedSpectrum(Int_t i){
  TH1* fhereluncor_temp = new TH1D(Form("fhereluncor%d",i),Form("%dth iteration",i),fnbins,0,fErelmax);
  fhereluncor_temp->Sumw2();
  while(GetNextVirtualPair()){
    if(fcounter%10000==0){
      ShowStatus(fcounter,GetNumVirtualPairs(), "MakeUncorrelatedSpectrum()");
      std::cout<<fcounter<<": "<<GetRelativeEnergy()<<", "<<GetWeight()<<std::endl;
      
    }
    if(GetWeight()>0){
      fhereluncor_temp->Fill(GetRelativeEnergy(),GetWeight());
    }
  }
  fhereluncor.push_back(fhereluncor_temp);
  std::cout<<"\n done."<<std::endl;
}

void TArtEasyEventMixing::CalcCorrelationFunction(Int_t i){
  TH1* fhcorrfunc_temp = new TH1D(Form("fhcorrfunc%d",i),Form("Correlation Function, %dth iteration",i),fnbins,0,fErelmax);
  fhcorrfunc_temp->Sumw2();
  fhcorrfunc_temp->Divide(fherel,fhereluncor[i]);
  fhcorrfunc.push_back(fhcorrfunc_temp);
}

void TArtEasyEventMixing::MakeCorrelationMatrix(Int_t i){
  while(GetNextVirtualPair()){
    Int_t ignored = 0;
    Double_t Erel_mix = GetRelativeEnergy();
    if(Erel_mix<fErelmax){
      MakeInterpolation(fhereluncor[i],Erel_mix,fweight,ferror);
      fweightmatrix[fcounter] = fweight;
      ferrorweight[fcounter] = ferror;
      // std::cout << " fweightmatrix["<<fcounter<<"] = "<<fweight;
      // std::cout << ", ferrorweight["<<fcounter<<"] = "<<ferror<<std::endl;

    }else{
      fweightmatrix[fcounter] = 0;
      ferrorweight[fcounter] = 1e5;
      fignored_frag[fcounter_frag]++;
      fignored_neut[fcounter_neut]++;
    }
  }
  TArtCore::Info(__FILE__,"Correlation Matrix has been calculated.");
}

void TArtEasyEventMixing::IterationOfMeanCorrelation(){
  Int_t m = 0;
  Double_t CmDif = 1;
  
  std::vector<Double_t> Cm_frag_old(fneve,0);
  std::vector<Double_t> Cm_neut_old(fneve,0);
  std::vector<Double_t> Em_frag(fneve,0);
  std::vector<Double_t> Em_neut(fneve,0);
  while (CmDif > 1e-3 && m<9){
    Double_t CmDif_Frag = 0.;
    Double_t CmDif_Neut = 0.;
    //Calculating mean orrelation function for fragment:
    /*
      \braket{C}^{(n)} (p_i) = 1/(N-1) \sum_{j=1\neq i}^N \frac{C^{(n-1)}(E_{rel,ij})}{\braket{C}^{(n)}(p_j)}
    */
    Cm_frag_old = fCm_frag;
    Cm_neut_old = fCm_neut;
    std::fill(fCm_frag.begin(),fCm_frag.end(),0);
    std::fill(fCm_neut.begin(),fCm_neut.end(),1);
    while(GetNextVirtualPair()){ //begin of loop for fragments
      if(fweightmatrix[fcounter] > 0){
	fCm_frag[fcounter_frag] += fweightmatrix[fcounter] / fCm_neut[fcounter_neut] / pow( ferrorweight[fcounter], 2.);
	Em_frag[fcounter_frag] += 1. / ( pow(ferrorweight[fcounter],2.));
       
	//	std::cout<< fweightmatrix[fcounter] <<" "<< fCm_neut[fcounter_neut] <<" "<< pow( ferrorweight[fcounter], 2)<<std::endl;
      }
      //end of neutron loop
      if(fcounter_neut == fneve - 1 || (fcounter_neut == fneve-2 && fcounter_frag == fneve-1)){
	if (Em_frag[fcounter_frag] > 0) {
	  fCm_frag[fcounter_frag] /= Em_frag[fcounter_frag];
	  //	  std::cout << " fCm_frag["<<fcounter_frag<<"] = "<<fCm_frag[fcounter_frag]<<std::endl;
	} else {
	  //	  std::cerr << "Em_frag = 0 !!!" << std::endl;
	  continue;
	}
	CmDif_Frag += std::abs(fCm_frag[fcounter_frag] - Cm_frag_old[fcounter_frag])/(fneve-fignored_frag[fcounter_frag]);
      }
    } // end of loop for fragments
    Cm_neut_old = fCm_neut;
    std::fill(fCm_neut.begin(),fCm_neut.end(),0);
    
    while(GetNextVirtualPair()){ //begin of loop for neutrons
      if(fweightmatrix[fcounter] > 0){
	fCm_neut[fcounter_neut] += fweightmatrix[fcounter] / fCm_frag[fcounter_frag] / ( ferrorweight[fcounter] * ferrorweight[fcounter] );
	Em_neut[fcounter_neut] += 1. / ( pow(ferrorweight[fcounter],2.));
      }
      //end of neutron loop for each fragment
      if(fcounter_frag == fneve - 1 || (fcounter_neut == fneve-1 && fcounter_frag==fneve-2)){
	if (Em_neut[fcounter_neut] > 0) {
	  fCm_neut[fcounter_neut] /= Em_neut[fcounter_neut];
	  //	  std::cout << " fCm_neut["<<fcounter_neut<<"] = "<<fCm_neut[fcounter_neut]<<std::endl;
	} else {
	  //	  std::cerr << "Em_neut = 0 !!!" << std::endl;
	  continue;
	}
	if(fneve-fignored_neut[fcounter_neut]>0){
	  CmDif_Neut += std::abs(fCm_neut[fcounter_neut] - Cm_neut_old[fcounter_neut])/(fneve-fignored_neut[fcounter_neut]);
	}
      }
    } // end of loop for neutrons

    //iteration?
    CmDif = CmDif_Frag + CmDif_Neut;
    m++;
    std::cout << " "<<m<<"th Iteration, Difference = "<<CmDif<<std::endl;

  }
}
void TArtEasyEventMixing::ReconstructData(){
  InitCounter();
  if(!(finit&&finit_erelmax&&finit_nbins)){
    TArtCore::Warning(__FILE__,"Please initialize Erel_max, Nbins and TLorentzVectors using SetErelMax(), SetErelMax and LoadData(). Aborting TArtEasyEventMixing::ReconstructData().");
    return;
  }
  //make histograms
  fherel = new TH1D("fherel_evmix","Correlated relative energy spectrum",fnbins,0,fErelmax);
  //first iteration
  MakeCorrelatedSpectrum();
  MakeUncorrelatedSpectrum(0);
  CalcCorrelationFunction(0);
  MakeCorrelationMatrix(0);
  wait();
  IterationOfMeanCorrelation();
  wait();
  
  MakeUncorrelatedSpectrum(1);
  wait();
  CalcCorrelationFunction(1);
  MakeCorrelationMatrix(1);

}

void TArtEasyEventMixing::ShowStatus(Int_t now, Int_t num,TString message){
  std::cout << "\r Making Histograms in "<< message.Data() <<": ";
  std::cout << "[";
  Int_t progress = (now*20)/num;
  for(int i=0;i<progress;i++){
    std::cout << "=";
  }
  for(int i=progress;i<20;i++){
    std::cout << "-";
  }
  std::cout<<"]"<<std::flush;
}


//Imported from Quentin-san's macro
// @input : Histogram
// @input : ValueToCheck
// @output: MeanValue
// @output: MeanValueError
void TArtEasyEventMixing::MakeInterpolation(TH1* Histogram, const Double_t ValueToCheck, Double_t &MeanValue, Double_t& MeanValueError){
  Double_t EnergyBinWidth = Histogram->GetBinWidth(1);
  TAxis *XAxis = (TAxis *)(Histogram->GetXaxis());
  Double_t MaximumEnergy = XAxis->GetXmax();
  Double_t MinimumEnergy = XAxis->GetXmin();
  Double_t NumberOfBin = XAxis->GetNbins();

  if (NumberOfBin != (MaximumEnergy - MinimumEnergy) / EnergyBinWidth){
    std::cerr << "MakeInterpolation :: Error ! Number of bin rescontructed is not consistent with bin width and maximum energy : " << NumberOfBin << " " << MaximumEnergy / EnergyBinWidth << std::endl;
  }

  Double_t MiddleOfFirstBin = MinimumEnergy + EnergyBinWidth /2;
  Int_t FirstBinNumberToCheck = (Int_t) (1 + (ValueToCheck-MinimumEnergy) / EnergyBinWidth);
  Int_t SecondBinNumberToCheck = 1;
  Double_t SideTest = MiddleOfFirstBin + EnergyBinWidth * (FirstBinNumberToCheck - 1);

  if (SideTest - ValueToCheck < 0) {
    SecondBinNumberToCheck = FirstBinNumberToCheck + 1;
  } else {
    SecondBinNumberToCheck = FirstBinNumberToCheck;
    FirstBinNumberToCheck = SecondBinNumberToCheck - 1;
  }
  
  if (FirstBinNumberToCheck < 1) {
    FirstBinNumberToCheck = 1;
    SecondBinNumberToCheck = 1;
  }

  if (FirstBinNumberToCheck > NumberOfBin) {
    FirstBinNumberToCheck = NumberOfBin;
    SecondBinNumberToCheck = NumberOfBin;
  }
  Double_t FirstBinValue = Histogram->GetBinContent(FirstBinNumberToCheck);
  Double_t FirstBinError = Histogram->GetBinError(FirstBinNumberToCheck);
  Double_t SecondBinValue = Histogram->GetBinContent(SecondBinNumberToCheck);
  Double_t SecondBinError = Histogram->GetBinError(SecondBinNumberToCheck);

  Int_t LowerBinLimit = -1;
  Int_t LowerBinLimitContent = -1;
  Int_t LowerBinLimitError = -1;
  Int_t UpperBinLimit = -1;
  Int_t UpperBinLimitContent = -1;
  Int_t UpperBinLimitError = -1;
  for(Int_t i = 1; i<NumberOfBin; i++) {
    if (Histogram->GetBinContent(i) > 0) {
      LowerBinLimit = i;
      LowerBinLimitContent = Histogram->GetBinContent(i);
      LowerBinLimitError = Histogram->GetBinError(i);
      break;
    }
  }

  for(Int_t i = NumberOfBin; i>0; i--) {
    if (Histogram->GetBinContent(i) > 0) {
      UpperBinLimit = i;
      UpperBinLimitContent = Histogram->GetBinContent(i);
      UpperBinLimitError = Histogram->GetBinError(i);
      break;
    }
  }

  if (FirstBinNumberToCheck < LowerBinLimit) { // below first not empty bin
    FirstBinNumberToCheck = LowerBinLimit;
    FirstBinValue = LowerBinLimitContent;
    FirstBinError = LowerBinLimitError;
    SecondBinNumberToCheck = LowerBinLimit;
    SecondBinValue = LowerBinLimitContent;
    SecondBinError = LowerBinLimitError;
  } else if (FirstBinNumberToCheck >= UpperBinLimit) { // above last not empty bin
    FirstBinNumberToCheck = UpperBinLimit;
    FirstBinValue = UpperBinLimitContent;
    FirstBinError = UpperBinLimitError;
    SecondBinNumberToCheck = UpperBinLimit;
    SecondBinValue = UpperBinLimitContent;
    SecondBinError = UpperBinLimitError;
  } else if (FirstBinValue == 0 || TMath::IsNaN(FirstBinValue)) { // empty bin in the middle
    Int_t LowerBin = 0;
    Double_t LowerBinContent = 0;
    for(Int_t i =1; i<NumberOfBin;i++) {
      LowerBin = FirstBinNumberToCheck-i;
      if (LowerBin >= LowerBinLimit){
	LowerBinContent = Histogram->GetBinContent(LowerBin);
	if (LowerBinContent > 0) {
	  FirstBinNumberToCheck = LowerBin;
	  FirstBinValue = LowerBinContent;
	  FirstBinError = Histogram->GetBinError(LowerBin);
	  break;
	}
      }else{
	FirstBinNumberToCheck = LowerBinLimit;
	FirstBinValue = LowerBinLimitContent;
	FirstBinError = LowerBinLimitError;
	break;
      }
    }
  }

  if (SecondBinValue == 0 || TMath::IsNaN(SecondBinValue)) { // empty bin in the middle too
    Int_t UpperBin = 0;
    Double_t UpperBinContent = 0;
    for(Int_t i =1; i<NumberOfBin;i++) {
      UpperBin = SecondBinNumberToCheck+i;
      if (UpperBin <= UpperBinLimit){
	UpperBinContent = Histogram->GetBinContent(UpperBin);
	if (UpperBinContent > 0) {
	  SecondBinNumberToCheck = UpperBin;
	  SecondBinValue = UpperBinContent;
	  SecondBinError = Histogram->GetBinError(UpperBin);
	  break;
	}
      } else {
	SecondBinNumberToCheck = UpperBinLimit;
	SecondBinValue = UpperBinLimitContent;
	SecondBinError = UpperBinLimitError;
	break;
      }
    }
  }

  Int_t BinSpread = SecondBinNumberToCheck - FirstBinNumberToCheck;
  if (BinSpread == 0){
    BinSpread = 1;
  }
  if ((FirstBinValue == 0) || TMath::IsNaN(FirstBinValue) || (SecondBinValue == 0) || TMath::IsNaN(SecondBinValue)){
    MeanValue = 0;
    MeanValueError = 0;
    std::clog << "MakeInterpolation :: Put Value & Error to 0\n";
  } else {
    Double_t FirstBinErrorMax = FirstBinValue + FirstBinError;
    Double_t FirstBinErrorMin = FirstBinValue - FirstBinError;
    Double_t SecondBinErrorMax = SecondBinValue + SecondBinError;
    Double_t SecondBinErrorMin = SecondBinValue - SecondBinError;
    Double_t SlopeValue = (SecondBinValue - FirstBinValue) / (EnergyBinWidth * BinSpread);
    Double_t SlopeErrorMax = (SecondBinErrorMax - FirstBinErrorMax) / (EnergyBinWidth * BinSpread);
    Double_t SlopeErrorMin = (SecondBinErrorMin - FirstBinErrorMin) / (EnergyBinWidth * BinSpread);
    MeanValue = FirstBinValue + SlopeValue * (ValueToCheck - (FirstBinNumberToCheck - 0.5) * EnergyBinWidth);
    Double_t ErrorfromMax = FirstBinErrorMax + SlopeErrorMax * (ValueToCheck - (FirstBinNumberToCheck - 0.5) * EnergyBinWidth);
    Double_t ErrorfromMin = FirstBinErrorMin + SlopeErrorMin * (ValueToCheck - (FirstBinNumberToCheck - 0.5) * EnergyBinWidth);
    Double_t ErrorMax = ErrorfromMax - MeanValue;
    Double_t ErrorMin = MeanValue - ErrorfromMin;
    MeanValueError = (ErrorMax + ErrorMin) / 2;
  }
}
