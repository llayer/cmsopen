#ifndef TauTrigger_h
#define TauTrigger_h

#include "TH1F.h"
#include "TTree.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


struct HighestPt{
    bool operator()( TLorentzVector t1, TLorentzVector t2 ) const{
        return t1.Pt() > t2.Pt() ;
    }
};


class TauTrigger : public edm::EDAnalyzer {

 public:

  explicit TauTrigger(const edm::ParameterSet&);
  ~TauTrigger();

 private:

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  //virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  void             fill_tau_trigger(int runNumber, std::vector<TLorentzVector> thejets, std::vector<TLorentzVector> theTaus,
                                    int filterStatus, int filterStatus2, std::vector<TLorentzVector> tauObjTrig,
                                    std::vector<TLorentzVector> jetObjTrig);
  double RelIso03(double TrkIso03, double ECaloIso03, double HCaloIso03, double pt );

  TH1F* reference_tau;
  TH1F* triggered_tau;
  TH1F* eff_tau;

  TH1F* HLTtau_pt;
  TH1F* HLTtau_pt_diff;
  TH1F* HLTtau_eta;
  TH1F* HLTtau_eta_diff;
  TH1F* HLTtau_DR_diff;

  TH1F* jet_all_mult;
  TH1F* jet_all_pt;
  TH1F* jet_all_eta;
  TH1F* jet_all_pt1;
  TH1F* jet_all_pt2;
  TH1F* jet_all_pt3;
  TH1F* jet_all_pt4;

  TH1F* jet_trigger_mult;
  TH1F* jet_trigger_pt;
  TH1F* jet_trigger_eta;
  TH1F* jet_trigger_pt1;
  TH1F* jet_trigger_pt2;
  TH1F* jet_trigger_pt3;
  TH1F* jet_trigger_pt4;

  /*
  TH1F* jet_4j1t_mult;
  TH1F* jet_4j1t_pt;
  TH1F* jet_4j1t_eta;
  TH1F* jet_4j1t_pt1;
  TH1F* jet_4j1t_pt2;
  TH1F* jet_4j1t_pt3;
  TH1F* jet_4j1t_pt4;
  */

  TH1F* tau_all_mult;
  TH1F* tau_all_pt;
  TH1F* tau_all_eta;
  TH1F* tau_all_loose;
  TH1F* tau_all_medium;
  TH1F* tau_all_tight;
  TH1F* tau_all_againste;
  TH1F* tau_all_againstmu;

  TH1F* tau_trigger_mult;
  TH1F* tau_trigger_pt;
  TH1F* tau_trigger_eta;
  TH1F* tau_trigger_loose;
  TH1F* tau_trigger_medium;
  TH1F* tau_trigger_tight;
  TH1F* tau_trigger_againste;
  TH1F* tau_trigger_againstmu;

  /*
  TH1F* tau_4j1t_mult;
  TH1F* tau_4j1t_pt;
  TH1F* tau_4j1t_eta;
  TH1F* tau_4j1t_pt1;
  TH1F* tau_4j1t_pt2;
  TH1F* tau_4j1t_pt3;
  TH1F* tau_4j1t_pt4;
  */
};

#endif
