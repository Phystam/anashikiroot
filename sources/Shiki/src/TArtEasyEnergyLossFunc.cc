#include "TArtEasyEnergyLossFunc.hh"
#include <map>
#include <utility>
#include <TObject.h>
#include <TString.h>
#include <vector>
using namespace std;

TArtEasyEnergyLossFunc::TArtEasyEnergyLossFunc(){

  fdefault.push_back(0);
  fdefault.push_back(1);
  fdefault.push_back(0);
  fdefault.push_back(0);

  fBeamElossPara[make_pair(make_pair(27,9),"C")].push_back(6.87655);
  fBeamElossPara[make_pair(make_pair(27,9),"C")].push_back(0.593068);
  fBeamElossPara[make_pair(make_pair(27,9),"C")].push_back(0.00203127);
  fBeamElossPara[make_pair(make_pair(27,9),"C")].push_back(-3.02836e-06);
  fBeamElossPara[make_pair(make_pair(29,9),"C")].push_back(9.12862);
  fBeamElossPara[make_pair(make_pair(29,9),"C")].push_back(0.589959);
  fBeamElossPara[make_pair(make_pair(29,9),"C")].push_back(0.00201206);
  fBeamElossPara[make_pair(make_pair(29,9),"C")].push_back(-2.9774e-06);
  fBeamElossPara[make_pair(make_pair(31,9),"C")].push_back(11.467);
  fBeamElossPara[make_pair(make_pair(31,9),"C")].push_back(0.585546);
  fBeamElossPara[make_pair(make_pair(31,9),"C")].push_back(0.00198841);
  fBeamElossPara[make_pair(make_pair(31,9),"C")].push_back(-2.90216e-06);

  //for Ne isotopes


  //30Ne+C
  fBeamElossPara[make_pair(make_pair(29,10),"C")].push_back(2.82761);
  fBeamElossPara[make_pair(make_pair(29,10),"C")].push_back(0.596318);
  fBeamElossPara[make_pair(make_pair(29,10),"C")].push_back(0.00206004);
  fBeamElossPara[make_pair(make_pair(29,10),"C")].push_back(-3.0867e-06);
  fBeamElossPara[make_pair(make_pair(30,10),"C")].push_back(4.2559);
  fBeamElossPara[make_pair(make_pair(30,10),"C")].push_back(0.593919);
  fBeamElossPara[make_pair(make_pair(30,10),"C")].push_back(0.00204671);
  fBeamElossPara[make_pair(make_pair(30,10),"C")].push_back(-3.04789e-06);
  fBeamElossPara[make_pair(make_pair(31,10),"C")].push_back(5.04995);
  fBeamElossPara[make_pair(make_pair(31,10),"C")].push_back(0.593876);
  fBeamElossPara[make_pair(make_pair(31,10),"C")].push_back(0.00204257);
  fBeamElossPara[make_pair(make_pair(31,10),"C")].push_back(-3.04541e-06);
  fBeamElossPara[make_pair(make_pair(32,10),"C")].push_back(4.5962);
  fBeamElossPara[make_pair(make_pair(32,10),"C")].push_back(0.595879);
  fBeamElossPara[make_pair(make_pair(32,10),"C")].push_back(0.00205003);
  fBeamElossPara[make_pair(make_pair(32,10),"C")].push_back(-3.07773e-06);

  fBeamElossPara[make_pair(make_pair(30,11),"C")].push_back(-2.69551);
  fBeamElossPara[make_pair(make_pair(30,11),"C")].push_back(0.601398);
  fBeamElossPara[make_pair(make_pair(30,11),"C")].push_back(0.00210123);
  fBeamElossPara[make_pair(make_pair(30,11),"C")].push_back(-3.17644e-06);
  fBeamElossPara[make_pair(make_pair(31,11),"C")].push_back(-1.83323);
  fBeamElossPara[make_pair(make_pair(31,11),"C")].push_back(0.601837);
  fBeamElossPara[make_pair(make_pair(31,11),"C")].push_back(0.00209827);
  fBeamElossPara[make_pair(make_pair(31,11),"C")].push_back(-3.18262e-06);
  fBeamElossPara[make_pair(make_pair(32,11),"C")].push_back(-0.441637);
  fBeamElossPara[make_pair(make_pair(32,11),"C")].push_back(0.599838);
  fBeamElossPara[make_pair(make_pair(32,11),"C")].push_back(0.0020856);
  fBeamElossPara[make_pair(make_pair(32,11),"C")].push_back(-3.148e-06);
  fBeamElossPara[make_pair(make_pair(33,11),"C")].push_back(0.850801);
  fBeamElossPara[make_pair(make_pair(33,11),"C")].push_back(0.597914);
  fBeamElossPara[make_pair(make_pair(33,11),"C")].push_back(0.00207395);
  fBeamElossPara[make_pair(make_pair(33,11),"C")].push_back(-3.11494e-06);
  fBeamElossPara[make_pair(make_pair(34,11),"C")].push_back(1.77357);
  fBeamElossPara[make_pair(make_pair(34,11),"C")].push_back(0.597398);
  fBeamElossPara[make_pair(make_pair(34,11),"C")].push_back(0.00206823);
  fBeamElossPara[make_pair(make_pair(34,11),"C")].push_back(-3.10616e-06);

  //_________________________________________________________
  //fragment
  //_______________________________________________________
  fFragElossPara[make_pair(make_pair(27,9),"C")].push_back(52.6345);
  fFragElossPara[make_pair(make_pair(27,9),"C")].push_back(0.541187);
  fFragElossPara[make_pair(make_pair(27,9),"C")].push_back(0.00181088);
  fFragElossPara[make_pair(make_pair(27,9),"C")].push_back(-2.54226e-06);
  fFragElossPara[make_pair(make_pair(29,9),"C")].push_back(51.0945);
  fFragElossPara[make_pair(make_pair(29,9),"C")].push_back(0.544085);
  fFragElossPara[make_pair(make_pair(29,9),"C")].push_back(0.00181836);
  fFragElossPara[make_pair(make_pair(29,9),"C")].push_back(-2.57029e-06);
  fFragElossPara[make_pair(make_pair(31,9),"C")].push_back(50.1539);
  fFragElossPara[make_pair(make_pair(31,9),"C")].push_back(0.544741);
  fFragElossPara[make_pair(make_pair(31,9),"C")].push_back(0.00181628);
  fFragElossPara[make_pair(make_pair(31,9),"C")].push_back(-2.55518e-06);

  //for Ne isotopes


  //30Ne+C
  fFragElossPara[make_pair(make_pair(29,10),"C")].push_back(54.9101);
  fFragElossPara[make_pair(make_pair(29,10),"C")].push_back(0.539811);
  fFragElossPara[make_pair(make_pair(29,10),"C")].push_back(0.00181673);
  fFragElossPara[make_pair(make_pair(29,10),"C")].push_back(-2.58317e-06);
  fFragElossPara[make_pair(make_pair(30,10),"C")].push_back(54.5095);
  fFragElossPara[make_pair(make_pair(30,10),"C")].push_back(0.53927);
  fFragElossPara[make_pair(make_pair(30,10),"C")].push_back(0.00181202);
  fFragElossPara[make_pair(make_pair(30,10),"C")].push_back(-2.55715e-06);
  fFragElossPara[make_pair(make_pair(31,10),"C")].push_back(53.7975);
  fFragElossPara[make_pair(make_pair(31,10),"C")].push_back(0.540349);
  fFragElossPara[make_pair(make_pair(31,10),"C")].push_back(0.00181367);
  fFragElossPara[make_pair(make_pair(31,10),"C")].push_back(-2.56086e-06);
  fFragElossPara[make_pair(make_pair(32,10),"C")].push_back(53.8043);
  fFragElossPara[make_pair(make_pair(32,10),"C")].push_back(0.540341);
  fFragElossPara[make_pair(make_pair(32,10),"C")].push_back(0.00181347);
  fFragElossPara[make_pair(make_pair(32,10),"C")].push_back(-2.56084e-06);

  fFragElossPara[make_pair(make_pair(30,11),"C")].push_back(58.7551);
  fFragElossPara[make_pair(make_pair(30,11),"C")].push_back(0.533971);
  fFragElossPara[make_pair(make_pair(30,11),"C")].push_back(0.00180502);
  fFragElossPara[make_pair(make_pair(30,11),"C")].push_back(-2.54968e-06);
  fFragElossPara[make_pair(make_pair(31,11),"C")].push_back(57.6324);
  fFragElossPara[make_pair(make_pair(31,11),"C")].push_back(0.536687);
  fFragElossPara[make_pair(make_pair(31,11),"C")].push_back(0.0018138);
  fFragElossPara[make_pair(make_pair(31,11),"C")].push_back(-2.58907e-06);
  fFragElossPara[make_pair(make_pair(32,11),"C")].push_back(57.1407);
  fFragElossPara[make_pair(make_pair(32,11),"C")].push_back(0.536296);
  fFragElossPara[make_pair(make_pair(32,11),"C")].push_back(0.00180956);
  fFragElossPara[make_pair(make_pair(32,11),"C")].push_back(-2.56202e-06);
  fFragElossPara[make_pair(make_pair(33,11),"C")].push_back(56.2935);
  fFragElossPara[make_pair(make_pair(33,11),"C")].push_back(0.53785);
  fFragElossPara[make_pair(make_pair(33,11),"C")].push_back(0.00181353);
  fFragElossPara[make_pair(make_pair(33,11),"C")].push_back(-2.57535e-06);
  fFragElossPara[make_pair(make_pair(34,11),"C")].push_back(55.6129);
  fFragElossPara[make_pair(make_pair(34,11),"C")].push_back(0.538781);
  fFragElossPara[make_pair(make_pair(34,11),"C")].push_back(0.00181531);
  fFragElossPara[make_pair(make_pair(34,11),"C")].push_back(-2.58003e-06);


}
