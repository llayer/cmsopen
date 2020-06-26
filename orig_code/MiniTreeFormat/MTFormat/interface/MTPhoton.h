#ifndef __TOP_TREE_MTPhoton_H__
#define __TOP_TREE_MTPhoton_H__


#include <map>
#include <vector>
#include <string>
#include "TLorentzVector.h"
#include "TVector3.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "MiniTreeFormat/NTFormat/interface/NTPhoton.h"

/** This is an extension of NTPhoton which contains additionnaly
  - MC information
  - Vertex information
MTPhoton doesn't inherit from NTPhoton.
*/

using namespace std;

namespace TopTree
{
  class MTPhoton:public NTPhoton
  {
  public:

    //methods
    MTPhoton ();
    ~MTPhoton ();
    void Reset ();
    void PrintAdditionalInfo(std::ostream & os = std::cout);
    void PrintP4(TLorentzVector a, std::ostream & os = std::cout){
        os << " p4 = ("<< a.Px() << "," << a.Py() << "," << a.Pz() << "," << a.E() << ") & pT = " << a.Pt() << " ";
    }

   

  };
}
#endif
