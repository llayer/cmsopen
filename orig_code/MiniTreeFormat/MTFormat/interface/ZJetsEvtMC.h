#ifndef __TOP_TREE_Z_JETSEVTMC_H__
#define __TOP_TREE_Z_JETSEVTMC_H__


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <vector>
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace TopTree
{

  class ZJetsEvtMC
  {

  public:

    //methods
    ZJetsEvtMC ();
    ~ZJetsEvtMC ();
    void Reset ();
    void GetZJetPartonTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose);

    //data members
    TLorentzVector p4_Z_gen;
    TLorentzVector p4_Lep1_gen;
    TLorentzVector p4_Lep2_gen;
    TLorentzVector p4_SumTauNeu_gen;
    TLorentzVector p4_SumTauANeu_gen;
    double Q_Lep1_gen;
    double Q_Lep2_gen;
    int Lep1_pdgID;
    int Lep2_pdgID;
    int Tmeme;
    int Zgtobb;

    std::vector < TLorentzVector > p4_Jets;

  };
}

#endif                          //__TOP_TREE_Z_JETSEVTMC_H__

