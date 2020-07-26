
#ifndef __TOP_TREE_NTEVENT_H__
#define __TOP_TREE_NTEVENT_H__

#include <iostream>
#include <string>
#include <vector>


#include "NTJet.h"
#include "NTElectron.h"
#include "NTPhoton.h"
#include "NTMuon.h"
#include "NTTau.h"
#include "NTMET.h"
#include "NTJetMet.h"
#include "NTVertex.h"
#include "NTTrack.h"
#include "NTCollection.h"
#include "WDecaysMC.h"
#include "ZDecaysMC.h"
#include "topDecaysMC.h"
#include "NTPU.h"

using namespace std;

/** NTEvent: main class stored in the NTuple. \\
Selected objects [leptons, Jet/MET collections, tracks, PV, BS], triggers, event info, basic MC info are stored.
Useless information might be deleted to keep the size of the NTuple reasonnable.\\
Addtional information could be found in the MiniTree.\\
*/


namespace TopTree
{
  class NTEvent
  {
  public:
    //methods
    NTEvent ();
    ~NTEvent ();
    void Reset ();
    void Print (std::ostream & os = std::cout);
    void PrintTriggerPassed (std::ostream & os = std::cout);    /** print the list of triggers passed */
    void PrintTriggerList (std::ostream & os);                   /** print the list of all triggers registered */
    bool TriggerPassed(std::string trigName);                   /** return true if passes trigger "trigName"  */
    bool TriggerStored(std::string trigName);                   /** return true if trigger "trigName" is stored  */
    
    // methods to instanciate a new object
    NTElectron *NewElectron ();
    NTPhoton *NewPhoton ();
    NTMuon *NewMuon ();
    TopTree::NTCollection<NTTau> *NewTauColl ();
    NTVertex *NewVertex ();
    NTJetMet *NewJetMet ();
    NTTrack *NewTrack ();
    NTPU *NewPU ();
    //using MTObjects
    void NewElectron (const NTElectron& electron);
    void NewPhoton (const NTPhoton& photon);
    void NewMuon (const NTMuon& muon);
    void NewTauColl (const TopTree::NTCollection<NTTau>& tau);
    void NewVertex (const NTVertex& vertex);
    void NewTrack (const NTTrack& track);
    void NewJetMet(const NTJetMet& jetmet);
    void NewPU (const NTPU& pileup);


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
    float match_HLT_Ele10_LW_L1R_recoEl;
    vector<std::pair<string, int> > prescales;
    
    std::vector < TLorentzVector > tauObjTrig;
    std::vector < TLorentzVector > jetObjTrig;
   
    // ---- Primary Vertices 
    std::vector < NTVertex > VertexVec;

    // ---- Beam Spot 
    TVector3 p3BS;
    TVector3 p3BSErr;
  
    // ---- PU and UN densities
    double rho_PUUE_dens;
    double neutralRho_PUUE_dens;
    int num_pileup_bc0;     // the number of pileup interactions of Bunch Crossing  0, that have been added to the event
    int num_pileup_bcp1;    // the number of pileup interactions of Bunch Crossing p1, that have been added to the event
    int num_pileup_bcm1;    // the number of pileup interactions of Bunch Crossing m1, that have been added to the event
    std::vector < NTPU > pileup;    // dedicated PU information

    
    //Only selected objects are stored below

    // --- JetMet  
    std::vector < NTJetMet > jetMetVec;

    // ---- leptons
    std::vector < NTElectron > electrons;
    std::vector < NTMuon > muons;
    std::vector < TopTree::NTCollection <NTTau> > tausVec;
    
    // ---- photons
    std::vector < NTPhoton > photons;   

    // ---- tracks
    std::vector < NTTrack > tracks;
    
    // --- generated taus
    std::vector < TLorentzVector> Generatedtaus;
    std::vector < TLorentzVector> GeneratedAtaus;
    
    std::vector < TLorentzVector > genBquarks;
    std::vector < TLorentzVector > genCquarks;
    std::vector < TLorentzVector > genLquarks;
    std::vector < TLorentzVector > genGquarks;
    
    std::vector<WDecaysMC> wAndDecays;
    std::vector<ZDecaysMC> zAndDecays;
    std::vector<TopDecaysMC> topAndDecays;
    
    
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

