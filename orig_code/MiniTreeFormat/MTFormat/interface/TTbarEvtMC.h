#ifndef __TOP_TREE_TTbarEvtMC_H__
#define __TOP_TREE_TTbarEvtMC_H__


#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <iostream>
#include <iomanip>
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace reco;

namespace TopTree
{
  class TTbarEvtMC
  {
  public:
    //methods 
    TTbarEvtMC ();
    ~TTbarEvtMC ();
    void Reset ();
    void GetTTbarTruth (edm::Handle < reco::GenParticleCollection > genParticles, bool verbose = false);

    //data members 
    TLorentzVector p4Parton1_gen;
    TLorentzVector p4Parton2_gen;

    TLorentzVector p4_Top_gen;
    TLorentzVector p4_ATop_gen;

    TLorentzVector p4_Lep_gen;
    TLorentzVector p4_ALep_gen;

    TLorentzVector p4_Neu_gen;
    TLorentzVector p4_ANeu_gen;

    TLorentzVector p4_Wminus_gen;
    TLorentzVector p4_Wplus_gen;

    TLorentzVector p4_B_gen;
    TLorentzVector p4_Bbar_gen;

    TLorentzVector p4_BRad_gen; // Radiative B coing from previous B (B->B+Xgluons)
    TLorentzVector p4_BbarRad_gen;

    TLorentzVector p4_SumTauNeu_gen;
    TLorentzVector p4_SumTauANeu_gen;

    double QLep_gen;            // charge of the lepton
    double QALep_gen;           // charge of the antilepton

    int mc_LepID;               // +-11,+-13,+-15 
    int mc_ALepID;              // +-11,+-13,+-15 

    int Tmeme;                  //  T = # of taus from W decays
    //  M = # of muons from W->tau->muon
    //  E = # of electrons from W->tau->electron
    //  M = # of muons from W->muon
    //  E = # of electrons from W->electron

      std::string TTbarDecayType;       // Dimuon,Dielectron,Dielecmuon,Singlemuon,Singleelectron,Singletauhad,Ditauhad,Had             


    int ProdProc;               // =1 for q + qbar -> gluon
    // =2 for gluon + gluon -> gluon



  };
}
#endif

