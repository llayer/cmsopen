#ifndef __MiniTreeProducer_H__
#define __MiniTreeProducer_H__
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

// -*- C++ -*-
//
// Package:    MiniTreeProducer
// Class:      MiniTreeProducer
// 
/**\class MiniTreeProducer MiniTreeProducer.cc TMP/MiniTreeProducer/src/MiniTreeProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Jeremy ANDREA
//         Created:  Sun Nov  1 21:30:20 CET 2009
// $Id: MiniTreeProducer.h,v 1.14 2011/12/09 14:10:57 jandrea Exp $
//
//

//----------------- system include files
#include <memory>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

//----------------- cmssw includes

#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/Run.h>

#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "FWCore/ServiceRegistry/interface/Service.h" 

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "JetMETCorrections/JetVertexAssociation/interface/JetVertexMain.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"


#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"



//--------------------PAT includes
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"

#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HepMCCandidate/interface/PdfInfo.h"


//--------------------ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TRandom.h"

//--------------------CLASS includes
#include "MiniTreeFormat/MTFormat/interface/MTEvent.h"
#include "MiniTreeFormat/NTFormat/interface/NTSampleInfo.h"


using namespace std;
using namespace edm;
using namespace reco;

//MiniTree classes
typedef TopTree::MTEvent        TopTreeEvent;
typedef TopTree::MTJet          TopTreeJet;
typedef TopTree::MTMET          TopTreeMET;
typedef TopTree::MTJetMet       TopTreeJetMet;
typedef TopTree::MTElectron     TopTreeElectron;
typedef TopTree::MTPhoton       TopTreePhoton;
typedef TopTree::MTMuon         TopTreeMuon;
typedef TopTree::MTTau          TopTreeTau;
typedef TopTree::MTTrack        TopTreeTrack;
typedef TopTree::MTVertex       TopTreePVx;
typedef TopTree::MTVertex       TopTreeBS;

typedef TopTree::NTSampleInfo   TopTreeSampleInfo;



// LHAPDF stuff
extern "C" {
  void initpdfset_ (char *, int len);
  void initpdfsetm_(int &, char *);
  void initpdf_(int &);
  void evolvepdf_(double &, double &, double *);
  void numberpdf_(int &);
}
  

class MiniTreeProducer : public edm::EDProducer {
 public:
  explicit MiniTreeProducer(const edm::ParameterSet&);
  ~MiniTreeProducer();
  
  
 private:
  virtual void  beginJob() ;
  virtual void  produce(edm::Event&, const edm::EventSetup&);
  virtual void  endJob() ;
  bool          MyJetCleaner(const vector<pat::Electron>*, pat::Jet);
  void          EventInfo();  
  int           getMuonOrigin(edm::Handle<reco::GenParticleCollection> , const pat::Muon * , reco::GenParticle&, reco::GenParticle&, reco::GenParticle&);
  int           getElectronOrigin(edm::Handle<reco::GenParticleCollection> particles, const pat::Electron * thePatElec,reco::GenParticle&, reco::GenParticle&, reco::GenParticle&);
  int           getTauDecay(edm::Handle<reco::GenParticleCollection> particles, const pat::Tau * thePatTau);
  void          setGenParticleFromCandidate(reco::GenParticle&, const reco::Candidate *);
  
  // ----------member data ---------------------------
 
  int           verbose;
 
  InputTag      TriggerMenu;
  bool          TriggerInfo;
  bool          saveAllTriggers;
  std::vector<std::string> triggerList;
  std::vector<std::string> prescaleList;
  std::string   hltJetsMatcher;
  std::string   hltTausMatcher;
  std::string   hltJetsMatcher45;
  std::string   hltTausMatcher45;

  InputTag      electronProducer;
  bool          doElectrons;
  double        electron_cut_pt;
  double        electron_cut_eta;
  
  InputTag      photonProducer;
  bool          doPhotons;
  double        photon_cut_pt;
  double        photon_cut_eta;
  
  InputTag      muonProducer;
  bool          doMuons;
  double        muon_cut_pt;
  double        muon_cut_eta;
  
  std::vector<std::string> tauProducer;
  bool          doTaus;
  double        tau_cut_pt;
  double        tau_cut_eta;
  
  InputTag      trackProducer; 
  bool          doTracks;
  double        track_cut_pt;
  double        track_cut_eta;

  InputTag      beamSpotProducer; 
  InputTag      vertexProducer;
  bool          saveAllVertex;
  InputTag      rho_PUUE_dens;
  InputTag      neutralRho_PUUE_dens;
  typedef std::vector<edm::ParameterSet> VParameters;
  VParameters jetmetProducer;
  double        jet_cut_pt;
  double        jet_cut_eta;
  
  JetVertexMain* vertexAlgo;
  string        cftbjetstring;
 
  bool          doGenParticleCollection;
 
  
  //Pointers to TopTree objects
  TopTreeSampleInfo* info;
 
  JetIDSelectionFunctor   *jetSel_;
  PFJetIDSelectionFunctor *pfJetSel_;
  bool          isAOD; 
  bool          isData; 
  
  bool          readPDFWeights; 
  
  
  
  double xfx(const double &x, const double &Q, int fl) {
    double f[13], mx = x, mQ = Q;
    evolvepdf_(mx, mQ, f);
    return f[fl+6];
  }

  
  
  
  
  
  
  
  
  
  

};

#endif

