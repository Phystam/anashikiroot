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
    if(A>25&&A<34&&Z>8&&Z<12){
      return fBeamElossPara[std::make_pair(std::make_pair(A,Z),target)];
    }else{
      return fdefault;
    }
  }
  std::vector<Double_t> GetFragEnergyLossPara(Int_t A, Int_t Z,TString target){
    if(A>25&&A<34&&Z>8&&Z<12){
      return fFragElossPara[std::make_pair(std::make_pair(A,Z),target)];
    }else{
      return fdefault;
    }
  }
private:
  Eloss_t fBeamElossPara;
  Eloss_t fFragElossPara;
  std::vector<Double_t> fdefault;
  ClassDef(TArtEasyEnergyLossFunc,1);
  
};
#endif
