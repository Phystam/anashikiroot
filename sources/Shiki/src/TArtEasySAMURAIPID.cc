#include "TArtEasySAMURAIPID.hh"
#include "TArtEasyPID.hh"
#include "TArtEasyBeam.hh"
#include "TArtEasyTarget.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtShikiParameters.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtCalibFDC1Track.hh"
#include "TArtCalibFDC2Track.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"
#include "TArtCalibHODPla.hh"
#include "TArtHODPla.hh"
#include "TArtMDF_s027_BrhoMDF.hh"
#include "TArtMDF_s027_FlMDF.hh"
#include "TArtMDF_s027_TofMDF.hh"
#include "TArtEasyMassExcess.hh"
#include <vector>
#include <TFile.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TString.h>
#include <TMath.h>
#include "TArtEasyEnergyLossFunc.hh"
#include "TArtMDF_Frag_Eloss_C.hh"
#include "TArtMDF_Frag_Eloss_Pb.hh"
#include "TArtMDF_Frag_Eloss_Emp.hh"
using namespace std;
TArtEasySAMURAIPID::TArtEasySAMURAIPID(){
  TArtCore::Info(__FILE__,"Creating SAMURAI detector objects...");
  sman = TArtStoreManager::Instance();
  feasypid=TArtEasyPID::Instance();
  feasybeam=TArtEasyBeam::Instance();
  fshikipara=TArtShikiParameters::Instance();
  feasytarget = fshikipara->FindEasyTarget(1);
  fselectedtarget=false;
  frunnum=0;
  fcalibhod=new TArtCalibHODPla();
  feasyex=new TArtEasyMassExcess();
  feasyeloss=new TArtEasyEnergyLossFunc();
  fbrhomdf=new TArtMDF_s027_BrhoMDF();
  ftofmdf=new TArtMDF_s027_TofMDF();
  fflmdf=new TArtMDF_s027_FlMDF();
  fcalibfdc1hit=new TArtCalibFDC1Hit();
  fcalibfdc2hit=new TArtCalibFDC2Hit();
  fcalibfdc1track=new TArtCalibFDC1Track();
  fcalibfdc2track=new TArtCalibFDC2Track();
  fFDC1Z=-2888.82+168.;
  fFDC2Z=2888.82+168.;// atode


  //   fFDC1Xoffset=0.185;
   // fFDC1Yoffset=1.375;
  //  fFDC1Xoffset=-0.119;
   //   fFDC1Yoffset=-0.119; //fixed
     fFDC1Xoffset=-0.482;
   fFDC1Yoffset=0;
  // fFDC1Xoffset=-1.002; //fixed

  fTDCmode=false;
  ClearData();
}

TArtEasySAMURAIPID::~TArtEasySAMURAIPID(){;}

void TArtEasySAMURAIPID::ClearData(){
  feasypid->ClearData();  
  feasybeam->ClearData();  
  fcalibhod->ClearData();
  fBrho      = -9999;
  fTOFTgtHOD = -9999;
  fTOFSim    = -9999;
  fTOFSBTTgt = -9999;
  fTOFSBTHOD = -9999;
  fFl        = -9999;
  fDeltaE    = -9999;
  fBeta      = -9999;
  fGamma     = -9999;
  fZet       = -9999;
  fAoZ       = -9999;
  fhodtime   = -9999;
  ftgttime   = -9999;
  fhodid     = -9999;
  fsbttime   = -9999;
  fhodqped   = -9999;
  fhodquped   = -9999;
  fhodqdped   = -9999;
  fhodtu     = -9999;
  fhodtd     = -9999;
  fEnergy    = -9999;
  fKE        = -9999;
  fMomentum  = -9999;
  fMomentum4D.SetXYZT(-9999,-9999,-9999,-9999);
  fnumhodpla = 0;

  //FDC
  fcalibfdc1hit->ClearData();
  fcalibfdc2hit->ClearData();
  if(!fTDCmode){
    fcalibfdc1track->ClearData();
    fcalibfdc2track->ClearData();
  }

  fFDC1X=-9999;
  fFDC1Y=-9999;
  fFDC2X=-9999;
  fFDC2Y=-9999;
  fFDC1A=-9999;
  fFDC1B=-9999;
  fFDC2A=-9999;
  fFDC2B=-9999;
  fFDC1_reconstructed=false;
  fFDC2_reconstructed=false;
  ffdc1hits.clear();
  ffdc2hits.clear();
  ffdc1tracks.clear();
  ffdc2tracks.clear();
  for(int i=0;i<14;i++){
    fFDC1PosLayer[i]=-9999;
    fFDC1Residual[i]=-9999;
    fFDC1DriftLength[i]=-9999;
    fFDC2PosLayer[i]=-9999;
    fFDC2Residual[i]=-9999;
    fFDC2DriftLength[i]=-9999;
    for(int j=0;j<112;j++){
      fFDC2ResidualWire[i][j]=-9999;
    }
  }

}

void TArtEasySAMURAIPID::LoadData(){
  fcalibhod->ReconstructData();
  feasypid->ReconstructData();
  feasybeam->ReconstructData();
}

void TArtEasySAMURAIPID::ReconstructData(){
  if(!fselectedtarget){
    TArtCore::Warning(__FILE__,"Target is not selected, use TArtEasySAMURAIPID::SelectTarget()");
    fselectedtarget=true;
  }
  LoadData();
  FDCReconstructData();
  Double_t fdc1a = atan((fFDC1X-feasybeam->GetTgtX())/(fFDC1Z-feasybeam->GetTgtZ()));
  Double_t fdc1b = atan((fFDC1Y-feasybeam->GetTgtY())/(fFDC1Z-feasybeam->GetTgtZ()));
  Double_t Input[]={GetFDC1X(),
		    fdc1a,
		    GetFDC1Y(),
		    fdc1b,
		    GetFDC2X(),
		    GetFDC2A()*0.001};
  fBrho=fbrhomdf->MDF(Input);  

  fFl=fflmdf->MDF(Input);  

  TArtHODPla* pla_qmax;
  {//hodscope
    Int_t numhodpla=fcalibhod->GetNumHODPla();
    Double_t q_max=0;
    Double_t qped_max=0;
    Double_t quped_max=0;
    Double_t qdped_max=0;
    Double_t t_qmax=0;
    Double_t tu_qmax=0;
    Double_t td_qmax=0;
    Int_t id_qmax=100;
    for(int i=0;i<numhodpla;i++){
      TArtHODPla* pla=fcalibhod->GetHODPla(i);
      if(pla->GetTURaw()>0&&pla->GetTDRaw()>0&&pla->GetQURaw()>0&&pla->GetQURaw()>0){
	fnumhodpla++;
	Double_t q=pla->GetQAveCal();
	Double_t qped=pla->GetQAvePed();
	Double_t quped=pla->GetQUPed();
	Double_t qdped=pla->GetQDPed();
	Double_t t=pla->GetTAveSlw();
	Double_t tu=pla->GetTUSlw();
	Double_t td=pla->GetTDSlw();
	Int_t id=pla->GetID();
	if(q>q_max){
	  q_max=q;
	  quped_max=quped;
	  qdped_max=qdped;
	  qped_max=qped;
	  t_qmax=t;
	  tu_qmax=tu;
	  td_qmax=td;
	  id_qmax=id;
	  pla_qmax=pla;
	}
      }
    }
    if(id_qmax>24){
      return;
    }
    fhodtime=t_qmax;
    fhodtu=tu_qmax;
    fhodtd=td_qmax;
    fhodquped=quped_max;
    fhodqdped=qdped_max;
    fhodid=id_qmax;
    fhodq=q_max;
    fhodqped=qped_max;
  }//end of hod scope
  fsbttime=feasypid->GetSBTTime();
  ftgttime=fsbttime+feasypid->GetTOFSBTTgt();
  fTOFSBTTgt=feasypid->GetTOFSBTTgt();//~15ns
  fTOFSBTHOD=fhodtime-fsbttime;//~65ns
  fTOFTgtHOD=fhodtime-ftgttime;//~50ns
  //
  //  std::cout<<fFl<<std::endl;
  //整数値のZを求める
  fBeta=fFl/fTOFTgtHOD/fclight;
  fGamma=1./sqrt(1.-fBeta*fBeta);
  fAoZ=fclight*fBrho/(famu_MeV*fBeta*fGamma);
  fZet=pla_qmax->GetZet(fBeta);

  //Zの位置補正・ラン補正
  RunCorrections();
  CalcXYCorrectionOfHODF();

  Int_t Z=GetZetInt();
  Int_t A=GetAInt();
  //Z(整数)とBrhoから運動量を求める
  fMomentum = (Double_t)Z*fBrho*fclight; //fclight:~300mm/ns
  //運動量とbetaからエネルギーを求める...えっこれアウトやん
  //ちゃんと質量を与えてあげないと意味わからんくない？
  Double_t mass = A*famu_MeV + feasyex->GetMassExcess(A,Z);
  fEnergy = sqrt(pow(fMomentum,2.)+pow(mass,2.));
  //  fEnergy = fMomentum/fBeta;
  //フラグメントの不変質量
  TVector3 vect=fFDC1Pos-feasybeam->GetTgtPos();
  Double_t mag = vect.Mag();
  vect = vect*(fMomentum/mag);
  fMomentum4D.SetVect(vect);
  fMomentum4D.SetE(fEnergy);
}

Double_t TArtEasySAMURAIPID::GetTOFSimTotal(){
  //  Double_t mass = (Double_t)GetAInt()*famu_MeV + feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t mass = 30.*famu_MeV + feasyex->GetMassExcess(30,10);
  Double_t betagamma = GetZetInt()/mass*fBrho*fclight;
  Double_t beta = betagamma/sqrt(1+pow(betagamma,2.));
  Double_t velosity=beta*fclight;
  Double_t tofsbttgt=feasypid->GetTOFSBTTgt();
  //  std::cout<<fFl/velosity<<" "<<tofsbttgt<<std::endl;
  return fFl/velosity+tofsbttgt;
}

Double_t TArtEasySAMURAIPID::GetTOFTgtHODSim(){
  //  Double_t mass = (Double_t)GetAInt()*famu_MeV + feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t mass = 30.*famu_MeV + feasyex->GetMassExcess(30,10);
  //  Double_t betagamma = GetZetInt()/mass*fBrho*fclight;
  Double_t betagamma = 10./mass*fBrho*fclight;
  Double_t beta = betagamma/sqrt(1+pow(betagamma,2.));
  Double_t velosity=beta*fclight;
  //  Double_t tofsbttgt=feasypid->GetTOFSBTTgt();
  //  std::cout<<fFl/velosity<<" "<<tofsbttgt<<std::endl;
  return fFl/velosity;
}

//target中心での4元運動量の計算
TLorentzVector TArtEasySAMURAIPID::GetMomentum4DAtTgt(){
  TLorentzVector momentum;
  TVector3 pvects = fMomentum4D.Vect();
  Double_t m = fMomentum4D.M();
  Double_t Ts=fMomentum4D.E()-m;
  //  std::vector<Double_t> para=feasyeloss->GetFragEnergyLossPara(GetAInt(),GetZetInt(),*feasytarget->GetDetectorName());
  // std::cout << GetAInt() <<"  "<<GetZetInt() <<"  "<<*feasytarget->GetDetectorName()<< std::endl;
  // std::cout <<" "<< para[0] <<" "<< para[1] <<" "<< para[2] <<" "<< para[3] << std::endl;
  //Double_t* para=feasytarget->GetFragE();
  Double_t u = m/famu_MeV;
  //  Ts = (Double_t)Ts/A;//Kinetic energy as AMeV
  Ts = (Double_t)Ts/u;//Kinetic energy as MeV/u
  Double_t Tt=0;
  Double_t input[]={GetAInt(),GetZetInt(),Ts};
  if(*feasytarget->GetDetectorName()=="C"){
    Tt = TArtMDF_Frag_Eloss_C::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Pb"){
    Tt = TArtMDF_Frag_Eloss_Pb::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Emp"){
    Tt = TArtMDF_Frag_Eloss_Emp::MDF(input);
  }
  if(*feasytarget->GetDetectorName()=="Al"){
    Tt = 285.814;
  }

  //  if(GetAInt()==31&&GetZetInt()==10){
    //    std::cout <<m<<" "<<Ts<<" "<<Tt<<" "<<para[0]<<" "<<para[1]<<" "<<para[2]<<" "<<para[3]<<std::endl;
  //  }
  Tt *= u;

  Double_t Et=Tt+m;
  Double_t pt=sqrt(Et*Et-m*m);
  Double_t theta=pvects.Theta();
  Double_t phi=pvects.Phi();
  TVector3 pvectt;
  pvectt.SetMagThetaPhi(pt,theta,phi);
  momentum.SetVect(pvectt);
  momentum.SetE(Et);
  return momentum;
}

void TArtEasySAMURAIPID::FDCReconstructData(){
  fcalibfdc1hit->ReconstructData();
  fcalibfdc2hit->ReconstructData();
  for(int id=0;id<fcalibfdc1hit->GetNumDCHit();id++){
    TArtDCHit* fdc1hit =fcalibfdc1hit->GetDCHit(id);
    ffdc1hits.push_back(fdc1hit);
  }
  for(int id=0;id<fcalibfdc2hit->GetNumDCHit();id++){
    TArtDCHit* fdc2hit =fcalibfdc2hit->GetDCHit(id);
    ffdc2hits.push_back(fdc2hit);
  }
  if(!fTDCmode){
    fcalibfdc1track->ReconstructData();
    fcalibfdc2track->ReconstructData();
    {
      TClonesArray *fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
      Int_t fdc1ntr = fdc1trks->GetEntries();

      //choose minimum chi2 track
      Double_t chi2min[]={9999,9999};//for x,y
      Double_t trackid_min[]={-999,-999};//for x,y
      for(Int_t itr=0;itr<fdc1ntr;itr++){
	TArtDCTrack* fdc1track=(TArtDCTrack*)fdc1trks->At(itr);
	ffdc1tracks.push_back(fdc1track);
	for(int j=0;j<2;j++){
	  if(fdc1track->GetPosition(j)>-9999){
	    Double_t chi2=fdc1track->GetChi2();
	    if(chi2min[j]>chi2){
	      chi2min[j]=chi2;
	      trackid_min[j]=itr;
	    }
	  }
	}
      }

      for(int xy=0;xy<2;xy++){
	if(trackid_min[xy]<0) continue;
	for(Int_t j=0;j<ffdc1tracks[trackid_min[xy]]->GetNumHitLayer();j++){
	  Int_t hitid=ffdc1tracks[trackid_min[xy]]->GetHitID(j);
	  Int_t layer=ffdc1hits[hitid]->GetLayer();
	  Int_t tdc = ffdc1hits[hitid]->GetTDC();
	  Double_t wirepos=ffdc1hits[hitid]->GetWirePosition();
	  Double_t x_drift = ffdc1tracks[trackid_min[xy]]->GetHitXPosition(j); //tracked position
	  Double_t x_track = ffdc1tracks[trackid_min[xy]]->CalcLocalHitPos(j);
	  Double_t driftlength = ffdc1tracks[trackid_min[xy]]->GetDriftLength(j);
	  fFDC1PosLayer[layer]=x_track;
	  fFDC1Residual[layer]=x_track-x_drift;
	  fFDC1DriftLength[layer]=driftlength;
	  fFDC1TDCLayer[layer]=tdc;
	}
      }
      if(trackid_min[0]>-1){
	SetFDC1X(ffdc1tracks[trackid_min[0]]->GetPosition(0)+fFDC1Xoffset);
	//	SetFDC1A(ffdc1tracks[trackid_min[0]]->GetAngle(0)*1000.);
	//FDC1A-> calculate from target position and FDC1 position

	SetFDC1XChi2(chi2min[0]);
      }
      if(trackid_min[1]>-1){
	SetFDC1Y(-ffdc1tracks[trackid_min[1]]->GetPosition(1)-fFDC1Yoffset);
	//SetFDC1B(-ffdc1tracks[trackid_min[1]]->GetAngle(1)*1000.);
	SetFDC1YChi2(chi2min[1]);
      }
      //calculate FDC1A&B from target position and FDC1 position
      Double_t fdc1a = atan((GetFDC1X()-feasybeam->GetTgtX())/(GetFDC1Z()-feasybeam->GetTgtZ())) * 1000;
      Double_t fdc1b = atan((GetFDC1Y()-feasybeam->GetTgtY())/(GetFDC1Z()-feasybeam->GetTgtZ())) * 1000;
      SetFDC1B(fdc1b);
    }//end of fdc1

    {//fdc2
      TClonesArray *fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");
      Int_t fdc2ntr = fdc2trks->GetEntries();
      //      cout << fdc2ntr << endl;
      //choose minimum chi2 track
      Double_t chi2min[]={9999,9999};//for x,y
      Double_t trackid_min[]={-999,-999};//for x,y
      for(Int_t itr=0;itr<fdc2ntr;itr++){
	TArtDCTrack* fdc2track=(TArtDCTrack*)fdc2trks->At(itr);
	ffdc2tracks.push_back(fdc2track);
	for(int j=0;j<2;j++){
	  if(fdc2track->GetPosition(j)>-9999){
	    Double_t chi2=fdc2track->GetChi2();
	    if(chi2min[j]>chi2){
	      chi2min[j]=chi2;
	      trackid_min[j]=itr;
	    }
	  }
	}
      }
      for(int xy=0;xy<2;xy++){
	if(trackid_min[xy]<0) continue;
	for(Int_t j=0;j<ffdc2tracks[trackid_min[xy]]->GetNumHitLayer();j++){
	  Int_t hitid=ffdc2tracks[trackid_min[xy]]->GetHitID(j);
	  Int_t layer=ffdc2hits[hitid]->GetLayer();
	  Int_t tdc = ffdc2hits[hitid]->GetTDC();
	  Double_t wirepos=ffdc2hits[hitid]->GetWirePosition();
	  Int_t wire=ffdc2hits[hitid]->GetWireID();
	  Double_t x_drift = ffdc2tracks[trackid_min[xy]]->GetHitXPosition(j);
	  Double_t x_track = ffdc2tracks[trackid_min[xy]]->CalcLocalHitPos(j);
	  Double_t driftlength = ffdc2tracks[trackid_min[xy]]->GetDriftLength(j);
	  fFDC2PosLayer[layer]=x_track;
	  fFDC2ResidualWire[layer][wire]=x_track-x_drift;
	  fFDC2Residual[layer]=x_track-x_drift;
	  fFDC2DriftLength[layer]=driftlength;
	  fFDC2TDCLayer[layer]=tdc;
	}
      }
      if(trackid_min[0]>-1){
	SetFDC2X(ffdc2tracks[trackid_min[0]]->GetPosition(0));
	SetFDC2A(ffdc2tracks[trackid_min[0]]->GetAngle(0)*1000.);
	SetFDC2XChi2(chi2min[0]);
      }
      if(trackid_min[1]>-1){
	SetFDC2Y(ffdc2tracks[trackid_min[1]]->GetPosition(1));
	SetFDC2B(ffdc2tracks[trackid_min[1]]->GetAngle(1)*1000.);
	SetFDC2YChi2(chi2min[1]);
      }
    }//end of fdc2
  }
}

void TArtEasySAMURAIPID::SetTDCDistribution(TString filename){
  feasybeam->SetTDCDistribution(filename);
  TFile* file = new TFile(filename.Data());
  if(file->IsOpen()){
    gROOT->cd();
    TH1* hist = NULL;
    // FDC1
    for(Int_t ihist=0;ihist<14;ihist++){
      hist = (TH1D*)file->Get(Form("hfdc1tdc%d",ihist));
      fcalibfdc1track->SetTDCDistribution(hist,ihist);
      delete hist;
      hist = NULL;
    }

    //FDC2
    for(Int_t ihist=0;ihist<14;ihist++){
      hist = (TH1D*)file->Get(Form("hfdc2tdc%d",ihist));
      fcalibfdc2track->SetTDCDistribution(hist,ihist);
      delete hist;
      hist = NULL;
    }
  }
  file->Close();
  delete file;
  return;
}

void TArtEasySAMURAIPID::SelectTarget(TString name){
  for(Int_t i=1;i<=fshikipara->GetNumTarget();i++){
    if(name== *(fshikipara->FindEasyTarget(i)->GetDetectorName())){
      feasytarget=fshikipara->FindEasyTarget(i);
    }
  }
  TArtCore::Info(__FILE__,Form("selected target is %s, position=%fmm",feasytarget->GetDetectorName()->Data(),feasytarget->GetPosZ()));
  feasypid->SelectTarget(name);
  feasybeam->SelectTarget(name);
  fselectedtarget=true;
}

void TArtEasySAMURAIPID::SelectTarget(Int_t ID){
  feasytarget=fshikipara->FindEasyTarget(ID);
  TArtCore::Info(__FILE__,Form("selected target is %s, position=%fmm",feasytarget->GetDetectorName()->Data(),feasytarget->GetPosZ()));
  feasypid->SelectTarget(ID);
  feasybeam->SelectTarget(ID);
  fselectedtarget=true;
}

void TArtEasySAMURAIPID::CalcXYCorrectionOfHODF(){
  Double_t p0,p1;
  switch(fhodid){
  case 9:
    p0=9.45996;
    p1=0.00126335;
    break;
  case 10:
    p0=9.40343;
    p1=0.00163013;
    break;
  case 11:
    p0=9.4735;
    p1=0.00233;
       break;
  default:
    p0=10;
    p1=0;
    break;
  }
  fZet=fZet/(p0+p1*GetHODX())*10.;
  fZet=fZet/(9.98+1.41403e-7*pow(GetHODY(),2)+1.07601e-11*pow(GetHODY(),4.))*10.;
}

void TArtEasySAMURAIPID::RunCorrections(){
  Double_t factor=1;
  switch(frunnum){
  case 238:
    factor=1.03213;
    break;
  case 239:
    factor=1.02662;
    break;
  case 240:
    factor=1.01764;
    break;
  case 241:
    factor=1.01752;
    break;
  case 242:
    factor=1.01774;
    break;
  case 243:
    factor=1.00606;
    break;
  case 244:
    factor=1.00044;
    break;
  default:
    factor=1;
    break;
  }
  if(fhodid==18){
    fZet=fZet/factor;
  }

}
