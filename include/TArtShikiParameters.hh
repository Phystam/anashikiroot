#ifndef _TARTSHIKIPARAMETERS_HH_
#define _TARTSHIKIPARAMETERS_HH_

#include <TNamed.h>
#include <TList.h>
#include <map>

class TXMLNode;
class TArtEasyTarget;
class TArtStoreManager;

typedef std::map<int, TArtEasyTarget*> TArtEasyTargetMap;

class TArtShikiParameters : public TNamed {

private:
  TArtShikiParameters(const char* name = "ShikiParameters",
		      const char* title = "ShikiParameters");

public:
  ~TArtShikiParameters();

  static TArtShikiParameters* Instance(const char* name = "ShikiParameters",
				       const char* title = "ShikiParameters");
  static void Delete();

  Bool_t LoadParameter(const char *xmlfile);
  // void PrintListOfEasyTarget();
  //  TList* GetListOfEasyTarget(){return listOfEasyTarget;}
  TArtEasyTarget* FindEasyTarget(Int_t id) const;
  Int_t GetNumTarget() const { return fnumtarget; }
private:
  void ParseParaList(TXMLNode* node);
  TArtEasyTarget* ParseEasyTarget(TXMLNode* node);
  std::map<int, TArtEasyTarget*> fIDEasyTargetMap;
  //  TList* listOfEasyTarget;
  Int_t fnumtarget;
  static TArtShikiParameters* fShikiParameters;
  TArtStoreManager* fStoreManager;
  ClassDef(TArtShikiParameters,1);
};
#endif
