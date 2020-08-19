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

  bool isData;
  double electron_cut_pt;
  double electron_cut_eta;
  double photon_cut_pt;
  double photon_cut_eta;
  double muon_cut_pt;
  double muon_cut_eta;
  double tau_cut_pt;
  double tau_cut_eta;
  double jet_cut_pt;
  double jet_cut_eta;

  TTree *tree;

  // Event information
  Int_t value_run;
  UInt_t value_lumi_block;
  ULong64_t value_event;

  // Trigger
  std::vector<std::string> interestingTriggers;
  std::vector<std::string> prescaleList;
  std::vector<std::string> filterList;
  const static int max_trig = 1000;
  bool value_trig[max_trig];
  const static int max_filt = 1000;
  bool value_filt[max_filt];
  const static int max_prescale = 1000;
  float value_prescale[max_prescale];

  // Vertices
  int value_ve_n;
  float value_ve_x;
  float value_ve_y;
  float value_ve_z;
  float value_ve_xerr;
  float value_ve_yerr;
  float value_ve_zerr;
  float value_ve_chi2;
  bool value_ve_isFake;
  bool value_ve_isValid;
  int value_ve_ndof;
  float value_ve_normalizedChi2;
  int value_ve_numberOfTracks;
  float value_ve_rho;
  float value_ve_bsx;
  float value_ve_bsy;
  float value_ve_bsz;

  // HLT taus
  const static int max_hlttau = 1000;
  UInt_t value_hlttau_n;
  float value_hlttau_pt[max_hlttau];
  float value_hlttau_px[max_hlttau];
  float value_hlttau_py[max_hlttau];
  float value_hlttau_pz[max_hlttau];
  float value_hlttau_e[max_hlttau];

  // HLT jets
  const static int max_hltjet = 1000;
  UInt_t value_hltjet_n;
  float value_hltjet_pt[max_hltjet];
  float value_hltjet_px[max_hltjet];
  float value_hltjet_py[max_hltjet];
  float value_hltjet_pz[max_hltjet];
  float value_hltjet_e[max_hltjet];

  // Muons
  const static int max_mu = 1000;
  UInt_t value_mu_n;
  float value_mu_isPFMuon[max_mu];
  float value_mu_isGlobalMuon[max_mu];
  float value_mu_isTrackerMuon[max_mu];
  float value_mu_isStandAloneMuon[max_mu];
  float value_mu_pt[max_mu];
  float value_mu_px[max_mu];
  float value_mu_py[max_mu];
  float value_mu_pz[max_mu];
  float value_mu_e[max_mu];
  float value_mu_eta[max_mu];
  float value_mu_phi[max_mu];
  float value_mu_mass[max_mu];
  float value_mu_charge[max_mu];
  float value_mu_x[max_mu];
  float value_mu_y[max_mu];
  float value_mu_z[max_mu];
  float value_mu_TrkIso03[max_mu];
  float value_mu_CaloIso03[max_mu];
  float value_mu_ECaloIso03[max_mu];
  float value_mu_HCaloIso03[max_mu];
  float value_mu_neutralHadronIso[max_mu];
  float value_mu_chargedHadronIso[max_mu];
  float value_mu_puChargedHadronIso[max_mu];
  float value_mu_photonIso[max_mu];
  float value_mu_trackIso[max_mu];
  float value_mu_dxy[max_mu];
  float value_mu_chi2[max_mu];

  // Electrons
  const static int max_el = 1000;
  UInt_t value_el_n;
  float value_el_pt[max_el];
  float value_el_px[max_el];
  float value_el_py[max_el];
  float value_el_pz[max_el];
  float value_el_e[max_el];
  float value_el_eta[max_el];
  float value_el_phi[max_el];
  float value_el_mass[max_el];
  float value_el_charge[max_el];
  float value_el_x[max_el];
  float value_el_y[max_el];
  float value_el_z[max_el];
  float value_el_TrkIso03[max_el];
  float value_el_CaloIso03[max_el];
  float value_el_ECaloIso03[max_el];
  float value_el_HCaloIso03[max_el];
  float value_el_TrkIso04[max_el];
  float value_el_CaloIso04[max_el];
  float value_el_ECaloIso04[max_el];
  float value_el_HCaloIso04[max_el];
  float value_el_neutralHadronIso[max_el];
  float value_el_chargedHadronIso[max_el];
  float value_el_puChargedHadronIso[max_el];
  float value_el_photonIso[max_el];
  float value_el_trackIso[max_el];
  int value_el_elecIdLoose[max_el];
  int value_el_elecIdTight[max_el];
  int value_el_elecIdRobustLoose[max_el];
  int value_el_elecIdRobustTight[max_el];
  int value_el_elecIdWP90_r[max_el];
  int value_el_elecIdWP90_c [max_el];
  int value_el_cutbasedid [max_el];
  float value_el_dxy[max_el];
  float value_el_chi2[max_el];

  // MET
  float value_met_pt;
  float value_met_px;
  float value_met_py;
  float value_met_pz;
  float value_met_e;
  float value_met_uncorrectedPt;
  float value_met_phi;
  float value_met_sumet;
  float value_met_corEx;
  float value_met_corEy;
  float value_met_significance;
  float value_met_covxx;
  float value_met_covxy;
  float value_met_covyy;

  // Taus
  const static int max_tau = 1000;
  UInt_t value_tau_n;
  float value_tau_pt[max_tau];
  float value_tau_px[max_tau];
  float value_tau_py[max_tau];
  float value_tau_pz[max_tau];
  float value_tau_e[max_tau];
  float value_tau_eta[max_tau];
  float value_tau_phi[max_tau];
  float value_tau_mass[max_tau];
  float value_tau_charge[max_tau];
  float value_tau_x[max_tau];
  float value_tau_y[max_tau];
  float value_tau_z[max_tau];
  float value_tau_leadTrackPt[max_tau];
  float value_tau_dxy[max_tau];
  int value_tau_decaymode[max_tau];
  float value_tau_reliso_all[max_tau];
  int value_tau_againstElectronLoose[max_tau];
  int value_tau_againstElectronMedium[max_tau];
  int value_tau_againstElectronTight[max_tau];
  int value_tau_againstMuonLoose[max_tau];
  int value_tau_againstMuonMedium[max_tau];
  int value_tau_againstMuonTight[max_tau];
  int value_tau_byLooseCombinedIsolationDeltaBetaCorr3Hits[max_tau];
  int value_tau_byMediumCombinedIsolationDeltaBetaCorr3Hits[max_tau];
  int value_tau_byTightCombinedIsolationDeltaBetaCorr3Hits[max_tau];
  int value_tau_byLooseCombinedIsolationDeltaBetaCorr[max_tau];
  int value_tau_byMediumCombinedIsolationDeltaBetaCorr[max_tau];
  int value_tau_byTightCombinedIsolationDeltaBetaCorr[max_tau];
  float value_tau_hlt40px[max_tau];
  float value_tau_hlt40py[max_tau];
  float value_tau_hlt40pz[max_tau];
  float value_tau_hlt40e[max_tau];
  float value_tau_hlt45px[max_tau];
  float value_tau_hlt45py[max_tau];
  float value_tau_hlt45pz[max_tau];
  float value_tau_hlt45e[max_tau];

  // Jets
  const static int max_jet = 1000;
  UInt_t value_jet_n;
  float value_jet_pt[max_jet];
  float value_jet_px[max_jet];
  float value_jet_py[max_jet];
  float value_jet_pz[max_jet];
  float value_jet_e[max_jet];
  float value_jet_eta[max_jet];
  float value_jet_phi[max_jet];
  float value_jet_mass[max_jet];
  float value_jet_charge[max_jet];
  float value_jet_x[max_jet];
  float value_jet_y[max_jet];
  float value_jet_z[max_jet];
  float value_jet_scale[max_jet];
  float value_jet_emEnergyFraction[max_jet];
  float value_jet_n90[max_jet];
  int value_jet_tcDisc[max_jet];
  int value_jet_svDisc[max_jet];
  int value_jet_svEffDisc[max_jet];
  int value_jet_smDisc[max_jet];
  float value_jet_hlt40px[max_jet];
  float value_jet_hlt40py[max_jet];
  float value_jet_hlt40pz[max_jet];
  float value_jet_hlt40e[max_jet];
  float value_jet_hlt45px[max_jet];
  float value_jet_hlt45py[max_jet];
  float value_jet_hlt45pz[max_jet];
  float value_jet_hlt45e[max_jet];

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

};

#endif
