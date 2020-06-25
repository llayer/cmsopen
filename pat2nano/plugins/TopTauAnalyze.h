#ifndef TopTauAnalyze_h
#define TopTauAnalyze_h

#include "TH1F.h"
#include "TTree.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class TopTauAnalyze : public edm::EDAnalyzer {

 public:

  explicit TopTauAnalyze(const edm::ParameterSet&);
  ~TopTauAnalyze();

 private:

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  TTree *tree;

  // Event information
  Int_t value_run;
  UInt_t value_lumi_block;
  ULong64_t value_event;

  // Vertices
  int value_ve_n;
  float value_ve_x;
  float value_ve_y;
  float value_ve_z;

  // Muons
  const static int max_mu = 1000;
  UInt_t value_mu_n;
  float value_mu_pt[max_mu];
  float value_mu_eta[max_mu];
  float value_mu_phi[max_mu];
  float value_mu_mass[max_mu];

  // Electrons
  const static int max_el = 1000;
  UInt_t value_el_n;
  float value_el_pt[max_el];
  float value_el_eta[max_el];
  float value_el_phi[max_el];
  float value_el_mass[max_el];

  // MET
  float value_met_pt;
  float value_met_phi;
  float value_met_sumet;

  // Taus
  const static int max_tau = 1000;
  UInt_t value_tau_n;
  float value_tau_pt[max_tau];
  float value_tau_eta[max_tau];
  float value_tau_phi[max_tau];
  float value_tau_mass[max_tau];

  // Jets
  const static int max_jet = 1000;
  UInt_t value_jet_n;
  float value_jet_pt[max_jet];
  float value_jet_eta[max_jet];
  float value_jet_phi[max_jet];
  float value_jet_mass[max_jet];

  // Gen Event
  float value_nLep_;
  float value_topPt_;
  float value_topEta_;
  float value_topPhi_;
  float value_topBarPt_;
  float value_topBarEta_;
  float value_topBarPhi_;
  float value_ttbarPt_;
  float value_ttbarEta_;
  float value_ttbarPhi_;

  edm::InputTag vertices_;
  edm::InputTag muons_;
  edm::InputTag electrons_;
  edm::InputTag met_;
  edm::InputTag taus_;
  edm::InputTag jets_;
  edm::InputTag genEvent_;

  TH1F *mult_;
  TH1F *en_;
  TH1F *pt_;
  TH1F *eta_;
  TH1F *phi_;
};

#endif
