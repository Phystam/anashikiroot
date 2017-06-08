#ifndef TARTEASYTIMER_HH
#define TARTEASYTIMER_HH

#include <iostream>
#include "TString.h"
#include "TDatime.h"
#include <TObject.h>

class TArtEasyTimer: public TObject{
public:

  TArtEasyTimer();
  ~TArtEasyTimer();
  // static Int_t GetSec();
  // static Int_t GetMin();
  // static Int_t GetHr();

friend std::ostream & operator << (std::ostream& out, const TArtEasyTimer& timer){
  TDatime* fnowtime=new TDatime();
  Int_t time=fnowtime->Convert()-timer.fstarttime->Convert();
  out << Form("%02d",time/3600)
      << ":" << Form("%02d",time/60%60)
      << ":" <<Form("%02d",time%60);
  delete fnowtime;
  return out;
}

private:
  TDatime* fstarttime;

  ClassDef(TArtEasyTimer,1)
};

#endif
