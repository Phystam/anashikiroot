#ifndef _TARTEASYENERGYLOSSFUNC_HH_
#define _TARTEASYENERGYLOSSFUNC_HH_

#include <map>
#include <utility> //to use std::pair
#include <TObject.h>
#include <TString.h>
#include <vector>

class TObject;
class TString;
typedef std::map<std::pair< std::pair<Int_t,Int_t>,TString >,std::vector<Double_t> > Eloss_t;
class TArtEasyEnergyLossFunc : public TObject {
public:
  TArtEasyEnergyLossFunc();
  virtual ~TArtEasyEnergyLossFunc(){}
  std::vector<Double_t> GetBeamEnergyLossPara(Int_t A, Int_t Z,TString target){
    if(target != "C" && target != "Pb" && target !="Emp"){
      return fdefault;
    }
    if( (Z==9 && (A==27||A==29||A==31))
	||(Z==10 && (A>28&&A<33))
	||(Z==11 && (A>29&&A<35))
	){ 
      return fBeamElossPara[std::make_pair(std::make_pair(A,Z),target)];
    }else{
      return fdefault;
    }
  }
  std::vector<Double_t> GetFragEnergyLossPara(Int_t A, Int_t Z,TString target){
    if(target != "C" && target != "Pb" && target !="Emp"){
      return fdefaultfrag;
    }
    if( (Z==9 && (A==27||A==29||A==31))
	||(Z==10 && (A>28&&A<33))
	||(Z==11 && (A>29&&A<35))
	){ 
      return fFragElossPara[std::make_pair(std::make_pair(A,Z),target)];
    }else{
      return fdefaultfrag;
    }
  }
private:
  Eloss_t fBeamElossPara;
  Eloss_t fFragElossPara;
  std::vector<Double_t> fdefault;
  std::vector<Double_t> fdefaultfrag;
  ClassDef(TArtEasyEnergyLossFunc,1);
  
};
#endif
