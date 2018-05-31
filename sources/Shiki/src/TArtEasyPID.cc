#include <Riostream.h>

#include "TArtEasyPID.hh"
#include "TArtEasyTarget.hh"
#include "TArtCalibPlastic.hh"
#include "TArtPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtIC.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtShikiParameters.hh"
#include "TArtEasyTarget.hh"
#include <TROOT.h>
#include <TMath.h>
#include "TArtEasyMassExcess.hh"
#include "TArtEasyEnergyLossFunc.hh"
#include "TArtMDF_TOFSBTTgt_C.hh"
#include "TArtMDF_TOFSBTTgt_Pb.hh"
#include "TArtMDF_TOFSBTTgt_Emp.hh"
#include "TArtMDF_Beam_Eloss_C.hh"
#include "TArtMDF_Beam_Eloss_Pb.hh"
#include "TArtMDF_Beam_Eloss_Emp.hh"
#include "TArtMDF_Eloss_Eout_C.hh"
#include "TArtMDF_Eloss_Eout_Pb.hh"
#include "TArtMDF_Eloss_Eout_Emp.hh"
//________________________________________________________
// TArtEasyPID::TArtEasyPID(){
//   TArtCore::Info(__FILE__,"Creating BigRIPS detector objects...");
//   fcalibpla = new TArtCalibPlastic();
//   fcalibic = new TArtCalibIC();
// }
TArtEasyPID* feasypid = 0;
TArtEasyPID::TArtEasyPID(Double_t Brho0){
  TArtCore::Info(__FILE__,"Creating BigRIPS detector objects...");
  fcalibpla = new TArtCalibPlastic();
  fcalibic = new TArtCalibIC();
  SetBrho0(Brho0);
  sman = TArtStoreManager::Instance();
  fshikipara=(const TArtShikiParameters*)sman->FindParameters("ShikiParameters");
  feasytarget=fshikipara->FindEasyTarget(1);
  feasyex=new TArtEasyMassExcess();
  feasyeloss=new TArtEasyEnergyLossFunc();
  
}

TArtEasyPID* TArtEasyPID::Instance(Double_t Brho0){
  if(!feasypid){
    feasypid=new TArtEasyPID(Brho0);
  }
  return feasypid;
}

TArtEasyPID::~TArtEasyPID(){
  ClearData();
}

//________________________________________________________
void TArtEasyPID::ClearData(){
  fcalibpla->ClearData();
  fcalibic->ClearData();
  for(int i=0;i<7;i++){
    fpla[i]=NULL;
  }
  ficb=NULL;
  fBrho  =-9999;
  fTOF713=-9999;
  fTOF37 =-9999;
  ff13Time=-9999;
  fDeltaE=-9999;
  fBeta  =-9999;
  fGamma =-9999;
  frejected_f3=false;
  frejected_f5=false;
  frejected_f7=false;
  frejected_tof=false;
  freconstructed=false;
}

void TArtEasyPID::ReconstructData(){
  if(freconstructed) return;
  fcalibpla->ReconstructData();
  for(Int_t i=0;i<fcalibpla->GetNumPlastic();i++){
    SetPlastic(i,(TArtPlastic*)fcalibpla->GetPlastic(i));
  }
  Double_t f3time = fpla[0]->GetTime();
  ff13Time = (fpla[3]->GetTime()+fpla[4]->GetTime())/2.;
  
  Double_t f7time = fpla[2]->GetTime();
  Double_t TOF713=ff13Time-f7time;
  Double_t TOF37=f7time-f3time;
  Double_t f5x=fpla[1]->GetXposition();
  //f3pla time rejection
  // {
  //   // Double_t sigma=2.645e-4;
  //   // Double_t mean =1.00628; // for 31Ne+C
  //   // Double_t sigma=3.03145e-4;
  //   // Double_t mean =1.00641;
  //   Double_t tlraw = fpla[0]->GetTLRaw();
  //   Double_t trraw = fpla[0]->GetTRRaw();
  //   // Double_t troffset = -312.7;
  //   // Double_t ratio = (trraw-troffset)/tlraw;
  //   //     if(ratio<mean-4.*sigma || ratio>mean+4.*sigma){
  //   //   frejected_f3=true;
  //   // }
  //   if(abs(tlraw-trraw)>100){//||tlraw<40000||tlraw>50000){
  //     frejected_f3=true;
  //   }
  // }
  //f5pla time rejection
  {
    if(f5x<-120||f5x>120){
      frejected_f5=true;
    }
    if(fpla[1]->GetTLRaw()<40500||fpla[1]->GetTRRaw()<40500){
      frejected_f5=true;
    }
  }
  //f7pla time rejection
  {
    Double_t tlraw = fpla[2]->GetTLRaw();
    Double_t trraw = fpla[2]->GetTRRaw();
    if(trraw-tlraw>-100||trraw-tlraw<-250){
      frejected_f7=true;
    }
  }

  //f7-f13 TOF rejection
  {
    // Double_t sigma=6.48837e-4;
    // Double_t mean =7.74421e-1; //for 31Ne+C
    

    if(TOF37-TOF713>60){
      frejected_tof=true;
    }
  }

  //  Double_t length = 35.608;//m, F7pla->F13pla1
  Double_t length = 35953;//mm, F7pla->F13pla1
  Double_t Beta = length/(TOF713*fclight)*1e6;
  Double_t Gamma = 1./sqrt(1.-Beta*Beta);

  fcalibic->ReconstructData();
  Double_t DeltaE=0;
  Double_t Z=0;
  Int_t nic = fcalibic->GetNumIC();
  for (int iic=0;iic<nic;++iic){
    TArtIC *ic = fcalibic->GetIC(iic);
    if (*(ic->GetDetectorName())=="ICB") {
      SetICB(ic);
      DeltaE=ic->GetEnergyAvSum();
      Z=ic->GetZet(Beta);
    }
  }

  //  Double_t brho = fBrho0*(1+f5x/3300.);// D=3300mm
  //  Double_t brho = fBrho0*(1+f5x/3079.12);// D=3300mm
  //  Double_t brho0_f713 = fBrho0-0.04642;
  Double_t brho = fBrho0*(1+f5x/3300.);// D=3300mm
  brho = 0.822563+0.572445*brho+0.0648057*pow(brho,2.)-0.00313776*pow(brho,3.);//fitting result
  


  Double_t AoZ = brho/(Beta*Gamma);
  AoZ=AoZ*fe/(fclight*famu);


  SetBrho(brho);
  SetTOF713(TOF713);
  SetTOF37(TOF37);
  SetDeltaE(DeltaE);
  SetBeta(Beta);
  SetGamma(Gamma);
  SetZet(Z);
  SetAoZ(AoZ);
}

Double_t TArtEasyPID::GetMass(){ 
  //mass calc
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  return M;
}

//_____________________________________________________________________________

//Energy / Momentum calculation from TOF only
Double_t TArtEasyPID::GetTotalEnergyFromTOF(){ 
  //total energy calc
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  return M*fGamma;
}

Double_t TArtEasyPID::GetTotalKineticEnergyFromTOF(){ 
  //total energy calc
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  return M*(fGamma-1.);
}

Double_t TArtEasyPID::GetKineticEnergyFromTOF(){
  //MeV/u
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  Double_t TKE=GetTotalKineticEnergyFromTOF();
  return TKE/u;
}

Double_t TArtEasyPID::E2Tgt(Double_t E){
  Double_t Etgt=0;
  Double_t input[]={GetAInt(),GetZetInt(),E};
  // Double_t* prm=feasytarget->GetBeamE();
  // Etgt=prm[0]+prm[1]*E+prm[2]*E*E+prm[3]*E*E*E;
  if(*feasytarget->GetDetectorName()=="C"){
    Etgt=TArtMDF_Beam_Eloss_C::MDF(input);
    //Etgt=TArtMDF_Eloss_Eout_C::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Pb"){
    Etgt=TArtMDF_Beam_Eloss_Pb::MDF(input);
    //Etgt=TArtMDF_Eloss_Eout_Pb::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Emp"){
    Etgt=TArtMDF_Beam_Eloss_Emp::MDF(input);
    //Etgt=TArtMDF_Eloss_Eout_Emp::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Al"){
    Etgt=285.814;//MeV
  }
  return Etgt;

}
//jump
Double_t TArtEasyPID::GetKineticEnergyAtTgtFromTOF(){
  //kinetic energy calc at target from simu (31Ne)

  Double_t KE=GetKineticEnergyFromTOF();
  return E2Tgt(KE);
}

Double_t TArtEasyPID::GetTotalKineticEnergyAtTgtFromTOF(){
  Double_t KE=GetKineticEnergyAtTgtFromTOF();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  return KE*u;
}

Double_t TArtEasyPID::GetTotalEnergyAtTgtFromTOF(){
  Double_t TKE=GetTotalKineticEnergyAtTgtFromTOF();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  return TKE+M;
}

Double_t TArtEasyPID::GetTotalMomentumFromTOF(){ //overload
  //total energy calc
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  return M*fBeta*fGamma;
}

Double_t TArtEasyPID::GetMomentumFromTOF(){ //overload
  //total energy calc
  Double_t P = GetTotalMomentumFromTOF();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  return P/u;
}
Double_t TArtEasyPID::GetMomentumAtTgtFromTOF(){ //overload
  Double_t TKE=GetTotalKineticEnergyAtTgtFromTOF();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  Double_t p = sqrt(TKE*(TKE+2.*M));
  return p/u;
}
Double_t TArtEasyPID::GetTotalMomentumAtTgtFromTOF(){ //overload
  Double_t TKE=GetTotalKineticEnergyAtTgtFromTOF();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t p = sqrt(TKE*(TKE+2.*M));
  return p;
}

//____________________________________________________________________________
//Energy/Momentum calculation from Brho
Double_t TArtEasyPID::GetTotalMomentumFromBrho(){
  //total momentum calc
  Double_t ptot = GetZetInt()*fBrho*fclight*1e-6;
  return ptot;
}
Double_t TArtEasyPID::GetMomentumFromBrho(){ 
  //total momentum calc
  Double_t ptot = GetZetInt()*fBrho*fclight*1e-6;
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  return ptot/u;
}

Double_t TArtEasyPID::GetTotalEnergyFromBrho(){
  //total energy calc
  Double_t ptot = GetTotalMomentumFromBrho();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t Etot = sqrt(ptot*ptot + M*M);
  return Etot;
}

Double_t TArtEasyPID::GetTotalKineticEnergyFromBrho(){ 
  //total energy calc
  Double_t TE=GetTotalEnergyFromBrho();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  return TE-M;
}

Double_t TArtEasyPID::GetKineticEnergyFromBrho(){
  //MeV/u
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  Double_t TKE=GetTotalKineticEnergyFromBrho();
  return TKE/u;
}
Double_t TArtEasyPID::GetKineticEnergyAtTgtFromBrho(){
  //kinetic energy calc at target from simu (31Ne)
  Double_t KE=GetKineticEnergyFromBrho();
  return E2Tgt(KE);
}

Double_t TArtEasyPID::GetTotalKineticEnergyAtTgtFromBrho(){
  Double_t KE=GetKineticEnergyAtTgtFromBrho();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  return KE*u;
}

Double_t TArtEasyPID::GetTotalEnergyAtTgtFromBrho(){
  Double_t TKE=GetTotalKineticEnergyAtTgtFromBrho();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  return TKE+M;
}
Double_t TArtEasyPID::GetMomentumAtTgtFromBrho(){ //overload
  Double_t TKE=GetTotalKineticEnergyAtTgtFromBrho();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t u = M/famu_MeV;
  Double_t p = sqrt(TKE*(TKE+2.*M));
  return p/u;
}
Double_t TArtEasyPID::GetTotalMomentumAtTgtFromBrho(){ //overload
  Double_t TKE=GetTotalKineticEnergyAtTgtFromBrho();
  Double_t M = GetAInt()*famu_MeV+feasyex->GetMassExcess(GetAInt(),GetZetInt());
  Double_t p = sqrt(TKE*(TKE+2.*M));
  return p;
}
//________________________________________________________________________
//TOF calculation

Double_t TArtEasyPID::GetTOFSBTTgt(){
  Double_t tofsbttgt=0;
  Double_t input[]={GetAInt(),GetZetInt(),GetTOF713()};
  if(*feasytarget->GetDetectorName()=="C"){
    tofsbttgt=TArtMDF_TOFSBTTgt_C::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Pb"){
    tofsbttgt=TArtMDF_TOFSBTTgt_Pb::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Emp"){
    tofsbttgt=TArtMDF_TOFSBTTgt_Emp::MDF(input);
  }
  else if(*feasytarget->GetDetectorName()=="Al"){
    tofsbttgt=13.000;//ns, fixed
  }
  return tofsbttgt;
}

void TArtEasyPID::SelectTarget(TString name){
  for(Int_t i=1;i<=fshikipara->GetNumTarget();i++){
    if(name== *(fshikipara->FindEasyTarget(i)->GetDetectorName())){
      feasytarget=fshikipara->FindEasyTarget(i);
    }
  }
  TArtCore::Info(__FILE__,Form("selected target is %s, position=%fmm",feasytarget->GetDetectorName()->Data(),feasytarget->GetPosZ()));
}

void TArtEasyPID::SelectTarget(Int_t ID){
    feasytarget=fshikipara->FindEasyTarget(ID);
  TArtCore::Info(__FILE__,Form("selected target is %s, position=%fmm",feasytarget->GetDetectorName()->Data(),feasytarget->GetPosZ()));
}
