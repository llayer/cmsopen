#ifndef __TOP_TREE_MTJetMet_H__
#define __TOP_TREE_MTJetMet_H__

#include <vector>
#include <string>

#include "MiniTreeFormat/MTFormat/interface/MTMET.h"
#include "MiniTreeFormat/MTFormat/interface/MTJet.h"
#include "MiniTreeFormat/MTFormat/interface/MTCollection.h"

/** MTJetMet: class jet/met for MiniTree. \\
Contains for a given algorithm the jet collection & the associated MET.
Does not inherit from NTJetMet. \\
*/

namespace TopTree
{
  class MTJetMet : public MTCollection<MTJet>
  {
  public:
    //methods
    MTJetMet ();
    ~MTJetMet ();
    void Reset ();
   
    //data members
    TopTree::MTMET met;
    double sumEtJet;
    double sumEtJetRaw;

  };
}
#endif
