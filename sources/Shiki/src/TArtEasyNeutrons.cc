#include "TArtEasyNeutrons.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtEasyPID.hh"
#include "TArtShikiParameters.hh"
#include "TArtEasyTarget.hh"
#include "TArtEasyBeam.hh"
#include "TArtEasyPID.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtCalibNeuLAND.hh"
#include "TArtCalibNeuLANDVETO.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNeuLANDPla.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtEasyNeutron.hh"

#include <Riostream.h>
#include <TROOT.h>
#include <TMath.h>
#include <TVector3.h>
#include <algorithm>
using namespace std;
TArtEasyNeutrons::TArtEasyNeutrons(){
  TArtCore::Info(__FILE__,"Creating Neutron detector objects...");
  fcalibnebula = new TArtCalibNEBULA();
  fcalibneuland = new TArtCalibNeuLAND();
  fcalibneulandveto = new TArtCalibNeuLANDVETO();
  feasypid=TArtEasyPID::Instance();
  feasybeam=TArtEasyBeam::Instance();
  fshikipara = TArtShikiParameters::Instance();
  ftgtz=feasybeam->GetTgtZ();

}

TArtEasyNeutrons::~TArtEasyNeutrons(){
  ClearData();
}

//________________________________________________________
void TArtEasyNeutrons::ClearData(){
  fcalibnebula->ClearData();
  fcalibneuland->ClearData();
  fcalibneulandveto->ClearData();
  // std::cout<<"clear start"<<std::endl;
  // std::cout<<" before clear: "<<(int)easyneuts.capacity()<<" "<<(int)easyneuts.size()<<std::endl;
  for(int i=0;i<(int)easyneuts.size();i++){
    delete easyneuts[i];
  }
  easyneuts.clear();
  
  //   std::cout<<" after clear: "<<(int)easyneuts.capacity()<<" "<<(int)easyneuts.size()<<std::endl;
  //  std::vector<TArtEasyNeutron*>().swap(easyneuts);//memory開放できるらしい
  //   std::cout<<" after swap: "<<(int)easyneuts.capacity()<<" "<<(int)easyneuts.size()<<std::endl;
  //  easyneuts.reserve(20);
  //  std::cout<<" after clear: "<<(int)easyneuts.capacity()<<" "<<(int)easyneuts.size()<<std::endl;
  fnumnebulapla=0;
  fisveto=false;
  feasypid->ClearData();
  feasybeam->ClearData();
}

void TArtEasyNeutrons::LoadData(){
  fcalibnebula->ReconstructData();
  fcalibneuland->ReconstructData();
  fcalibneulandveto->ReconstructData();
  feasybeam->ReconstructData();
  feasypid->ReconstructData();
}

void TArtEasyNeutrons::ReconstructData(){
  LoadData();
  
  //target time/position
  Double_t tgttime = feasypid->GetSBTTime()+feasypid->GetTOFSBTTgt();
  TVector3 targetpos = feasybeam->GetTgtPos();
  //  cout << targetpos.X() <<" "<<targetpos.Y()<<endl;
  if(targetpos.X()<-1000){
    targetpos.SetX(0);
  }
  if(targetpos.Y()<-1000){
    targetpos.SetY(0);
  }
  //NEBULA refill
  fnumnebulapla = fcalibnebula->GetNumNEBULAPla();
  for(Int_t i=0;i<fnumnebulapla;i++){
    TArtNEBULAPla* pla=fcalibnebula->GetNEBULAPla(i);
    //    cout << "NEBULA ID:" <<pla->GetID()<< endl;
    if(pla->GetID()==145)continue;
    if(!(pla->GetTURaw()>0&&pla->GetTDRaw()>0))continue;
    TArtEasyNeutron* easyneut = new TArtEasyNeutron;
    
    if(pla->GetID()<121){
      easyneut->SetID(pla->GetID()+400);
    }else if(pla->GetID()>=121){
      easyneut->SetID(pla->GetID()+408);
    }
    easyneut->SetLayer(pla->GetLayer());
    easyneut->SetSubLayer(pla->GetSubLayer());
    if(pla->GetSubLayer()==0){
      easyneut->SetIsVeto(true);
    }else{
      easyneut->SetIsVeto(false);
    }
    easyneut->SetT1Raw(pla->GetTURaw());
    easyneut->SetT2Raw(pla->GetTDRaw());
    easyneut->SetT1Cal(pla->GetTUCal());
    easyneut->SetT2Cal(pla->GetTDCal());
    easyneut->SetT1Slw(pla->GetTUSlw());
    easyneut->SetT2Slw(pla->GetTDSlw());
    easyneut->SetQ1Raw(pla->GetQURaw());
    easyneut->SetQ2Raw(pla->GetQDRaw());
    easyneut->SetQ1Ped(pla->GetQUPed());
    easyneut->SetQ2Ped(pla->GetQDPed());
    easyneut->SetQ1Cal(pla->GetQUCal());
    easyneut->SetQ2Cal(pla->GetQDCal());
    easyneut->SetQAveCal(pla->GetQAveCal());
    easyneut->SetTAveCal(pla->GetTAveCal());
    easyneut->SetTAveSlw(pla->GetTAveSlw());
    easyneut->SetDTCal(pla->GetDTCal());
    easyneut->SetDTSlw(pla->GetDTSlw());
    easyneut->SetPos(pla->GetPos(0),pla->GetPos(1),pla->GetPos(2));
    //    cout << pla->GetDTCal()<<" "<<pla->GetDTSlw()<<" "<<pla->GetPos(1) << endl;

    TVector3 neutronpos(pla->GetPos(0),pla->GetPos(1),pla->GetPos(2));
    Double_t tof = pla->GetTAveSlw()-tgttime;//ns
    TVector3 flvec=neutronpos-targetpos;

    Double_t fl = flvec.Mag();//mm
    Double_t v = fl/tof;//mm/ns
    Double_t beta = v/fclight;
    Double_t gamma = 1./sqrt(1-beta*beta);
    easyneut->SetTOF(tof);
    easyneut->SetBeta(beta);
    easyneut->SetGamma(gamma);
    easyneut->SetEnergy(fMass*gamma);
    easyneut->SetMomentum(fMass*beta*gamma);
    TLorentzVector momentum4d;
    TVector3 pvect_u = flvec*(1./fl);
    pvect_u = pvect_u*fMass*beta*gamma;
    //    momentum4d.SetVectMag(pvect_u,fMass*gamma);
    momentum4d.SetVectM(pvect_u,fMass);
    easyneut->SetMomentum4D(momentum4d);
    if(!TMath::IsNaN(tof)){
      easyneuts.push_back(easyneut);
    }
  }

  //NeuLAND
  fnumneulandpla = fcalibneuland->GetNumNeuLANDPla();
  for(Int_t i=0;i<fnumneulandpla;i++){
    TArtNeuLANDPla* pla=fcalibneuland->GetNeuLANDPla(i);
    //    cout << "NeuLAND ID:"<< pla->GetID() << endl;
    if(!pla->GetBothFired())continue;
    TArtEasyNeutron* easyneut = new TArtEasyNeutron;
    easyneut->SetID(pla->GetID());//

    easyneut->SetLayer(0);
    easyneut->SetSubLayer(pla->GetLayer()+1);
    easyneut->SetT1Raw(pla->GetTac(0));
    easyneut->SetT2Raw(pla->GetTac(1));
    easyneut->SetT1Cal(pla->GetTRaw(0));
    easyneut->SetT2Cal(pla->GetTRaw(1));
    easyneut->SetT1Slw(pla->GetTCal(0));
    easyneut->SetT2Slw(pla->GetTCal(1));
    easyneut->SetQ1Raw(pla->GetQRaw(0));
    easyneut->SetQ2Raw(pla->GetQRaw(1));
    easyneut->SetQ1Ped(0);
    easyneut->SetQ2Ped(0);
    easyneut->SetQ1Cal(pla->GetQCal(0));
    easyneut->SetQ2Cal(pla->GetQCal(1));
    easyneut->SetQAveCal(sqrt(pla->GetQCal(0)*pla->GetQCal(1)));
    easyneut->SetTAveCal((pla->GetTRaw(0)+pla->GetTRaw(1))/2.);
    easyneut->SetTAveSlw((pla->GetTCal(0)+pla->GetTCal(1))/2.);
    easyneut->SetDTCal(pla->GetTRaw(1)-pla->GetTRaw(0));
    easyneut->SetDTSlw(pla->GetTCal(1)-pla->GetTCal(1));
    easyneut->SetPos(pla->GetX(),pla->GetY(),pla->GetZ());

    TVector3 neutronpos(pla->GetX(),pla->GetY(),pla->GetZ());
    Double_t tof = (pla->GetTCal(0)+pla->GetTCal(1))/2.-tgttime;//ns
    TVector3 flvec=neutronpos-targetpos;
    Double_t fl = flvec.Mag();//mm
    Double_t v = fl/tof;//mm/ns
    Double_t beta = v/fclight;
    Double_t gamma = 1./sqrt(1-beta*beta);
    easyneut->SetTOF(tof);
    easyneut->SetBeta(beta);
    easyneut->SetGamma(gamma);
    easyneut->SetEnergy(fMass*gamma);
    easyneut->SetMomentum(fMass*beta*gamma);
    TLorentzVector momentum4d;
    TVector3 pvect_u = flvec*(1./fl);
    pvect_u = pvect_u*fMass*beta*gamma;
    momentum4d.SetVectM(pvect_u,fMass);
    easyneut->SetMomentum4D(momentum4d);
    if(!TMath::IsNaN(tof)){
      easyneuts.push_back(easyneut);
    }
  }
  
  fnumneulandvetopla = fcalibneulandveto->GetNumNeuLANDVETOPla();
  for(Int_t i=0;i<fnumneulandvetopla;i++){
    TArtNEBULAPla* pla=fcalibneulandveto->GetNeuLANDVETOPla(i);
    if(pla->GetID()==9)continue;
    if(!(pla->GetTURaw()>0&&pla->GetTDRaw()>0))continue;
    TArtEasyNeutron* easyneut = new TArtEasyNeutron;
    
    easyneut->SetID(pla->GetID()+400+120);
    easyneut->SetLayer(0);
    easyneut->SetSubLayer(0);
    easyneut->SetIsVeto(true);
    fisveto=true;
    easyneut->SetT1Raw(pla->GetTURaw());
    easyneut->SetT2Raw(pla->GetTDRaw());
    easyneut->SetT1Cal(pla->GetTUCal());
    easyneut->SetT2Cal(pla->GetTDCal());
    easyneut->SetT1Slw(pla->GetTUSlw());
    easyneut->SetT2Slw(pla->GetTDSlw());
    easyneut->SetQ1Raw(pla->GetQURaw());
    easyneut->SetQ2Raw(pla->GetQDRaw());
    easyneut->SetQ1Ped(pla->GetQUPed());
    easyneut->SetQ2Ped(pla->GetQDPed());
    easyneut->SetQ1Cal(pla->GetQUCal());
    easyneut->SetQ2Cal(pla->GetQDCal());
    easyneut->SetQAveCal(pla->GetQAveCal());
    easyneut->SetTAveCal(pla->GetTAveCal());
    easyneut->SetTAveSlw(pla->GetTAveSlw());
    easyneut->SetDTCal(pla->GetDTCal());
    easyneut->SetDTSlw(pla->GetDTSlw());
    easyneut->SetPos(pla->GetPos(0),pla->GetPos(1),pla->GetPos(2));

    TVector3 neutronpos(pla->GetPos(0),pla->GetPos(1),pla->GetPos(2));
    Double_t tof = pla->GetTAveSlw()-tgttime;//ns
    TVector3 flvec=neutronpos-targetpos;
    Double_t fl = flvec.Mag();//mm
    Double_t v = fl/tof;//mm/ns
    Double_t beta = v/fclight;
    Double_t gamma = 1./sqrt(1-beta*beta);
    easyneut->SetTOF(tof);
    easyneut->SetBeta(beta);
    easyneut->SetGamma(gamma);
    easyneut->SetEnergy(fMass*gamma);
    easyneut->SetMomentum(fMass*beta*gamma);
    TLorentzVector momentum4d;
    TVector3 pvect_u = flvec*(1./fl);
    pvect_u = pvect_u*fMass*beta*gamma;
    momentum4d.SetVectM(pvect_u,fMass);
    easyneut->SetMomentum4D(momentum4d);
    if(!TMath::IsNaN(tof)){
      easyneuts.push_back(easyneut);
    }
  }
  if((int)easyneuts.size()>1){
    std::sort(easyneuts.begin(),easyneuts.end(),CompareForSort);
  }
}

