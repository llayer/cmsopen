#ifndef __TOP_TREE_MTEVEMT_H__
#define __TOP_TREE_MTEVEMT_H__

#include <string>
#include <vector>
#include <map>


#include "MTJet.h"
#include "MTElectron.h"
#include "MTPhoton.h"
#include "MTMuon.h"
#include "MTTau.h"
#include "MTMET.h"
#include "MTJetMet.h"
#include "MTVertex.h"
#include "MTTrack.h"
#include "MTCollection.h"
//MC classes
#include "TTbarEvtMC.h"
#include "ZJetsEvtMC.h"
#include "WJetsEvtMC.h"
#include "WWEvtMC.h"
#include "../../NTFormat/interface/WDecaysMC.h"
#include "../../NTFormat/interface/ZDecaysMC.h"
#include "../../NTFormat/interface/topDecaysMC.h"
#include "MTPU.h"

using namespace std;

/** MTEvent: main class stored in the MiniTree. \\
All main objects [leptons, Jet/MET collections, tracks, PV, BS], triggers, event info, detailled MC info are stored.
*/


namespace TopTree
{
  class MTEvent
  {
  public:
    //methods
    MTEvent ();
    ~MTEvent ();
    void Reset ();
    void Print (std::ostream & os = std::cout);
    void PrintTriggerPassed (std::ostream & os = std::cout);    /** print the list of triggers passed */
    void PrintTriggerList (std::ostream & os);                   /** print the list of all triggers registered */
    bool TriggerPassed(std::string trigName);                   /** return true if passes trigger "trigName"  */
    bool TriggerStored(std::string trigName);                   /** return true if trigger "trigName" is stored  */
    // methods to instanciate a new object
    MTElectron *NewElectron ();
    MTPhoton   *NewPhoton ();
    MTMuon     *NewMuon ();
    MTCollection<MTTau> *NewTauColl ();
    MTVertex   *NewVertex ();
    MTJetMet   *NewJetMet ();
    MTTrack    *NewTrack ();
    /*
    TTbarEvtMC *NewTTbarEvtMC ();
    WJetsEvtMC *NewWJetsEvtMC ();
    ZJetsEvtMC *NewZJetsEvtMC ();
    WWEvtMC *NewWWEvtMC ();
    */
    MTPU    *NewPU ();

    //data members

    // ---- General event information.
    unsigned int runNb;
    unsigned int eventNb;
    double eventWeight;
    // ---- Luminosity block
    int lumiblock;

    // ---- Monte Carlo information
    int procId;
    std::string eventType;
    int TMEME;/**
    //  T = # of taus from W decays     \\
    //  M = # of muons from W->tau->muon        \\
    //  E = # of electrons from W->tau->electron        \\
    //  M = # of muons from W->muon     \\
    //  E = # of electrons from W->electron     \\
    */
    int ptHat;
    pair<int,int> pdgIdPartons;/** pdgId of the initial partons */
    
    // ---- Trigger info 
    vector<std::pair<string, bool> > triggers;
    vector<std::pair<string, int > > prescales;
    float match_HLT_Ele10_LW_L1R_recoEl;
    
    std::vector < TLorentzVector > tauObjTrig;
    std::vector < TLorentzVector > jetObjTrig;

    // ---- Primary Vertices 
    std::vector < MTVertex > VertexVec;

    // ---- Beam Spot 
    TVector3 p3BS;
    TVector3 p3BSErr;

    // ---- PU and UN densities
    double rho_PUUE_dens;
    double neutralRho_PUUE_dens;

    int num_pileup_bc0;     // the number of pileup interactions of Bunch Crossing 0, that have been added to the event
    int num_pileup_bcp1;     // the number of pileup interactions of Bunch Crossing +1, that have been added to the event
    int num_pileup_bcm1;     // the number of pileup interactions of Bunch Crossing -1, that have been added to the event
    std::vector < MTPU > pileup;    // dedicated PU information
   
    
    // --- JetMet  
    std::vector < MTJetMet > jetMetVec;

    // ---- leptons
    std::vector < MTElectron > electrons;
    std::vector < MTMuon > muons;
    std::vector < MTCollection<MTTau> > tausVec;
    
    // ---- photons
    std::vector < MTPhoton > photons;   

    // ---- tracks
    std::vector < MTTrack > tracks;

    //Skimmed GenParticleCollection+ daughters' and mothers' lists 
    reco::GenParticleCollection GPC;
    std::map < int, std::vector < int > >GPC_list_index_dau;
    std::map < int, std::vector < int > >GPC_list_index_mot;
    
    
    std::vector<WDecaysMC> wAndDecays;
    std::vector<ZDecaysMC> zAndDecays;
    std::vector<TopDecaysMC> topAndDecays;
    
    std::vector < TLorentzVector > Generatedtaus;
    std::vector < TLorentzVector > GeneratedAtaus;
    
    std::vector < TLorentzVector > genBquarks;
    std::vector < TLorentzVector > genCquarks;
    std::vector < TLorentzVector > genLquarks;
    std::vector < TLorentzVector > genGquarks;
     
    void fillGenInfo(edm::Handle < reco::GenParticleCollection > genParticles);
    
    
    // ---- PDF weights
    std::vector < float > pdfWeights;
    
    
    int partonFlavor1;
    int partonFlavor2;
    
    float x1;
    float x2;
    
    float Q_scale;  
      
  };
}
#endif
