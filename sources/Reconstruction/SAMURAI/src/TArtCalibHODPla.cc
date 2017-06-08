#include "TArtCalibHODPla.hh" 
#include "TArtHODPla.hh"
#include "TArtHODPlaPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>
#include <TRandom.h>
//__________________________________________________________
TArtCalibHODPla::TArtCalibHODPla(const TString outdataname) 
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating the HOD Plastic detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtSAMURAIParameters*)sman->FindParameters("SAMURAIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: SAMURAIParameters");
    throw;
  }

  fHODPlaArray = new TClonesArray("TArtHODPla",10);
  fHODPlaArray->SetName("SAMURAIHODPla");
  fHODPlaArray->SetOwner();
  sman->AddDataContainer(fHODPlaArray);
}

//__________________________________________________________
TArtCalibHODPla::~TArtCalibHODPla()  {
  ClearData();
  delete fHODPlaArray;
}

//__________________________________________________________
void TArtCalibHODPla::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();

    if((device == SAMURAI) &&
       (HODFQ == detector || HODPQ == detector || HODPQ2 == detector || HODFT == detector || HODPT == detector)) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibHODPla::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(!(HODFQ == detector || HODPQ == detector || HODPQ2 == detector || HODFT == detector || HODPT == detector)) return;
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtHODPlaPara *para = (TArtHODPlaPara*)setup->FindHODPlaPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtHODPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtHODPlaPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t npla = fHODPlaArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in HODPlaArray: %d, HOD Plastic-ID:%d", npla, id);
    TArtHODPla * pla = FindHODPla(id);

    //    std::cout << id << " " << geo << " " << val << std::endl;
    if(NULL==pla){
      new ((*fHODPlaArray)[npla]) TArtHODPla();
      pla = (TArtHODPla *)fHODPlaArray->At(npla);
      pla->SetID(id);
      fHODPlaParaArray.push_back(para);
    }

    // set raw data
    if(HODFQ == detector || HODPQ == detector || HODPQ2 == detector){
      if(mm==*((TArtRIDFMap *)para->GetQUMap()))
	pla->SetQURaw(val);
      else if(mm==*((TArtRIDFMap *)para->GetQDMap())){
	pla->SetQDRaw(val);
      }
    }
    if(HODFT == detector || HODPT == detector){
      if(mm==*((TArtRIDFMap *)para->GetTUMap()))
	pla->SetTURaw(val);
      else if(mm==*((TArtRIDFMap *)para->GetTDMap()))
	pla->SetTDRaw(val);
    }

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibHODPla::ClearData()   {
  fHODPlaArray->Clear("C");
  fHODPlaParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibHODPla::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumHODPla();i++){
    TArtHODPla *pla = (TArtHODPla*)fHODPlaArray->At(i);
    TArtHODPlaPara *para = fHODPlaParaArray[i];
    pla->SetZCoef(0,para->GetZCoef0());
    pla->SetZCoef(1,para->GetZCoef1());
    pla->SetZCoef(2,para->GetZCoef2());
    pla->SetZCoef(3,para->GetZCoef3());
    Double_t TUCal = -9999., TDCal = -9999.;
    Bool_t   UFired = false, DFired = false, Fired = false;
    Int_t TURaw = pla->GetTURaw();
    Int_t TDRaw = pla->GetTDRaw();
    Int_t QURaw = pla->GetQURaw();
    Int_t QDRaw = pla->GetQDRaw();

    Double_t QUPed = ((Double_t)QURaw + gRandom->Uniform(-0.5,0.5)) - para->GetQPedUp();
    Double_t QDPed = ((Double_t)QDRaw + gRandom->Uniform(-0.5,0.5)) - para->GetQPedDown();
    Double_t QUCal = QUPed * para->GetQCalUp();
    Double_t QDCal = QDPed * para->GetQCalDown();

    //    cout << QURaw

    Double_t TAveCal = -9999.; 
    Double_t TUSlw   = -9999.; 
    Double_t TDSlw   = -9999.; 
    Double_t TAveSlw = -9999.; 

    Double_t QAveRaw = -9999.;
    Double_t QAvePed = -9999.;
    Double_t QAveCal = -9999. ;

    //QAve calc
    if(QURaw>0 && QURaw<4096 && QDRaw>0 && QDRaw<4096){
      QAveRaw = sqrt((Double_t)QURaw*(Double_t)QDRaw);
      QAvePed = sqrt(QUPed*QDPed);
      QAveCal = sqrt(QUCal*QDCal);
    }

    //Tcal&Tslw calc
    if(TURaw>0 && TURaw<4096) {
      UFired = true;
      TUCal = ((Double_t)TURaw + gRandom->Uniform(-0.5,0.5)) * para->GetTCalUp() + para->GetTOffUp();
      if(QURaw>0){
	//	TUSlw = TURaw + para->GetTUSlewA()/(TMath::Sqrt(QURaw)) + para->GetTUSlewB();
	//	TUSlw = TUSlw * para->GetTCalUp();
	TUSlw = TUCal - para->GetTUSlewA()/TMath::Sqrt(QUPed) - para->GetTDSlewB();
      }
//      else{
//	TUSlw = TUCal;
//      }
    }

    if(TDRaw>0 && TDRaw<4096) {
      DFired = true;
      TDCal = ((Double_t)TDRaw + gRandom->Uniform(-0.5,0.5)) * para->GetTCalDown() + para->GetTOffDown();
      if(QDRaw>0){
	//	TDSlw = TDRaw + para->GetTDSlewA()/(TMath::Sqrt(QDRaw)) + para->GetTDSlewB();
	//	TDSlw = TDSlw * para->GetTCalDown();
	TDSlw = TDCal - para->GetTDSlewA()/TMath::Sqrt(QDPed) - para->GetTDSlewB();
      }
//      else{
//	TDCal = TDSlw;
//      }
    }
    
    if(UFired && DFired) Fired = true;
    //TAve calc
    if(Fired) {
      TAveCal = (TDCal+TUCal)/2.+para->GetTOff();
      TAveSlw = (TDSlw+TUSlw)/2.;
      //TArtCore::Info(__FILE__,"ch %d: fpl: %d, tlraw: %d, trraw: %d, tl: %f, tr: %f, t:%f", i , fFocalPlane, fTLRaw,fTRRaw,(float)fTL,(float)fTR,(float)fTOF);
      //fTDiff = fTL-fTR;
      //fX     = fDT2X[0] * pow(fTDiff,2) + fDT2X[1] * fTDiff + fDT2X[2];
    }


    pla->SetQAveRaw(QAveRaw);
    pla->SetQUPed(QUPed);		   
    pla->SetQDPed(QDPed);
    pla->SetQAvePed(QAvePed);
    pla->SetQUCal(QUCal);		   
    pla->SetQDCal(QDCal);
    pla->SetQAveCal(QAveCal);

    pla->SetTUCal(TUCal);
    pla->SetTDCal(TDCal);
    pla->SetTAveCal(TAveCal);
    pla->SetTUSlw(TUSlw);
    pla->SetTDSlw(TDSlw);
    pla->SetTAveSlw(TAveSlw);

    // copy some information from para to data container
    pla->SetID(para->GetID());
    pla->SetDetectorName(*(para->GetDetectorName()));
    pla->SetFpl(para->GetFpl());
  } // for(Int_t i=0;i<GetNumHODPla();i++)

  fHODPlaArray->Sort(); // sorting by qmax

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtHODPla * TArtCalibHODPla::GetHODPla(Int_t i) {
  return (TArtHODPla *)fHODPlaArray->At(i);
}
//__________________________________________________________
TArtHODPlaPara * TArtCalibHODPla::GetHODPlaPara(Int_t i) {
  return (TArtHODPlaPara *)fHODPlaParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibHODPla::GetNumHODPla() {
  return fHODPlaArray->GetEntries();
}
//__________________________________________________________
TArtHODPla * TArtCalibHODPla::FindHODPla(Int_t id){
  for(Int_t i=0;i<GetNumHODPla();i++)
    if(id == ((TArtHODPla*)fHODPlaArray->At(i))->GetID())
      return (TArtHODPla*)fHODPlaArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtHODPla * TArtCalibHODPla::FindHODPla(TString *n){
  for(Int_t i=0;i<GetNumHODPla();i++)
    if(*n == *(((TArtHODPla*)fHODPlaArray->At(i))->GetDetectorName()))
      return (TArtHODPla*)fHODPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtHODPla: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtHODPla * TArtCalibHODPla::FindHODPla(char *n){
  for(Int_t i=0;i<GetNumHODPla();i++)
    if(strcmp(((TArtHODPla*)fHODPlaArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtHODPla*)fHODPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtHODPla: %s", n);
  return NULL;
}
