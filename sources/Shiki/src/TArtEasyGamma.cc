#include "TArtEasyGamma.hh"
#include <Riostream.h>
#include "TArtCalibCATANA.hh"
#include "TArtCATANACsI.hh"
#include "TArtCATANAParameters.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
//________________________________________________________
TArtEasyGamma::TArtEasyGamma(){
  TArtCore::Info(__FILE__,"Creating CATANA detector objects...");
  fcalibcatana = new TArtCalibCATANA();
}

TArtEasyGamma::~TArtEasyGamma(){
  ClearData();
}

//________________________________________________________
void TArtEasyGamma::ClearData(){
  fcalibcatana->ClearData();
}

void TArtEasyGamma::ReconstructData(){
  fcalibcatana->ReconstructData();
}

//_____________________________________________________________________________
