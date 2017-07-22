#ifndef _TARTEASYMASSEXCESS_HH_
#define _TARTEASYMASSEXCESS_HH_

#include <map>
#include <utility> //to use std::pair
#include <TObject.h>

class TObject;
class TArtEasyMassExcess : public TObject {
public:
  TArtEasyMassExcess();
  virtual ~TArtEasyMassExcess(){}
  Double_t GetMassExcess(Int_t A, Int_t Z){
    if(A>25&&A<34&&Z>9&&Z<12){
      return fMassExcess[std::make_pair(A,Z)];
    }else{
      return 0;
    }
  }
private:
  std::map<std::pair<Int_t,Int_t>,Double_t> fMassExcess;
  ClassDef(TArtEasyMassExcess,1);
  
};
#endif
