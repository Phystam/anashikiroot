#ifndef _TARTEASYSCALER_H_
#define _TARTEASYSCALER_H_
#include <TObject.h>

class TString;
class TObject;
class TArtEasyScaler : public TObject{
public:
  TArtEasyScaler(ULong64_t g,ULong64_t ug,ULong64_t bnh,ULong64_t bneuh,ULong64_t bch, ULong64_t ck,ULong64_t cm);
  TArtEasyScaler(TString scrfile);
  TArtEasyScaler(ULong64_t runnum);
  TArtEasyScaler();
  virtual ~TArtEasyScaler(){}
  void Init();
  void SetEasyScaler(TString scrfile);
  void SetEasyScaler(ULong64_t runnum);
  ULong64_t GetGated(){return fgated;}
  ULong64_t GetUngated(){return fungated;}
  ULong64_t GetBxNEBxH(){return fBxNEBxH;}
  ULong64_t GetBxNeuxH(){return fBxNeuxH;}
  ULong64_t GetBxCATxH(){return fBxCATxH;}
  ULong64_t GetClock1kHz(){return fclock1k;}
  ULong64_t GetClock1MHz(){return fclock1M;}
  Double_t GetLiveTime(){return (Double_t)fgated/(Double_t)fungated;}
  TArtEasyScaler operator+(TArtEasyScaler &scr2);
  TArtEasyScaler* operator+(TArtEasyScaler* scr2);
private:
  ULong64_t fgated;
  ULong64_t fungated;
  ULong64_t fBxNEBxH;
  ULong64_t fBxNeuxH;
  ULong64_t fBxCATxH;
  ULong64_t fclock1k;
  ULong64_t fclock1M;
  ClassDef(TArtEasyScaler,1)
};
#endif
