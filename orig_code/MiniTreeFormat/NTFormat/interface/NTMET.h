#ifndef __TOP_TREE_NTMET_H__
#define __TOP_TREE_NTMET_H__


#include <iostream>
#include "TLorentzVector.h"

/** NTMET: class MET for NTuple. \\
*/

using namespace std;

namespace TopTree
{
  class NTMET
  {
  public:

    //methods
    NTMET ();
    ~NTMET ();
    void Reset ();
    void Dump(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }
    NTMET GetNTMET(){ return *this;};
    NTMET* GetNTMETPointer(){ return this;};

    //data members
    TLorentzVector p4;          /** missing Et vector -->> 4D vector since no 2D object available in [root/5.14.00f-CMS3q] ... */
    double correction;
    
    //turn-on weight
    double turnOnWeight;
    
  };
}
#endif
