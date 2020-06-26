// system include files
#include <memory>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <TH1F.h>

//----------------- cmssw includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/Run.h>
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "JetMETCorrections/JetVertexAssociation/interface/JetVertexMain.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


//--------------------PAT includes
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

//--------------------ROOT includes
#include "TTree.h"

//--------------------TopTree includes
#include "MiniTreeFormat/NTFormat/interface/NTEvent.h"
#include "MiniTreeFormat/MTFormat/interface/MTEvent.h"
#include "MiniTreeFormat/NTFormat/interface/NTSampleInfo.h"

#include <vector>

using namespace std;
using namespace TopTree;

// Class declaration ----------------------------------

class NTupleProducer:public edm::EDAnalyzer
{
public:
  explicit NTupleProducer (const edm::ParameterSet &);
   ~NTupleProducer ();

private:
  virtual void beginJob ();
  virtual void analyze (const edm::Event &, const edm::EventSetup &);
  virtual void endJob ();

  int GetNumberOfIDMuons(TopTree::NTEvent     event, double muon_cut_pt_skim, double muon_cut_iso_skim);
  int GetNumberOfMuons(TopTree::NTEvent     event, double muon_cut_pt_skim, double muon_cut_iso_skim);
  int GetNumberOfIDElectrons(TopTree::NTEvent event, double electron_cut_pt_skim, double electron_cut_iso_skim);
  int GetNumberOfElectrons(TopTree::NTEvent event, double electron_cut_pt_skim, double electron_cut_iso_skim);
  int GetNumberOfTaus(TopTree::NTEvent event, double tau_cut_pt_skim, std::string algo);
  int GetNumberOfJets(TopTree::NTEvent event, double jet_cut_pt_skim, double jet_cut_eta_skim, std::string algo);
  
  TTree *TopDilep;
  TopTree::NTEvent * ntevt;
  edm::Service < TFileService > fs;

  int verbose;
  bool saveAllTriggers;
  vector<string> saveTriggersWith;
  std::vector < std::string > triggerList;

  bool doElectrons;
  double electron_cut_pt;
  double electron_cut_eta;

  bool doPhotons;
  double photon_cut_pt;
  double photon_cut_eta;

  bool doMuons;
  double muon_cut_pt;
  double muon_cut_eta;

  bool doTaus;
  vector < string > tauAlgos;
  double tau_cut_pt;
  double tau_cut_eta;

  bool doTracks;
  double track_cut_pt;
  double track_cut_eta;

  bool doJets;
  vector < string > jetAlgos;
  double jet_cut_pt;
  double jet_cut_eta;

  bool saveAllVertex;
  bool saveMCInfo;
 
  //Skimming 
  int numberOfLept4Skim;

  int numberOfMuon4Skim;
  bool useMuonIdSkim;
  double muon_cut_pt_skim;
  double muon_cut_iso_skim;

  int numberOfElec4Skim;
  bool useElectronIdSkim;
  double electron_cut_pt_skim;
  double electron_cut_iso_skim;
  
  bool doTMEMESkimming;
  vector<int> TMEMESkimList; 
  bool doMCDiLepSkimming;
  vector<string> MCDiLepList; 

  bool doTriggerSkimming;
  std::vector < std::string > triggerSkimList;
 
  bool doTauSkimming;
  int numberOfTau4Skim;
  double tau_cut_pt_skim;
  std::string tau_algo_skim;
  
  bool doJetSkimming;
  int numberOfJet4Skim;
  double jet_cut_pt_skim; 
  double jet_cut_eta_skim;
  std::string jet_algo_skim;
 
  bool doTauJetAna_TmemeTauJetSkimming;// for MC ttbar only
  bool doTauJetAna_TauJetSkimming;
  bool doTauJetAna_TauJetTrigSkimming;   
  
  TH1F * theNormHisto;
  TH1F * theNormHistoByTMEME;
  
};


// constructors and destructor
NTupleProducer::NTupleProducer (const edm::ParameterSet & iConfig)
{
  cout << "Constructor of NTupleProducer" << endl;
  theNormHisto = fs->make < TH1F >  ("theNormHisto", "theNormHisto", 1, 0.5, 1.5);
  theNormHistoByTMEME = fs->make < TH1F >  ("theNormHistoByTMEME", "theNormHistoByTMEME", 22223, -0.5, 22222.5);
  TopDilep = fs->make < TTree > ("Event", "");
  ntevt = new TopTree::NTEvent();
  TopDilep->Branch ("NTEvent", "TopTree::NTEvent", &ntevt, 32000, 3);// compress= 3 seems to be the optimal !

  //Read configuration file 
  verbose         = iConfig.getParameter < int >("verbose");
  saveAllTriggers = iConfig.getParameter < bool > ("saveAllTriggers");
  saveTriggersWith    = iConfig.getParameter < vector < string > >("saveTriggersWith");
  triggerList     = iConfig.getParameter < vector < string > >("triggerList");

  doElectrons      = iConfig.getParameter < bool > ("doElectrons");
  electron_cut_pt  = iConfig.getParameter < double >("electron_cut_pt");
  electron_cut_eta = iConfig.getParameter < double >("electron_cut_eta");

  doPhotons      = iConfig.getParameter < bool > ("doPhotons");
  photon_cut_pt  = iConfig.getParameter < double >("photon_cut_pt");
  photon_cut_eta = iConfig.getParameter < double >("photon_cut_eta");

  doMuons      = iConfig.getParameter < bool > ("doMuons");
  muon_cut_pt  = iConfig.getParameter < double >("muon_cut_pt");
  muon_cut_eta = iConfig.getParameter < double >("muon_cut_eta");

  doTaus      = iConfig.getParameter < bool > ("doTaus");
  tauAlgos    = iConfig.getParameter < vector < string > >("tauAlgos");
  tau_cut_pt  = iConfig.getParameter < double >("tau_cut_pt");
  tau_cut_eta = iConfig.getParameter < double >("tau_cut_eta");

  doTracks      = iConfig.getParameter < bool > ("doTracks");
  track_cut_pt  = iConfig.getParameter < double >("track_cut_pt");
  track_cut_eta = iConfig.getParameter < double >("track_cut_eta");

  doJets      = iConfig.getParameter < bool > ("doJets");
  jetAlgos    = iConfig.getParameter < vector < string > >("jetAlgos");
  jet_cut_pt  = iConfig.getParameter < double >("jet_cut_pt");
  jet_cut_eta = iConfig.getParameter < double >("jet_cut_eta");

  saveAllVertex = iConfig.getParameter < bool > ("saveAllVertex");
  saveMCInfo = iConfig.getParameter < bool > ("saveMCInfo");
  
  //Skimming 
 
  numberOfLept4Skim = iConfig.getParameter < int >("numberOfLept4Skim");
  numberOfMuon4Skim = iConfig.getParameter < int >("numberOfMuon4Skim");
  useMuonIdSkim = iConfig.getParameter < bool >("useMuonIdSkim");
  muon_cut_pt_skim = iConfig.getParameter < double >("muon_cut_pt_skim");
  muon_cut_iso_skim = iConfig.getParameter < double >("muon_cut_iso_skim");
  numberOfElec4Skim = iConfig.getParameter < int >("numberOfElec4Skim");
  useElectronIdSkim = iConfig.getParameter < bool >("useElectronIdSkim");
  electron_cut_pt_skim = iConfig.getParameter < double >("electron_cut_pt_skim");
  electron_cut_iso_skim = iConfig.getParameter < double >("electron_cut_iso_skim");
  
  doTMEMESkimming = iConfig.getParameter < bool >("doTMEMESkimming");
  TMEMESkimList = iConfig.getParameter < vector < int > >("TMEMESkimList"); 
  doMCDiLepSkimming = iConfig.getParameter < bool >("doMCDiLepSkimming");
  MCDiLepList = iConfig.getParameter < vector < string > >("MCDiLepList"); 
  
  doTriggerSkimming = iConfig.getParameter < bool >("doTriggerSkimming");
  triggerSkimList   = iConfig.getParameter < vector < string > >("triggerSkimList");
  
  doTauSkimming = iConfig.getParameter < bool >("doTauSkimming");
  numberOfTau4Skim = iConfig.getParameter < int >("numberOfTau4Skim");
  tau_cut_pt_skim = iConfig.getParameter < double >("tau_cut_pt_skim");
  tau_algo_skim = iConfig.getParameter < std::string >("tau_algo_skim");
  
  doJetSkimming = iConfig.getParameter < bool >("doJetSkimming");
  numberOfJet4Skim = iConfig.getParameter < int >("numberOfJet4Skim");
  jet_cut_pt_skim = iConfig.getParameter < double >("jet_cut_pt_skim"); 
  jet_cut_eta_skim = iConfig.getParameter < double >("jet_cut_eta_skim");
  jet_algo_skim = iConfig.getParameter < std::string >("jet_algo_skim");
   
  doTauJetAna_TmemeTauJetSkimming = iConfig.getParameter < bool >("doTauJetAna_TmemeTauJetSkimming");
  doTauJetAna_TauJetSkimming      = iConfig.getParameter < bool >("doTauJetAna_TauJetSkimming");
  doTauJetAna_TauJetTrigSkimming  = iConfig.getParameter < bool >("doTauJetAna_TauJetTrigSkimming");   
 
}

NTupleProducer::~NTupleProducer ()
{
  //delete the pointers
  TopDilep = 0;
  delete TopDilep;
  ntevt = 0;
  delete ntevt;
}


// ------------ method called to for each event  ------------
void NTupleProducer::analyze (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{

  using namespace edm;
  using namespace TopTree;
  using namespace std;


  edm::Handle < TopTree::MTEvent > topTreeEvt;
  iEvent.getByType (topTreeEvt);
  
  const TopTree::MTEvent * event = topTreeEvt.product ();
 
  ntevt->Reset();
  if (verbose > 0) {
    cout << " ----- New event  ---- " << endl;
    cout << "Evt nb: " << event->eventNb << endl;
  }
  
 
  // ---- Filling general event information.
  ntevt->runNb       = event->runNb;
  ntevt->eventNb     = event->eventNb;
  ntevt->eventWeight = event->eventWeight;
  ntevt->lumiblock   = event->lumiblock;
  ntevt->procId      = event->procId;
  ntevt->eventType   = event->eventType;
  ntevt->TMEME       = event->TMEME;
  ntevt->ptHat       = event->ptHat;
  ntevt->pdgIdPartons = event->pdgIdPartons;
  ntevt->match_HLT_Ele10_LW_L1R_recoEl = event->match_HLT_Ele10_LW_L1R_recoEl;
  ntevt->p3BS = event->p3BS;
  ntevt->p3BSErr = event->p3BSErr;
  ntevt->rho_PUUE_dens = event->rho_PUUE_dens;
  ntevt->num_pileup_bc0 = event->num_pileup_bc0;
  ntevt->num_pileup_bcp1 = event->num_pileup_bcp1;
  ntevt->num_pileup_bcm1 = event->num_pileup_bcm1;
  if (saveMCInfo){ 
        ntevt->genBquarks = event->genBquarks; 
        ntevt->genCquarks = event->genCquarks;
        ntevt->genLquarks = event->genLquarks;
        ntevt->genGquarks = event->genGquarks;
        ntevt->wAndDecays    = event->wAndDecays;
        ntevt->zAndDecays    = event->zAndDecays;
        ntevt->topAndDecays  = event->topAndDecays;
  }
  
  ntevt->partonFlavor1 = event->partonFlavor1;
  ntevt->partonFlavor2 = event->partonFlavor2;
  ntevt->x1            = event->x1;
  ntevt->x2            = event->x2;
  ntevt->Q_scale       = event->Q_scale;
  
  ntevt->pdfWeights       = event->pdfWeights;
  
  bool passTriggerSkim = false;
  
  if (saveAllTriggers){
    ntevt->triggers = event->triggers;
    if (doTriggerSkimming) 
      {
      for (unsigned int x = 0; x < event->triggers.size (); x++) 
      {
       for (unsigned int z = 0; z < triggerSkimList.size (); z++) {
         if (event->triggers[x].first.find(triggerSkimList[z])!=std::string::npos && event->triggers[x].second == true) passTriggerSkim = true;       
       }
      }
     }
  }
  else {
    for (unsigned int x = 0; x < event->triggers.size (); x++) {
      bool saved = false;
      for (unsigned int z = 0; z < triggerList.size (); z++) {
        if (event->triggers[x].first == triggerList[z]){
          ntevt->triggers.push_back (event->triggers[x]);
          saved = true;
        }
      }
      if (doTriggerSkimming) 
      {
       for (unsigned int z = 0; z < triggerSkimList.size (); z++) {
         if (event->triggers[x].first == triggerSkimList[z]) passTriggerSkim = true;
       }
      }
      for (unsigned int z = 0; z < saveTriggersWith.size(); z++){
        if(event->triggers[x].first.find(saveTriggersWith[z])< event->triggers[x].first.size() && !saved){
          ntevt->triggers.push_back (event->triggers[x]);
        }
      }
    }
  }
  

  // --- Filling prescales
  if(event->prescales.size()>0)
  ntevt->prescales = event->prescales;
  
  /*for (unsigned int x = 0; x < event->prescales.size (); x++){
       ntevt->prescales.push_back (event->prescales[x]);
  }*/
  
  // --- Filling electrons
  if (!doElectrons)
    if (verbose > 1)
      cout << "Do not fill electrons" << endl;
  if (doElectrons) {
    if (verbose > 1)
      cout << "Filling electrons" << endl;
    for (unsigned int i = 0; i < event->electrons.size (); i++) {
      if (event->electrons[i].p4.Pt () > electron_cut_pt && fabs (event->electrons[i].p4.Eta ()) < electron_cut_eta)
        ntevt->NewElectron (dynamic_cast < const TopTree::NTElectron & >(event->electrons[i]));
    }
  }
 
  // --- Filling Photons
  if (!doPhotons)
    if (verbose > 1)
      cout << "Do not fill Photons" << endl;
  if (doPhotons) {
    if (verbose > 1)
      cout << "Filling Photons" << endl;
    for (unsigned int i = 0; i < event->photons.size (); i++) {
      if (event->photons[i].p4.Pt () > photon_cut_pt && fabs (event->photons[i].p4.Eta ()) < photon_cut_eta)
        ntevt->NewPhoton (dynamic_cast < const TopTree::NTPhoton & >(event->photons[i]));
    }
  }
  // pileup
  for (unsigned int i = 0; i < event->pileup.size (); i++) {
        ntevt->NewPU (dynamic_cast < const TopTree::NTPU & >(event->pileup[i]));
  }


  // --- Filling muons
  if (!doMuons)
    if (verbose > 1)
      cout << "Do not fill muons" << endl;
  if (doMuons) {
    if (verbose > 1)
      cout << "Filling muons" << endl;
    for (unsigned int i = 0; i < event->muons.size (); i++) {
      if (event->muons[i].p4.Pt () > muon_cut_pt && fabs (event->muons[i].p4.Eta ()) < muon_cut_eta)
        ntevt->NewMuon (dynamic_cast < const TopTree::NTMuon & >(event->muons[i]));
    }
  }
 
  // --- Filling taus
  if (!doTaus)
    if (verbose > 1)
      cout << "Do not fill taus" << endl;
  if (doTaus) {
    if (verbose > 1)
      cout << "Filling taus" << endl;
      
   for (unsigned int i = 0; i < event->tausVec.size (); i++) {
    bool doIt = false;
    for (unsigned int x = 0; x < tauAlgos.size (); x++)
      if (tauAlgos[x] == event->tausVec[i].algo)
        doIt = true;
    if (!doIt)
      continue;
 
    NTCollection<TopTree::NTTau> tausVec;
    tausVec.algo = event->tausVec[i].algo;
    
    for (unsigned int j = 0; j < event->tausVec[i].objects.size (); j++) {
      if (event->tausVec[i].objects[j].p4.Pt () > tau_cut_pt && fabs (event->tausVec[i].objects[j].p4.Eta ()) < tau_cut_eta)
        tausVec.objects.push_back (dynamic_cast < const TopTree::NTTau & >(event->tausVec[i].objects[j]));
    }
    ntevt->NewTauColl(tausVec);
   }
  }
 
  // --- Filling JetMet
  for (unsigned int i = 0; i < event->jetMetVec.size (); i++) {
    bool doIt = false;
    for (unsigned int x = 0; x < jetAlgos.size (); x++)
      if (jetAlgos[x] == event->jetMetVec[i].algo)
        doIt = true;
    if (!doIt)
      continue;
    TopTree::NTJetMet jetmet;
    jetmet.algo = event->jetMetVec[i].algo;
    jetmet.sumEtJet = event->jetMetVec[i].sumEtJet;
    jetmet.sumEtJetRaw = event->jetMetVec[i].sumEtJetRaw;
    //MET
    jetmet.met = dynamic_cast < TopTree::NTMET & >(const_cast < TopTree::MTMET & >(event->jetMetVec[i].met));
    //Jets
    for (unsigned int j = 0; j < event->jetMetVec[i].objects.size (); j++) {
      if (event->jetMetVec[i].objects[j].p4.Pt () > jet_cut_pt && fabs (event->jetMetVec[i].objects[j].p4.Eta ()) < jet_cut_eta)
        jetmet.objects.push_back (dynamic_cast < TopTree::NTJet & >(const_cast < TopTree::MTJet & >(event->jetMetVec[i].objects[j])));
    }
    ntevt->NewJetMet (jetmet);
  }
 
  // --- Filling tracks
  if (!doTracks)
    if (verbose > 1)
      cout << "Do not fill tracks" << endl;
  if (doTracks) {
    if (verbose > 1)
      cout << "Filling tracks" << endl;
    for (unsigned int i = 0; i < event->tracks.size (); i++) {
      if (event->tracks[i].p4.Pt () > track_cut_pt && fabs (event->tracks[i].p4.Eta ()) < track_cut_eta)
        ntevt->NewTrack (dynamic_cast < const TopTree::NTTrack & >(event->tracks[i]));
    }
  }
 
  // --- Filling vertices
  for (unsigned int i = 0; i < event->VertexVec.size (); i++) {
    ntevt->NewVertex (dynamic_cast < const TopTree::NTVertex & >(event->VertexVec[i]));
    if (i == 0 && !saveAllVertex)
      break;
  }



  // ---  Filling generated taus
  ntevt->Generatedtaus  = event->Generatedtaus;
  ntevt->GeneratedAtaus = event->GeneratedAtaus;
  
  ntevt->tauObjTrig  = event->tauObjTrig;
  ntevt->jetObjTrig  = event->jetObjTrig;
  
  //apply skimming
  
  int nmuon = 0; 
  int nelec = 0; 
  
  int passSkim = false;
  int passSkimMuon = false;
  int passSkimElec = false;
  
  if(numberOfLept4Skim != -1){
    if(useMuonIdSkim) nmuon = GetNumberOfIDMuons(*ntevt, muon_cut_pt_skim, muon_cut_iso_skim);
    else nmuon = GetNumberOfMuons(*ntevt, muon_cut_pt_skim, muon_cut_iso_skim);
    if(useElectronIdSkim) nelec = GetNumberOfIDElectrons(*ntevt, electron_cut_pt_skim, electron_cut_iso_skim);
    else nelec = GetNumberOfElectrons(*ntevt, electron_cut_pt_skim, electron_cut_iso_skim);
    if(nmuon >= numberOfMuon4Skim) passSkimMuon = true;
    if(nelec >= numberOfElec4Skim) passSkimElec = true;
  }//else passSkim = true;
  

  int ntotlepton = nmuon+nelec;
  if(ntotlepton >= numberOfLept4Skim && passSkimMuon && passSkimElec )passSkim = true;
 
 
  int passTauSkim = false;
  if(doTauSkimming){
    int ntau = GetNumberOfTaus(*ntevt, tau_cut_pt_skim, tau_algo_skim);
    if(ntau >= numberOfTau4Skim) passTauSkim = true;
  }
  
  int passJetSkim = false;
  if(doJetSkimming){
    int njet = GetNumberOfJets(*ntevt, jet_cut_pt_skim, jet_cut_eta_skim, jet_algo_skim);
    if(njet >= numberOfJet4Skim) passJetSkim = true;
  }
  
  bool passTMEME = false;
  if(doTMEMESkimming){
        for(unsigned int x = 0; x<TMEMESkimList.size(); x++){
                if(ntevt->TMEME== TMEMESkimList[x])
                        passTMEME = true;
        } 
        //if(!passTMEME) passSkim = false;
  }

  bool passChannel = false;
  if(doMCDiLepSkimming){
        for(unsigned int x = 0 ; x<MCDiLepList.size(); x++){
                int TMEME = ntevt->TMEME;
                if(MCDiLepList[x] == string("ee") && (TMEME == 2  || TMEME == 10101 || TMEME == 20200 )) passChannel = true;
                if(MCDiLepList[x] == string("mumu") && (TMEME == 20 || TMEME == 11010 || TMEME == 22000 )) passChannel = true;
                if(MCDiLepList[x] == string("emu") && (TMEME == 11 || TMEME == 11001 || TMEME == 10110 || TMEME == 21100 )) passChannel = true;
                if(MCDiLepList[x] == string("tautau") && (TMEME == 20000 )) passChannel = true;
                if(MCDiLepList[x] == string("ljets") && (TMEME == 1 || TMEME == 10 || TMEME == 10000 || TMEME == 11000 || TMEME == 10100 )) passChannel = true;
                if(MCDiLepList[x] == string("had") && (TMEME == 0 )) passChannel = true;
        }
        //if(!passChannel) passSkim = false;
  }
  
  theNormHisto->Fill(1);
  theNormHistoByTMEME->Fill(event->TMEME);

  
  if ( (doTauJetAna_TmemeTauJetSkimming==false && doTauJetAna_TauJetSkimming==false && doTauJetAna_TauJetTrigSkimming==false ) && 
       ((numberOfLept4Skim == -1 && !doTMEMESkimming && !doMCDiLepSkimming && !doTriggerSkimming && !doTauSkimming && !doJetSkimming)||
        (numberOfLept4Skim != -1 && passSkim)|| 
        (doTMEMESkimming   && passTMEME)|| 
        (doMCDiLepSkimming && passChannel)|| 
        (doTriggerSkimming && passTriggerSkim)|| 
        (doTauSkimming     && passTauSkim)||
        (doJetSkimming     && passJetSkim)    )) { TopDilep->Fill ();}
        
  
  else if (doTauJetAna_TmemeTauJetSkimming==true && (doTMEMESkimming && passTMEME) && (doTauSkimming && passTauSkim) && (doJetSkimming && passJetSkim)) TopDilep->Fill ();        
  else if (doTauJetAna_TauJetTrigSkimming==true && (doTriggerSkimming && passTriggerSkim) && (doTauSkimming && passTauSkim) && (doJetSkimming && passJetSkim)) TopDilep->Fill ();       
  else if (doTauJetAna_TauJetSkimming==true && (doTauSkimming && passTauSkim) && (doJetSkimming && passJetSkim)) TopDilep->Fill ();       
 
  
 
  event = 0;
  delete event;

}

void NTupleProducer::beginJob ()
{
  std::cout << "BeginJob of NTupleProducer" << std::endl;
  std::cout << "=========================================================== " << std::endl;
  std::cout << " " << std::endl;
  
  int nSkim = 0;
  if(numberOfLept4Skim!=-1)   {nSkim++; std::cout << "You are about to skim events according to the presence of reconstructed electrons and muons : numberOfLept4Skim!=-1" << std::endl;}
  if(doTMEMESkimming==true)   {nSkim++; std::cout << "You are about to skim events according to TMEME : doTMEMESkimming==true" << std::endl;}
  if(doMCDiLepSkimming==true) {nSkim++; std::cout << "You are about to skim events according to MCDiLepSkimming information : doMCDiLepSkimming==true" << std::endl;}
  if(doTauSkimming==true)     {nSkim++; std::cout << "You are about to skim events according to the presence of reconstructed taus : doTauSkimming==true" << std::endl;}
  if(doTriggerSkimming==true) {nSkim++; std::cout << "You are about to skim events according to trigger information : doTriggerSkimming==true" << std::endl;}
  if(doJetSkimming==true)    {nSkim++; std::cout << "You are about to skim events according to jet information : doJetSkimming==true" << std::endl;}
  
  std::cout << " " << std::endl;
  if (nSkim>1) std::cout << "YOU ARE ABOUT TO FILTER EVENTS ACCORDING TO MULTIPLE SKIM CRITERIA, IS THIS WHAT YOU WANT TO DO ? " << std::endl;
  std::cout << " " << std::endl;
  std::cout << "=========================================================== " << std::endl;

}

// ------------ method called once each job just after ending the event loop  ------------
void NTupleProducer::endJob ()
{ 
  std::cout << "EndJob of NTupleProducer" << std::endl; 
  std::cout << "=========================================================== " << std::endl;
  std::cout << " " << std::endl;
  if(numberOfLept4Skim!=-1)    std::cout << "Events have been skimmed according to the presence of reconstructed electrons and muons : numberOfLept4Skim!=-1" << std::endl;
  if(doTMEMESkimming==true)    std::cout << "Events have been skimmed according to TMEME : doTMEMESkimming==true" << std::endl;
  if(doMCDiLepSkimming==true)  std::cout << "Events have been skimmed according to MCDiLepSkimming information : doMCDiLepSkimming==true" << std::endl;
  if(doTauSkimming==true)      std::cout << "Events have been skimmed according to the presence of reconstructed taus : doTauSkimming==true" << std::endl;
  if(doTriggerSkimming==true)  std::cout << "Events have been skimmed according to trigger information : doTriggerSkimming==true" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "=========================================================== " << std::endl;
}





int NTupleProducer::GetNumberOfIDMuons(TopTree::NTEvent     event, double muon_cut_pt_skim, double muon_cut_iso_skim){

  int nmuons = 0;
  std::vector<NTMuon> themuons = event.muons;
  for(unsigned int imuon=0; imuon< themuons.size(); imuon++){
    NTMuon muon = themuons[imuon];// to avoid const problem while calling NTLepton methods!
    if(muon.MuonType<100)                           continue; // isGlobalMuon
    if((muon.MuonType-(muon.MuonType/10)*10) ==0)   continue; //isTrackerMuon
    if(muon.Chi2           >= 10)        continue;
    if(muon.NTrValidHits   <= 10)        continue;
    if(muon.NValidHits     <=  0)        continue;
    if(fabs(muon.D0Inner)  >= 0.02)      continue;
    if(fabs(muon.p4.Eta()) >= 2.4)       continue;
    if(muon_cut_pt_skim>0 && muon.p4.Pt() < muon_cut_pt_skim) continue;
    if(muon_cut_iso_skim>0 && muon.RelIso03() > muon_cut_iso_skim) continue;
    nmuons++;
  }
  
  return nmuons;
  
}

int NTupleProducer::GetNumberOfMuons(TopTree::NTEvent     event, double muon_cut_pt_skim, double muon_cut_iso_skim){

  int nmuons = 0;
  std::vector<NTMuon> themuons = event.muons;
  for(unsigned int imuon=0; imuon< themuons.size(); imuon++){
    NTMuon muon = themuons[imuon];// to avoid const problem while calling NTLepton methods!
    if(muon_cut_pt_skim>0 && muon.p4.Pt() < muon_cut_pt_skim) continue;
    if(muon_cut_iso_skim>0 && muon.RelIso03() > muon_cut_iso_skim) continue;
    nmuons++;
  }
  return nmuons;
  
}



int NTupleProducer::GetNumberOfIDElectrons(TopTree::NTEvent    event, double electron_cut_pt_skim, double electron_cut_iso_skim){


  int nelectrons = 0;
  
  std::vector<NTElectron> theelecs = event.electrons;
  for(unsigned int ielec=0; ielec< theelecs.size(); ielec++){
    NTElectron electron = theelecs[ielec];// to avoid const problem while calling NTLepton methods
    
    bool hadId(electron.elecIdWP90_r & 0x1);
    bool isNotConv(electron.elecIdWP90_r & 0x4);    
    
    if(!electron.isGsfElectron)                    continue; 
    if(!hadId)                                     continue; //hadId
    if(!isNotConv)                                 continue; //isNotConv
    //if(electron.elspike>0.95)             continue;
    if(electron.isEcalDriven!=1)                   continue;
    if(fabs(electron.D0)      >=0.04)    continue; 
    if(fabs(electron.p4.Eta())>=2.5)     continue;
    if(electron.ET_SC         <=15)      continue;
    if(electron_cut_pt_skim>0 && electron.p4.Pt() < electron_cut_pt_skim) continue;
    if(electron_cut_iso_skim>0 && electron.RelIso03() > electron_cut_iso_skim) continue;
    nelectrons++;
  }
  
  
  return nelectrons;
  

}

int NTupleProducer::GetNumberOfElectrons(TopTree::NTEvent    event, double electron_cut_pt_skim, double electron_cut_iso_skim){


  int nelectrons = 0;
  
  std::vector<NTElectron> theelecs = event.electrons;
  for(unsigned int ielec=0; ielec< theelecs.size(); ielec++){
    NTElectron electron = theelecs[ielec];// to avoid const problem while calling NTLepton methods
    if(electron_cut_pt_skim>0 && electron.p4.Pt() < electron_cut_pt_skim) continue;
    if(electron_cut_iso_skim>0 && electron.RelIso03() > electron_cut_iso_skim) continue;
    nelectrons++;
  }
  
  
  return nelectrons;
  

}


int NTupleProducer::GetNumberOfTaus(TopTree::NTEvent event, double tau_cut_pt_skim, std::string algo){

  int ntaus = 0;
  std::vector<TopTree::NTCollection <NTTau> > tausVec = event.tausVec;
  for(unsigned int ivec=0; ivec< tausVec.size(); ivec++){
     if(tausVec[ivec].algo == algo){
       for(unsigned int itau=0; itau< tausVec[ivec].objects.size(); itau++){
          NTTau tau = tausVec[ivec].objects[itau];
          if(tau_cut_pt_skim>0 && tau.p4.Pt() < tau_cut_pt_skim) continue;
          ntaus++;
       }
     }
  }
  return ntaus;
  
}

int NTupleProducer::GetNumberOfJets(TopTree::NTEvent event, double jet_cut_pt_skim, double jet_cut_eta_skim, std::string algo){

  int njets = 0;
  std::vector <TopTree::NTJetMet > jetMetVec = event.jetMetVec;
  for(unsigned int ivec=0; ivec< jetMetVec.size(); ivec++){
     if(jetMetVec[ivec].algo == algo){
       for(unsigned int ijet=0; ijet< jetMetVec[ivec].objects.size(); ijet++){
          NTJet jet = jetMetVec[ivec].objects[ijet];
          if(jet_cut_pt_skim>0 && jet.p4.Pt() < jet_cut_pt_skim) continue;
          if(fabs(jet.p4.Eta()) > jet_cut_eta_skim) continue;
          njets++;
       }
     }
  }
  return njets;
  
}

DEFINE_FWK_MODULE (NTupleProducer);
