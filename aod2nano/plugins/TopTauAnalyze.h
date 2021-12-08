#ifndef TopTauAnalyze_h
#define TopTauAnalyze_h

#include "TH1F.h"
#include "TH2F.h"
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
  //virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  int getTauDecay(edm::Handle<reco::GenParticleCollection> particles, const pat::Tau * thePatTau);
  int GetTTbarTruth (edm::Handle < reco::GenParticleCollection > genParticles);
  bool pass_prefilter();
  bool pass_jet_trigger_prefilter();
  bool pass_tau_trigger_prefilter();
  bool pass_trigger();
  bool pass_tau_prefilter();
  std::vector<float> getJERFactor(float eta);


  bool isData;
  bool isTT;
  std::string inFile;
  bool prefilter;
  bool prefilter_jet_trigger;
  bool prefilter_tau_trigger;
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

  // Event tree
  TTree *tree;
  /*
  TTree *btag;
  int b_flavour;
  float b_pt;
  float b_eta;
  float b_csvDisc;
  */

  // Jet uncertainty corrector
  JetCorrectionUncertainty* jecUnc;
  std::string              jecUncName;

  // Tree to store additional informations per analyzed file
  TTree *info;
  int nEventsTotal;
  int nEventsFiltered;

  // Histos for b-tagging info
  TH2F *h2_BTaggingEff_Denom_b;
  TH2F *h2_BTaggingEff_Denom_c;
  TH2F *h2_BTaggingEff_Denom_udsg;
  TH2F *h2_BTaggingEff_Num_b;
  TH2F *h2_BTaggingEff_Num_c;
  TH2F *h2_BTaggingEff_Num_udsg;

  // Event information
  Int_t value_run;
  UInt_t value_lumi_block;
  ULong64_t value_event;
  Int_t value_isData;
  Int_t value_isTT;

  // MC event
  float cteq66_pdf_weights[45];
  float genWeight;
  int partonFlavor1;
  int partonFlavor2;
  float x1;
  float x2;
  float qScale;
  int num_pileup_bc0;
  int num_pileup_bcp1;
  int num_pileup_bcm1;
  int runNb;
  int n_bc;

  // Trigger
  std::vector<std::string> interestingTriggers;
  std::vector<std::string> prescaleList;
  std::vector<std::string> filterList;
  const static int max_trig = 1000;
  int value_trig[max_trig];
  const static int max_filt = 1000;
  int value_filt[max_filt];
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
  float value_hlttau_eta[max_hlttau];
  float value_hlttau_phi[max_hlttau];
  float value_hlttau_mass[max_hlttau];

  // HLT jets
  const static int max_hltjet = 1000;
  UInt_t value_hltjet_n;
  float value_hltjet_pt[max_hltjet];
  float value_hltjet_px[max_hltjet];
  float value_hltjet_py[max_hltjet];
  float value_hltjet_pz[max_hltjet];
  float value_hltjet_e[max_hltjet];
  float value_hltjet_eta[max_hltjet];
  float value_hltjet_phi[max_hltjet];
  float value_hltjet_mass[max_hltjet];

  // Muons
  const static int max_mu = 1000;
  UInt_t value_mu_n;
  int value_mu_isPFMuon[max_mu];
  int value_mu_isGlobalMuon[max_mu];
  int value_mu_isTrackerMuon[max_mu];
  int value_mu_isStandAloneMuon[max_mu];
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
  float value_mu_genpx[max_mu];
  float value_mu_genpy[max_mu];
  float value_mu_genpz[max_mu];
  float value_mu_gene[max_mu];

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
  float value_el_genpx[max_el];
  float value_el_genpy[max_el];
  float value_el_genpz[max_el];
  float value_el_gene[max_el];

  // MET
  float value_met_pt;
  float value_met_px;
  float value_met_py;
  float value_met_pz;
  float value_met_e;
  float value_met_phi;
  float value_met_eta;
  float value_met_mass;

  // PFMET
  float value_metpf_pt;
  float value_metpf_px;
  float value_metpf_py;
  float value_metpf_pz;
  float value_metpf_e;
  float value_metpf_phi;
  float value_metpf_eta;
  float value_metpf_mass;

  // PFMET Type1 corrected
  float value_metpf1corr_pt;
  float value_metpf1corr_px;
  float value_metpf1corr_py;
  float value_metpf1corr_pz;
  float value_metpf1corr_e;
  float value_metpf1corr_phi;
  float value_metpf1corr_eta;
  float value_metpf1corr_mass;

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
  int value_tau_againstElectronLooseMVA3[max_tau];
  int value_tau_againstElectronMediumMVA3[max_tau];
  int value_tau_againstElectronTightMVA3[max_tau];
  int value_tau_againstElectronVTightMVA3[max_tau];
  int value_tau_againstMuonLoose[max_tau];
  int value_tau_againstMuonMedium[max_tau];
  int value_tau_againstMuonTight[max_tau];
  int value_tau_againstMuonLoose2[max_tau];
  int value_tau_againstMuonMedium2[max_tau];
  int value_tau_againstMuonTight2[max_tau];
  int value_tau_againstMuonLoose3[max_tau];
  int value_tau_againstMuonMedium3[max_tau];
  int value_tau_againstMuonTight3[max_tau];
  int value_tau_byLooseIsolationMVA[max_tau];
  int value_tau_byMediumIsolationMVA[max_tau];
  int value_tau_byTightIsolationMVA[max_tau];
  int value_tau_byLooseIsolationMVA2[max_tau];
  int value_tau_byMediumIsolationMVA2[max_tau];
  int value_tau_byTightIsolationMVA2[max_tau];
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
  float value_tau_genpx[max_tau];
  float value_tau_genpy[max_tau];
  float value_tau_genpz[max_tau];
  float value_tau_gene[max_tau];
  float value_tau_leptonOrigin[max_tau];
  float value_tau_decay[max_tau];

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
  float value_jet_ndaus[max_jet];
  float value_jet_nhf[max_jet];
  float value_jet_nef[max_jet];
  float value_jet_cef[max_jet];
  float value_jet_chf[max_jet];
  float value_jet_nch[max_jet];
  float value_jet_csvDisc[max_jet];
  float value_jet_tcDisc[max_jet];
  //float value_jet_svDisc[max_jet];
  //float value_jet_svEffDisc[max_jet];
  //float value_jet_smDisc[max_jet];
  float value_jet_hlt40px[max_jet];
  float value_jet_hlt40py[max_jet];
  float value_jet_hlt40pz[max_jet];
  float value_jet_hlt40e[max_jet];
  float value_jet_hlt45px[max_jet];
  float value_jet_hlt45py[max_jet];
  float value_jet_hlt45pz[max_jet];
  float value_jet_hlt45e[max_jet];
  int value_jet_flavour[max_jet];
  float value_jec_up[max_jet];
  float value_jec_down[max_jet];
  float value_jer_cent[max_jet];
  float value_jer_down[max_jet];
  float value_jer_up[max_jet];
  float value_jet_genpx[max_jet];
  float value_jet_genpy[max_jet];
  float value_jet_genpz[max_jet];
  float value_jet_gene[max_jet];
  float value_jet_genpartonpx[max_jet];
  float value_jet_genpartonpy[max_jet];
  float value_jet_genpartonpz[max_jet];
  float value_jet_genpartone[max_jet];

  // Gen Event
  int value_tmeme_;
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
