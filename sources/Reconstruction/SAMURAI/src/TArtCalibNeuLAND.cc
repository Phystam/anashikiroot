#include "TArtCalibNeuLAND.hh" 
#include "TArtPlastic.hh"
#include "TArtCalibSAMURAITZero.hh"
#include "TArtTZero.hh"
#include "TArtMath.hh"

#include "TMath.h"
#include "TClonesArray.h"
#include "TRandom3.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <cmath>
#include <limits>
#include "TCanvas.h"
//__________________________________________________________
TArtCalibNeuLAND::TArtCalibNeuLAND()
  : TArtReconstruction("NeuLANDPla"), fSAMURAIParameters(0)
{
  TArtCore::Info(__FILE__,"Creating the NeuLAND detector objects...");

  fStoreManager = TArtStoreManager::Instance();
  fNeuLANDPlaArray = new TArtNeuLANDPlaArray("TArtNeuLANDPla", 50);
  fNeuLANDPlaArray->SetOwner();
  fNeuLANDPlaArray->SetName("NeuLANDPla");

  fNeuLANDPlaParaArray = new TArtNeuLANDPlaParaArray("TArtNeuLANDPlaPara", 300);
  fNeuLANDPlaParaArray->SetOwner();
  fNeuLANDPlaParaArray->SetName("NeuLANDPlaPara");
  fStoreManager->AddDataContainer(fNeuLANDPlaArray);

  fSAMURAIParameters = (const TArtSAMURAIParameters*)fStoreManager->FindParameters("SAMURAIParameters");
  if(!fSAMURAIParameters){
    TArtCore::Info(__FILE__,"Could not find SAMURAIParameters.");
    throw;
  }

  //  fCalibSAMURAIT0 = (TArtCalibSAMURAIT0*)fStoreManager->FindCalibContainer("CalibSAMURAIT0");
  //  if(!fCalibSAMURAIT0){
  //    TArtCore::Info(__FILE__, "t0 will be NaN during this analysis.");
  //  }
  //  fT0Array = (TClonesArray *)fStoreManager->FindDataContainer("SAMURAITZero");

  fT0Array = (TClonesArray *)fStoreManager->FindDataContainer("SAMURAIT0");
  if(!fT0Array) TArtCore::Info(__FILE__, "t0 will be NaN during this analysis.");

  tac16_array = new TList;
  fNeuLANDMasterStart = NULL;

}

//__________________________________________________________
TArtCalibNeuLAND::~TArtCalibNeuLAND()
{
  delete fNeuLANDPlaParaArray;
  delete fNeuLANDPlaArray;
  delete tac16_array;
  delete fNeuLANDMasterStart;
}

//__________________________________________________________
void TArtCalibNeuLAND::LoadData()
{
  TArtRawEventObject* fEvent = (TArtRawEventObject*)fStoreManager->FindDataContainer("RawEvent");

  for(Int_t i=0; i<fEvent->GetNumSeg(); ++i){
    TArtRawSegmentObject* seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(detector == NEULAND) LoadData(seg);
  }

}

void TArtCalibNeuLAND::LoadData(TArtRawSegmentObject* seg)
{
  Int_t device = seg->GetDevice();
  Int_t detector = seg->GetDetector();
  if(device != SAMURAI) return;
  if(detector != NEULAND) return;
  if(seg->GetNumTacquilaData()==0) return;

  // get only tac reference at first
  for(Int_t j=0; j<seg->GetNumTacquilaData(); ++j){
    TArtRawTacquilaDataObject* d = seg->GetTacquilaData(j);
    Int_t ch  = d->GetChannel();
    if(16 == ch){ // reference channel
      Tac16 *reftac = new Tac16;
      reftac->sam = d->GetSam(); 
      reftac->gtb = d->GetGtb(); 
      reftac->mod = d->GetModule();
      reftac->tac = d->GetTac();
      //TArtCore::Info(__FILE__,"%d, %d, %d, %d", reftac->sam, reftac->gtb, reftac->mod, reftac->tac);
//std::cout << reftac->sam << ' ' << reftac->gtb << ' ' << reftac->mod << ' ' << reftac->tac << ' ' << ch << std::endl;
      tac16_array->Add(reftac);
    }
  }
  tac16_array->Sort();

  for(Int_t j=0; j<seg->GetNumTacquilaData(); ++j){
    TArtRawTacquilaDataObject* d = seg->GetTacquilaData(j);
    Int_t ch  = d->GetChannel(); if(16==ch) continue;
    Int_t sam = d->GetSam(); 
    Int_t gtb = d->GetGtb(); 
    Int_t mod = d->GetModule();
    TArtTacquilaMap mm(sam,gtb,mod,ch);
    const TArtNeuLANDPlaPara* para = fSAMURAIParameters->FindNeuLANDPlaPara(&mm);
    if(NULL == para && ch != 16){ // ch==16 is for T16 reference.
      TArtCore::Debug(__FILE__,"Could not find TArtNeuLANDPlaPara...: %s", mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtNeuLANDPlaPara...: %s, : %s", mm.GetMapInfo(), (para->GetDetectorName()).Data());
    }

    if(!FindNeuLANDPlaPara(para->GetID())){
      Int_t npara = fNeuLANDPlaParaArray->GetEntries();
      new ((*fNeuLANDPlaParaArray)[npara]) TArtNeuLANDPlaPara(*para);

      fIDNPlaParaMap.insert(std::pair<int, int>(para->GetID(), npara));
    }

    // SAMURAI Master Start is connected to this channel.
    TArtNeuLANDPla *pla;
    if (6 == sam && 1 == gtb && 7 == mod && 11 == ch) {
      pla = new TArtNeuLANDPla();
      fNeuLANDMasterStart = pla;
      pla->SetID(para->GetID());
      pla->SetFpl(para->GetFpl());
      pla->SetDetectorName(para->GetDetectorName());
      pla->SetTCycle(0,d->GetCycle());
      pla->SetTac(0,d->GetTac());
      pla->SetTacRef(0,GetTac16(sam,gtb,mod));
      pla->SetFired(0);
    }
    else {
      pla = FindNeuLANDPla(para->GetID());
      if(!pla){
        Int_t npla = fNeuLANDPlaArray->GetEntries();
        pla = new ((*fNeuLANDPlaArray)[npla]) TArtNeuLANDPla();
        pla->SetID(para->GetID());
        pla->SetFpl(para->GetFpl());
        pla->SetDetectorName(para->GetDetectorName());
        //pla->SetDetectorName(*(para->GetDetectorName()));
        pla->SetLayer(para->GetLayer());
        pla->SetBarID(para->GetBarID());

        fIDNPlaMap.insert(std::pair<int, int>(para->GetID(), npla));
      }

      if(para->GetMap(0) == mm){
        pla->SetQRaw(0,d->GetAdc());
        pla->SetTCycle(0,d->GetCycle());
        pla->SetTac(0,d->GetTac());
        pla->SetTacRef(0,GetTac16(sam,gtb,mod));
        pla->SetFired(0);
      }else{
        pla->SetQRaw(1,d->GetAdc());
        pla->SetTCycle(1,d->GetCycle());
        pla->SetTac(1,d->GetTac());
        pla->SetTacRef(1,GetTac16(sam,gtb,mod));
        pla->SetFired(1);
      } 
    }
    pla->SetTacquilaID(sam, gtb, mod, ch);
  } // for(Int_t j=0; j<seg->GetNumTacquilaData(); ++j)

  fDataLoaded = true;
}

//__________________________________________________________
Int_t TArtCalibNeuLAND::GetTac16(Int_t sam, Int_t gtb, Int_t mod) const
{
  TIter next(tac16_array);
  while (Tac16 *p = (Tac16 *)next()) 
    if(p->sam == sam && p->gtb == gtb && p->mod == mod) return p->tac;

  TArtCore::Error(__FILE__,"Could not find corresponding tac16!! sam:%d gtb:%d mod:%d",sam,gtb,mod);
  return -1;
}

//__________________________________________________________
void TArtCalibNeuLAND::ReconstructData()
{
  if(!fDataLoaded) LoadData();

  Double_t time_ms = 0;
  if (NULL != fNeuLANDMasterStart) {
    TArtNeuLANDPla* pla = fNeuLANDMasterStart;
    Int_t id = pla->GetID();
    const TArtNeuLANDPlaPara* para = FindNeuLANDPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
    }
    else {
      Double_t time_ref;
      //      if ( true ) {//not to use
      // time_ms = para->GetTDCTCal(0, pla->GetTac(0), false);
      // time_ref = para->GetTDCTCal(0, pla->GetTacRef(0), true);
      time_ms = Tac2ns(pla->GetTac(0),pla->GetID(), 0, false);
      time_ref = Tac2ns(pla->GetTacRef(0),pla->GetID(), 0, true);

      //      }//   else if ( true ){ //phystam
      // 	const Double_t* p = para->GetTac2nsPara(0, 0);
      // 	const Double_t* pref = para->GetTac2nsPara(0, 1);
      // 	// time = Tac2ns(pla->GetTac(0), para->GetTac2nsPara(0, 0));
      //   // time_ref = Tac2ns(pla->GetTacRef(0), para->GetTac2nsPara(0, 1));
      // 	time_ms = Tac2ns(pla->GetTac(0), p);
      //   time_ref = Tac2ns(pla->GetTacRef(0), pref);
      // }
      // else {
      // time_ms = 24.9982/(para->GetTDC25ns(0)-para->GetTDC0ns(0)) * ((Double_t)pla->GetTac(0) - para->GetTDC0ns(0));
      // time_ref = 24.9982/(para->GetTDC25ns_t17(0)-para->GetTDC0ns_t17(0)) * ((Double_t)pla->GetTacRef(0) - para->GetTDC0ns_t17(0));
      // }
      pla->SetTacCal(0,time_ms);
      pla->SetTacRefCal(0,time_ref);
      time_ms = time_ms + 25.*pla->GetTCycle(0) - time_ref;
      pla->SetTRaw(0,time_ms);
    }
  }

  for(Int_t i=0;i<GetNumNeuLANDPla();++i){
    TArtNeuLANDPla* pla = GetNeuLANDPla(i);
    Int_t id = pla->GetID();
    const TArtNeuLANDPlaPara* para = FindNeuLANDPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    // if(pla->GetQRaw0() < 0 || pla->GetQRaw0() < 0) TArtCore::Warning(__FILE__,"Both PMT seems not to be fired. ID:%d", id); 
    // start reconstruction

    for(int j=0;j<2;j++){
      Double_t qped = pla->GetQRaw(j)-para->GetQPed(j);
      pla->SetQPed(j,qped);
      Double_t energy = QDC2Energy(qped);
      Double_t energy_final = j==0 ? 
	energy * para->GetEDiffOffset() * para->GetESyncOffset() : 
	energy / para->GetEDiffOffset() * para->GetESyncOffset();
      pla->SetQCal(j,energy_final);
      Double_t time;
      Double_t time_ref;
      //      if (para->HasTDCTCal()) {
      time = Tac2ns(pla->GetTac(j),pla->GetID(),j,false);
      time_ref = Tac2ns(pla->GetTacRef(j),pla->GetID(),j,true);
      // time = 24.9982/(para->GetTDC25ns(j)-para->GetTDC0ns(j)) * ((Double_t)pla->GetTac(j) - para->GetTDC0ns(j));
      // time_ref = 24.9982/(para->GetTDC25ns_t17(j)-para->GetTDC0ns_t17(j)) * ((Double_t)pla->GetTacRef(j) - para->GetTDC0ns_t17(j));
      pla->SetTacCal(j,time);
      pla->SetTacRefCal(j,time_ref);

      time = time + 25.*pla->GetTCycle(j) - time_ref;
      time -= time_ms;

      pla->SetTRaw(j,time);

      time = j == 0 ? 
	time - para->GetTDiffOffset()/2. - para->GetTSyncOffset() + WalkCorrection(energy) : 
	time + para->GetTDiffOffset()/2. - para->GetTSyncOffset() + WalkCorrection(energy) ;
      time = time - para->GetTOFZero() + para->GetTOFClight();
      pla->SetTCal(j,time);
    }
    pla->SetPos((pla->GetTCal(0) - pla->GetTCal(1)) * para->GetVScint() * 10 /* 10 for cm -> mm)*/);
    // std::cout << pla->GetFired(0) << "  " << pla->GetTRaw(0) << "  "  << pla->GetFired(1) << "  " << pla->GetTRaw(1) << std::endl;
    pla->SetZPos(para->GetZPos()+gRandom->Uniform(-25,25));
  }
  
  fReconstructed = true;
}

//__________________________________________________________
Double_t TArtCalibNeuLAND::QDC2Energy(Double_t channel)
{
  Double_t value=20*(9.e-6*channel*channel+0.0285*channel);
  return value;
}

//__________________________________________________________
Double_t TArtCalibNeuLAND::WalkCorrection(Double_t e)
{
  Double_t y=0;

  Double_t par1=1500.; // +-0.2238
  Double_t par2=0.00075;//+-2.355e-05

  y=par1*pow(e,par2)-(par1*pow(400.,par2)); // Michael's

  return y;
}

//__________________________________________________________
Int_t TArtCalibNeuLAND::GetNumNeuLANDPla() const
{
  return fNeuLANDPlaArray->GetEntries();
}

//__________________________________________________________
TArtNeuLANDPla* TArtCalibNeuLAND::GetNeuLANDPla(Int_t i) const
{
  return (TArtNeuLANDPla*)fNeuLANDPlaArray->At(i);
}

//__________________________________________________________
TArtNeuLANDPla const *TArtCalibNeuLAND::GetNeuLANDMasterStart() const
{
  return fNeuLANDMasterStart;
}

//__________________________________________________________
TArtNeuLANDPla* TArtCalibNeuLAND::FindNeuLANDPla(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaMap.find(id);
  if(it != fIDNPlaMap.end()) return GetNeuLANDPla(it->second);
  else return 0;
}

//__________________________________________________________
TArtNeuLANDPlaArray* TArtCalibNeuLAND::GetNeuLANDPlaArray() const
{
  return fNeuLANDPlaArray;
}

//__________________________________________________________
Int_t TArtCalibNeuLAND::GetNumNeuLANDPlaPara() const
{
  return fNeuLANDPlaParaArray->GetEntries();
}

//__________________________________________________________
TArtNeuLANDPlaPara* TArtCalibNeuLAND::GetNeuLANDPlaPara(Int_t i) const
{
  return (TArtNeuLANDPlaPara*)fNeuLANDPlaParaArray->At(i);
}

//__________________________________________________________
TArtNeuLANDPlaPara* TArtCalibNeuLAND::FindNeuLANDPlaPara(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaParaMap.find(id);
  if(it != fIDNPlaParaMap.end()) return GetNeuLANDPlaPara(it->second);
  else return 0;
}

//__________________________________________________________
TArtNeuLANDPlaParaArray* TArtCalibNeuLAND::GetNeuLANDPlaParaArray() const
{
  return fNeuLANDPlaParaArray;
}

//__________________________________________________________
void TArtCalibNeuLAND::ClearData()
{
  //  fNeutron.ClearData();

  fNeuLANDPlaArray->Delete();
  fNeuLANDPlaParaArray->Delete();
  tac16_array->Delete();
  fIDNPlaMap.clear();
  fIDNPlaParaMap.clear();
  fDataLoaded = false;
  fReconstructed = false;
  delete fNeuLANDMasterStart;
  fNeuLANDMasterStart = NULL;
}

// Double_t TArtCalibNeuLAND::Tac2ns(Int_t x,Double_t const *p) {
//   Double_t val;
//   if(x<p[3]){
//     val=p[0]+p[1]*x;
//   }else{
//     val=p[0]+p[2]*pow(p[3],2.)
//       +(p[1]-2.*p[2]*p[3])*x
//       +p[2]*pow(x,2.);
//   }
//   return val;
// }

Double_t TArtCalibNeuLAND::Tac2ns(Int_t ch, Int_t id, Int_t i, bool is_ref) {
  if(is_ref){
    Double_t prev_ns=tacrefint[id-1][i][ch-1];
    Double_t center_ns=tacrefint[id-1][i][ch];
    return gRandom->Uniform(prev_ns,center_ns);
  }else{
    Double_t prev_ns=tacint[id-1][i][ch-1];
    Double_t center_ns=tacint[id-1][i][ch];
    return gRandom->Uniform(prev_ns,center_ns);
  }
}

void TArtCalibNeuLAND::SetTacDistribution(TH2* tac0,TH2* tac1,TH2* tacref0,TH2* tacref1){ //2d hist
  for(int id=0;id<401;id++){
    std::cout << "\r integrating histograms: id="<<id<<std::flush;
    Double_t normfactor0=tac0->Integral(id+1,id+1,1000.,3500);
    Double_t normfactor1=tac1->Integral(id+1,id+1,1000.,3500);
    Double_t normfactorref0=tacref0->Integral(id+1,id+1,1000,3500);
    Double_t normfactorref1=tacref1->Integral(id+1,id+1,1000,3500);
    for(int ch=1000;ch<3500;ch++){
      tacint[id][0][ch]= (Double_t)tac0->Integral(id+1,id+1,1000,ch)/normfactor0*25.;
      tacint[id][1][ch]= (Double_t)tac1->Integral(id+1,id+1,1000,ch)/normfactor1*25.;
      tacrefint[id][0][ch]= (Double_t)tacref0->Integral(id+1,id+1,1000,ch)/normfactorref0*25.;
      tacrefint[id][1][ch]= (Double_t)tacref1->Integral(id+1,id+1,1000,ch)/normfactorref1*25.;
    }
  }
}
void TArtCalibNeuLAND::SetTacDistribution(const char* file){
  TFile* tacfile=new TFile(file);
  TH2* tac0=(TH2D*)tacfile->Get("hidt0tacraw");
  TH2* tac1=(TH2D*)tacfile->Get("hidt1tacraw");
  TH2* tacref0=(TH2D*)tacfile->Get("hidt0tacrefraw");
  TH2* tacref1=(TH2D*)tacfile->Get("hidt1tacrefraw");
  // TCanvas* can=new TCanvas;
  // tac0->Draw("colz");
  // can->Update();
  SetTacDistribution(tac0,tac1,tacref0,tacref1);
}
