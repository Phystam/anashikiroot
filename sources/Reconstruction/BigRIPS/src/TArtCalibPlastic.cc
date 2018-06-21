#include "TArtCalibPlastic.hh" 
#include "TArtPlastic.hh"
#include "TArtPlasticPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtTOF.hh" 
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>
#include <TRandom.h>
#include <vector>
using namespace std;
//__________________________________________________________
TArtCalibPlastic::TArtCalibPlastic(const TString outdataname)
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating the Plastic detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }
  fPlasticArray = new TClonesArray("TArtPlastic",10);
  fPlasticArray->SetName(outdataname);
  fPlasticArray->SetOwner();
  sman->AddDataContainer(fPlasticArray);
  TList* listOfPlasticPara = setup->GetListOfPlasticPara();
  TIter next(listOfPlasticPara);
  for(int np=0;TArtPlasticPara *p = (TArtPlasticPara *)next();np++){
    TArtPlastic *pla = (TArtPlastic *) fPlasticArray->ConstructedAt(np);

    pla->SetID(p->GetID());
    pla->SetDetectorName(*(p->GetDetectorName()));
    pla->SetFpl(p->GetFpl());
    pla->SetZoffset(p->GetZoffset());

    fPlasticParaArray.push_back(p);
  }

}

//__________________________________________________________
TArtCalibPlastic::~TArtCalibPlastic()  {
  ClearData();
  fPlasticArray->Delete();
  fPlasticParaArray.clear();   
  delete fPlasticArray;
}

//__________________________________________________________
void TArtCalibPlastic::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(PLAT == detector || STOPPLA == detector || RF == detector || PLAQ == detector) LoadData(seg); // STOPPLA is CFD TDC for 2014 Uranium campaign, TI
  }
  return;

}

//__________________________________________________________
void TArtCalibPlastic::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  vector<Int_t> tlrawlist;
  vector<Int_t> trrawlist;
  vector<Int_t> tlidlist;
  vector<Int_t> tridlist;
  if(!(PLAT == detector || STOPPLA == detector || RF == detector || PLAQ == detector)) return;  // STOPPLA is CFD TDC for 2014 Uranium campaign, TI
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    Int_t edge = (Int_t)d->GetEdge();
    if(edge == 1) continue;

    TArtRIDFMap mm(fpl,detector,geo,ch);

    TArtPlasticPara *para = (TArtPlasticPara*)setup->FindPlasticPara(&mm);
    if(NULL == para){
      //      TArtCore::Warning(__FILE__,"Could not find TArtPlasticPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtPlasticPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    TArtPlastic * pla = (TArtPlastic *)fPlasticArray->At(id-1);

    if(id != pla->GetID())
      TArtCore::Error(__FILE__,"Plastic ID is different: %d, %d", id, pla->GetID());

    if(NULL==pla)
      TArtCore::Error(__FILE__,"Could not find Plastic...: ID:%d, Dev:%d, %s", id, device, mm.GetMapInfo());

    // set raw data
    if(PLAQ == detector){
      if(mm==*((TArtRIDFMap *)para->GetQLMap())){
	if(pla->GetQLRaw()<=0)pla->SetQLRaw(val);
      }
      if(mm==*((TArtRIDFMap *)para->GetQRMap())){
	if(pla->GetQRRaw()<=0)pla->SetQRRaw(val);
      }
    }
    if(PLAT == detector || STOPPLA == detector || RF == detector ){
      if(mm==*((TArtRIDFMap *)para->GetTLMap())){
	// if(pla->GetTLRaw()<=0){ 
	//   pla->SetTLRaw(val);
	// }
	// else{
	//   TArtCore::Warning(__FILE__,"set multiple tdc for one left pla data cont.");
	// }
	
	tlrawlist.push_back(val);
	tlidlist.push_back(id);
      }
      if(mm==*((TArtRIDFMap *)para->GetTRMap())){
	// if(pla->GetTRRaw()<=0){
	//   pla->SetTRRaw(val);
	// }
	// else{
	//   TArtCore::Warning(__FILE__,"set multiple tdc for one right pla data cont.");
	// }
	trrawlist.push_back(val);
	tridlist.push_back(id);
      }
    }
    pla->SetDataState(1);
  }  
  //multiple tdc near search
  // cout << "before search:" << (int) tlrawlist.size() << endl;
  NearSearch(tlrawlist,trrawlist,tlidlist,tridlist);
  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibPlastic::ClearData()   {
  for(int i=0;i<fPlasticArray->GetEntries();i++)
    ((TArtPlastic*)fPlasticArray->At(i))->Clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibPlastic::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumPlastic();i++){
    TArtPlastic *pla = (TArtPlastic*)fPlasticArray->At(i);
    if(!pla->IsDataValid()) continue;

    TArtPlasticPara *para = fPlasticParaArray[i];

    Double_t fTLCal = -9999, fTRCal = -9999;
    Bool_t fLFired = false, fRFired = false, fFired = false;
    Double_t fTLRaw = pla->GetTLRaw();
    Double_t fTRRaw = pla->GetTRRaw();
    Double_t fQLRaw = pla->GetQLRaw();
    Double_t fQRRaw = pla->GetQRRaw();

    Double_t fTAveCal = -9999;
    Double_t fTLSlw = -9999;
    Double_t fTRSlw = -9999;
    Double_t fTAveSlw = -9999;
    Double_t fTAveSlwOff = -9999;
    Double_t fTDiff = -9999;
    Double_t fX = -9999;
    Double_t fQLPed = -9999;
    Double_t fQRPed = -9999;
    Double_t fQAvePed = -9999;

    if(fQRRaw>0 && fQRRaw<9999){
      fRFired = true;
      fQRPed = fQRRaw - para->GetQPedRight();
    }
    if(fQLRaw>0 && fQLRaw<9999){
      fLFired = true;
      fQLPed = fQLRaw - para->GetQPedLeft();
    }
    if(fLFired && fRFired){
      fQAvePed = TMath::Sqrt(fQLPed*fQRPed);
    }
    fLFired=false;
    fRFired=false;
    
    if(fTLRaw>para->GetTDCUnderflow() && fTLRaw<para->GetTDCOverflow()) {
      fLFired = true;
      fTLCal = (fTLRaw + gRandom->Uniform(-0.5,0.5)) * para->GetTCalLeft();
      if(fQAvePed>0){
	//	fTLSlw = fTLRaw + para->GetTLSlewA()/(TMath::Sqrt(fQLRaw)) + para->GetTLSlewB();
	//	fTLSlw = fTLSlw * para->GetTCalLeft();
	
	fTLSlw = fTLCal - ( para->GetTLSlewA() * pow(fQAvePed,para->GetTLSlewB()) + para->GetTLSlewC() );
      }
      else{
	fTLSlw = fTLCal;
      }
    }

    if(fTRRaw>para->GetTDCUnderflow() && fTRRaw<para->GetTDCOverflow()) {
      fRFired = true;
      fTRCal = (fTRRaw + gRandom->Uniform(-0.5,0.5)) * para->GetTCalRight();
      if(fQAvePed>0){
	// fTRSlw = fTRRaw + para->GetTRSlewA()/(TMath::Sqrt(fQRRaw)) + para->GetTRSlewB();
	// fTRSlw = fTRSlw * para->GetTCalRight();
	fTRSlw = fTRCal - ( para->GetTRSlewA() * pow(fQAvePed,para->GetTRSlewB()) + para->GetTRSlewC() );
      }
      else{
        fTRSlw = fTRCal;
      }
    }
    if(fLFired && fRFired) fFired = true;

    if(fFired) {
      fTAveCal = (fTLCal+fTRCal)/2.;
      fTAveSlw = (fTLSlw+fTRSlw)/2.;
      fTAveSlwOff = fTAveSlw + para->GetToffset();
      //TArtCore::Info(__FILE__,"ch %d: fpl: %d, tlraw: %d, trraw: %d, tl: %f, tr: %f, t:%f", i , fFocalPlane, fTLRaw,fTRRaw,(float)fTL,(float)fTR,(float)fTOF);
      //fTDiff = fTL-fTR;
      fTDiff = fTRSlw-fTLSlw;
      fX = para->GetXCal0() * TMath::ATan( fTDiff * para->GetXCal1() ) + para->GetXCal2();
      Double_t corr[]={4.84429e+00,2.56768e+00,3.39064e+00,-6.97872e-02};
      
      fX -= corr[0] + corr[1]*sin(corr[2]+corr[3]*fX);
      //      fX -= corr2[0] + corr2[1]*sin(corr2[2]+corr2[3]*fTDiff);
      //F5 only
      // Double_t f5x_para[]={-5.71224,41.8764,-0.591498,-0.264201,0.291574,-0.142484,-0.0197175,0.00793507};
      
      // fX = 0;
      // //      for(int j=0;j< sizeof(f5x_para);j++){
      // //      for(int j=0;j< 7;j++){
      // 	fX += f5x_para[j]*pow(fTDiff,j);
      // }
      //fX     = fDT2X[0] * pow(fTDiff,2) + fDT2X[1] * fTDiff + fDT2X[2];
    }


    pla->SetTAveCal(fTAveCal);
    pla->SetTLCal(fTLCal);
    pla->SetTRCal(fTRCal);
    pla->SetTLSlw(fTLSlw);
    pla->SetTRSlw(fTRSlw);
    pla->SetTAveSlw(fTAveSlw);
    pla->SetTAveSlwOff(fTAveSlwOff);
    pla->SetQLPed(fQLPed);
    pla->SetQRPed(fQRPed);
    pla->SetQAvePed(fQAvePed);
    pla->SetTDiff(fTDiff);
    pla->SetXposition(fX);
  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtPlastic * TArtCalibPlastic::GetPlastic(Int_t i) {
  return (TArtPlastic *)fPlasticArray->At(i);
}
//__________________________________________________________
TArtPlasticPara * TArtCalibPlastic::GetPlasticPara(Int_t i) {
  return (TArtPlasticPara *)fPlasticParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibPlastic::GetNumPlastic() {
  return fPlasticArray->GetEntries();
}
//__________________________________________________________
TArtPlastic * TArtCalibPlastic::FindPlastic(Int_t id){
  for(Int_t i=0;i<GetNumPlastic();i++)
    if(id == ((TArtPlastic*)fPlasticArray->At(i))->GetID())
      return (TArtPlastic*)fPlasticArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtPlastic * TArtCalibPlastic::FindPlastic(TString *n){
  for(Int_t i=0;i<GetNumPlastic();i++)
    if(*n == *(((TArtPlastic*)fPlasticArray->At(i))->GetDetectorName()))
      return (TArtPlastic*)fPlasticArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtPlastic * TArtCalibPlastic::FindPlastic(char *n){
  for(Int_t i=0;i<GetNumPlastic();i++)
    if(strcmp(((TArtPlastic*)fPlasticArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtPlastic*)fPlasticArray->At(i);
  return NULL;
}

//_________________________________________________________
void TArtCalibPlastic::NearSearch(vector<int> tlrawlist,vector<int> trrawlist,vector<int> tlidlist,vector<int>tridlist){
  if((int)tlrawlist.size()==0 ||
     (int)trrawlist.size()==0 ||
     (int)tlidlist.size() ==0 ||
     (int)tlidlist.size() ==0 ){
    return;
  }

  // cout << "in search1:" << (int) tlrawlist.size() << endl;
  // cout << "in search2:" << (int) trrawlist.size() << endl;
  // cout << "in search1:" << (int) tlidlist.size() << endl;
  // cout << "in search2:" << (int) tridlist.size() << endl;
  vector<int> taveraw;
  vector<int> tdiff;
  vector<int> tlraw;
  vector<int> trraw;
  TArtPlastic* pla;
  vector<int> idlist;
  vector<int> id;
  //data calc

  for(Int_t idata=0;idata<(int)tlrawlist.size();idata++){
    for(Int_t jdata=0;jdata<(int)trrawlist.size();jdata++){
      //acquire both id

      Int_t idl = tlidlist[idata];
      Int_t idr = tridlist[jdata];
      
      if(idl!=idr){
	continue;
      }else{//leftID==rightID
	tdiff.push_back(abs(tlrawlist[idata]-trrawlist[jdata]));
	taveraw.push_back((tlrawlist[idata]+trrawlist[jdata])/2);
	id.push_back(idl);
	tlraw.push_back(tlrawlist[idata]);
	trraw.push_back(trrawlist[jdata]);
      }
    }
  }
  // cout << "ok" << " " << (int)tdiff.size()<<endl;

  //data fill by id
  for(int kid=1;kid<8;kid++){
    pla = (TArtPlastic *)fPlasticArray->At(kid-1);    
    vector<int> tdiffbyid;
    vector<int> tlrawbyid;
    vector<int> trrawbyid;
    int mindiff=100000;
    int mintlraw=-9999;
    int mintrraw=-9999;
    int avediff=100000;
    //listing
    for(int i=0;i<(int)tdiff.size();i++){
      if(id[i]!=kid){
	continue;
      }else{
	tdiffbyid.push_back(tdiff[i]);
	tlrawbyid.push_back(tlraw[i]);
	trrawbyid.push_back(trraw[i]);
	// cout << tdiff[i] << endl;
      }
    }// end of for(i<tdiff.size)
    // searching
    if((int)tdiffbyid.size()==0){
      pla->SetTLRaw(-9999);
      pla->SetTRRaw(-9999);
      continue;
    }else if((int)tdiffbyid.size()==1){
      pla->SetTLRaw(tlrawbyid[0]);
      pla->SetTRRaw(trrawbyid[0]);
      continue;
    }else{
      //select near 40000ch hit
      for(int j=0;j<(int)tdiffbyid.size();j++){
	if(tdiffbyid[j]<mindiff){
	  mindiff=tdiffbyid[j];
	  mintlraw=tlrawbyid[j];
	  mintrraw=trrawbyid[j];
	}
      }
      pla->SetTLRaw(mintlraw);
      pla->SetTRRaw(mintrraw);
    }
  }//end of for(kid)
  //    cout <<"ok"<<endl;
}
