#ifndef __TOP_TREE_NTVERTEX_H__
#define __TOP_TREE_NTVERTEX_H__

#include <iostream>
#include "TLorentzVector.h"
#include "TVector3.h"

/** NTVertex: class vertex for NTuple. \\
*/

using namespace std;

namespace TopTree
{
  class NTVertex
  {
  public:
    //methods
    NTVertex ();
    ~NTVertex ();
    void Reset ();
    void Dump(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }
    void PrintP3(TVector3 a, std::ostream & os = std::cout){
        os << " p3 = ("<< a.X() << "," << a.Y() << "," << a.Z() << ") ";
    }
    NTVertex GetNTVertex(){ return *this;};
    NTVertex* GetNTVertexPointer(){ return this;};

    //data members
    TVector3 p3;/**X-Y-Z coordinates */

    double chi2;
    int isFake;
    int isValid;
    double ndof;
    double rho; 

    double normalizedChi2;
    int NumberOfTracks;

  };
}
#endif
