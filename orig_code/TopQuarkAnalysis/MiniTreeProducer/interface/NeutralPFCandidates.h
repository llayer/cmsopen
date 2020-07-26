// -*- C++ -*-
//
// Package:    NeutralPFCandidates
// Class:      NeutralPFCandidates
// 
/**\class NeutralPFCandidates NeutralPFCandidates.cc TopQuarkAnalysis/NeutralPFCandidates/src/NeutralPFCandidates.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrea Jeremy,B25/117,6262,
//         Created:  Mon May  2 10:05:12 CEST 2011
// $Id: NeutralPFCandidates.h,v 1.2 2011/05/03 11:58:24 alebihan Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"



//
// class declaration
//

class NeutralPFCandidates : public edm::EDProducer {
   public:
      explicit NeutralPFCandidates(const edm::ParameterSet&);
      ~NeutralPFCandidates();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
      
      edm::InputTag      inputPFcandidates_;
      
      
      
};
