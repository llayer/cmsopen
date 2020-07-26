#ifndef __TOP_TREE_WW_EVTMC_H__
#define __TOP_TREE_WW_EVTMC_H__

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <vector>
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace TopTree
{

  class WWEvtMC
  {

  public:

    //methods
    WWEvtMC ();
    ~WWEvtMC ();
    void Reset ();
    void GetWWTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose);

    //data members  
    TLorentzVector p4_Lep_gen;
    TLorentzVector p4_ALep_gen;

    double Q_Lep_gen;
    double Q_ALep_gen;

    TLorentzVector p4_Neu_gen;
    TLorentzVector p4_ANeu_gen;

    TLorentzVector p4_Vminus_gen;
    TLorentzVector p4_Vplus_gen;

    TLorentzVector p4_SumTauNeu_gen;
    TLorentzVector p4_SumTauANeu_gen;

    int Tmeme;

  };
}




#endif                          //__TOP_TREE_WW_PARTON_EVT_H__
