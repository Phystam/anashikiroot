#include "TArtShikiParameters.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"

#include "TArtEasyTarget.hh"

#include <TList.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
TArtShikiParameters* TArtShikiParameters::fShikiParameters = 0;

TArtShikiParameters::TArtShikiParameters(const char *name, const char *title)
  : TNamed(name, title){
  TArtCore::Info(__FILE__,"Creating Shiki-nyan setup...");
  fStoreManager = TArtStoreManager::Instance();
  fStoreManager->AddParameters(this);
  fnumtarget=0;
}

TArtShikiParameters* TArtShikiParameters::Instance(const char* name, const char* title)
{
  if(!fShikiParameters) {
    fShikiParameters = new TArtShikiParameters(name, title);
  }
  return fShikiParameters;
}

TArtShikiParameters::~TArtShikiParameters(){
  // {
  //   TArtEasyTargetMap::iterator itr = fIDEasyTargetMap->begin();
  //   std::vector<const TArtEasyTarget*> pvec;
  //   while(itr != fIDEasyTargetMap->end()){
  //     std::vector<const TArtEasyTarget*>::iterator it_pvec = pvec.begin();
  //     if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
  // 	pvec.push_back(itr->second);
  // 	delete itr->second;
  // 	itr->second = 0;
  //     }
  //     ++itr;
  //   }
  // }
  delete fShikiParameters;
  fShikiParameters = 0;
}

void TArtShikiParameters::Delete(){
  delete fShikiParameters;
  fShikiParameters = 0;
}


Bool_t TArtShikiParameters::LoadParameter(const char *xmlfile)
{
  TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
  TDOMParser domParser;
  domParser.SetValidate(false);
  Int_t parsecode = domParser.ParseFile(xmlfile);
  if(parsecode < 0){
    std::cerr << domParser.GetParseCodeMessage(parsecode) << std::endl;
    return false;
  }
  TXMLNode* node = domParser.GetXMLDocument()->GetRootNode();
  ParseParaList(node->GetChildren());
  return true;
}

void TArtShikiParameters::ParseParaList(TXMLNode *node)
{
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "EasyTarget") == 0){

      ParseEasyTarget(node->GetChildren());
    }
  }
}

TArtEasyTarget *TArtShikiParameters::ParseEasyTarget(TXMLNode *node)
{
  TArtEasyTarget* easytarget = new TArtEasyTarget;
  fnumtarget++;
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node
    
    if(strcmp(node->GetNodeName(), "ID") == 0){ 
      Int_t id = atoi(node->GetText());
      easytarget->SetID(id);
    }else if(strcmp(node->GetNodeName(), "NAME") == 0){
      TString name = node->GetText();
      easytarget->SetDetectorName(name);
    }else if(strcmp(node->GetNodeName(), "FPl") == 0){
      Int_t fpl = atoi(node->GetText());
      easytarget->SetFpl(fpl);
    }else if(strcmp(node->GetNodeName(), "PosZ") == 0){
      Double_t posz = atof(node->GetText());
      easytarget->SetPosZ(posz);
    }else if(strcmp(node->GetNodeName(), "TOF0") == 0){
      Double_t tof0 = atof(node->GetText());
      easytarget->SetTOF(0,tof0);
    }else if(strcmp(node->GetNodeName(), "TOF1") == 0){
      Double_t tof1 = atof(node->GetText());
      easytarget->SetTOF(1,tof1);
    }else if(strcmp(node->GetNodeName(), "TOF2") == 0){
      Double_t tof2 = atof(node->GetText());
      easytarget->SetTOF(2,tof2);
    }else if(strcmp(node->GetNodeName(), "TOF3") == 0){
      Double_t tof3 = atof(node->GetText());
      easytarget->SetTOF(3,tof3);
    }else if(strcmp(node->GetNodeName(), "BeamE0") == 0){
      Double_t BeamE0 = atof(node->GetText());
      easytarget->SetBeamE(0,BeamE0);
    }else if(strcmp(node->GetNodeName(), "BeamE1") == 0){
      Double_t BeamE1 = atof(node->GetText());
      easytarget->SetBeamE(1,BeamE1);
    }else if(strcmp(node->GetNodeName(), "BeamE2") == 0){
      Double_t BeamE2 = atof(node->GetText());
      easytarget->SetBeamE(2,BeamE2);
    }else if(strcmp(node->GetNodeName(), "BeamE3") == 0){
      Double_t BeamE3 = atof(node->GetText());
      easytarget->SetBeamE(3,BeamE3);
    }else if(strcmp(node->GetNodeName(), "FragE0") == 0){
      Double_t FragE0 = atof(node->GetText());
      easytarget->SetFragE(0,FragE0);
    }else if(strcmp(node->GetNodeName(), "FragE1") == 0){
      Double_t FragE1 = atof(node->GetText());
      easytarget->SetFragE(1,FragE1);
    }else if(strcmp(node->GetNodeName(), "FragE2") == 0){
      Double_t FragE2 = atof(node->GetText());
      easytarget->SetFragE(2,FragE2);
    }else if(strcmp(node->GetNodeName(), "FragE3") == 0){
      Double_t FragE3 = atof(node->GetText());
      easytarget->SetFragE(3,FragE3);
    }
  }
  fIDEasyTargetMap.insert(std::pair<int,TArtEasyTarget*>(easytarget->GetID(), easytarget));
  return easytarget;
}

// void TArtShikiParameters::PrintListOfEasyTarget() {
//   TArtEasyTargetMap::iterator it = fEasyTargetMap->begin();
//   while(it !=  fIDEasyTargetMap->end()){
//     std::cout << *(it->second) << std::endl;
//     ++it;
//   }
// }

TArtEasyTarget* TArtShikiParameters::FindEasyTarget(Int_t id) const {
  std::map<int,TArtEasyTarget*>::const_iterator it = fIDEasyTargetMap.find(id);
  if(it != fIDEasyTargetMap.end()) return it->second;
  else return 0;
}
