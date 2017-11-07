#include "TArtEasyEnergyLossFunc.hh"
#include <map>
#include <utility>
#include <TObject.h>
#include <TString.h>
#include <vector>
using namespace std;

TArtEasyEnergyLossFunc::TArtEasyEnergyLossFunc(){
  fdefault.push_back(2.82761);
  fdefault.push_back(0.596318);
  fdefault.push_back(0.00206004);
  fdefault.push_back(-3.0867e-06);

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
  fdefaultfrag.push_back(54.9101);	  
  fdefaultfrag.push_back(0.539811);	  
  fdefaultfrag.push_back(0.00181673);  
  fdefaultfrag.push_back(-2.58317e-06);


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

  //Pb target--------------------------------------------------------------
  fBeamElossPara[make_pair(make_pair(27,9),"Pb")].push_back(14.2772);
  fBeamElossPara[make_pair(make_pair(27,9),"Pb")].push_back(0.580946);
  fBeamElossPara[make_pair(make_pair(27,9),"Pb")].push_back(0.00196202);
  fBeamElossPara[make_pair(make_pair(27,9),"Pb")].push_back(-2.83175e-06);
  fBeamElossPara[make_pair(make_pair(29,9),"Pb")].push_back(15.2521);
  fBeamElossPara[make_pair(make_pair(29,9),"Pb")].push_back(0.581835);
  fBeamElossPara[make_pair(make_pair(29,9),"Pb")].push_back(0.00195941);
  fBeamElossPara[make_pair(make_pair(29,9),"Pb")].push_back(-2.84332e-06);
  fBeamElossPara[make_pair(make_pair(31,9),"Pb")].push_back(16.9741);
  fBeamElossPara[make_pair(make_pair(31,9),"Pb")].push_back(0.578844);
  fBeamElossPara[make_pair(make_pair(31,9),"Pb")].push_back(0.00194289);
  fBeamElossPara[make_pair(make_pair(31,9),"Pb")].push_back(-2.79235e-06);

  //for Ne isotopes


  //30Ne+C
  fBeamElossPara[make_pair(make_pair(29,10),"Pb")].push_back(10.7164);
  fBeamElossPara[make_pair(make_pair(29,10),"Pb")].push_back(0.585374);
  fBeamElossPara[make_pair(make_pair(29,10),"Pb")].push_back(0.00199119);
  fBeamElossPara[make_pair(make_pair(29,10),"Pb")].push_back(-2.91024e-06);
  fBeamElossPara[make_pair(make_pair(30,10),"Pb")].push_back(11.6317);
  fBeamElossPara[make_pair(make_pair(30,10),"Pb")].push_back(0.584237);
  fBeamElossPara[make_pair(make_pair(30,10),"Pb")].push_back(0.00198356);
  fBeamElossPara[make_pair(make_pair(30,10),"Pb")].push_back(-2.88959e-06);
  fBeamElossPara[make_pair(make_pair(31,10),"Pb")].push_back(12.6481);
  fBeamElossPara[make_pair(make_pair(31,10),"Pb")].push_back(0.582454);
  fBeamElossPara[make_pair(make_pair(31,10),"Pb")].push_back(0.00197401);
  fBeamElossPara[make_pair(make_pair(31,10),"Pb")].push_back(-2.8595e-06);
  fBeamElossPara[make_pair(make_pair(32,10),"Pb")].push_back(12.5418);
  fBeamElossPara[make_pair(make_pair(32,10),"Pb")].push_back(0.582932);
  fBeamElossPara[make_pair(make_pair(32,10),"Pb")].push_back(0.00197585);
  fBeamElossPara[make_pair(make_pair(32,10),"Pb")].push_back(-2.86748e-06);

  fBeamElossPara[make_pair(make_pair(30,11),"Pb")].push_back(6.65882);
  fBeamElossPara[make_pair(make_pair(30,11),"Pb")].push_back(0.588317);
  fBeamElossPara[make_pair(make_pair(30,11),"Pb")].push_back(0.00201903);
  fBeamElossPara[make_pair(make_pair(30,11),"Pb")].push_back(-2.96497e-06);
  fBeamElossPara[make_pair(make_pair(31,11),"Pb")].push_back(7.42414);
  fBeamElossPara[make_pair(make_pair(31,11),"Pb")].push_back(0.588168);
  fBeamElossPara[make_pair(make_pair(31,11),"Pb")].push_back(0.00201484);
  fBeamElossPara[make_pair(make_pair(31,11),"Pb")].push_back(-2.96006e-06);
  fBeamElossPara[make_pair(make_pair(32,11),"Pb")].push_back(8.53736);
  fBeamElossPara[make_pair(make_pair(32,11),"Pb")].push_back(0.586501);
  fBeamElossPara[make_pair(make_pair(32,11),"Pb")].push_back(0.00200489);
  fBeamElossPara[make_pair(make_pair(32,11),"Pb")].push_back(-2.93252e-06);
  fBeamElossPara[make_pair(make_pair(33,11),"Pb")].push_back(9.31544);
  fBeamElossPara[make_pair(make_pair(33,11),"Pb")].push_back(0.585978);
  fBeamElossPara[make_pair(make_pair(33,11),"Pb")].push_back(0.00199954);
  fBeamElossPara[make_pair(make_pair(33,11),"Pb")].push_back(-2.92202e-06);
  fBeamElossPara[make_pair(make_pair(34,11),"Pb")].push_back(9.80607);
  fBeamElossPara[make_pair(make_pair(34,11),"Pb")].push_back(0.586448);
  fBeamElossPara[make_pair(make_pair(34,11),"Pb")].push_back(0.00199845);
  fBeamElossPara[make_pair(make_pair(34,11),"Pb")].push_back(-2.92757e-06);

  //_________________________________________________________
  //fragment
  //_______________________________________________________
  fFragElossPara[make_pair(make_pair(27,9),"Pb")].push_back(46.8736);
  fFragElossPara[make_pair(make_pair(27,9),"Pb")].push_back(0.55053);
  fFragElossPara[make_pair(make_pair(27,9),"Pb")].push_back(0.00183769);
  fFragElossPara[make_pair(make_pair(27,9),"Pb")].push_back(-2.61709e-06);
  fFragElossPara[make_pair(make_pair(29,9),"Pb")].push_back(46.0985);
  fFragElossPara[make_pair(make_pair(29,9),"Pb")].push_back(0.551186);
  fFragElossPara[make_pair(make_pair(29,9),"Pb")].push_back(0.00183519);
  fFragElossPara[make_pair(make_pair(29,9),"Pb")].push_back(-2.60531e-06);
  fFragElossPara[make_pair(make_pair(31,9),"Pb")].push_back(45.6555);
  fFragElossPara[make_pair(make_pair(31,9),"Pb")].push_back(0.550579);
  fFragElossPara[make_pair(make_pair(31,9),"Pb")].push_back(0.00182829);
  fFragElossPara[make_pair(make_pair(31,9),"Pb")].push_back(-2.57151e-06);

  //for Ne isotopes


  //30Ne+C


  fFragElossPara[make_pair(make_pair(29,10),"Pb")].push_back(49.2971);
  fFragElossPara[make_pair(make_pair(29,10),"Pb")].push_back(0.545795);
  fFragElossPara[make_pair(make_pair(29,10),"Pb")].push_back(0.00182844);
  fFragElossPara[make_pair(make_pair(29,10),"Pb")].push_back(-2.57845e-06);
  fFragElossPara[make_pair(make_pair(30,10),"Pb")].push_back(48.4812);
  fFragElossPara[make_pair(make_pair(30,10),"Pb")].push_back(0.547916);
  fFragElossPara[make_pair(make_pair(30,10),"Pb")].push_back(0.00183474);
  fFragElossPara[make_pair(make_pair(30,10),"Pb")].push_back(-2.60649e-06);
  fFragElossPara[make_pair(make_pair(31,10),"Pb")].push_back(48.0284);
  fFragElossPara[make_pair(make_pair(31,10),"Pb")].push_back(0.548489);
  fFragElossPara[make_pair(make_pair(31,10),"Pb")].push_back(0.00183442);
  fFragElossPara[make_pair(make_pair(31,10),"Pb")].push_back(-2.60469e-06);
  fFragElossPara[make_pair(make_pair(32,10),"Pb")].push_back(47.7013);
  fFragElossPara[make_pair(make_pair(32,10),"Pb")].push_back(0.549948);
  fFragElossPara[make_pair(make_pair(32,10),"Pb")].push_back(0.0018406);
  fFragElossPara[make_pair(make_pair(32,10),"Pb")].push_back(-2.63208e-06);

  fFragElossPara[make_pair(make_pair(30,11),"Pb")].push_back(51.499);
  fFragElossPara[make_pair(make_pair(30,11),"Pb")].push_back(0.544194);
  fFragElossPara[make_pair(make_pair(30,11),"Pb")].push_back(0.00183526);
  fFragElossPara[make_pair(make_pair(30,11),"Pb")].push_back(-2.61734e-06);
  fFragElossPara[make_pair(make_pair(31,11),"Pb")].push_back(50.581);
  fFragElossPara[make_pair(make_pair(31,11),"Pb")].push_back(0.546599);
  fFragElossPara[make_pair(make_pair(31,11),"Pb")].push_back(0.00184233);
  fFragElossPara[make_pair(make_pair(31,11),"Pb")].push_back(-2.64913e-06);
  fFragElossPara[make_pair(make_pair(32,11),"Pb")].push_back(50.2158);
  fFragElossPara[make_pair(make_pair(32,11),"Pb")].push_back(0.546391);
  fFragElossPara[make_pair(make_pair(32,11),"Pb")].push_back(0.00183894);
  fFragElossPara[make_pair(make_pair(32,11),"Pb")].push_back(-2.6299e-06);
  fFragElossPara[make_pair(make_pair(33,11),"Pb")].push_back(50.1562);
  fFragElossPara[make_pair(make_pair(33,11),"Pb")].push_back(0.54506);
  fFragElossPara[make_pair(make_pair(33,11),"Pb")].push_back(0.00183059);
  fFragElossPara[make_pair(make_pair(33,11),"Pb")].push_back(-2.59139e-06);
  fFragElossPara[make_pair(make_pair(34,11),"Pb")].push_back(49.2979);
  fFragElossPara[make_pair(make_pair(34,11),"Pb")].push_back(0.547439);
  fFragElossPara[make_pair(make_pair(34,11),"Pb")].push_back(0.0018378);
  fFragElossPara[make_pair(make_pair(34,11),"Pb")].push_back(-2.62291e-06);

  //Empty target----------------------------------------------------------
  fBeamElossPara[make_pair(make_pair(27,9),"Emp")].push_back(6.87655);
  fBeamElossPara[make_pair(make_pair(27,9),"Emp")].push_back(0.593068);
  fBeamElossPara[make_pair(make_pair(27,9),"Emp")].push_back(0.00203127);
  fBeamElossPara[make_pair(make_pair(27,9),"Emp")].push_back(-3.02836e-06);
  fBeamElossPara[make_pair(make_pair(29,9),"Emp")].push_back(9.12862);
  fBeamElossPara[make_pair(make_pair(29,9),"Emp")].push_back(0.589959);
  fBeamElossPara[make_pair(make_pair(29,9),"Emp")].push_back(0.00201206);
  fBeamElossPara[make_pair(make_pair(29,9),"Emp")].push_back(-2.9774e-06);
  fBeamElossPara[make_pair(make_pair(31,9),"Emp")].push_back(11.467);
  fBeamElossPara[make_pair(make_pair(31,9),"Emp")].push_back(0.585546);
  fBeamElossPara[make_pair(make_pair(31,9),"Emp")].push_back(0.00198841);
  fBeamElossPara[make_pair(make_pair(31,9),"Emp")].push_back(-2.90216e-06);

  //for Ne isotopes


  //30Ne+C
  fBeamElossPara[make_pair(make_pair(29,10),"Emp")].push_back(2.82761);
  fBeamElossPara[make_pair(make_pair(29,10),"Emp")].push_back(0.596318);
  fBeamElossPara[make_pair(make_pair(29,10),"Emp")].push_back(0.00206004);
  fBeamElossPara[make_pair(make_pair(29,10),"Emp")].push_back(-3.0867e-06);
  fBeamElossPara[make_pair(make_pair(30,10),"Emp")].push_back(4.2559);
  fBeamElossPara[make_pair(make_pair(30,10),"Emp")].push_back(0.593919);
  fBeamElossPara[make_pair(make_pair(30,10),"Emp")].push_back(0.00204671);
  fBeamElossPara[make_pair(make_pair(30,10),"Emp")].push_back(-3.04789e-06);
  fBeamElossPara[make_pair(make_pair(31,10),"Emp")].push_back(5.04995);
  fBeamElossPara[make_pair(make_pair(31,10),"Emp")].push_back(0.593876);
  fBeamElossPara[make_pair(make_pair(31,10),"Emp")].push_back(0.00204257);
  fBeamElossPara[make_pair(make_pair(31,10),"Emp")].push_back(-3.04541e-06);
  fBeamElossPara[make_pair(make_pair(32,10),"Emp")].push_back(4.5962);
  fBeamElossPara[make_pair(make_pair(32,10),"Emp")].push_back(0.595879);
  fBeamElossPara[make_pair(make_pair(32,10),"Emp")].push_back(0.00205003);
  fBeamElossPara[make_pair(make_pair(32,10),"Emp")].push_back(-3.07773e-06);

  fBeamElossPara[make_pair(make_pair(30,11),"Emp")].push_back(-2.69551);
  fBeamElossPara[make_pair(make_pair(30,11),"Emp")].push_back(0.601398);
  fBeamElossPara[make_pair(make_pair(30,11),"Emp")].push_back(0.00210123);
  fBeamElossPara[make_pair(make_pair(30,11),"Emp")].push_back(-3.17644e-06);
  fBeamElossPara[make_pair(make_pair(31,11),"Emp")].push_back(-1.83323);
  fBeamElossPara[make_pair(make_pair(31,11),"Emp")].push_back(0.601837);
  fBeamElossPara[make_pair(make_pair(31,11),"Emp")].push_back(0.00209827);
  fBeamElossPara[make_pair(make_pair(31,11),"Emp")].push_back(-3.18262e-06);
  fBeamElossPara[make_pair(make_pair(32,11),"Emp")].push_back(-0.441637);
  fBeamElossPara[make_pair(make_pair(32,11),"Emp")].push_back(0.599838);
  fBeamElossPara[make_pair(make_pair(32,11),"Emp")].push_back(0.0020856);
  fBeamElossPara[make_pair(make_pair(32,11),"Emp")].push_back(-3.148e-06);
  fBeamElossPara[make_pair(make_pair(33,11),"Emp")].push_back(0.850801);
  fBeamElossPara[make_pair(make_pair(33,11),"Emp")].push_back(0.597914);
  fBeamElossPara[make_pair(make_pair(33,11),"Emp")].push_back(0.00207395);
  fBeamElossPara[make_pair(make_pair(33,11),"Emp")].push_back(-3.11494e-06);
  fBeamElossPara[make_pair(make_pair(34,11),"Emp")].push_back(1.77357);
  fBeamElossPara[make_pair(make_pair(34,11),"Emp")].push_back(0.597398);
  fBeamElossPara[make_pair(make_pair(34,11),"Emp")].push_back(0.00206823);
  fBeamElossPara[make_pair(make_pair(34,11),"Emp")].push_back(-3.10616e-06);

  //_________________________________________________________
  //fragment
  //_______________________________________________________
  fFragElossPara[make_pair(make_pair(27,9),"Emp")].push_back(13.8561);
  fFragElossPara[make_pair(make_pair(27,9),"Emp")].push_back(0.584865);
  fFragElossPara[make_pair(make_pair(27,9),"Emp")].push_back(0.00200171);
  fFragElossPara[make_pair(make_pair(27,9),"Emp")].push_back(-2.96698e-06);
  fFragElossPara[make_pair(make_pair(29,9),"Emp")].push_back(15.4964);	   
  fFragElossPara[make_pair(make_pair(29,9),"Emp")].push_back(0.58276);	   
  fFragElossPara[make_pair(make_pair(29,9),"Emp")].push_back(0.00198627);  
  fFragElossPara[make_pair(make_pair(29,9),"Emp")].push_back(-2.92628e-06);
  fFragElossPara[make_pair(make_pair(31,9),"Emp")].push_back(17.2152);
  fFragElossPara[make_pair(make_pair(31,9),"Emp")].push_back(0.579586);
  fFragElossPara[make_pair(make_pair(31,9),"Emp")].push_back(0.001967);
  fFragElossPara[make_pair(make_pair(31,9),"Emp")].push_back(-2.8647e-06);

  //for Ne isotopes


  //30Ne+C


  fFragElossPara[make_pair(make_pair(29,10),"Emp")].push_back(10.7061);
  fFragElossPara[make_pair(make_pair(29,10),"Emp")].push_back(0.587635);	  
  fFragElossPara[make_pair(make_pair(29,10),"Emp")].push_back(0.00202972);
  fFragElossPara[make_pair(make_pair(29,10),"Emp")].push_back(-3.03127e-06);
  fFragElossPara[make_pair(make_pair(30,10),"Emp")].push_back(11.8234);
  fFragElossPara[make_pair(make_pair(30,10),"Emp")].push_back(0.585644);
  fFragElossPara[make_pair(make_pair(30,10),"Emp")].push_back(0.00201776);
  fFragElossPara[make_pair(make_pair(30,10),"Emp")].push_back(-2.99499e-06);
  fFragElossPara[make_pair(make_pair(31,10),"Emp")].push_back(12.4526);
  fFragElossPara[make_pair(make_pair(31,10),"Emp")].push_back(0.585522);
  fFragElossPara[make_pair(make_pair(31,10),"Emp")].push_back(0.00201296);
  fFragElossPara[make_pair(make_pair(31,10),"Emp")].push_back(-2.9879e-06);
  fFragElossPara[make_pair(make_pair(32,10),"Emp")].push_back(11.9364);
  fFragElossPara[make_pair(make_pair(32,10),"Emp")].push_back(0.587806);
  fFragElossPara[make_pair(make_pair(32,10),"Emp")].push_back(0.00202172);
  fFragElossPara[make_pair(make_pair(32,10),"Emp")].push_back(-3.026e-06);

  fFragElossPara[make_pair(make_pair(30,11),"Emp")].push_back(6.62444);
  fFragElossPara[make_pair(make_pair(30,11),"Emp")].push_back(0.591039);
  fFragElossPara[make_pair(make_pair(30,11),"Emp")].push_back(0.00206575);
  fFragElossPara[make_pair(make_pair(30,11),"Emp")].push_back(-3.11213e-06);
  fFragElossPara[make_pair(make_pair(31,11),"Emp")].push_back(7.24475);
  fFragElossPara[make_pair(make_pair(31,11),"Emp")].push_back(0.591513);
  fFragElossPara[make_pair(make_pair(31,11),"Emp")].push_back(0.00206287);
  fFragElossPara[make_pair(make_pair(31,11),"Emp")].push_back(-3.1157e-06);
  fFragElossPara[make_pair(make_pair(32,11),"Emp")].push_back(8.24273);
  fFragElossPara[make_pair(make_pair(32,11),"Emp")].push_back(0.590265);
  fFragElossPara[make_pair(make_pair(32,11),"Emp")].push_back(0.00205267);
  fFragElossPara[make_pair(make_pair(32,11),"Emp")].push_back(-3.08906e-06);
  fFragElossPara[make_pair(make_pair(33,11),"Emp")].push_back(9.17737);
  fFragElossPara[make_pair(make_pair(33,11),"Emp")].push_back(0.588964);
  fFragElossPara[make_pair(make_pair(33,11),"Emp")].push_back(0.00204318);
  fFragElossPara[make_pair(make_pair(33,11),"Emp")].push_back(-3.06209e-06);
  fFragElossPara[make_pair(make_pair(34,11),"Emp")].push_back(10.004);
  fFragElossPara[make_pair(make_pair(34,11),"Emp")].push_back(0.58806);
  fFragElossPara[make_pair(make_pair(34,11),"Emp")].push_back(0.00203551);
  fFragElossPara[make_pair(make_pair(34,11),"Emp")].push_back(-3.04317e-06);


}
