
#ifndef __TOP_TREE_NTTRACK_H__
#define __TOP_TREE_NTTRACK_H__

#include <iostream>
#include "TLorentzVector.h"

using namespace std;

/** NTTrack: class track for NTuple. \\
*/

namespace TopTree
{
  class NTTrack
  {
  public:
    //methods
    NTTrack ();
    ~NTTrack ();
    void Reset ();
    void Dump(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }
    NTTrack GetNTTrack(){ return *this;};
    NTTrack* GetNTTrackPointer(){ return this;};

    //data members
    TLorentzVector p4;
    int charge;
    double Chi2;
    double d0;
    double d0Error;
    double dxy;
    double dxy_BS;
    double dxyError;
    double dz;
    double dzError;
  
  };
}
#endif

