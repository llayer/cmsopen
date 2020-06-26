#ifndef __TOP_TREE_MTLEPTON_H__
#define __TOP_TREE_MTLEPTON_H__


#include <map>
#include <vector>
#include <string>
#include "TLorentzVector.h"
#include "TVector3.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

/** This is an extension of NTLepton which contains additionnaly
  - MC information
  - Vertex information
MTLepton doesn't inherit from NTLepton.
*/

using namespace std;

namespace TopTree
{
  class MTLepton
  {
  public:

    //methods
    MTLepton ();
    ~MTLepton ();
    void Reset ();
    void PrintAdditionalInfo(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }

    //data members
    TLorentzVector p4Gen;

    //variable for ID
    double DB;                  // IP wrt the beamline (?)

    // MC information 
    reco::GenParticle GenMother;
    reco::GenParticle GenGrandMother;
    reco::GenParticle GenGGrandMother;

  };
}
#endif
