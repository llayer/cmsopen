


#include "TopQuarkAnalysis/MiniTreeProducer/interface/NeutralPFCandidates.h"




NeutralPFCandidates::NeutralPFCandidates(const edm::ParameterSet& iConfig)
{
   //register your products

   produces<reco::PFCandidateCollection>();



   inputPFcandidates_     = iConfig.getParameter < edm::InputTag >         ("inputPFcandidates");

   //now do what ever other initialization is needed

}


NeutralPFCandidates::~NeutralPFCandidates()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
NeutralPFCandidates::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   reco::PFCandidateCollection neutralPfCandidates;


   Handle<reco::PFCandidateCollection> pIn;
   iEvent.getByLabel(inputPFcandidates_,pIn);



   for (reco::PFCandidateCollection::const_iterator itpf = pIn->begin(), itpfend = pIn->end(); itpf!= itpfend; ++itpf) {

     if( (*itpf).charge() == 0 ) neutralPfCandidates.push_back(*itpf);

   }

   std::auto_ptr<reco::PFCandidateCollection> pOut(new reco::PFCandidateCollection(neutralPfCandidates));
   iEvent.put(pOut);

/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/



}

// ------------ method called once each job just before starting event loop  ------------
void
NeutralPFCandidates::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
NeutralPFCandidates::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void
NeutralPFCandidates::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
NeutralPFCandidates::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void
NeutralPFCandidates::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
NeutralPFCandidates::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
NeutralPFCandidates::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(NeutralPFCandidates);
