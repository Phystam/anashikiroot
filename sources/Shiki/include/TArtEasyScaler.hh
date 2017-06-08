#ifndef _TARTEASYSCALER_H_
#define _TARTEASYSCALER_H_
#include <TObject.h>

class TString;
class TArtEasyScaler : public TObject{
public:
  TArtEasyScaler(Int_t g,Int_t ug,Int_t bnh,Int_t bneuh,Int_t bch, Int_t ck,Int_t cm);
  TArtEasyScaler(TString scrfile);
  TArtEasyScaler(Int_t runnum);
  TArtEasyScaler();
  virtual ~TArtEasyScaler(){}
  void Init();
  void SetEasyScaler(TString scrfile);
  Int_t GetGated(){return fgated;}
  Int_t GetUngated(){return fungated;}
  Int_t GetBxNEBxH(){return fBxNEBxH;}
  Int_t GetBxNeuxH(){return fBxNeuxH;}
  Int_t GetBxCATxH(){return fBxCATxH;}
  Int_t GetClock1kHz(){return fclock1k;}
  Int_t GetClock1MHz(){return fclock1M;}
  Double_t GetLiveTime(){return (Double_t)fgated/(Double_t)fungated;}
  TArtEasyScaler operator+(TArtEasyScaler &scr2);
  TArtEasyScaler* operator+(TArtEasyScaler* scr2);
private:
  Int_t fgated;
  Int_t fungated;
  Int_t fBxNEBxH;
  Int_t fBxNeuxH;
  Int_t fBxCATxH;
  Int_t fclock1k;
  Int_t fclock1M;
  ClassDef(TArtEasyScaler,1)
};
#endif
