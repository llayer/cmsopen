#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
#include "TopTauAnalyze.h"

TopTauAnalyze::TopTauAnalyze(const edm::ParameterSet& cfg):
  vertices_(cfg.getParameter<edm::InputTag>("vertices")),
  muons_(cfg.getParameter<edm::InputTag>("muons")),
  electrons_(cfg.getParameter<edm::InputTag>("electrons")),
  met_(cfg.getParameter<edm::InputTag>("met")),
  taus_(cfg.getParameter<edm::InputTag>("taus")),
  jets_(cfg.getParameter<edm::InputTag>("jets")),
  genEvent_(cfg.getParameter<edm::InputTag>("genEvent"))
{
  edm::Service<TFileService> fs;

  tree = fs->make<TTree>("Events", "Events");

  // Event information
  tree->Branch("run", &value_run);
  tree->Branch("luminosityBlock", &value_lumi_block);
  tree->Branch("event", &value_event);

  // Vertices
  tree->Branch("PV_npvs", &value_ve_n, "PV_npvs/I");
  tree->Branch("PV_x", &value_ve_x, "PV_x/F");
  tree->Branch("PV_y", &value_ve_y, "PV_y/F");
  tree->Branch("PV_z", &value_ve_z, "PV_z/F");

  // Muons
  tree->Branch("nMuon", &value_mu_n, "nMuon/i");
  tree->Branch("Muon_pt", value_mu_pt, "Muon_pt[nMuon]/F");
  tree->Branch("Muon_eta", value_mu_eta, "Muon_eta[nMuon]/F");
  tree->Branch("Muon_phi", value_mu_phi, "Muon_phi[nMuon]/F");
  tree->Branch("Muon_mass", value_mu_mass, "Muon_mass[nMuon]/F");

  // Electrons
  tree->Branch("nElectron", &value_el_n, "nElectron/i");
  tree->Branch("Electron_pt", value_el_pt, "Electron_pt[nElectron]/F");
  tree->Branch("Electron_eta", value_el_eta, "Electron_eta[nElectron]/F");
  tree->Branch("Electron_phi", value_el_phi, "Electron_phi[nElectron]/F");
  tree->Branch("Electron_mass", value_el_mass, "Electron_mass[nElectron]/F");

  // Taus
  tree->Branch("nTau", &value_tau_n, "nTau/i");
  tree->Branch("Tau_pt", value_tau_pt, "Tau_pt[nTau]/F");
  tree->Branch("Tau_eta", value_tau_eta, "Tau_eta[nTau]/F");
  tree->Branch("Tau_phi", value_tau_phi, "Tau_phi[nTau]/F");
  tree->Branch("Tau_mass", value_tau_mass, "Tau_mass[nTau]/F");

  // MET
  tree->Branch("MET_pt", &value_met_pt, "MET_pt/F");
  tree->Branch("MET_phi", &value_met_phi, "MET_phi/F");
  tree->Branch("MET_sumet", &value_met_sumet, "MET_sumet/F");

  // Jets
  tree->Branch("nJet", &value_jet_n, "nJet/i");
  tree->Branch("Jet_pt", value_jet_pt, "Jet_pt[nJet]/F");
  tree->Branch("Jet_eta", value_jet_eta, "Jet_eta[nJet]/F");
  tree->Branch("Jet_phi", value_jet_phi, "Jet_phi[nJet]/F");
  tree->Branch("Jet_mass", value_jet_mass, "Jet_mass[nJet]/F");

  // genEvent
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


  mult_ = fs->make<TH1F>("mult", "multiplicity (taus)", 30,  0 ,   30);
  en_   = fs->make<TH1F>("en"  , "energy (taus)",       60,  0., 300.);
  pt_   = fs->make<TH1F>("pt"  , "pt (taus}",           60,  0., 300.);
  eta_  = fs->make<TH1F>("eta" , "eta (taus)",          30, -3.,   3.);
  phi_  = fs->make<TH1F>("phi" , "phi (taus)",          40, -4.,   4.);
}

TopTauAnalyze::~TopTauAnalyze()
{
}

void
TopTauAnalyze::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{
  std::cout << "Start ana";

  edm::Handle<std::vector<reco::Vertex> > vertices;
  evt.getByLabel(vertices_, vertices);
  edm::Handle<std::vector<pat::Tau> > taus;
  evt.getByLabel(taus_, taus);
  edm::Handle<std::vector<pat::Jet> > jets;
  evt.getByLabel(jets_, jets);
  edm::Handle<std::vector<pat::Electron> > electrons;
  evt.getByLabel(electrons_, electrons);
  edm::Handle<std::vector<pat::Muon> > muons;
  evt.getByLabel(muons_, muons);
  edm::Handle<std::vector<pat::MET> > met;
  evt.getByLabel(met_, met);
  edm::Handle<TtGenEvent> genEvent;
  evt.getByLabel(genEvent_, genEvent);

  mult_->Fill( taus->size() );
  for(std::vector<pat::Tau>::const_iterator tau=taus->begin(); tau!=taus->end(); ++tau){
    en_ ->Fill( tau->energy() );
    pt_ ->Fill( tau->pt()     );
    eta_->Fill( tau->eta()    );
    phi_->Fill( tau->phi()    );
  }

  // Event information
  value_run = evt.run();
  value_lumi_block = evt.luminosityBlock();
  value_event = evt.id().event();

  // Vertex
  value_ve_n = vertices->size();
  value_ve_x = vertices->begin()->x();
  value_ve_y = vertices->begin()->y();
  value_ve_z = vertices->begin()->z();

  // Muon
  value_mu_n = 0;
  const float mu_min_pt = 3;
  for (auto it = muons->begin(); it != muons->end(); it++) {
    if (it->pt() > mu_min_pt) {
      value_mu_pt[value_mu_n] = it->pt();
      value_mu_eta[value_mu_n] = it->eta();
      value_mu_phi[value_mu_n] = it->phi();
      value_mu_mass[value_mu_n] = it->mass();
      value_mu_n++;
    }
  }

  // Electron
  value_el_n = 0;
  const float el_min_pt = 5;
  for (auto it = electrons->begin(); it != electrons->end(); it++) {
    if (it->pt() > el_min_pt) {
      value_el_pt[value_el_n] = it->pt();
      value_el_eta[value_el_n] = it->eta();
      value_el_phi[value_el_n] = it->phi();
      value_el_mass[value_el_n] = it->mass();
      value_el_n++;
    }
  }

  // Tau
  const float tau_min_pt = 15;
  //std::cout << std::endl << tau_min_pt << std::endl;
  value_tau_n = 0;
  for (auto it = taus->begin(); it != taus->end(); it++) {
    if (it->pt() > tau_min_pt) {
      value_tau_pt[value_tau_n] = it->pt();
      value_tau_eta[value_tau_n] = it->eta();
      value_tau_phi[value_tau_n] = it->phi();
      value_tau_mass[value_tau_n] = it->mass();
      value_tau_n++;
    }
  }

  // MET
  value_met_pt = met->begin()->pt();
  value_met_phi = met->begin()->phi();
  value_met_sumet = met->begin()->sumEt();

  // Jets
  const float jet_min_pt = 15;
  value_jet_n = 0;
  for (auto it = jets->begin(); it != jets->end(); it++) {
    //std::cout << it->pt();
    if (it->pt() > jet_min_pt) {
      value_jet_pt[value_jet_n] = it->pt();
      value_jet_eta[value_jet_n] = it->eta();
      value_jet_phi[value_jet_n] = it->phi();
      value_jet_mass[value_jet_n] = it->mass();
      value_jet_n++;
    }
  }

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

  }

  tree->Fill();

}

void TopTauAnalyze::beginJob()
{
}

void TopTauAnalyze::endJob()
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TopTauAnalyze);
