#ifndef _TARTTOFPARA_H_
#define _TARTTOFPARA_H_

#include <TNamed.h>

class TArtTOFPara : public TNamed {
  
 public:
  TArtTOFPara(){}
  TArtTOFPara(const char* name,  const char *upla, const char *dpla, Double_t toffset=0, Int_t mfpl=-1, Int_t ufpl=-1, Int_t dfpl=-1) :
    offset(toffset) {
    SetName(name);
    upstream_plname = upla;
    downstream_plname = dpla;
    middle_fpl = mfpl;
    upstream_fpl = ufpl;
    downstream_fpl = dfpl;

    char dname[64];
    sprintf(dname,"TOF%sto%s",upla,dpla);
    detname = dname;
  }
  TArtTOFPara(char *upla, char *dpla, Double_t toffset=0, Int_t mfpl=-1, Int_t ufpl=-1, Int_t dfpl=-1) :
    offset(toffset) {
    upstream_plname = upla;
    downstream_plname = dpla;
    middle_fpl = mfpl;
    upstream_fpl = ufpl;
    downstream_fpl = dfpl;

    char dname[64];
    sprintf(dname,"TOF%sto%s",upla,dpla);
    detname = dname;
  }
  ~TArtTOFPara(){}

  void SetUpstreamPlName   (char *v) { upstream_plname = v; }
  void SetDownstreamPlName (char *v) { downstream_plname = v; }
  void SetOffset        (Double_t v) { offset = v; }
  void SetMiddleFpl        (Int_t v) { middle_fpl = v; }
  void SetUpstreamFpl        (Int_t v) { upstream_fpl = v; }
  void SetDownstreamFpl        (Int_t v) { downstream_fpl = v; }

  TString * GetDetectorName      () { return &detname; }
  TString * GetUpstreamPlName() { return &upstream_plname; }
  TString * GetDownstreamPlName() { return &downstream_plname; }
  Double_t GetOffset() const { return offset; }
  Int_t GetMiddleFpl() const { return middle_fpl; }
  Int_t GetUpstreamFpl() const { return upstream_fpl; }
  Int_t GetDownstreamFpl() const { return downstream_fpl; }

 private:
  TString   detname;
  TString   upstream_plname;
  TString   downstream_plname;
  Double_t  offset;
  Int_t     middle_fpl;
  Int_t     upstream_fpl;
  Int_t     downstream_fpl;

  ClassDef(TArtTOFPara,1);

};

#endif
