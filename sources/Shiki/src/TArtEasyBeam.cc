#include <Riostream.h>

#include "TArtEasyBeam.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtShikiParameters.hh"
#include "TArtEasyTarget.hh"
#include "TArtCore.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TFile.h>
#include <TH1.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TString.h>
using namespace std;

TArtEasyBeam* feasybeam = 0;
//________________________________________________________
TArtEasyBeam::TArtEasyBeam(){
  TArtCore::Info(__FILE__,"Creating BigRIPS detector objects...");
  fcalibbdc1hit=new TArtCalibBDC1Hit();
  fcalibbdc2hit=new TArtCalibBDC2Hit();
  fcalibbdc1track=new TArtCalibBDC1Track();
  fcalibbdc2track=new TArtCalibBDC2Track();
  sman = TArtStoreManager::Instance();

  fshikipara=(const TArtShikiParameters*)sman->FindParameters("ShikiParameters");
  fBDC1Z=-6365.6+62.4;
  fBDC2Z=-5365.96+62.4;

  //default target
  feasytarget=fshikipara->FindEasyTarget(1);
  fTgtZ = feasytarget->GetPosZ();
  cout << "tgtz="<<fTgtZ<<endl;
  cout << "tgtname="<<feasytarget->GetDetectorName()->Data()<<endl;
  //  fTgtZ =-4978.89+488.;


  fBDC1Xoffset=-0.705;
  fBDC1Yoffset=-0.31;
  fBDC2Xoffset=-0.555;
  fBDC2Yoffset=-0.66;

  //  fc1 = new TCanvas("fc1","fc1");
  //  fc2 = new TCanvas("fc2","fc2");
  sman = TArtStoreManager::Instance();
  fTDCmode=false;
  fBeammode=true;
  fintgt=true;
  ftrackX = new TF1("ftrackX","pol1");
  ftrackY = new TF1("ftrackY","pol1");
}

TArtEasyBeam::~TArtEasyBeam(){
  ClearData();
}

TArtEasyBeam* TArtEasyBeam::Instance(){
  if(!feasybeam){
    feasybeam=new TArtEasyBeam();
  }
  return feasybeam;
}

//________________________________________________________
void TArtEasyBeam::ClearData(){
  fcalibbdc1hit->ClearData();
  fcalibbdc2hit->ClearData();
  if(!fTDCmode){
    fcalibbdc1track->ClearData();
    fcalibbdc2track->ClearData();
  }
  fBDC1A=-9999;
  fBDC1B=-9999;
  fBDC2A=-9999;
  fBDC2B=-9999;
  fTgtPos.SetXYZ(-9999,-9999,fTgtZ);
  fBDC1Pos.SetXYZ(-9999,-9999,fBDC1Z);
  fBDC2Pos.SetXYZ(-9999,-9999,fBDC2Z);
  fTgtA =-9999;
  fTgtB =-9999;
  fintgt=true;
  fBDC1_reconstructed=false;
  fBDC2_reconstructed=false;
  fbdc1hits.clear();
  fbdc2hits.clear();
  fbdc1tracks.clear();
  fbdc2tracks.clear();
  for(int i=0;i<8;i++){
    fBDC1PosLayer[i]=-9999;
    fBDC1Residual[i]=-9999;
    fBDC1DriftLength[i]=-9999;
    fBDC2PosLayer[i]=-9999;
    fBDC2Residual[i]=-9999;
    fBDC2DriftLength[i]=-9999;
  }
  return;
}

void TArtEasyBeam::ReconstructData(){
  if(fBDC1_reconstructed&&fBDC2_reconstructed)
    return;

  fcalibbdc1hit->ReconstructData();
  fcalibbdc2hit->ReconstructData();
  //TDC
  for(int id=0;id<fcalibbdc1hit->GetNumDCHit();id++){
    TArtDCHit* bdc1hit=fcalibbdc1hit->GetDCHit(id);
    fbdc1hits.push_back(bdc1hit);
  }

  for(int id=0;id<fcalibbdc2hit->GetNumDCHit();id++){
    TArtDCHit* bdc2hit =fcalibbdc2hit->GetDCHit(id);
    fbdc2hits.push_back(bdc2hit);
  }


  if(fBeammode){
    fcalibbdc1track->ReconstructData();
    fcalibbdc2track->ReconstructData();


    {//bdc1
      TClonesArray *bdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
      Int_t bdc1ntr = bdc1trks->GetEntries();

      //choose minimum chi2 track
      Double_t chi2min[]={9999,9999};//for x,y
      Double_t trackid_min[]={-999,-999};//for x,y
      for(Int_t itr=0;itr<bdc1ntr;itr++){
	TArtDCTrack* bdc1track=(TArtDCTrack*)bdc1trks->At(itr);
	fbdc1tracks.push_back(bdc1track);
	for(int j=0;j<2;j++){
	  if(bdc1track->GetPosition(j)>-9999){
	    Double_t chi2=bdc1track->GetChi2();
	    if(chi2min[j]>chi2){
	      chi2min[j]=chi2;
	      trackid_min[j]=itr;
	    }
	  }
	}
      }

      for(int xy=0;xy<2;xy++){
	if(trackid_min[xy]<0) continue;
	for(Int_t j=0;j<fbdc1tracks[trackid_min[xy]]->GetNumHitLayer();j++){
	  Int_t hitid=fbdc1tracks[trackid_min[xy]]->GetHitID(j);
	  Int_t layer=fbdc1hits[hitid]->GetLayer();
	  Int_t tdc = fbdc1hits[hitid]->GetTDC();
	  Double_t wirepos=fbdc1hits[hitid]->GetWirePosition();
	  Double_t x_drift = fbdc1tracks[trackid_min[xy]]->GetHitXPosition(j);
	  Double_t x_track = fbdc1tracks[trackid_min[xy]]->CalcLocalHitPos(j);
	  Double_t driftlength = fbdc1tracks[trackid_min[xy]]->GetDriftLength(j);
	  fBDC1PosLayer[layer]=x_track;
	  fBDC1Residual[layer]=x_track-x_drift;
	  fBDC1DriftLength[layer]=driftlength;
	  fBDC1TDCLayer[layer]=tdc;
	}
      }
      if(trackid_min[0]>-1){
	SetBDC1X(fbdc1tracks[trackid_min[0]]->GetPosition(0)+fBDC1Xoffset);
	SetBDC1A(fbdc1tracks[trackid_min[0]]->GetAngle(0)*1000.);
	SetBDC1XChi2(chi2min[0]);
      }
      if(trackid_min[1]>-1){
	SetBDC1Y(fbdc1tracks[trackid_min[1]]->GetPosition(1)+fBDC1Yoffset);
	SetBDC1B(fbdc1tracks[trackid_min[1]]->GetAngle(1)*1000.);
	SetBDC1YChi2(chi2min[1]);
      }
      fBDC1_reconstructed=true;
    }//end of bdc1

    {//bdc2
      TClonesArray *bdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
      Int_t bdc2ntr = bdc2trks->GetEntries();

      //choose minimum chi2 track
      Double_t chi2min[]={9999,9999};//for x,y
      Double_t trackid_min[]={-999,-999};//for x,y
      for(Int_t itr=0;itr<bdc2ntr;itr++){
	TArtDCTrack* bdc2track=(TArtDCTrack*)bdc2trks->At(itr);
	fbdc2tracks.push_back(bdc2track);
	for(int j=0;j<2;j++){
	  if(bdc2track->GetPosition(j)>-9999){
	    Double_t chi2=bdc2track->GetChi2();
	    if(chi2min[j]>chi2){
	      chi2min[j]=chi2;
	      trackid_min[j]=itr;
	    }
	  }
	}
      }

      for(int xy=0;xy<2;xy++){
	if(trackid_min[xy]<0) continue;
	for(Int_t j=0;j<fbdc2tracks[trackid_min[xy]]->GetNumHitLayer();j++){
	  Int_t hitid=fbdc2tracks[trackid_min[xy]]->GetHitID(j);
	  Int_t layer=fbdc2hits[hitid]->GetLayer();
	  Int_t tdc = fbdc2hits[hitid]->GetTDC();
	  Double_t wirepos=fbdc2hits[hitid]->GetWirePosition();
	  Double_t x_drift = fbdc2tracks[trackid_min[xy]]->GetHitXPosition(j);
	  Double_t x_track = fbdc2tracks[trackid_min[xy]]->CalcLocalHitPos(j);
	  Double_t driftlength = fbdc2tracks[trackid_min[xy]]->GetDriftLength(j);
	  fBDC2PosLayer[layer]=x_track;
	  fBDC2Residual[layer]=x_track-x_drift;
	  fBDC2DriftLength[layer]=driftlength;
	  fBDC2TDCLayer[layer]=tdc;
	}
      }
      if(trackid_min[0]>-1){
	SetBDC2X(fbdc2tracks[trackid_min[0]]->GetPosition(0) + fBDC2Xoffset);
	SetBDC2A(fbdc2tracks[trackid_min[0]]->GetAngle(0)*1000.);
	SetBDC2XChi2(chi2min[0]);
      }
      if(trackid_min[1]>-1){
	SetBDC2Y(fbdc2tracks[trackid_min[1]]->GetPosition(1) + fBDC2Yoffset);
	SetBDC2B(fbdc2tracks[trackid_min[1]]->GetAngle(1)*1000.);
	SetBDC2YChi2(chi2min[1]);
      }
      fBDC2_reconstructed=true;
    }//end of bdc2

    if(fBDC1Pos.X()>-9999 && fBDC2Pos.X()>-9999 && 
       fBDC1Pos.Y()>-9999 && fBDC2Pos.Y()>-9999 
       ){//Global tracking; Target position
      Double_t Xarr[]={fBDC1Pos.X(),fBDC2Pos.X()};
      Double_t Yarr[]={fBDC1Pos.Y(),fBDC2Pos.Y()};
      Double_t Zarr[]={fBDC1Pos.Z(),fBDC2Pos.Z()};
      Double_t ax = (Xarr[1]-Xarr[0])/(Zarr[1]-Zarr[0]);
      Double_t ay = (Yarr[1]-Yarr[0])/(Zarr[1]-Zarr[0]);
      Double_t bx = Xarr[0]-ax*Zarr[0];
      Double_t by = Yarr[0]-ay*Zarr[0];
      SetTgtX(ax*fTgtZ+bx);
      SetTgtY(ay*fTgtZ+by);
      SetTgtZ(fTgtZ);
      SetTgtA(atan(ax)*1000.);
      SetTgtB(atan(ay)*1000.);
      Double_t r=sqrt(pow(fTgtPos.X(),2.)+pow(fTgtPos.Y(),2.));
      if(r>=40.){
	fintgt=false;
      }
    }


  }//end of if(fBeammode || fTotalmode)

  return;
}

void TArtEasyBeam::SetTDCDistribution(TString filename){
  TFile* file = new TFile(filename.Data());
  if(file->IsOpen()){
    gROOT->cd();
    TH1* hist = NULL;
    // BDC1
    for(Int_t ihist=0;ihist<8;ihist++){
      hist = (TH1D*)file->Get(Form("hbdc1tdc%d",ihist));
      fcalibbdc1track->SetTDCDistribution(hist,ihist);
      delete hist;
      hist = NULL;
    }

    // BDC2
    for(Int_t ihist=0;ihist<8;ihist++){
      hist = (TH1D*)file->Get(Form("hbdc2tdc%d",ihist));
      fcalibbdc2track->SetTDCDistribution(hist,ihist);
      delete hist;
      hist = NULL;
    }
  }
  return;
}


TLorentzVector TArtEasyBeam::GetMomentumVector(Double_t p,Double_t E){//p,E is "total" momentum/energy
  TVector3 beam_direction = fBDC2Pos-fBDC1Pos;
  Double_t length = beam_direction.Mag();
  beam_direction = beam_direction*(p/length);
  TLorentzVector P;
  P.SetVect(beam_direction);
  P.SetE(E);
  return P;
}

void TArtEasyBeam::SelectTarget(TString name){
  for(Int_t i=1;i<=fshikipara->GetNumTarget();i++){
    if(name== *(fshikipara->FindEasyTarget(i)->GetDetectorName())){
      feasytarget=fshikipara->FindEasyTarget(i);
    }
  }
  TArtCore::Info(__FILE__,Form("selected target is %s, position=%fmm",feasytarget->GetDetectorName()->Data(),feasytarget->GetPosZ()));
  fTgtZ=feasytarget->GetPosZ();
}
