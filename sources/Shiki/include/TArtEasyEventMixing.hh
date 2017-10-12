/*
This privides event mixing scheme

*/

#ifndef EASY_EVENT_MIXING_HH
#define EASY_EVENT_MIXING_HH

#include <vector>
#include <TLorentzVector.h>

class TLorentzVector;
class TArtEasyEventMixing {
public:
  TArtEasyEventMixing();
  TArtEasyEventMixing();
  void LoadEvents(std::vector<TLorentzVector> part1, std::vector<TLorentzVector> part2);
  void DoMixing();
  void DoIteration();
  Double_t GetNumVirtualPair();
  
private:
  std::vector<TLorentzVector> fpart1;
  std::vector<TLorentzVector> fpart2;
};
#endif
