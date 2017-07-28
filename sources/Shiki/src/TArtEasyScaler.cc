#include <TString.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include "TArtEasyScaler.hh"

using namespace std;
TArtEasyScaler::TArtEasyScaler(Int_t g,Int_t ug,Int_t bnh,Int_t bneuh,Int_t bch, Int_t ck,Int_t cm){
  Init();
  fgated  =g;
  fungated=ug;
  fBxNEBxH=bnh;
  fBxNeuxH=bneuh;
  fBxCATxH=bch;
  fclock1k=ck;
  fclock1M=cm;
}

TArtEasyScaler::TArtEasyScaler(TString scrfile){
  Init();
  SetEasyScaler(scrfile);
}

TArtEasyScaler::TArtEasyScaler(Int_t runnum){
  Init();
  TString scrfile=Form("scr/run%04d.scr",runnum);
  SetEasyScaler(scrfile);
}
TArtEasyScaler::TArtEasyScaler(){
  Init();
}

void TArtEasyScaler::Init(){
  fgated   = 1;
  fungated = 1;
  fBxNEBxH = 1;
  fBxNeuxH = 1;
  fBxCATxH = 1;
  fclock1k = 1;
  fclock1M = 1;
}

void TArtEasyScaler::SetEasyScaler(TString scrfile){
  ifstream ist(scrfile.Data());
  string header;
  getline(ist,header);
  //  cout << header << endl;
  while(!ist.eof()){
    string subject;
    string scr;
    string total;
    getline(ist,total);
    stringstream ss(total);
    getline(ss,subject,':');
    getline(ss,scr);
    //    cout <<subject << " " << scr << endl;
    if(subject==" gated trig"){
      fgated=atoi(scr.c_str());
    }
    if(subject=="       GTO8"){
      fungated=atoi(scr.c_str());
    }
    if(subject=="       GTO2"){
      fBxNeuxH=atoi(scr.c_str());
    }
    if(subject=="       GTO3"){
      fBxCATxH=atoi(scr.c_str());
    }
    if(subject=="    BxNEBxH"){
      fBxNEBxH=atoi(scr.c_str());
    }
    if(subject==" Clock 1kHz"){
      fclock1k=atoi(scr.c_str());
    }
    if(subject==" Clock 1MHz"){
      fclock1M=atoi(scr.c_str());
    }
  }
}

TArtEasyScaler TArtEasyScaler::operator+(TArtEasyScaler &scr2){
  return TArtEasyScaler(fgated+scr2.GetGated(),
			fungated+scr2.GetUngated(),
			fBxNEBxH+scr2.GetBxNEBxH(),
			fBxNeuxH+scr2.GetBxNeuxH(),
			fBxCATxH+scr2.GetBxCATxH(),
			fclock1k+scr2.GetClock1kHz(),
			fclock1M+scr2.GetClock1MHz());
}

TArtEasyScaler* TArtEasyScaler::operator+(TArtEasyScaler* scr2){
  TArtEasyScaler* scr=new TArtEasyScaler(fgated+scr2->GetGated(),
					 fungated+scr2->GetUngated(),
					 fBxNEBxH+scr2->GetBxNEBxH(),
					 fBxNeuxH+scr2->GetBxNeuxH(),
					 fBxCATxH+scr2->GetBxCATxH(),
					 fclock1k+scr2->GetClock1kHz(),
					 fclock1M+scr2->GetClock1MHz());
  return scr;
}
