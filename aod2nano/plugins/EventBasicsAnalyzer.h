#ifndef EventBasicsAnalyzer_h
#define EventBasicsAnalyzer_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "TTree.h"

/**
 *    \class   EventBasicsAnalyzer EventBasicsAnalyzer.h "TopAnalysis/TopAnalyzer/interface/EventBasicsAnalyzer.h"
 *    */

class TTree;

class EventBasicsAnalyzer : public edm::EDAnalyzer {

public:

  /// default constructor
  explicit EventBasicsAnalyzer(const edm::ParameterSet&);
  /// default destructor
  ~EventBasicsAnalyzer(){};

  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

protected:
  // Event tree
  TTree *tree;
  // Event information
  Int_t value_run;
  UInt_t value_lumi_block;
  ULong64_t value_event;

  // MC event
  float cteq66_pdf_weights[45];


};

#endif
