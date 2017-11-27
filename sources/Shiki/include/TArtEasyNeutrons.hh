
// Neutron reconstruction class
// by T.Tomai

#ifndef _TARTEASYNEUTRONS_H_
#define _TARTEASYNEUTRONS_H_

#include <TNamed.h>
#include <TClonesArray.h>
#include "TArtReconstruction.hh"
#include <vector>
#include "TArtEasyNeutron.hh"
#include "TArtCalibNeuLAND.hh"
class TArtSAMURAIParameters;
class TArtShikiParameters;
class TArtStoreManager;
class TArtCalibNEBULA;
class TArtCalibNeuLAND;
class TArtCalibNeuLANDVETO;
class TArtEasyBeam;
class TArtEasyPID;
class TVector3;


class TArtEasyNeutrons : public TArtReconstruction{

 public:

  TArtEasyNeutrons();
  virtual ~TArtEasyNeutrons();

  //  void Initialize();
  void ClearData();
  void LoadData();
  void ReconstructData();
  void SetTacDistribution(const char* file){
    fhas_dist=true;
    fcalibneuland->SetTacDistribution(file);
  }
  TArtEasyBeam* GetEasyBeam(){return feasybeam;}
  TArtEasyPID* GetEasyPID(){return feasypid;}
  TArtCalibNEBULA* GetCalibNEBULA(){return fcalibnebula;}
  TArtCalibNeuLAND* GetCalibNeuLAND(){return fcalibneuland;}
  TArtCalibNeuLANDVETO* GetCalibNeuLANDVETO(){return fcalibneulandveto;}
  Bool_t IsVeto(){return fisveto;}
  Int_t GetNumNEBULAPla(){return fnumnebulapla;}
  Int_t GetNumHits(){return (Int_t)easyneuts.size();}
  TArtEasyNeutron* GetEasyNeutron(Int_t i){return easyneuts[i];}
  TArtEasyNeutron* GetFastestNeutron(){return easyneuts[0];}
  std::vector<TArtEasyNeutron*> GetVectorEasyNeutron(){return easyneuts;}
  
  static bool CompareForSort(const TArtEasyNeutron* left,const TArtEasyNeutron* right) {
    if(isnan(left->GetTOF())&&isnan(right->GetTOF())){
      return true;
    }
    if(isnan(right->GetTOF())){
      return true;
    }
    if(isnan(left->GetTOF())){
      return false;
    }
    // std::cout <<"comparing< "<<left->GetTOF()<<" and "<<right->GetTOF() <<std::endl;
    // std::cout <<" result-> "<< (left->GetTOF() < right->GetTOF()) <<std::endl;
    return left->GetTOF() < right->GetTOF();
  }


private:

  // pointer to subsystem objects used for Neutron
  TArtCalibNEBULA* fcalibnebula;
  TArtCalibNeuLAND* fcalibneuland;
  TArtCalibNeuLANDVETO* fcalibneulandveto;
  Int_t fnumneutpla,fnumnebulapla,fnumneulandpla,fnumneulandvetopla;
  std::vector<TArtEasyNeutron*> easyneuts;
  TArtShikiParameters* fshikipara;
  //  TArtEasyTarget* feasytgt;
  TArtEasyBeam* feasybeam;
  TArtEasyPID* feasypid;
  Double_t ftgtz;
  Bool_t fisveto;
  Bool_t fhas_dist;
  static const Double_t fclight=299.792458;// mm/ns
  //  static const Double_t fe=1.6021766208e-19;// C
  static const Double_t famu=1.660538921e-27;// kg
  static const Double_t famu_MeV=931.494061;//MeV
  static const Double_t fMass=939.565;//MeV/c2
  ClassDef(TArtEasyNeutrons,1);
};

#endif
