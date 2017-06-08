#ifndef _TARTDCHITPARA_H_
#define _TARTDCHITPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtDCHit.hh"

class TArtDCHitPara : public TNamed {

 public:
  TArtDCHitPara(){}
  TArtDCHitPara(Int_t i, TString n, Int_t f, 
		Int_t l, Int_t idp, 
		Int_t wid, Double_t wp, Double_t wz,
		Double_t tz,
		Double_t c0, Double_t c1, Double_t c2) : 
    id(i), detname(n), fpl(f), layer(l), id_plane(idp), wireid(wid), wirepos(wp), wirez(wz), tzero_offset(tz),coef0(c0),coef1(c1),coef2(c2){ }
  ~TArtDCHitPara() { }

  void SetPalameters(Int_t i, TString n, Int_t f, 
		     Int_t l, Int_t idp,
		     Int_t wid, Double_t wp, Double_t wz,
		     Double_t tz,
		     Double_t c0, Double_t c1, Double_t c2){
    id=i; detname=n; fpl=f; layer=l; id_plane=idp; wireid=wid; wirepos=wp; wirez=wz; tzero_offset=tz; coef0=c0; coef1=c1; coef2=c2;}

  void SetMap(Int_t dt, Int_t tg, Int_t tc) {
    map.SetMap(fpl,dt,tg,tc); 
    //cout << "TDC is mapped to " << map.GetMapInfo() << " " << map.GetName() << endl;
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Int_t GetLayer() const { return layer; }
  Int_t GetPlaneID() const { return id_plane; }
  Int_t GetWireID() const { return wireid; }
  Double_t GetWirePosition() const { return wirepos; }
  Double_t GetWireZPosition() const { return wirez; }
  Double_t GetTZeroOffset() const { return tzero_offset; } 
  Double_t GetSTCCoef(Int_t i) const { 
    if(i==0)return coef0;//i=0,1,2
    if(i==1)return coef1;//i=0,1,2
    if(i==2)return coef2;//i=0,1,2
    return -9999;
  }

  TArtRIDFMap * GetTDCMap() { return &map;  }

  friend std::ostream & operator << (std::ostream& out, const TArtDCHitPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Layer: " << p.layer << ", ";
    out << "Plane-ID: " << p.id_plane << ", ";
    out << "Wire ID: " << p.wireid << ", ";
    out << "Wire Position: " << p.wirepos << ", ";
    out << "Wire Z-Pos: " << p.wirez << ", ";
    out << "TDC Zero-Offset: " << p.tzero_offset << ", ";
    out << "STC correction Coef0: " << p.coef0 << ", ";
    out << "STC correction Coef1: " << p.coef1 << ", ";
    out << "STC correction Coef2: " << p.coef2 << ", ";
    out << "Map of TDC: "  << p.map;
    return out;
  }

private:

  Int_t     id;
  TString   detname;
  Int_t     fpl;

  Int_t layer;
  Int_t id_plane;
  Int_t wireid; // wire id
  Double_t wirepos; // wire position in a layer
  Double_t wirez; // wire z position

  // calibration value for TDC // not used so far
  Double_t  tzero_offset;

  Double_t coef0;
  Double_t coef1;
  Double_t coef2;
  TArtRIDFMap map;

  ClassDef(TArtDCHitPara,1);

};

#endif
