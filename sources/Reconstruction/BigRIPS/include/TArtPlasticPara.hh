#ifndef _TARTPLASTICPARA_H_
#define _TARTPLASTICPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtPlastic.hh"

class TArtPlasticPara : public TNamed {

 public:
  TArtPlasticPara(){}
  TArtPlasticPara(Int_t i, TString n, Int_t f, 
		  Double_t tcl, Double_t tcr, 
		  Double_t qpedl, Double_t qpedr, 
		  Double_t ts_al, Double_t ts_bl, Double_t ts_cl,
		  Double_t ts_ar, Double_t ts_br, Double_t ts_cr,
		  Double_t ts_aav, Double_t ts_bav, Double_t ts_cav,
		  Double_t tdc_uf, Double_t tdc_of, 
		  Double_t z, Double_t toff,
		  Double_t xc0, Double_t xc1, Double_t xc2) :
    id(i), detname(n), fpl(f), tcal_left(tcl), tcal_right(tcr), qped_left(qpedl), qped_right(qpedr), tslew_left_a(ts_al), tslew_left_b(ts_bl), tslew_left_c(ts_cl), tslew_right_a(ts_ar), tslew_right_b(ts_br), tslew_right_c(ts_cr),tslew_ave_a(ts_aav), tslew_ave_b(ts_bav), tslew_ave_c(ts_cav), tdc_underflow(tdc_uf), tdc_overflow(tdc_of), zoffset(z), toffset(toff), xcal0(xc0), xcal1(xc1), xcal2(xc2){ }
  ~TArtPlasticPara() { }

  void SetParameters(Int_t i, TString n, Int_t f, 
		     Double_t tcl, Double_t tcr, 
		     Double_t qpedl, Double_t qpedr, 
		     Double_t ts_al, Double_t ts_bl, Double_t ts_cl,
		     Double_t ts_ar, Double_t ts_br, Double_t ts_cr,
		     Double_t ts_aav, Double_t ts_bav, Double_t ts_cav,
		     Double_t tdc_uf, 
		     Double_t tdc_of, 
		     Double_t z,Double_t toff,
		     Double_t xc0, Double_t xc1, Double_t xc2) {
    id=i; detname=n; fpl=f; 
    tcal_left=tcl; tcal_right=tcr; qped_left=qpedl; qped_right=qpedr; 
    tslew_left_a=ts_al; tslew_left_b=ts_bl; tslew_left_c=ts_cl;
    tslew_right_a=ts_ar; tslew_right_b=ts_br; tslew_right_c=ts_cr;
    tslew_right_a=ts_aav; tslew_right_b=ts_bav; tslew_right_c=ts_cav;
    tdc_underflow=tdc_uf; tdc_overflow=tdc_of; zoffset=z; toffset=toff; 
    xcal0=xc1;xcal1=xc1;xcal2=xc2;}

  void SetMap(Int_t tf, Int_t td, 
	      Int_t tlg, Int_t tlc, Int_t trg, Int_t trc, 
	      Int_t qf, Int_t qd, 
	      Int_t qlg, Int_t qlc, Int_t qrg, Int_t qrc) {
    map_tleft.SetMap (tf,td,tlg,tlc); 
    map_tright.SetMap(tf,td,trg,trc); 
    map_qleft.SetMap (qf,qd,qlg,qlc); 
    map_qright.SetMap(qf,qd,qrg,qrc); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Double_t GetTCalLeft() const { return tcal_left; } 
  Double_t GetTCalRight() const { return tcal_right; } 
  Double_t GetQPedLeft() const { return qped_left; } 
  Double_t GetQPedRight() const { return qped_right; } 
  Double_t GetTLSlewA() const { return tslew_left_a; } 
  Double_t GetTLSlewB() const { return tslew_left_b; } 
  Double_t GetTLSlewC() const { return tslew_left_c; } 
  Double_t GetTRSlewA() const { return tslew_right_a; } 
  Double_t GetTRSlewB() const { return tslew_right_b; } 
  Double_t GetTRSlewC() const { return tslew_right_c; } 
  Double_t GetTAveSlewA() const { return tslew_ave_a; } 
  Double_t GetTAveSlewB() const { return tslew_ave_b; } 
  Double_t GetTAveSlewC() const { return tslew_ave_c; } 
  Double_t GetTDCUnderflow() const {return tdc_underflow;}
  Double_t GetTDCOverflow() const {return tdc_overflow;}
  Double_t GetZoffset() const { return zoffset; } 
  Double_t GetToffset() const { return toffset; }
  Double_t GetXCal0() const { return xcal0; }
  Double_t GetXCal1() const { return xcal1; }
  Double_t GetXCal2() const { return xcal2; }
  TArtRIDFMap * GetTLMap() { return &map_tleft;  }
  TArtRIDFMap * GetTRMap() { return &map_tright; }
  TArtRIDFMap * GetQLMap() { return &map_qleft;  }
  TArtRIDFMap * GetQRMap() { return &map_qright; }

  friend std::ostream & operator << (std::ostream& out, const TArtPlasticPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Time Calib Left: " << p.tcal_left << ", ";
    out << "Time Calib Right: " << p.tcal_right << ", ";
    out << "QDC Pedestal Left: " << p.qped_left << ", ";
    out << "QDC Pedestal Right: " << p.qped_right << ", ";
    out << "Time Slewing Corr. A of Left: " << p.tslew_left_a << ", ";
    out << "Time Slewing Corr. B of Left: " << p.tslew_left_b << ", ";
    out << "Time Slewing Corr. C of Left: " << p.tslew_left_c << ", ";
    out << "Time Slewing Corr. A of Right: " << p.tslew_right_a << ", ";
    out << "Time Slewing Corr. B of Right: " << p.tslew_right_b << ", ";
    out << "Time Slewing Corr. C of Right: " << p.tslew_right_c << ", ";
    out << "Time Slewing Corr. A of Ave: " << p.tslew_ave_a << ", ";
    out << "Time Slewing Corr. B of Ave: " << p.tslew_ave_b << ", ";
    out << "Time Slewing Corr. C of Ave: " << p.tslew_ave_c << ", ";
    out << "Time offset: " << p.toffset << std::endl;
    out << "Time Calib Right: " << p.tcal_right << ", ";
    out << "TDC Underflow: " << p.tdc_underflow << std::endl;
    out << "TDC Overflow: " << p.tdc_overflow << std::endl;
    out << "Z-position offset: " << p.zoffset << std::endl;
    out << "X-position parameter 0: " << p.xcal0 << std::endl;
    out << "X-position parameter 1: " << p.xcal1 << std::endl;
    out << "X-position parameter 2: " << p.xcal2 << std::endl;
    out << "Map of T-Left: "  << p.map_tleft;
    out << "Map of T-Right: " << p.map_tright;
    out << "Map of Q-Left: "  << p.map_qleft;
    out << "Map of Q-Right: " << p.map_qright;
    return out;
  }

private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  // calibration value for TDC -> timing
  Double_t  tcal_left;
  Double_t  tcal_right;
  Double_t  qped_left;
  Double_t  qped_right;

  // Slew correction parameters for left, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_left_a;
  Double_t  tslew_left_b;
  Double_t  tslew_left_c;
  // Slew correction parameters for right, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_right_a;
  Double_t  tslew_right_b;
  Double_t  tslew_right_c;
  Double_t  tslew_ave_a;
  Double_t  tslew_ave_b;
  Double_t  tslew_ave_c;
  Double_t  tdc_underflow;
  Double_t  tdc_overflow;
  Double_t  zoffset;
  Double_t  toffset;
  Double_t  xcal0;
  Double_t  xcal1;
  Double_t  xcal2;
  TArtRIDFMap map_tleft;
  TArtRIDFMap map_tright;
  TArtRIDFMap map_qleft;
  TArtRIDFMap map_qright;

  ClassDef(TArtPlasticPara,1);

};

#endif
