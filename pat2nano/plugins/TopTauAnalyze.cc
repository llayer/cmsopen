#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "TopTauAnalyze.h"

TopTauAnalyze::TopTauAnalyze(const edm::ParameterSet& cfg):
  taus_(cfg.getParameter<edm::InputTag>("taus")),
  jets_(cfg.getParameter<edm::InputTag>("jets"))
{
  edm::Service<TFileService> fs;

  tree = fs->make<TTree>("Events", "Events");
  tree->Branch("nTau", &value_tau_n, "nTau/i");
  tree->Branch("Tau_pt", value_tau_pt, "Tau_pt[nTau]/F");
  tree->Branch("Tau_eta", value_tau_eta, "Tau_eta[nTau]/F");
  tree->Branch("Tau_phi", value_tau_phi, "Tau_phi[nTau]/F");
  tree->Branch("Tau_mass", value_tau_mass, "Tau_mass[nTau]/F");

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
  edm::Handle<std::vector<pat::Tau> > taus;
  edm::Handle<std::vector<pat::Jet> > jets;
  evt.getByLabel(taus_, taus);
  evt.getByLabel(jets_, jets);

  mult_->Fill( taus->size() );
  for(std::vector<pat::Tau>::const_iterator tau=taus->begin(); tau!=taus->end(); ++tau){
    en_ ->Fill( tau->energy() );
    pt_ ->Fill( tau->pt()     );
    eta_->Fill( tau->eta()    );
    phi_->Fill( tau->phi()    );
  }


  const float tau_min_pt = 15;
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
