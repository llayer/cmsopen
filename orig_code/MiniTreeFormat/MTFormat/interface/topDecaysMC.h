#ifndef __TOP_TREE_TOPDECAY_EVTMC_H__
#define __TOP_TREE_TOPDECAY_EVTMC_H__

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <vector>
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace TopTree
{

  class TopDecaysMC
  {

  public:

    //methods
    TopDecaysMC (){};
    ~TopDecaysMC (){};
    //data members
    
    
    double Q_gen;
    TLorentzVector p4_b_gen;
    TLorentzVector p4_t_gen;
    
    int mcLepId;

  };
}




#endif                          //__TOP_TREE_TOPDECAY_PARTON_EVT_H__
