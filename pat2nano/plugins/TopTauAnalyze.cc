#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "TLorentzVector.h"
#include "TopTauAnalyze.h"

typedef math::XYZPoint Point;

TopTauAnalyze::TopTauAnalyze(const edm::ParameterSet& cfg)
{
  edm::Service<TFileService> fs;

  isData            = cfg.getParameter < bool >      ("isData");
  electron_cut_pt   = cfg.getParameter < double >    ("electron_cut_pt");
  electron_cut_eta  = cfg.getParameter < double >    ("electron_cut_eta");
  muon_cut_pt       = cfg.getParameter < double >    ("muon_cut_pt");
  muon_cut_eta      = cfg.getParameter < double >    ("muon_cut_eta");
  tau_cut_pt        = cfg.getParameter < double >    ("tau_cut_pt");
  tau_cut_eta       = cfg.getParameter < double >    ("tau_cut_eta");
  jet_cut_pt        = cfg.getParameter < double >    ("jet_cut_pt");
  jet_cut_eta       = cfg.getParameter < double >    ("jet_cut_eta");



  tree = fs->make<TTree>("Events", "Events");

  // Event information
  tree->Branch("run", &value_run);
  tree->Branch("luminosityBlock", &value_lumi_block);
  tree->Branch("event", &value_event);

  // Trigger
  interestingTriggers = {
    "HLT_QuadJet40_IsoPFTau40", "HLT_QuadJet45_IsoPFTau45", "HLT_Mu15_v", "HLT_Mu20_v", "HLT_Mu24_v", "HLT_Mu30_v",
    "HLT_Mu15_v1", "HLT_Mu15_v2", "HLT_Mu15_v3", "HLT_Mu15_v4", "HLT_Mu15_v5",
    "HLT_Mu20_v1", "HLT_Mu20_v2", "HLT_Mu20_v3", "HLT_Mu20_v4", "HLT_Mu20_v5",
    "HLT_Mu24_v1", "HLT_Mu24_v2", "HLT_Mu24_v3", "HLT_Mu24_v4", "HLT_Mu24_v5",
    "HLT_Mu30_v1", "HLT_Mu30_v2", "HLT_Mu30_v3", "HLT_Mu30_v4", "HLT_Mu30_v5"
  };
  for(size_t i = 0; i < interestingTriggers.size(); i++) {
    tree->Branch(interestingTriggers[i].c_str(), value_trig + i, (interestingTriggers[i] + "/I").c_str());
  }


  prescaleList = {
    "HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5",
    "HLT_QuadJet40_IsoPFTau40_v1","HLT_QuadJet40_IsoPFTau40_v2","HLT_QuadJet40_IsoPFTau40_v3",
    "HLT_QuadJet40_IsoPFTau40_v4","HLT_QuadJet40_IsoPFTau40_v5","HLT_QuadJet40_IsoPFTau40_v6",
    "HLT_QuadJet40_IsoPFTau40_v7","HLT_QuadJet40_IsoPFTau40_v8","HLT_QuadJet40_IsoPFTau40_v9",
    "HLT_QuadJet40_IsoPFTau40_v10","HLT_QuadJet40_IsoPFTau40_v11","HLT_QuadJet40_IsoPFTau40_v12",
    "HLT_QuadJet40_IsoPFTau40_v13","HLT_QuadJet40_IsoPFTau40_v14","HLT_QuadJet40_IsoPFTau40_v15"
  };
  std::string prefix_prescale = "Prescale_";
  for(size_t i = 0; i < prescaleList.size(); i++) {
    std::string prescale_name = prefix_prescale + prescaleList[i];
    tree->Branch( prescale_name.c_str(), value_prescale + i, (prescale_name + "/F").c_str());
  }

  filterList = {
    "hltQuadJet40", "hltQuadJet45", "hltQuadJet40IsoPFTau40", "HLTPFTauTightIsoSequence", "hltPFTau5Track", "hltPFTau5Track5",
    "hltFilterPFTauTrack5TightIsoL1QuadJet20Central", "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40",
    "hltQuadJet45IsoPFTau45", "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45",
    "hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45"
  };
  std::string prefix_filt = "HLTFilter_";
  for(size_t i = 0; i < filterList.size(); i++) {
    std::string filt_name = prefix_filt + filterList[i];
    //std::cout << filt_name << std::endl;
    tree->Branch( filt_name.c_str(), value_filt + i, (filt_name + "/I").c_str());
  }

  // Vertices
  tree->Branch("PV_npvs", &value_ve_n, "PV_npvs/I");
  tree->Branch("PV_x", &value_ve_x, "PV_x/F");
  tree->Branch("PV_y", &value_ve_y, "PV_y/F");
  tree->Branch("PV_z", &value_ve_z, "PV_z/F");
  tree->Branch("PV_xerr", &value_ve_xerr, "PV_xerr/F");
  tree->Branch("PV_yerr", &value_ve_yerr, "PV_yerr/F");
  tree->Branch("PV_zerr", &value_ve_zerr, "PV_zerr/F");
  tree->Branch("PV_chi2", &value_ve_chi2, "PV_chi2/F");
  tree->Branch("PV_isFake", &value_ve_isFake, "PV_isFake/I");
  tree->Branch("PV_isValid", &value_ve_isValid, "PV_isValid/I");
  tree->Branch("PV_ndof", &value_ve_ndof, "PV_ndof/I");
  tree->Branch("PV_normalizedChi2", &value_ve_normalizedChi2, "PV_normalizedChi2/F");
  tree->Branch("PV_numberOfTracks", &value_ve_numberOfTracks, "PV_numberOfTracks/I");
  tree->Branch("PV_rho", &value_ve_rho, "PV_rho/F");
  tree->Branch("PV_bsx", &value_ve_bsx, "PV_bsx/F");
  tree->Branch("PV_bsy", &value_ve_bsy, "PV_bsy/F");
  tree->Branch("PV_bsz", &value_ve_bsz, "PV_bsz/F");

  // HLT taus
  tree->Branch("nTauHLT", &value_hlttau_n, "nTauHLT/i");
  tree->Branch("TauHLT_pt", value_hlttau_pt, "TauHLT_pt[nTauHLT]/F");
  tree->Branch("TauHLT_px", value_hlttau_px, "TauHLT_px[nTauHLT]/F");
  tree->Branch("TauHLT_py", value_hlttau_py, "TauHLT_py[nTauHLT]/F");
  tree->Branch("TauHLT_pz", value_hlttau_pz, "TauHLT_pz[nTauHLT]/F");
  tree->Branch("TauHLT_e", value_hlttau_e, "TauHLT_e[nTauHLT]/F");

  // HLT jets
  tree->Branch("nJetHLT", &value_hltjet_n, "nJetHLT/i");
  tree->Branch("JetHLT_pt", value_hltjet_pt, "JetHLT_pt[nJetHLT]/F");
  tree->Branch("JetHLT_px", value_hltjet_px, "JetHLT_px[nJetHLT]/F");
  tree->Branch("JetHLT_py", value_hltjet_py, "JetHLT_py[nJetHLT]/F");
  tree->Branch("JetHLT_pz", value_hltjet_pz, "JetHLT_pz[nJetHLT]/F");
  tree->Branch("JetHLT_e", value_hltjet_e, "JetHLT_e[nJetHLT]/F");

  // Muons
  tree->Branch("nMuon", &value_mu_n, "nMuon/i");
  tree->Branch("Muon_isPFMuon", value_mu_isPFMuon, "Muon_isPFMuon[nMuon]/I");
  tree->Branch("Muon_isGlobalMuon", value_mu_isGlobalMuon, "Muon_isGlobalMuon[nMuon]/I");
  tree->Branch("Muon_isTrackerMuon", value_mu_isTrackerMuon, "Muon_isTrackerMuon[nMuon]/I");
  tree->Branch("Muon_isStandAloneMuon", value_mu_isStandAloneMuon, "Muon_isStandAloneMuon[nMuon]/I");
  tree->Branch("Muon_pt", value_mu_pt, "Muon_pt[nMuon]/F");
  tree->Branch("Muon_px", value_mu_px, "Muon_px[nMuon]/F");
  tree->Branch("Muon_py", value_mu_py, "Muon_py[nMuon]/F");
  tree->Branch("Muon_pz", value_mu_pz, "Muon_pz[nMuon]/F");
  tree->Branch("Muon_e", value_mu_e, "Muon_e[nMuon]/F");
  tree->Branch("Muon_eta", value_mu_eta, "Muon_eta[nMuon]/F");
  tree->Branch("Muon_phi", value_mu_phi, "Muon_phi[nMuon]/F");
  tree->Branch("Muon_mass", value_mu_mass, "Muon_mass[nMuon]/F");
  tree->Branch("Muon_charge", value_mu_charge, "Muon_charge[nMuon]/F");
  tree->Branch("Muon_x", value_mu_x, "Muon_x[nMuon]/F");
  tree->Branch("Muon_y", value_mu_y, "Muon_y[nMuon]/F");
  tree->Branch("Muon_z", value_mu_z, "Muon_z[nMuon]/F");
  tree->Branch("Muon_TrkIso03", value_mu_TrkIso03, "Muon_TrkIso03[nMuon]/F");
  tree->Branch("Muon_CaloIso03", value_mu_CaloIso03, "Muon_CaloIso03[nMuon]/F");
  tree->Branch("Muon_ECaloIso03", value_mu_ECaloIso03, "Muon_ECaloIso03[nMuon]/F");
  tree->Branch("Muon_HCaloIso03", value_mu_HCaloIso03, "Muon_HCaloIso03[nMuon]/F");
  tree->Branch("Muon_neutralHadronIso", value_mu_neutralHadronIso, "Muon_neutralHadronIso[nMuon]/F");
  tree->Branch("Muon_chargedHadronIso", value_mu_chargedHadronIso, "Muon_chargedHadronIso[nMuon]/F");
  tree->Branch("Muon_puChargedHadronIso", value_mu_puChargedHadronIso, "Muon_puChargedHadronIso[nMuon]/F");
  tree->Branch("Muon_photonIso", value_mu_photonIso, "Muon_photonIso[nMuon]/F");
  tree->Branch("Muon_trackIso", value_mu_trackIso, "Muon_trackIso[nMuon]/F");
  tree->Branch("Muon_dxy", value_mu_dxy, "Muon_dxy[nMuon]/F");
  tree->Branch("Muon_chi2", value_mu_chi2, "Muon_chi2[nMuon]/F");
  if ( isData != 1 ) {
    tree->Branch("Muon_genpx", value_mu_genpx, "Muon_genpx[nMuon]/F");
    tree->Branch("Muon_genpy", value_mu_genpy, "Muon_genpy[nMuon]/F");
    tree->Branch("Muon_genpz", value_mu_genpz, "Muon_genpz[nMuon]/F");
    tree->Branch("Muon_gene", value_mu_gene, "Muon_gene[nMuon]/F");
  }

  // Electrons
  tree->Branch("nElectron", &value_el_n, "nElectron/i");
  tree->Branch("Electron_pt", value_el_pt, "Electron_pt[nElectron]/F");
  tree->Branch("Electron_px", value_el_px, "Electron_px[nElectron]/F");
  tree->Branch("Electron_py", value_el_py, "Electron_py[nElectron]/F");
  tree->Branch("Electron_pz", value_el_pz, "Electron_pz[nElectron]/F");
  tree->Branch("Electron_e", value_el_e, "Electron_e[nElectron]/F");
  tree->Branch("Electron_eta", value_el_eta, "Electron_eta[nElectron]/F");
  tree->Branch("Electron_phi", value_el_phi, "Electron_phi[nElectron]/F");
  tree->Branch("Electron_mass", value_el_mass, "Electron_mass[nElectron]/F");
  tree->Branch("Electron_charge", value_el_charge, "Electron_charge[nElectron]/F");
  tree->Branch("Electron_x", value_el_x, "Electron_x[nElectron]/F");
  tree->Branch("Electron_y", value_el_y, "Electron_y[nElectron]/F");
  tree->Branch("Electron_z", value_el_z, "Electron_z[nElectron]/F");
  tree->Branch("Electron_TrkIso03", value_el_TrkIso03, "Electron_TrkIso03[nElectron]/F");
  tree->Branch("Electron_CaloIso03", value_el_CaloIso03, "Electron_CaloIso03[nElectron]/F");
  tree->Branch("Electron_ECaloIso03", value_el_ECaloIso03, "Electron_ECaloIso03[nElectron]/F");
  tree->Branch("Electron_HCaloIso03", value_el_HCaloIso03, "Electron_HCaloIso03[nElectron]/F");
  tree->Branch("Electron_neutralHadronIso", value_el_neutralHadronIso, "Electron_neutralHadronIso[nElectron]/F");
  tree->Branch("Electron_chargedHadronIso", value_el_chargedHadronIso, "Electron_chargedHadronIso[nElectron]/F");
  tree->Branch("Electron_puChargedHadronIso", value_el_puChargedHadronIso, "Electron_puChargedHadronIso[nElectron]/F");
  tree->Branch("Electron_photonIso", value_el_photonIso, "Electron_photonIso[nElectron]/F");
  tree->Branch("Electron_trackIso", value_el_trackIso, "Electron_trackIso[nElectron]/F");
  tree->Branch("Electron_dxy", value_el_dxy, "Electron_dxy[nElectron]/F");
  tree->Branch("Electron_chi2", value_el_chi2, "Electron_chi2[nElectron]/F");
  tree->Branch("Electron_elecIdLoose", value_el_elecIdLoose, "Electron_elecIdLoose[nElectron]/I");
  tree->Branch("Electron_elecIdTight", value_el_elecIdTight, "Electron_elecIdTight[nElectron]/I");
  tree->Branch("Electron_elecIdRobustLoose", value_el_elecIdRobustLoose, "Electron_elecIdRobustLoose[nElectron]/I");
  tree->Branch("Electron_elecIdRobustTight", value_el_elecIdRobustTight, "Electron_elecIdRobustTight[nElectron]/I");
  tree->Branch("Electron_elecIdWP90_r", value_el_elecIdWP90_r, "Electron_elecIdWP90_r[nElectron]/I");
  tree->Branch("Electron_elecIdWP90_c", value_el_elecIdWP90_c, "Electron_elecIdWP90_c[nElectron]/I");
  tree->Branch("Electron_cutbasedid", value_el_cutbasedid, "Electron_cutbasedid[nElectron]/I");
  if ( isData != 1 ) {
    tree->Branch("Electron_genpx", value_el_genpx, "Electron_genpx[nElectron]/F");
    tree->Branch("Electron_genpy", value_el_genpy, "Electron_genpy[nElectron]/F");
    tree->Branch("Electron_genpz", value_el_genpz, "Electron_genpz[nElectron]/F");
    tree->Branch("Electron_gene", value_el_gene, "Electron_gene[nElectron]/F");
  }

  // Taus
  tree->Branch("nTau", &value_tau_n, "nTau/i");
  tree->Branch("Tau_pt", value_tau_pt, "Tau_pt[nTau]/F");
  tree->Branch("Tau_px", value_tau_px, "Tau_px[nTau]/F");
  tree->Branch("Tau_py", value_tau_py, "Tau_py[nTau]/F");
  tree->Branch("Tau_pz", value_tau_pz, "Tau_pz[nTau]/F");
  tree->Branch("Tau_e", value_tau_e, "Tau_e[nTau]/F");
  tree->Branch("Tau_eta", value_tau_eta, "Tau_eta[nTau]/F");
  tree->Branch("Tau_phi", value_tau_phi, "Tau_phi[nTau]/F");
  tree->Branch("Tau_mass", value_tau_mass, "Tau_mass[nTau]/F");
  tree->Branch("Tau_charge", value_tau_charge, "Tau_charge[nTau]/F");
  tree->Branch("Tau_x", value_tau_x, "Tau_x[nTau]/F");
  tree->Branch("Tau_y", value_tau_y, "Tau_y[nTau]/F");
  tree->Branch("Tau_z", value_tau_z, "Tau_z[nTau]/F");
  tree->Branch("Tau_leadTrackPt", value_tau_leadTrackPt, "Tau_leadTrackPt[nTau]/F");
  tree->Branch("Tau_dxy", value_tau_dxy, "Tau_dxy[nTau]/F");
  tree->Branch("Tau_againstElectronLoose", value_tau_againstElectronLoose, "Tau_againstElectronLoose[nTau]/I");
  tree->Branch("Tau_againstElectronMedium", value_tau_againstElectronMedium, "Tau_againstElectronMedium[nTau]/I");
  tree->Branch("Tau_againstElectronTight", value_tau_againstElectronTight, "Tau_againstElectronTight[nTau]/I");
  tree->Branch("Tau_againstMuonLoose", value_tau_againstMuonLoose, "Tau_againstMuonLoose[nTau]/I");
  tree->Branch("Tau_againstMuonMedium", value_tau_againstMuonMedium, "Tau_againstMuonMedium[nTau]/I");
  tree->Branch("Tau_againstMuonTight", value_tau_againstMuonTight, "Tau_againstMuonTight[nTau]/I");
  tree->Branch("Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits", value_tau_byLooseCombinedIsolationDeltaBetaCorr3Hits, "Tau_byLooseCombinedIsolationDeltaBetaCorr3Hits[nTau]/I");
  tree->Branch("Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits", value_tau_byMediumCombinedIsolationDeltaBetaCorr3Hits, "Tau_byMediumCombinedIsolationDeltaBetaCorr3Hits[nTau]/I");
  tree->Branch("Tau_byTightCombinedIsolationDeltaBetaCorr3Hits", value_tau_byTightCombinedIsolationDeltaBetaCorr3Hits, "Tau_byTightCombinedIsolationDeltaBetaCorr3Hits[nTau]/I");
  tree->Branch("Tau_byLooseCombinedIsolationDeltaBetaCorr", value_tau_byLooseCombinedIsolationDeltaBetaCorr, "Tau_byLooseCombinedIsolationDeltaBetaCorr[nTau]/I");
  tree->Branch("Tau_byMediumCombinedIsolationDeltaBetaCorr", value_tau_byMediumCombinedIsolationDeltaBetaCorr, "Tau_byMediumCombinedIsolationDeltaBetaCorr[nTau]/I");
  tree->Branch("Tau_byTightCombinedIsolationDeltaBetaCorr", value_tau_byTightCombinedIsolationDeltaBetaCorr, "Tau_byTightCombinedIsolationDeltaBetaCorr[nTau]/I");
  tree->Branch("Tau_pxHLT40", value_tau_hlt40px, "Tau_pxHLT40[nTau]/F");
  tree->Branch("Tau_pyHLT40", value_tau_hlt40py, "Tau_pyHLT40[nTau]/F");
  tree->Branch("Tau_pzHLT40", value_tau_hlt40pz, "Tau_pzHLT40[nTau]/F");
  tree->Branch("Tau_eHLT40", value_tau_hlt40e, "Tau_eHLT40[nTau]/F");
  tree->Branch("Tau_pxHLT45", value_tau_hlt45px, "Tau_pxHLT45[nTau]/F");
  tree->Branch("Tau_pyHLT45", value_tau_hlt45py, "Tau_pyHLT45[nTau]/F");
  tree->Branch("Tau_pzHLT45", value_tau_hlt45pz, "Tau_pzHLT45[nTau]/F");
  tree->Branch("Tau_eHLT45", value_tau_hlt45e, "Tau_eHLT45[nTau]/F");
  if ( isData != 1 ) {
    tree->Branch("Tau_genpx", value_tau_genpx, "Tau_genpx[nTau]/F");
    tree->Branch("Tau_genpy", value_tau_genpy, "Tau_genpy[nTau]/F");
    tree->Branch("Tau_genpz", value_tau_genpz, "Tau_genpz[nTau]/F");
    tree->Branch("Tau_gene", value_tau_gene, "Tau_gene[nTau]/F");
    tree->Branch("Tau_leptonOrigin", value_tau_leptonOrigin, "Tau_leptonOrigin[nTau]/I");
    tree->Branch("Tau_decay", value_tau_decay, "Tau_decay[nTau]/I");
  }

  // MET
  tree->Branch("MET_pt", &value_met_pt, "MET_pt/F");
  tree->Branch("MET_phi", &value_met_phi, "MET_phi/F");
  tree->Branch("MET_sumet", &value_met_sumet, "MET_sumet/F");

  // Jets
  tree->Branch("nJet", &value_jet_n, "nJet/i");
  tree->Branch("Jet_pt", value_jet_pt, "Jet_pt[nJet]/F");
  tree->Branch("Jet_px", value_jet_px, "Jet_px[nJet]/F");
  tree->Branch("Jet_py", value_jet_py, "Jet_py[nJet]/F");
  tree->Branch("Jet_pz", value_jet_pz, "Jet_pz[nJet]/F");
  tree->Branch("Jet_e", value_jet_e, "Jet_e[nJet]/F");
  tree->Branch("Jet_eta", value_jet_eta, "Jet_eta[nJet]/F");
  tree->Branch("Jet_phi", value_jet_phi, "Jet_phi[nJet]/F");
  tree->Branch("Jet_mass", value_jet_mass, "Jet_mass[nJet]/F");
  tree->Branch("Jet_charge", value_jet_charge, "Jet_charge[nJet]/F");
  tree->Branch("Jet_x", value_jet_x, "Jet_x[nJet]/F");
  tree->Branch("Jet_y", value_jet_y, "Jet_y[nJet]/F");
  tree->Branch("Jet_z", value_jet_z, "Jet_z[nJet]/F");
  tree->Branch("Jet_scale", value_jet_scale, "Jet_scale[nJet]/F");
  tree->Branch("Jet_tcDisc", value_jet_tcDisc, "Jet_tcDisc[nJet]/F");
  tree->Branch("Jet_svDisc", value_jet_svDisc, "Jet_svDisc[nJet]/F");
  tree->Branch("Jet_svEffDisc", value_jet_svEffDisc, "Jet_svEffDisc[nJet]/F");
  tree->Branch("Jet_smDisc", value_jet_smDisc, "Jet_smDisc[nJet]/F");
  tree->Branch("Jet_pxHLT40", value_jet_hlt40px, "Jet_pxHLT40[nJet]/F");
  tree->Branch("Jet_pyHLT40", value_jet_hlt40py, "Jet_pyHLT40[nJet]/F");
  tree->Branch("Jet_pzHLT40", value_jet_hlt40pz, "Jet_pzHLT40[nJet]/F");
  tree->Branch("Jet_eHLT40", value_jet_hlt40e, "Jet_eHLT40[nJet]/F");
  tree->Branch("Jet_pxHLT45", value_jet_hlt45px, "Jet_pxHLT45[nJet]/F");
  tree->Branch("Jet_pyHLT45", value_jet_hlt45py, "Jet_pyHLT45[nJet]/F");
  tree->Branch("Jet_pzHLT45", value_jet_hlt45pz, "Jet_pzHLT45[nJet]/F");
  tree->Branch("Jet_eHLT45", value_jet_hlt45e, "Jet_eHLT45[nJet]/F");
  if ( isData != 1 ) {
    tree->Branch("Jet_genpx", value_jet_genpx, "Jet_genpx[nJet]/F");
    tree->Branch("Jet_genpy", value_jet_genpy, "Jet_genpy[nJet]/F");
    tree->Branch("Jet_genpz", value_jet_genpz, "Jet_genpz[nJet]/F");
    tree->Branch("Jet_gene", value_jet_gene, "Jet_gene[nJet]/F");
    tree->Branch("Jet_genpartonpx", value_jet_genpartonpx, "Jet_genpartonpx[nJet]/F");
    tree->Branch("Jet_genpartonpy", value_jet_genpartonpy, "Jet_genpartonpy[nJet]/F");
    tree->Branch("Jet_genpartonpz", value_jet_genpartonpz, "Jet_genpartonpz[nJet]/F");
    tree->Branch("Jet_genpartone", value_jet_genpartone, "Jet_genpartone[nJet]/F");
  }

  // genEvent
  if( isData == false ){
    tree->Branch("genEvent_nLep", &value_nLep_, "genEvent_nLep/F");
    tree->Branch("genEvent_topPt", &value_topPt_, "genEvent_topPt/F");
    tree->Branch("genEvent_topEta", &value_topEta_, "genEvent_topEta/F");
    tree->Branch("genEvent_topPhi", &value_topPhi_, "genEvent_topPhi/F");
    tree->Branch("genEvent_topBarPt", &value_topBarPt_, "genEvent_topBarPt/F");
    tree->Branch("genEvent_topBarEta", &value_topBarEta_, "genEvent_topBarEta/F");
    tree->Branch("genEvent_topBarPhi", &value_topBarPhi_, "genEvent_topBarPhi/F");
    tree->Branch("genEvent_ttbarPt", &value_ttbarPt_, "genEvent_ttbarPt/F");
    tree->Branch("genEvent_ttbarEta", &value_ttbarEta_, "genEvent_ttbarEta/F");
    tree->Branch("genEvent_ttbarPhi", &value_ttbarPhi_, "genEvent_ttbarPhi/F");
  }

  // Aditional information
  info = fs->make<TTree>("Info", "Info");
  nEventsTotal = 0;
  info->Branch("nEventsTotal", &nEventsTotal);
  nEventsFiltered = 0;
  info->Branch("nEventsFiltered", &nEventsFiltered);

}

TopTauAnalyze::~TopTauAnalyze()
{
}

void TopTauAnalyze::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{

  using namespace edm;
  using namespace reco;
  using namespace std;

  //std::cout << endl << "Start analysis for event: " << value_event << endl;

  ///////////////////////////
  // Event information
  ///////////////////////////
  value_run = evt.run();
  value_lumi_block = evt.luminosityBlock();
  value_event = evt.id().event();

  ///////////////////////////
  // Trigger
  ///////////////////////////

  // Trigger results
  //std::cout << endl << "Trigger names: " << endl;
  Handle<TriggerResults> trigger;
  evt.getByLabel(InputTag("TriggerResults", "", "HLT"), trigger);

  //std::cout << "Start analysis for event: " << endl;
  // Accepted triggers
  const edm::TriggerNames & triggerNames = evt.triggerNames(*trigger);
  const std::vector<std::string> & triggerNames_ = triggerNames.triggerNames();
  for (unsigned int i = 0; i < interestingTriggers.size(); i++) {
    value_trig[i] = 0;
  }
  for (unsigned int i = 0; i < trigger->size(); i++) {
    //if( trigger->accept(i) == 1 ){
      //std::cout<< triggerNames_[i] << std::endl;
    //}
    for (unsigned int j = 0; j < interestingTriggers.size(); j++) {
      if( triggerNames_[i].find(interestingTriggers[j]) != std::string::npos ){
        //std::cout << triggerNames_[i] << std::endl;
        //std::cout << "Accept triggers: " << trigger->accept(i) << std::endl;
        value_trig[j] = int(trigger->accept(i));
      }
    }
  }

  Handle<pat::TriggerEvent> patTriggerEvent;
  evt.getByLabel(InputTag("patTriggerEventPF"), patTriggerEvent);
  // Trigger event
  pat::TriggerEvent pTrigEvt;
  pTrigEvt = *patTriggerEvent;

  /*
  auto filters = pTrigEvt.acceptedFilters();
  for (unsigned int i = 0; i < filters.size(); i++) {
    std::cout<< endl << filters[i]->label() << filters[i]->status() << endl;
  }
  */

  // Filters
  //std::cout << endl << "Filter names: " << endl;
  for (unsigned int i = 0; i < filterList.size(); i++) {
    const pat::TriggerFilter* filter = pTrigEvt.filter(filterList[i]);
    if (filter){
      //std::cout<< filterList[i] << " " << filter->status() << std::endl;
      value_filt[i] = int(filter->status());
    }
  }

  // Prescales
  for (unsigned int i = 0; i < prescaleList.size (); i++) {
    if(pTrigEvt.path(prescaleList[i])!=0){
      value_prescale[i] = pTrigEvt.path(prescaleList[i])->prescale();
      //cout << prescaleList[i] << "   " << value_prescale[i] << endl;
    }
     else{
      value_prescale[i] = -9999.;
    }
  }

  // Matching
  const pat::helper::TriggerMatchHelper matchHelper;
  const pat::TriggerObjectRefVector trigRefs( patTriggerEvent->objects( trigger::TriggerJet ) );
  value_hltjet_n = 0;
  for ( pat::TriggerObjectRefVector::const_iterator it = trigRefs.begin(); it != trigRefs.end(); ++it )
  {
    //cout << endl << "Jet trigger" << (*it)->px() << (*it)->py() << (*it)->pz() << (*it)->energy() << endl;
    if (pTrigEvt.objectInFilter( (*it), "hltQuadJet40IsoPFTau40") ||
       pTrigEvt.objectInFilter( (*it), "hltQuadJet45IsoPFTau45")){

      //cout << endl << "Jet trigger in filter" << (*it)->px() << (*it)->py() << (*it)->pz() << (*it)->energy() << endl;
      value_hltjet_pt[value_hltjet_n] = (*it)->pt();
      value_hltjet_px[value_hltjet_n] = (*it)->px();
      value_hltjet_py[value_hltjet_n] = (*it)->py();
      value_hltjet_pz[value_hltjet_n] = (*it)->pz();
      value_hltjet_e[value_hltjet_n] = (*it)->energy();
      value_hltjet_n++;


    }
  }

  const pat::TriggerObjectRefVector trigRefs2( patTriggerEvent->objects( trigger::TriggerTau ) );
  value_hlttau_n = 0;
  for ( pat::TriggerObjectRefVector::const_iterator it = trigRefs2.begin(); it != trigRefs2.end(); ++it )
  {
    //cout << endl << "Tau trigger" << (*it)->px() << (*it)->py() << (*it)->pz() << (*it)->energy() << endl;
    if (pTrigEvt.objectInFilter( (*it), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40") ||
        pTrigEvt.objectInFilter( (*it), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45") ||
        pTrigEvt.objectInFilter( (*it), "hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45")
       ){
      //cout << endl << "Tau trigger in filter" << (*it)->px() << (*it)->py() << (*it)->pz() << (*it)->energy() << endl;
      value_hlttau_pt[value_hlttau_n] = (*it)->pt();
      value_hlttau_px[value_hlttau_n] = (*it)->px();
      value_hlttau_py[value_hlttau_n] = (*it)->py();
      value_hlttau_pz[value_hlttau_n] = (*it)->pz();
      value_hlttau_e[value_hlttau_n] = (*it)->energy();
      value_hlttau_n++;
    }
  }

  ////////////////////////////
  // Primary Vertex & Beamspot
  ////////////////////////////

  Handle<VertexCollection> vertices;
  evt.getByLabel(InputTag("goodOfflinePrimaryVertices"), vertices);

  value_ve_n = vertices->size();
  value_ve_x = vertices->begin()->x();
  value_ve_y = vertices->begin()->y();
  value_ve_z = vertices->begin()->z();
  value_ve_xerr = vertices->begin()->xError();
  value_ve_yerr = vertices->begin()->yError();
  value_ve_zerr = vertices->begin()->zError();
  value_ve_chi2 = vertices->begin()->chi2();
  value_ve_isFake = vertices->begin()->isFake();
  value_ve_isValid = vertices->begin()->isValid();
  value_ve_ndof = vertices->begin()->ndof();
  value_ve_normalizedChi2 = vertices->begin()->normalizedChi2();
  value_ve_numberOfTracks = vertices->begin()->tracksSize();
  value_ve_rho = vertices->begin()->position().rho();

  // Beam spot
  Handle < reco::BeamSpot > beamSpot;
  evt.getByLabel(InputTag("offlineBeamSpot"), beamSpot);
  Point bspot = beamSpot.isValid ()? beamSpot->position () : Point (0, 0, 0);
  value_ve_bsx = bspot.X();
  value_ve_bsy = bspot.Y();
  value_ve_bsz = bspot.Z();

  ///////////////////////////
  // Muons
  ///////////////////////////
  // Run 1 Top recommendation: https://twiki.cern.ch/twiki/bin/view/CMS/TopMUORun1
  // 2011 Data recommentation: https://twiki.cern.ch/twiki/bin/view/CMS/TWikiTopRefEventSel2011#Basic_objects_general_info
  // https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/TopQuarkAnalysis/Configuration/python/patRefSel_refMuJets.py
  Handle<vector<pat::Muon>> muons;
  evt.getByLabel(InputTag("selectedPatMuonsPF"), muons);
  value_mu_n = 0;
  for (auto it = muons->begin(); it != muons->end(); it++) {
    if ((it->pt() > muon_cut_pt) && (abs (it->eta ()) < muon_cut_eta )) {

      value_mu_isPFMuon[value_mu_n] = it->isPFMuon();
      value_mu_isGlobalMuon[value_mu_n] = it->isGlobalMuon();
      value_mu_isTrackerMuon[value_mu_n] = it->isTrackerMuon();
      value_mu_isStandAloneMuon[value_mu_n] = it->isStandAloneMuon();
      value_mu_pt[value_mu_n] = it->pt();
      value_mu_px[value_mu_n] = it->px();
      value_mu_py[value_mu_n] = it->py();
      value_mu_pz[value_mu_n] = it->pz();
      value_mu_e[value_mu_n] = it->energy();
      value_mu_eta[value_mu_n] = it->eta();
      value_mu_phi[value_mu_n] = it->phi();
      value_mu_mass[value_mu_n] = it->mass();
      value_mu_charge[value_mu_n] = it->charge();
      value_mu_x[value_mu_n] = it->vx();
      value_mu_y[value_mu_n] = it->vy();
      value_mu_z[value_mu_n] = it->vz();
      value_mu_TrkIso03[value_mu_n] = it->trackIso();
      value_mu_CaloIso03[value_mu_n] = it->caloIso();
      value_mu_ECaloIso03[value_mu_n] = it->ecalIso();
      value_mu_HCaloIso03[value_mu_n] = it->trackIso();
      value_mu_neutralHadronIso[value_mu_n] = it->neutralHadronIso();
      value_mu_chargedHadronIso[value_mu_n] = it->chargedHadronIso();
      value_mu_puChargedHadronIso[value_mu_n] = it->puChargedHadronIso();
      value_mu_photonIso[value_mu_n] = it->photonIso();
      value_mu_trackIso[value_mu_n] = it->trackIso();
      auto globalTrack = it->globalTrack();
      if (globalTrack.isNonnull ()) {
        value_mu_dxy[value_mu_n] = globalTrack->dxy (bspot);
        value_mu_chi2[value_mu_n] = globalTrack->normalizedChi2 ();
      }else{
        value_mu_dxy[value_mu_n] = -999.;
        value_mu_chi2[value_mu_n] = -999.;
      }

      if(!isData){
        if(it->genLepton() != 0){
          value_mu_genpx[value_mu_n] = it->genLepton()->px();
          value_mu_genpy[value_mu_n] = it->genLepton()->py();
          value_mu_genpz[value_mu_n] = it->genLepton()->pz();
          value_mu_gene[value_mu_n] = it->genLepton()->energy();
        }
      }

      value_mu_n++;
    }
  }


  ///////////////////////////
  // Electron
  ///////////////////////////
  // Run 1: https://twiki.cern.ch/twiki/bin/view/CMS/TopEGMRun1#Electron_Object_Selection
  // 2011 data: https://twiki.cern.ch/twiki/bin/view/CMS/TWikiTopRefEventSel2011#Basic_objects_general_info
  Handle<vector<pat::Electron>> electrons;
  evt.getByLabel(InputTag("selectedPatElectronsPF"), electrons);
  value_el_n = 0;
  for (auto it = electrons->begin(); it != electrons->end(); it++) {
    if ((it->pt() > electron_cut_pt) && (abs (it->eta ()) < electron_cut_eta )) {

      value_el_pt[value_el_n] = it->pt();
      value_el_px[value_el_n] = it->px();
      value_el_py[value_el_n] = it->py();
      value_el_pz[value_el_n] = it->pz();
      value_el_e[value_el_n] = it->energy();
      value_el_eta[value_el_n] = it->eta();
      value_el_phi[value_el_n] = it->phi();
      value_el_mass[value_el_n] = it->mass();
      value_el_charge[value_el_n] = it->charge();
      value_el_x[value_el_n] = it->vx();
      value_el_y[value_el_n] = it->vy();
      value_el_z[value_el_n] = it->vz();
      value_el_TrkIso03[value_el_n] = it->trackIso();
      value_el_CaloIso03[value_el_n] = it->caloIso();
      value_el_ECaloIso03[value_el_n] = it->ecalIso();
      value_el_HCaloIso03[value_el_n] = it->trackIso();
      value_el_neutralHadronIso[value_el_n] = it->neutralHadronIso();
      value_el_chargedHadronIso[value_el_n] = it->chargedHadronIso();
      value_el_puChargedHadronIso[value_el_n] = it->puChargedHadronIso();
      value_el_photonIso[value_el_n] = it->photonIso();
      value_el_trackIso[value_el_n] = it->trackIso();
      value_el_elecIdLoose[value_el_n] = it->electronID ("eidLoose");
      value_el_elecIdTight[value_el_n] = it->electronID ("eidTight");
      value_el_elecIdRobustLoose[value_el_n] = it->electronID ("eidRobustLoose");
      value_el_elecIdRobustTight[value_el_n] = it->electronID ("eidRobustTight");
      value_el_cutbasedid[value_el_n] = it->passingCutBasedPreselection();
      //value_el_elecIdWP90_r[value_el_n] = it->electronID("simpleEleId90relIso");
      //value_el_elecIdWP90_c [value_el_n] = it->electronID("simpleEleId90cIso");
      auto trk = it->gsfTrack();
      if (trk.isNonnull ()) {
        value_el_dxy[value_el_n] = trk->dxy (bspot);
        value_el_chi2[value_el_n] = trk->normalizedChi2 ();
      }else{
        value_el_dxy[value_el_n] = -999.;
        value_el_chi2[value_el_n] = -999.;
      }

      if(!isData){
        if(it->genLepton() != 0){
          value_el_genpx[value_el_n] = it->genLepton()->px();
          value_el_genpy[value_el_n] = it->genLepton()->py();
          value_el_genpz[value_el_n] = it->genLepton()->pz();
          value_el_gene[value_el_n] = it->genLepton()->energy();
        }
      }

      value_el_n++;
    }
  }

  ///////////////////////////
  // Tau
  ///////////////////////////
  //https://twiki.cern.ch/twiki/bin/view/CMS/TopTAURun1
  // TODO: check switchToPFTauHPS(process)
  Handle<vector<pat::Tau>> taus;
  evt.getByLabel(InputTag("selectedPatTausPF"), taus);
  value_tau_n = 0;
  int iTau=0;
  for (auto it = taus->begin(); it != taus->end(); it++) {

    //cout<< " Found tau " << endl;

    if ((it->pt() > tau_cut_pt) && (abs (it->eta ()) < tau_cut_eta )) {
      //std::cout << std::endl << it->pt() << std::endl;
      value_tau_pt[value_tau_n] = it->pt();
      value_tau_px[value_tau_n] = it->px();
      value_tau_py[value_tau_n] = it->py();
      value_tau_pz[value_tau_n] = it->pz();
      value_tau_e[value_tau_n] = it->energy();
      value_tau_eta[value_tau_n] = it->eta();
      value_tau_phi[value_tau_n] = it->phi();
      value_tau_mass[value_tau_n] = it->mass();
      value_tau_charge[value_tau_n] = it->charge();
      value_tau_x[value_tau_n] = it->vx();
      value_tau_y[value_tau_n] = it->vy();
      value_tau_z[value_tau_n] = it->vz();
      value_tau_decaymode[value_tau_n] = it->decayMode();

      if( it->leadPFChargedHadrCand().isAvailable()) {
        if (it->leadPFChargedHadrCand()->trackRef().isNonnull()){
          value_tau_leadTrackPt[value_tau_n] = it->leadPFChargedHadrCand()->trackRef()->pt();
          value_tau_dxy[value_tau_n] = it->leadPFChargedHadrCand()->trackRef()->dxy(bspot);
        }else if  (it->leadPFChargedHadrCand()->gsfTrackRef().isNonnull()){
          value_tau_leadTrackPt[value_tau_n] = it->leadPFChargedHadrCand()->gsfTrackRef()->pt();
          value_tau_dxy[value_tau_n] = it->leadPFChargedHadrCand()->gsfTrackRef()->dxy(bspot);
        }else{
         value_tau_leadTrackPt[value_tau_n] = -999.;
         value_tau_dxy[value_tau_n] = -999.;
        }
      }else {
        value_tau_leadTrackPt[value_tau_n] = -999.;
        value_tau_dxy[value_tau_n] = -999.;
      }

      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauIDRecommendation
      auto tauIDs = it->tauIDs();
      for (unsigned int i = 0; i < tauIDs.size (); i++) {
        //std::cout << "tau discr " << tauIDs[i].first << " value " << tauIDs[i].second << std::endl;
        if(tauIDs[i].first == "againstElectronLoose") value_tau_againstElectronLoose[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "againstElectronMedium") value_tau_againstElectronMedium[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "againstElectronTight") value_tau_againstElectronTight[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "againstMuonLoose") value_tau_againstMuonLoose[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "againstMuonMedium") value_tau_againstMuonMedium[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "againstMuonTight") value_tau_againstMuonTight[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "byLooseCombinedIsolationDeltaBetaCorr") value_tau_byLooseCombinedIsolationDeltaBetaCorr[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "byMediumCombinedIsolationDeltaBetaCorr") value_tau_byMediumCombinedIsolationDeltaBetaCorr[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "byTightCombinedIsolationDeltaBetaCorr") value_tau_byTightCombinedIsolationDeltaBetaCorr[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "byLooseCombinedIsolationDeltaBetaCorr3Hits") value_tau_byLooseCombinedIsolationDeltaBetaCorr3Hits[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "byMediumCombinedIsolationDeltaBetaCorr3Hits") value_tau_byMediumCombinedIsolationDeltaBetaCorr3Hits[value_tau_n] = tauIDs[i].second;
        if(tauIDs[i].first == "byTightCombinedIsolationDeltaBetaCorr3Hits") value_tau_byTightCombinedIsolationDeltaBetaCorr3Hits[value_tau_n] = tauIDs[i].second;
      }


      const pat::TriggerObjectRef trigRef40( matchHelper.triggerMatchObject( taus, iTau,
                                              "tauMatchHLTTausPF", evt, pTrigEvt ) );
      if ( trigRef40.isAvailable() ) {
        //cout<< endl << "Trigref 40 avail" << endl;
        value_tau_hlt40px[value_tau_n] = trigRef40->px();
        value_tau_hlt40py[value_tau_n] = trigRef40->py();
        value_tau_hlt40pz[value_tau_n] = trigRef40->pz();
        value_tau_hlt40e[value_tau_n] = trigRef40->energy();
      }else{
        //std::cout << std::endl << "No trigger match" << std::endl;
        value_tau_hlt40px[value_tau_n] = -999.;
        value_tau_hlt40py[value_tau_n] = -999.;
        value_tau_hlt40pz[value_tau_n] = -999.;
        value_tau_hlt40e[value_tau_n] = -999.;
      }

      const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( taus, iTau,
                                              "tauMatchHLTTaus45PF", evt, pTrigEvt ) );
      if ( trigRef45.isAvailable() ) {
        //std::cout << std::endl << "Found trigger match" << std::endl;
        value_tau_hlt45px[value_tau_n] = trigRef45->px();
        value_tau_hlt45py[value_tau_n] = trigRef45->py();
        value_tau_hlt45pz[value_tau_n] = trigRef45->pz();
        value_tau_hlt45e[value_tau_n] = trigRef45->energy();
      }else{
        //std::cout << std::endl << "No trigger match" << std::endl;
        value_tau_hlt45px[value_tau_n] = -999.;
        value_tau_hlt45py[value_tau_n] = -999.;
        value_tau_hlt45pz[value_tau_n] = -999.;
        value_tau_hlt45e[value_tau_n] = -999.;
      }

      if(!isData){
        if(it->genLepton() != 0){
          value_tau_genpx[value_tau_n] = it->genLepton()->px();
          value_tau_genpy[value_tau_n] = it->genLepton()->py();
          value_tau_genpz[value_tau_n] = it->genLepton()->pz();
          value_tau_gene[value_tau_n] = it->genLepton()->energy();

          Handle < reco::GenParticleCollection > genParticles;
          evt.getByLabel ("genParticles", genParticles);

          value_tau_decay[value_tau_n] = getTauDecay(genParticles, &(*it));
          cout<< " Found tau with decay number" << value_tau_decay[value_tau_n] << endl;

        }

        int leptonOrigin = 0;
        reco::GenParticleRef tauref = it->genParticleById(15,0,false);
        if (tauref.isNonnull()) { leptonOrigin = 15;   }
        reco::GenParticleRef tauref2 = it->genParticleById(-15,0,false);
        if (tauref2.isNonnull()){ leptonOrigin = -15;  }
        reco::GenParticleRef elref = it->genParticleById(11,0,false);
        if (elref.isNonnull()  && elref->pt()>10.)  { leptonOrigin = 11;  }
        reco::GenParticleRef elref2 = it->genParticleById(-11,0,false);
        if (elref2.isNonnull() && elref2->pt()>10.) { leptonOrigin = -11; }
        reco::GenParticleRef muref = it->genParticleById(13,0,false);
        if (muref.isNonnull()  && muref->pt()>10.)  { leptonOrigin = 13;  }
        reco::GenParticleRef muref2 = it->genParticleById(-13,0,false);
        if (muref2.isNonnull() && muref2->pt()>10.) { leptonOrigin = -13; }
        value_tau_leptonOrigin[value_tau_n] = leptonOrigin;

        cout<< " Found tau with lepton origin" << leptonOrigin << endl;

      }
      value_tau_n++;
    }
    iTau++;
  }

  ///////////////////////////
  // MET
  ///////////////////////////
  edm::Handle<std::vector<pat::MET> > met;
  evt.getByLabel(InputTag("patMETsPF"), met);
  value_met_pt = met->begin()->pt();
  value_met_px = met->begin()->px();
  value_met_py = met->begin()->py();
  value_met_pz = met->begin()->pz();
  value_met_e = met->begin()->energy();
  value_met_phi = met->begin()->phi();
  value_met_sumet = met->begin()->sumEt();
  value_met_significance = met->begin()->significance();
  auto cov = met->begin()->getSignificanceMatrix();
  value_met_covxx = cov[0][0];
  value_met_covxy = cov[0][1];
  value_met_covyy = cov[1][1];

  ///////////////////////////
  // Jets
  ///////////////////////////
  // Jet Met Run 1: https://twiki.cern.ch/twiki/bin/view/CMS/TopJMERun1
  // 2011 data: https://twiki.cern.ch/twiki/bin/view/CMS/TWikiTopRefEventSel2011#Basic_objects_general_info
  Handle<vector<pat::Jet>> jets;
  evt.getByLabel(InputTag("selectedPatJetsPF"), jets);
  value_jet_n = 0;
  int iJet = 0;
  //std::cout << std::endl << jets->size() << std::endl;
  for (auto it = jets->begin(); it != jets->end(); it++) {
    //std::cout << it->pt();
    if ( (it->pt() > jet_cut_pt) && (abs (it->eta ()) < jet_cut_eta )) {

      value_jet_pt[value_jet_n] = it->pt();
      value_jet_px[value_jet_n] = it->px();
      value_jet_py[value_jet_n] = it->py();
      value_jet_pz[value_jet_n] = it->pz();
      value_jet_e[value_jet_n] = it->energy();
      value_jet_eta[value_jet_n] = it->eta();
      value_jet_phi[value_jet_n] = it->phi();
      value_jet_mass[value_jet_n] = it->mass();
      value_jet_charge[value_jet_n] = it->charge();
      value_jet_x[value_jet_n] = it->vx();
      value_jet_y[value_jet_n] = it->vy();
      value_jet_z[value_jet_n] = it->vz();
      value_jet_scale[value_jet_n] = it->jecFactor("Uncorrected");
      value_jet_tcDisc[value_jet_n] = it->bDiscriminator ("trackCountingHighEffBJetTags");
      value_jet_svDisc[value_jet_n] = it->bDiscriminator ("simpleSecondaryVertexBJetTags");
      value_jet_svEffDisc[value_jet_n] = it->bDiscriminator ("simpleSecondaryVertexHighEffBJetTags");
      value_jet_smDisc[value_jet_n] = it->bDiscriminator ("softMuonBJetTags");

      //cout<< endl << "bTag " << it->bDiscriminator ("simpleSecondaryVertexHighEffBJetTags") <<endl;
      //cout<< endl << "bTag " << it->bDiscriminator ("trackCountingHighEffBJetTags") <<endl;
      //cout<< endl << "bTag " << it->bDiscriminator ("simpleSecondaryVertexBJetTags") <<endl;
      //cout<< endl << "bTag " << it->bDiscriminator ("softMuonBJetTags") <<endl;


      const pat::TriggerObjectRef trigRef40( matchHelper.triggerMatchObject( jets, iJet,
                                              "jetMatchHLTJetsPF", evt, pTrigEvt ) );
      if ( trigRef40.isAvailable() ) {
        //cout<< endl << "Trigref 40 avail" << endl;
        value_jet_hlt40px[value_jet_n] = trigRef40->px();
        value_jet_hlt40py[value_jet_n] = trigRef40->py();
        value_jet_hlt40pz[value_jet_n] = trigRef40->pz();
        value_jet_hlt40e[value_jet_n] = trigRef40->energy();
      }else{
        //std::cout << std::endl << "Found no match" << std::endl;
        value_jet_hlt40px[value_jet_n] = -9999;
        value_jet_hlt40py[value_jet_n] = -9999;
        value_jet_hlt40pz[value_jet_n] = -9999;
        value_jet_hlt40e[value_jet_n] = -9999;
      }


      const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( jets, iJet,
                                              "jetMatchHLTJets45PF", evt, pTrigEvt ) );
      if ( trigRef45.isAvailable() ) {
        //std::cout << std::endl << "Found trigger match" << std::endl;
        value_jet_hlt45px[value_jet_n] = trigRef45->px();
        value_jet_hlt45py[value_jet_n] = trigRef45->py();
        value_jet_hlt45pz[value_jet_n] = trigRef45->pz();
        value_jet_hlt45e[value_jet_n] = trigRef45->energy();
      }else{
        //std::cout << std::endl << "Found no match" << std::endl;
        value_jet_hlt45px[value_jet_n] = -9999;
        value_jet_hlt45py[value_jet_n] = -9999;
        value_jet_hlt45pz[value_jet_n] = -9999;
        value_jet_hlt45e[value_jet_n] = -9999;
      }
      value_jet_n++;
    }

    if(!isData){
      if(it->genJet() != 0){
        value_jet_genpx[value_jet_n] = it->genJet()->px();
        value_jet_genpy[value_jet_n] = it->genJet()->py();
        value_jet_genpz[value_jet_n] = it->genJet()->pz();
        value_jet_gene[value_jet_n] = it->genJet()->energy();
      }
      if(it->genParton() != 0 ){
        value_jet_genpartonpx[value_jet_n] = it->genParton()->px();
        value_jet_genpartonpy[value_jet_n] = it->genParton()->py();
        value_jet_genpartonpz[value_jet_n] = it->genParton()->pz();
        value_jet_genpartone[value_jet_n] = it->genParton()->energy();
      }
    }

    iJet++;
  }


  ///////////////////////////
  // TTbar event
  ///////////////////////////


  if( isData == false){



    Handle < reco::GenParticleCollection > genParticles;
    evt.getByLabel ("genParticles", genParticles);

    int tmeme = GetTTbarTruth(genParticles);
    std::cout << "TMEME" << tmeme << std::endl;


    /*
    edm::Handle<TtGenEvent> genEvent;
    evt.getByLabel("genEvt", genEvent);
    if( genEvent->isTtBar() ){

      value_nLep_ = genEvent->numberOfLeptons();
      value_topPt_ = genEvent->top()->pt();
      value_topEta_ = genEvent->top()->eta();
      value_topPhi_ = genEvent->top   ()->phi();
      value_topBarPt_ = genEvent->topBar()->pt();
      value_topBarEta_ = genEvent->topBar()->eta();
      value_topBarPhi_ = genEvent->topBar()->phi();
      value_ttbarPt_ = genEvent->topPair()->pt();
      value_ttbarEta_ = genEvent->topPair()->eta();
      value_ttbarPhi_ = genEvent->topPair()->phi();

    }*/

  }

  tree->Fill();

}


int TopTauAnalyze::getTauDecay(edm::Handle<reco::GenParticleCollection> genParticles, const pat::Tau *thePatTau){

  int tauDecay = -999;
  bool matchedGenLepton = false;
  reco::GenParticleCollection::const_iterator pMatched;

  for (reco::GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++p){
    reco::Candidate * aGenTau = (dynamic_cast<reco::Candidate *>(const_cast<reco::GenParticle *>(&*p)));


    if (abs(p->pdgId()) == 15 && p->status() == 2){

      std::cout << "Stable tau" << std::endl;
      std::cout << "Taugen " << aGenTau->pt() << " Taupat " << thePatTau->genLepton()->pt() << std::endl;

      if ((thePatTau->genLepton() != NULL) && abs(thePatTau->genLepton()->pt()-aGenTau->pt()) < 0.00001){
        matchedGenLepton = true;
        pMatched = p;
      }
    }
  }

  if (matchedGenLepton){
    tauDecay = -99;

    int tau_children_n = pMatched->numberOfDaughters();
    int sumPdgId = 0;

       for (int k=0; k<tau_children_n; k++)
       {
         int dpdgId = abs(pMatched->daughter(k)->pdgId());
         sumPdgId += dpdgId;

         if (dpdgId == 223 || dpdgId == 221 || dpdgId == 213 || dpdgId == 113 || dpdgId == 323)
         {
           if(pMatched->daughter(k)->status() != 1)
           {
           sumPdgId -= dpdgId;

           for (unsigned int ww=0; ww<pMatched->daughter(k)->numberOfDaughters(); ww++)
           {
            sumPdgId += abs(pMatched->daughter(k)->daughter(ww)->pdgId());

            if (abs(pMatched->daughter(k)->daughter(ww)->pdgId())==311 && pMatched->daughter(k)->daughter(ww)->status()!=1)
            {
              sumPdgId -= 311;
              for (unsigned int v=0; v<pMatched->daughter(k)->daughter(ww)->numberOfDaughters(); v++)
              {
               sumPdgId += pMatched->daughter(k)->daughter(ww)->daughter(v)->pdgId();
              }
             }
            }
           }
          }
         }


      if (sumPdgId ==227)                                                                       { tauDecay = 0;  }//pi+nu
      if (sumPdgId ==229)                                                                       { tauDecay = 1;  }//pi+pi0nu
      if (sumPdgId ==449 || sumPdgId ==338 || sumPdgId ==340)                                   { tauDecay = 2;  }//pi+2pi0nu
      if (sumPdgId ==560)                                                                       { tauDecay = 3;  }//pi+3pi0nu
      if (sumPdgId ==671)                                                                       { tauDecay = 4;  }//pi+4pi0nu
      if (sumPdgId ==315)                                                                       { tauDecay = 5;  }//pi+gammanu
      if (sumPdgId ==360 || sumPdgId ==382)                                                     { tauDecay = 6;  }//pi+pi0nugamma(s)
      if (sumPdgId ==537 || sumPdgId ==357 || sumPdgId ==538)                                   { tauDecay = 7;  }//pi+k0nu
      if (sumPdgId ==468 || sumPdgId ==648 || sumPdgId ==487 || sumPdgId==667 || sumPdgId ==847){ tauDecay = 8;  }//pi+2n(w K0)nu
      if (sumPdgId ==760 || sumPdgId ==769 || sumPdgId ==759)                                   { tauDecay = 9;  }//pi+3n(w pi0)nu
      if (sumPdgId ==471)                                                                       { tauDecay = 10; }//pi+2pi0nugamma

      if (sumPdgId ==649)                                                                       { tauDecay = 30; }//3pi+nu
      if (sumPdgId ==760)                                                                       { tauDecay = 31; }//3pi+pi0nu
      if (sumPdgId ==782)                                                                       { tauDecay = 34; }//3pi+pi0nugamma
      if (sumPdgId ==871)                                                                       { tauDecay = 32; }//3pi+2pi0nu
      if (sumPdgId ==982)                                                                       { tauDecay = 33; }//3pi+3pi0nu

      if (sumPdgId ==337)                                                                       { tauDecay = 20; }//k+nu
      if (sumPdgId ==448)                                                                       { tauDecay = 21; }//k+pi0nu
      if (sumPdgId ==467 || sumPdgId ==647)                                                     { tauDecay = 22; }//k+k0nu
      if (sumPdgId ==559)                                                                       { tauDecay = 23; }//k+2pi0nu
      if (sumPdgId ==578 || sumPdgId ==758)                                                     { tauDecay = 24; }//k+pi0k0nu
      if (sumPdgId ==670)                                                                       { tauDecay = 25; }//k+3pi0nu

      if (sumPdgId ==869)                                                                       { tauDecay = 40; }//k+k+pi+nu

      if (sumPdgId ==1071)                                                                      { tauDecay = 50; }//5pinu
      if (sumPdgId ==1182)                                                                      { tauDecay = 51; }//5pipi0nu

      if (sumPdgId ==39)                                                                        { tauDecay = 100;}//enunu
      if (sumPdgId ==43)                                                                        { tauDecay = 200;}//mununu

   }

  return tauDecay;
}


int TopTauAnalyze::GetTTbarTruth (edm::Handle < reco::GenParticleCollection > genParticles){


  double Qlep1 = 0;
  double Qlep2 = 0;
  const reco::Candidate *lep1 = 0;
  const reco::Candidate *lep2 = 0;
  const reco::Candidate *W1 = 0;
  const reco::Candidate *W2 = 0;
  const reco::Candidate *Nu1 = 0;
  const reco::Candidate *Nu2 = 0;
  const reco::Candidate *Top1 = 0;
  const reco::Candidate *Top2 = 0;
  const reco::Candidate *B1 = 0;
  const reco::Candidate *B2 = 0;
  const reco::Candidate *B1Rad = 0;
  const reco::Candidate *B2Rad = 0;
  const reco::Candidate *TauNu1 = 0;
  const reco::Candidate *TauNu2 = 0;
  const reco::Candidate *TauANu1 = 0;
  const reco::Candidate *TauANu2 = 0;


  //---------------------------------------------------------------------------
  //                 decay channel: (DecChan)
  //---------------------------------------------------------------------------


  // Define the type of tt events final states
  //
  //   TMEME variable means :
  //
  //  T = # of taus from W decays
  //  M = # of muons from W->tau->muon
  //  E = # of electrons from W->tau->electron
  //  M = # of muons from W->muon
  //  E = # of electrons from W->electron
  //

  int tmeme = 0;

  for (size_t i = 0; i < genParticles->size (); ++i) {
    const reco::GenParticle & paIt = (*genParticles)[i];
    const reco::Candidate *W = &(*genParticles)[i];


  //    // finding top and antitop
  //    //------------------------
    if (abs (paIt.pdgId ()) == 6 && paIt.status () == 3) {
      if (paIt.pdgId () == 6) {
        Top1 = &(*genParticles)[i];
      }
      if (paIt.pdgId () == -6) {
        Top2 = &(*genParticles)[i];
      }
    }

  //       // finding B1 & B2:
  //       //------------------------
    if (abs (paIt.pdgId ()) == 6 && paIt.status () == 3) {
      int FoundWB = 0;
      int FoundB1 = -1;
      int FoundB2 = -1;
      for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
        if (abs (paIt.daughter (j)->pdgId ()) == 24 || abs (paIt.daughter (j)->pdgId ()) == 5) {
          FoundWB++;
          if (paIt.daughter (j)->pdgId () == 5)
            FoundB1 = j;
          if (paIt.daughter (j)->pdgId () == -5)
            FoundB2 = j;
        }
      }
      if (FoundWB >= 2) {
        if (FoundB1 >= 0)
          B1 = paIt.daughter (FoundB1);
        if (FoundB2 >= 0)
          B2 = paIt.daughter (FoundB2);
      }
    }

    if (B1) {
      for (unsigned int j = 0; j < B1->numberOfDaughters (); j++) {
        if (abs (B1->daughter (j)->pdgId ()) == 5 && B1->daughter (j)->status () == 2)
          B1Rad = B1->daughter (j);
      }
    }

    if (B2) {
      for (unsigned int j = 0; j < B2->numberOfDaughters (); j++) {
        if (abs (B2->daughter (j)->pdgId ()) == 5 && B2->daughter (j)->status () == 2)
          B2Rad = B2->daughter (j);
      }
    }

    if (abs (paIt.pdgId ()) != 24)
      continue;
    if (paIt.status () != 3)
      continue;

    for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
      if (abs (paIt.daughter (j)->pdgId ()) == 12 || abs (paIt.daughter (j)->pdgId ()) == 14 || abs (paIt.daughter (j)->pdgId ()) == 16) {
  //     // finding nue/antinue ( t->W->nue )
  //     //--------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 12) {
          if (paIt.daughter (j)->pdgId () == -12) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 12) {
            Nu2 = paIt.daughter (j);
          }
        }
  //     // finding numu/antinumu ( t->W->numu )
  //     //------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 14) {
          if (paIt.daughter (j)->pdgId () == -14) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 14) {
            Nu2 = paIt.daughter (j);
          }
        }
  //     // finding nutau/antinutau ( t->W->nutau )
  //     //---------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 16) {
          if (paIt.daughter (j)->pdgId () == -16) {
            Nu1 = paIt.daughter (j);
          }
          if (paIt.daughter (j)->pdgId () == 16) {
            Nu2 = paIt.daughter (j);
          }
        }

      }

      if (abs (paIt.daughter (j)->pdgId ()) == 11 || abs (paIt.daughter (j)->pdgId ()) == 13 || abs (paIt.daughter (j)->pdgId ()) == 15) {

  //     // finding electron/positron ( t->W->e )
  //     //--------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 11) {
          tmeme += 1;
          if (paIt.daughter (j)->pdgId () == 11) {
            W1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 11) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -11) {
            W2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 11) ? -1 : 1;
          }
        }
  //     // finding muon/antimuon ( t->W->muon )
  //     //-------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 13) {
          tmeme += 10;
          if (paIt.daughter (j)->pdgId () == 13) {
            W1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 13) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -13) {
            W2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 13) ? -1 : 1;
          }
        }
  //     // finding tau/antitau ( t->W->tau )
  //     //-------------------------------------
        if (abs (paIt.daughter (j)->pdgId ()) == 15) {
          tmeme += 10000;
          if (paIt.daughter (j)->pdgId () == 15) {
            W1 = &(*genParticles)[i];
            lep1 = paIt.daughter (j);
            Qlep1 = (paIt.daughter (j)->pdgId () == 15) ? -1 : 1;
          }
          if (paIt.daughter (j)->pdgId () == -15) {
            W2 = &(*genParticles)[i];
            lep2 = paIt.daughter (j);
            Qlep2 = (paIt.daughter (j)->pdgId () == 15) ? -1 : 1;
          }
        }


        int indxtau = -1;
        for (unsigned int k = 0; k < W->numberOfDaughters (); k++) {
          if (abs (W->daughter (k)->pdgId ()) == 15)
            indxtau = k;
        }
        while (indxtau >= 0) {
          //if (!W)
            //cout << "NULL " << endl;
          bool FoundTau = false;
          for (unsigned int k = 0; k < W->numberOfDaughters (); k++) {
            if (abs (W->daughter (k)->pdgId ()) == 24)
              continue;
            if (abs (W->daughter (k)->pdgId ()) == 11 || abs (W->daughter (k)->pdgId ()) == 13) {
              if (abs (W->daughter (k)->pdgId ()) == 11)
                tmeme += 100;
              if (abs (W->daughter (k)->pdgId ()) == 13)
                tmeme += 1000;
              indxtau = -1;
            }
            if (abs (W->daughter (k)->pdgId ()) == 15) {
              indxtau = k;
              FoundTau = true;
            }

            if (abs (W->pdgId ()) == 15) {
              if (W->daughter (k)->pdgId () == 16) {
                TauNu1 = W->daughter (k);
              }
              else if (W->daughter (k)->pdgId () == -16) {
                TauANu1 = W->daughter (k);
              }
              else if (W->daughter (k)->pdgId () == 12 || W->daughter (k)->pdgId () == 14) {
                TauNu2 = W->daughter (k);
              }
              else if (W->daughter (k)->pdgId () == -12 || W->daughter (k)->pdgId () == -14) {
                TauANu2 = W->daughter (k);
              }
            }
          }
          if (FoundTau) {
            W = W->daughter (indxtau);
          }
          else {
            indxtau = -1;
          }
        }
      }
    }
  }

  // FIXME hack to avoid warnings for unused variables
  (void)Qlep1;
  (void)Qlep2;
  (void)lep1;
  (void)lep2;
  (void)W1;
  (void)W2;
  (void)Nu1;
  (void)Nu2;
  (void)Top1;
  (void)Top2;
  (void)B1Rad;
  (void)B2Rad;
  (void)TauNu1;
  (void)TauNu2;
  (void)TauANu1;
  (void)TauANu2;

  return tmeme;

}

// ------------ method called when starting to processes a luminosity block  ------------
void TopTauAnalyze::beginLuminosityBlock(const edm::LuminosityBlock & lumi, const edm::EventSetup & setup)
{

  /*
  edm::Handle<edm::MergeableCounter> nEventsTotalCounter;
  //nEventsTotalCounter.getByLabel(lumi,"nEventsFiltered");
  lumi.getByLabel(edm::InputTag("nEventsFiltered"), nEventsTotalCounter);
  std::cout << "*****************************" << std::endl;
  std::cout << "NEVENTS " << nEventsTotalCounter->value << std::endl;
  std::cout << "*****************************" << std::endl;
  //nEventsTotal += nEventsTotalCounter->value;

  /*
  edm::Handle<int> nEventsFilteredCounter;
  lumi.getByLabel("nEventsFiltered", nEventsFilteredCounter);
  //nEventsFiltered += nEventsFilteredCounter->value;
  */
}


// ------------ method called when ending the processing of a luminosity block  ------------
void TopTauAnalyze::endLuminosityBlock(const edm::LuminosityBlock & lumi, const edm::EventSetup & setup)
{

  edm::Handle<edm::MergeableCounter> nEventsTotalCounter;
  lumi.getByLabel(edm::InputTag("nEventsTotal"), nEventsTotalCounter);
  std::cout << "*****************************" << std::endl;
  std::cout << "NEVENTS " << nEventsTotalCounter->value << std::endl;
  std::cout << "*****************************" << std::endl;
  nEventsTotal += nEventsTotalCounter->value;

  edm::Handle<edm::MergeableCounter> nEventsFilteredCounter;
  lumi.getByLabel("nEventsFiltered", nEventsFilteredCounter);
  nEventsFiltered += nEventsFilteredCounter->value;

}



void TopTauAnalyze::beginJob()
{
}

void TopTauAnalyze::endJob()
{

  // Fill information about the analysis of the file
  info->Fill();

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TopTauAnalyze);
                                                        
    
