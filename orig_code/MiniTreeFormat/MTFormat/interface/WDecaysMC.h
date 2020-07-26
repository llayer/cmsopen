#ifndef __TOP_TREE_WDECAY_EVTMC_H__
#define __TOP_TREE_WDECAY_EVTMC_H__

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <vector>
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace TopTree
{

  class WDecaysMC
  {

  public:

    //methods
    WDecaysMC (){};
    ~WDecaysMC (){};
    //void Reset ();
    //void GetWJetPartonTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose);

    //data members
    TLorentzVector p4_W_gen;
    TLorentzVector p4_Lep_gen;
    double Q_Lep_gen;

    TLorentzVector p4_Neu_gen;

    TLorentzVector p4_SumTauNeu_gen;
    
    
    
    

    int Tmeme;
    int mcLepId;

  };
}




#endif                          //__TOP_TREE_WDECAY_PARTON_EVT_H__
