#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

class PDFFilter : public edm::EDFilter {
public:
  explicit PDFFilter(edm::ParameterSet const& ps);
  ~PDFFilter() override;

  bool filter(edm::Event& e, edm::EventSetup const& c) override;

private:
  // value between 0 and 1
  std::vector<std::string> eventlist;
};

PDFFilter::PDFFilter(edm::ParameterSet const& ps) : eventlist(ps.getParameter<std::vector < std::string >>("eventlist")) {

  std::cout<< std::endl << "Eventlist size: " << eventlist.size() << std::endl;
  //for (unsigned int i = 0; i < eventlist.size (); i++) {
  //  std::cout<< eventlist[i];
  //}
}

PDFFilter::~PDFFilter() {}

bool PDFFilter::filter(edm::Event& event, edm::EventSetup const&){


  int run = event.run();
  int lumi_block = event.luminosityBlock();
  int ev_id = event.id().event();

  std::string key = std::to_string(run) + std::to_string(lumi_block) + std::to_string(ev_id);


  if (std::find(eventlist.begin(), eventlist.end(), key) != eventlist.end()){
    std::cout << "Element found";

    std::cout<< std::endl << "Run ID " << run;
    std::cout<< std::endl << "Lumi ID " << lumi_block;
    std::cout<< std::endl << "Event ID " << ev_id;
    std::cout<< std::endl << "key " << key << std::endl;
    return true;
  }
  else{
    //std::cout << "Element not found";
    return false;
  }
  //if (1. < acceptRate_)
  //  return true;
  //return false;

}

DEFINE_FWK_MODULE(PDFFilter);
