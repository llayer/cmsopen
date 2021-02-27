#include "EventBasicsAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


EventBasicsAnalyzer::EventBasicsAnalyzer(const edm::ParameterSet& cfg)
{
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("Events", "Events");

  // Event information
  tree->Branch("run", &value_run);
  tree->Branch("luminosityBlock", &value_lumi_block);
  tree->Branch("event", &value_event);
  tree->Branch("MCEvt_cteq66_pdf_weights", cteq66_pdf_weights, "MCEvt_cteq66_pdf_weights[45]/F");
}


void EventBasicsAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{

  ///////////////////////////
  // Event information
  ///////////////////////////
  value_run = event.run();
  value_lumi_block = event.luminosityBlock();
  value_event = event.id().event();

  edm::InputTag pdfWeightTag("pdfWeights:cteq66"); // or any other PDF set
  edm::Handle<std::vector<double> > weightHandle;
  event.getByLabel(pdfWeightTag, weightHandle);
  std::vector<double> weights = (*weightHandle);
  std::cout << "Event weight for central PDF:" << weights[0] << " Size: " << weights.size() <<std::endl;
  unsigned int nmembers = weights.size();
  for (unsigned int i=0; i<nmembers; i++) {
    cteq66_pdf_weights[i] = weights[i];
    std::cout << "Event weight: " << cteq66_pdf_weights[i] <<std::endl;
  }
  tree->Fill();

}

void EventBasicsAnalyzer::beginJob()
{
}

void
EventBasicsAnalyzer::endJob()
{
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EventBasicsAnalyzer);
