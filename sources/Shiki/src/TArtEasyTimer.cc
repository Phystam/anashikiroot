#include "TArtEasyTimer.hh"
#include <TDatime.h>

TArtEasyTimer::TArtEasyTimer(){
  fstarttime = new TDatime();
}

TArtEasyTimer::~TArtEasyTimer(){
  delete fstarttime;
  //  delete fnowtime;
}
// Int_t TArtEasyTimer::GetSec(){
//   fnowtime->Set();
//   Int_t tdiff = fnowtime->Convert() - fstarttime->Convert();
//   return tdiff%60;
// }
// Int_t TArtEasyTimer::GetMin(){
//   fnowtime->Set();
//   Int_t tdiff = fnowtime->Convert() - fstarttime->Convert();
//   return (tdiff/60)%60;
// }
// Int_t TArtEasyTimer::GetHr(){
//   fnowtime->Set();
//   Int_t tdiff = fnowtime->Convert() - fstarttime->Convert();
//   return (tdiff/3600)%24;
// }

// std::ostream & operator << (std::ostream& out, const TArtEasyTimer& timer){
//   TDatime* fnowtime = new TDatime();
//   Int_t ftdiff = fnowtime->Convert() - timer.fstarttime->Convert();
//   out << Form("%02d",ftdiff/3600) 
//       << ":" << Form("%02d",ftdiff/60) 
//       << ":" <<Form("%02d",ftdiff%60);
//   delete fnowtime;
//   return out;
// }
