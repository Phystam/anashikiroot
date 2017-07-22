#include "TArtEasyMassExcess.hh"
#include <map>
#include <utility>
#include <TObject.h>
using namespace std;

TArtEasyMassExcess::TArtEasyMassExcess(){
  //for F isotopes
  fMassExcess[make_pair(26,9)]=18.65;
  fMassExcess[make_pair(27,9)]=25.45;
  fMassExcess[make_pair(29,9)]=40.15;
  fMassExcess[make_pair(31,9)]=56.14;

  //for Ne isotopes
  fMassExcess[make_pair(26,10)]=0.48;
  fMassExcess[make_pair(27,10)]=7.05;
  fMassExcess[make_pair(28,10)]=11.30;
  fMassExcess[make_pair(29,10)]=18.40;
  fMassExcess[make_pair(30,10)]=23.28;
  fMassExcess[make_pair(31,10)]=31.18;
  fMassExcess[make_pair(32,10)]=37;

  //for Na isotopes
  fMassExcess[make_pair(26,11)]=-6.86;
  fMassExcess[make_pair(27,11)]=-5.52;
  fMassExcess[make_pair(28,11)]=-0.99;
  fMassExcess[make_pair(29,11)]=2.68;
  fMassExcess[make_pair(30,11)]=8.47;
  fMassExcess[make_pair(31,11)]=12.25;
  fMassExcess[make_pair(32,11)]=18.64;
  fMassExcess[make_pair(33,11)]=23.78;
  fMassExcess[make_pair(34,11)]=31.68;

}
