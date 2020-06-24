#include "EventBasicsAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "TH1F.h"



EventBasicsAnalyzer::EventBasicsAnalyzer(const edm::ParameterSet& cfg):
  vertex_(cfg.getParameter<edm::InputTag>("vertex")),
  tracks_(cfg.getParameter<edm::InputTag>("tracks")),
  met_   (cfg.getParameter<edm::InputTag>("met"   ))
{ 
}

void EventBasicsAnalyzer::beginJob()
{
  edm::Service<TFileService> fs;
  if( !fs ) throw edm::Exception( edm::errors::Configuration, "TFile Service is not registered in cfg file" );

  hists_["z_vtx"  ] = fs->make<TH1F>("z_vtx"  , "z_vtx"  , 32, -16.,  16.);
  hists_["n_trk"  ] = fs->make<TH1F>("n_trk"  , "n_trk"  , 50,   0., 200.);
  hists_["n_hpTrk"] = fs->make<TH1F>("n_hpTrk", "n_hpTrk", 50,   0., 200.);
  hists_["f_hpTrk"] = fs->make<TH1F>("f_hpTrk", "f_hpTrk", 20,   0.,   1.);
  hists_["met"    ] = fs->make<TH1F>("met"    , "met"    , 50,   0., 100.);

}

void
EventBasicsAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{

  edm::Handle<reco::VertexCollection> vertex;
  event.getByLabel(vertex_, vertex);

  hists_.find("z_vtx")->second->Fill( vertex->begin()->z() );

  edm::Handle<reco::TrackCollection> tracks;
  event.getByLabel(tracks_, tracks);

  const reco::TrackCollection* trkColl = tracks.product();

  hists_.find("n_trk")->second->Fill( trkColl->size() );
   
  unsigned nTrkHighPurity = 0;
 
  for(reco::TrackCollection::const_iterator it = trkColl->begin(); it != trkColl->end(); ++it) {
    if( it->quality(reco::TrackBase::qualityByName("highPurity")) )
      nTrkHighPurity++;
  }

  hists_.find("n_hpTrk")->second->Fill( nTrkHighPurity );

  hists_.find("f_hpTrk")->second->Fill( (float)nTrkHighPurity / (float)trkColl->size() );



  edm::Handle<std::vector<pat::MET> > met;
  event.getByLabel(met_, met);

  hists_.find("met")->second->Fill( met->begin()->pt() );

}

void
EventBasicsAnalyzer::endJob() 
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EventBasicsAnalyzer);
