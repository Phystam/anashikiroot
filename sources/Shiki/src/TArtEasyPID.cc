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
  {
    // Double_t sigma=2.645e-4;
    // Double_t mean =1.00628; // for 31Ne+C
    Double_t sigma=3.03145e-4;
    Double_t mean =1.00641;
    Double_t tlraw = fpla[0]->GetTLRaw();
    Double_t trraw = fpla[0]->GetTRRaw();
    Double_t troffset = -312.7;
    Double_t ratio = (trraw-troffset)/tlraw;
    if(ratio<mean-4.*sigma || ratio>mean+4.*sigma){
      frejected_f3=true;
    }
  }
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
    Double_t sigma=2.92014e-4;
    Double_t mean =0.991798;
    Double_t tlraw = fpla[2]->GetTLRaw();
    Double_t trraw = fpla[2]->GetTRRaw();
    Double_t troffset = 165.;
    Double_t ratio = (trraw-troffset)/tlraw;
    if(ratio<mean-6.*sigma || ratio>mean+6.*sigma){
      frejected_f7=true;
    }
  }

  //f7-f13 TOF rejection
  {
    // Double_t sigma=6.48837e-4;
    // Double_t mean =7.74421e-1; //for 31Ne+C
    Double_t sigma = 6.79681e-4;
    Double_t mean = 7.77812e-1;
    if(TOF713/TOF37<mean-6.*sigma || TOF713/TOF37>mean+6.*sigma){
      frejected_tof=true;
    }
  }

  //  Double_t brho = fBrho0*(1+f5x/3300.);// D=3300mm
  //  Double_t brho = fBrho0*(1+f5x/3079.12);// D=3300mm
  Double_t brho0_f713 = fBrho0-0.04642;
  Double_t brho = brho0_f713*(1+f5x/3095.08);// D=3300mm

  //  Double_t length = 35.608;//m, F7pla->F13pla1
  Double_t length = 35953;//mm, F7pla->F13pla1
  Double_t Beta = length/(TOF713*fclight)*1e6;
  Double_t Gamma = 1./sqrt(1.-Beta*Beta);

  Double_t AoZ = brho/(Beta*Gamma);
  AoZ=AoZ*fe/(fclight*famu);

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


  SetBrho(brho);
  SetTOF713(TOF713);
  SetTOF37(TOF37);
  SetDeltaE(DeltaE);
  SetBeta(Beta);
  SetGamma(Gamma);
  SetZet(Z);
  SetAoZ(AoZ);
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
  //  Double_t para[] = {-8.87313,1.02961,-3.97733e-5};//empty target
  //Double_t para[] = {-45.9326,1.0811,-5.78495e-5};//C target
  Double_t* para=feasytarget->GetBeamE();  
  E=para[0]+para[1]*E+para[2]*E*E+para[3]*E*E*E;
  return E;
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
  //kinetic energy Mev/u -> TOF(SBT1 - target front) (31Ne)
  Double_t KE = GetKineticEnergy();
  //  Double_t para[] = {11.2764,1306.94,-2.65721e-3,1.22599e-6};//no change ?
  //  Double_t para[] = {24.1673,-0.0666612,0.000130404,-9.76461e-08}; //for gamma run
  Double_t* para=feasytarget->GetTOF();
  //  return (para[0]+para[1]/KE+para[2]*KE+para[3]*KE*KE);
  return (para[0]+para[1]*KE+para[2]*KE*KE+para[3]*KE*KE*KE);
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
