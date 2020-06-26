
#ifndef __TOP_TREE_NTJetMet_H__
#define __TOP_TREE_NTJetMet_H__

#include <string>

#include "../interface/NTMET.h"
#include "../interface/NTJet.h"
#include "../interface/NTCollection.h"

/** NTJetMet: class jet/met for NTuple. \\
Contains for a given algorithm the jet collection & the associated MET.
*/

namespace TopTree
{
  class NTJetMet:public NTCollection<NTJet>
  {
  public:
    //methods
    NTJetMet ();
    ~NTJetMet ();
    void Reset ();
    
    //data members
    TopTree::NTMET met;
    double sumEtJet;
    double sumEtJetRaw;
    

  };
}
#endif
