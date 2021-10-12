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
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/WeightsInfo.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "TLorentzVector.h"
#include "TauTrigger.h"

typedef math::XYZPoint Point;

TauTrigger::TauTrigger(const edm::ParameterSet& cfg)
{
  edm::Service<TFileService> fs;

  //tree = fs->make<TTree>("Events", "Events");

  Double_t x2bins[19] = {0,5,10,15,20,25,30,32.5,35,37.5,40,42.5,45,47.5,50,60,70,80,90};
  reference_tau = fs->make<TH1F>("reference_tau","reference_tau", 18, x2bins);
  triggered_tau= fs->make<TH1F>("triggered_tau","triggered_tau", 18, x2bins);

  eff_tau= fs->make<TH1F>("eff_tau","eff_tau", 18, x2bins);
  reference_tau->Sumw2();
  triggered_tau->Sumw2();
  eff_tau->Sumw2();

  HLTtau_pt       = fs->make<TH1F>("HLTtau_pt","HLTtau_pt", 50,0,100);
  HLTtau_eta      = fs->make<TH1F>("HLTtau_eta","HLTtau_eta", 100,0,2.5);
  HLTtau_pt_diff  = fs->make<TH1F>("HLTtau_pt_diff","HLTtau_pt_diff", 100,-100,100);
  HLTtau_eta_diff = fs->make<TH1F>("HLTtau_eta_diff","HLTtau_eta_diff", 100,-0.2,0.2);
  HLTtau_DR_diff  = fs->make<TH1F>("HLTtau_DR_diff","HLTtau_DR_diff", 100,0,0.1);


  // Histos for jets
  // All
  jet_all_mult = fs->make<TH1F>("Multiplicity_jet_all","Nof jets",9,3.5,12.5);
  jet_all_pt = fs->make<TH1F>("Pt_jet_jet_all","p_{T}(jet) (GeV)",200,0,400);
  jet_all_eta = fs->make<TH1F>("Eta_jet_jet_all","#eta(jet)",60,-3,3);
  jet_all_pt1 = fs->make<TH1F>("Pt_1_jet_jet_all","p_{T}(jet)",50,0,100);
  jet_all_pt2 = fs->make<TH1F>("Pt_2_jet_jet_all","p_{T}(jet)",50,0,100);
  jet_all_pt3 = fs->make<TH1F>("Pt_3_jet_jet_all","p_{T}(jet)",50,0,100);
  jet_all_pt4 = fs->make<TH1F>("Pt_4_jet_jet_all","p_{T}(jet)",50,0,100);

  // Trigger
  jet_trigger_mult = fs->make<TH1F>("Multiplicity_trigger","Nof jets",9,3.5,12.5);
  jet_trigger_pt = fs->make<TH1F>("Pt_trigger","p_{T}(jet) (GeV)",200,0,400);
  jet_trigger_eta = fs->make<TH1F>("Eta_trigger","#eta(jet)",60,-3,3);
  jet_trigger_pt1 = fs->make<TH1F>("Pt_1_trigger","p_{T}(jet)",50,0,100);
  jet_trigger_pt2 = fs->make<TH1F>("Pt_2_trigger","p_{T}(jet)",50,0,100);
  jet_trigger_pt3 = fs->make<TH1F>("Pt_3_trigger","p_{T}(jet)",50,0,100);
  jet_trigger_pt4 = fs->make<TH1F>("Pt_4_trigger","p_{T}(jet)",50,0,100);

  // 4jet + 1tau
  /*
  jet_4j1t_mult = fs->make<TH1F>("Multiplicity_4j1t","Nof jets",9,3.5,12.5);
  jet_4j1t_pt = fs->make<TH1F>("Pt_4j1t","p_{T}(jet) (GeV)",200,0,400);
  jet_4j1t_eta = fs->make<TH1F>("Eta_4j1t","#eta(jet)",60,-3,3);
  jet_4j1t_pt1 = fs->make<TH1F>("Pt_1_4j1t","p_{T}(jet)",50,0,100);
  jet_4j1t_pt2 = fs->make<TH1F>("Pt_2_4j1t","p_{T}(jet)",50,0,100);
  jet_4j1t_pt3 = fs->make<TH1F>("Pt_3_4j1t","p_{T}(jet)",50,0,100);
  jet_4j1t_pt4 = fs->make<TH1F>("Pt_4_4j1t","p_{T}(jet)",50,0,100);
  */

  // Tau
  // All
  tau_all_mult = fs->make<TH1F>("taumutiplicity_tau_all", "Nof taus" ,10,0,10);
  tau_all_pt = fs->make<TH1F>("Pt_tau_all", "p_{T}(#tau) (GeV)",125,0,250);
  tau_all_eta = fs->make<TH1F>("Eta_tau_all", "#eta(#tau)",60,-3,3);
  tau_all_loose = fs->make<TH1F>("byLooseIsolation_tau_all", "byLooseIsolation(#tau)",3,-1,2);
  tau_all_medium = fs->make<TH1F>("byMediumIsolation_tau_all","byMediumIsolation(#tau)",3,-1,2);
  tau_all_tight = fs->make<TH1F>("byTightIsolation_tau_all", "byTightIsolation(#tau)",3,-1,2);
  tau_all_againste = fs->make<TH1F>("againstElectronLoose_tau_all", "againstElectronLoose(#tau)",3,-1,2.);
  tau_all_againstmu = fs->make<TH1F>("againstMuonLoose_tau_all", "againstMuonLoose(#tau)",3,-1,2.);

  // Trigger
  tau_trigger_mult = fs->make<TH1F>("taumutiplicity_tau_trigger", "Nof taus" ,10,0,10);
  tau_trigger_pt = fs->make<TH1F>("Pt_tau_trigger", "p_{T}(#tau) (GeV)",125,0,250);
  tau_trigger_eta = fs->make<TH1F>("Eta_tau_trigger", "#eta(#tau)",60,-3,3);
  tau_trigger_loose = fs->make<TH1F>("byLooseIsolation_tau_trigger", "byLooseIsolation(#tau)",3,-1,2);
  tau_trigger_medium = fs->make<TH1F>("byMediumIsolation_tau_trigger","byMediumIsolation(#tau)",3,-1,2);
  tau_trigger_tight = fs->make<TH1F>("byTightIsolation_tau_trigger", "byTightIsolation(#tau)",3,-1,2);
  tau_trigger_againste = fs->make<TH1F>("againstElectronLoose_tau_trigger", "againstElectronLoose(#tau)",3,-1,2.);
  tau_trigger_againstmu = fs->make<TH1F>("againstMuonLoose_tau_trigger", "againstMuonLoose(#tau)",3,-1,2.);

  // 4j + 1t
  /*
  tau_4j1t_mult = fs->make<TH1F>("taumutiplicity_tau_4j1t", "Nof taus" ,10,0,10);
  tau_4j1t_pt = fs->make<TH1F>("Pt_tau_4j1t", "p_{T}(#tau) (GeV)",125,0,250);
  tau_4j1t_eta = fs->make<TH1F>("Eta_tau_4j1t", "#eta(#tau)",60,-3,3);
  tau_4j1t_loose = fs->make<TH1F>("byLooseIsolation_tau_4j1t", "byLooseIsolation(#tau)",3,-1,2);
  tau_4j1t_medium = fs->make<TH1F>("byMediumIsolation_tau_4j1t","byMediumIsolation(#tau)",3,-1,2);
  tau_4j1t_tight = fs->make<TH1F>("byTightIsolation_tau_4j1t", "byTightIsolation(#tau)",3,-1,2);
  tau_4j1t_againste = fs->make<TH1F>("againstElectronLoose_tau_4j1t", "againstElectronLoose(#tau)",3,-1,2.);
  tau_4j1t_againstmu = fs->make<TH1F>("againstMuonLoose_tau_4j1t", "againstMuonLoose(#tau)",3,-1,2.);
  */

  // Trigger
  /*
  interestingTriggers = {
    "HLT_QuadJet40_IsoPFTau40", "HLT_QuadJet45_IsoPFTau45", "HLT_Mu15_v", "HLT_Mu20_v", "HLT_Mu24_v", "HLT_Mu30_v",
    "HLT_Mu15_v1", "HLT_Mu15_v2", "HLT_Mu15_v3", "HLT_Mu15_v4", "HLT_Mu15_v5",
    "HLT_Mu20_v1", "HLT_Mu20_v2", "HLT_Mu20_v3", "HLT_Mu20_v4", "HLT_Mu20_v5",
    "HLT_Mu24_v1", "HLT_Mu24_v2", "HLT_Mu24_v3", "HLT_Mu24_v4", "HLT_Mu24_v5",
    "HLT_Mu30_v1", "HLT_Mu30_v2", "HLT_Mu30_v3", "HLT_Mu30_v4", "HLT_Mu30_v5"
  };
  */

}

TauTrigger::~TauTrigger()
{
}

void TauTrigger::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{


  using namespace edm;
  using namespace reco;
  using namespace std;


  ///////////////////////////
  // Event information
  ///////////////////////////
  int runNumber = evt.run();
  //lumiBlock = evt.luminosityBlock();
  //int eventID = evt.id().event();
  if (runNumber > 178420) return;

  //std::cout << endl << "Analyzing event with ID: " << runNumber << "  " << eventID << endl;

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
  //for (unsigned int i = 0; i < interestingTriggers.size(); i++) {
  //  value_trig[i] = 0;
  //}

  int HLT_QuadJet45_IsoPFTau45 = -99;

  for (unsigned int i = 0; i < trigger->size(); i++) {
    if( triggerNames_[i].find( "HLT_QuadJet45_IsoPFTau45" ) != std::string::npos ){
      HLT_QuadJet45_IsoPFTau45 = int(trigger->accept(i));
    }
  }

  /*
  Handle<pat::TriggerEvent> patTriggerEvent;
  evt.getByLabel(InputTag("patTriggerEventPF"), patTriggerEvent);
  // Trigger event
  pat::TriggerEvent pTrigEvt;
  pTrigEvt = *patTriggerEvent;


  // Filters
  //std::cout << endl << "Filter names: " << endl;
  for (unsigned int i = 0; i < filterList.size(); i++) {
    const pat::TriggerFilter* filter = pTrigEvt.filter(filterList[i]);
    if (filter){
      //std::cout<< filterList[i] << " " << filter->status() << std::endl;
      value_filt[i] = int(filter->status());
    }
  }
  */


  //********************************************************
  // Retrieve dynamic prescales for triggers
  //********************************************************


  /*
  pat::TriggerEvent pTrigEvt;

  Handle< pat::TriggerEvent > patTriggerEvent;
  evt.getByLabel(InputTag("patTriggerEventPF2PAT", patTriggerEvent );
  pTrigEvt = *patTriggerEvent;
  */

  Handle<pat::TriggerEvent> patTriggerEvent;
  evt.getByLabel(InputTag("patTriggerEventPF2PAT"), patTriggerEvent);
  // Trigger event
  pat::TriggerEvent pTrigEvt;
  pTrigEvt = *patTriggerEvent;

  int filter_status = -99;
  const pat::TriggerFilter* filter_hltQuadJet45IsoPFTau45 = pTrigEvt.filter("hltQuadJet45IsoPFTau45");
  if (filter_hltQuadJet45IsoPFTau45) filter_status = filter_hltQuadJet45IsoPFTau45->status();

  int filter_status2 = -99;
  const pat::TriggerFilter* filter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45");
  if (filter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45) filter_status2 = filter_hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45->status();

  int filter_status3 = -99;
  const pat::TriggerFilter* filter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45");
  if (filter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45) filter_status3 = filter_hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45->status();

  int filter_status4 = 0;
  if ((filter_status2 == 1) || (filter_status3 == 1)) filter_status4 = 1;

  //std::cout<< filter_status << filter_status4 << std::endl;


  // Matching
  const pat::helper::TriggerMatchHelper matchHelper;
  const pat::TriggerObjectRefVector trigRefs( patTriggerEvent->objects( trigger::TriggerJet ) );
  std::vector < TLorentzVector > sel_hltjets;
  for ( pat::TriggerObjectRefVector::const_iterator it = trigRefs.begin(); it != trigRefs.end(); ++it )
  {
    //cout << endl << "Jet trigger" << (*it)->px() << (*it)->py() << (*it)->pz() << (*it)->energy() << endl;
    if ( pTrigEvt.objectInFilter( (*it), "hltQuadJet45IsoPFTau45")){

      TLorentzVector p4;
      p4.SetPxPyPzE((*it)->px (), (*it)->py (), (*it)->pz (), (*it)->energy ());
      sel_hltjets.push_back(p4);

    }
  }

  const pat::TriggerObjectRefVector trigRefs2( patTriggerEvent->objects( trigger::TriggerTau ) );
  std::vector < TLorentzVector > sel_hlttaus;
  for ( pat::TriggerObjectRefVector::const_iterator it = trigRefs2.begin(); it != trigRefs2.end(); ++it )
  {
    //cout << endl << "Tau trigger" << (*it)->px() << (*it)->py() << (*it)->pz() << (*it)->energy() << endl;
    if (pTrigEvt.objectInFilter( (*it), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45") ||
        pTrigEvt.objectInFilter( (*it), "hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45")
       ){
         TLorentzVector p4;
         p4.SetPxPyPzE((*it)->px (), (*it)->py (), (*it)->pz (), (*it)->energy ());
         sel_hlttaus.push_back(p4);
    }
  }

  ////////////////////////////
  // Primary Vertex & Beamspot
  ////////////////////////////


  Handle<VertexCollection> vertices;
  evt.getByLabel(InputTag("goodOfflinePrimaryVertices"), vertices);

  double value_ve_z = vertices->begin()->z();

  // Beam spot
  Handle < reco::BeamSpot > beamSpot;
  evt.getByLabel(InputTag("offlineBeamSpot"), beamSpot);
  Point bspot = beamSpot.isValid ()? beamSpot->position () : Point (0, 0, 0);

  ///////////////////////////
  // Muons
  ///////////////////////////

  Handle<vector<pat::Muon>> muons;
  evt.getByLabel(InputTag("selectedPatMuonsPF2PAT"), muons);

  std::vector < TLorentzVector > sel_muons;

  // PtThreshold="10." EtaThreshold="2.4" RelIso="0.15" D0Cut="0.02" VertexMatchThr="1." NofValidHits="0"  NofValidTkHits="10" NormChi2="10" />

  double muon_cut_pt = 10.;
  double muon_cut_eta = 2.4;
  double muon_reliso = 0.15;
  double muon_vtxmatch = 1.;

  for (auto it = muons->begin(); it != muons->end(); it++) {

    // if(muon.MuonType<100)                         continue; // isGlobalMuon CHECK!!

    if (it->pt () < muon_cut_pt || fabs (it->eta ()) > muon_cut_eta) continue;
    if( !(it->isGlobalMuon () || it->isTrackerMuon())) continue;
    double relIso03 = RelIso03(it->trackIso(), it->ecalIso(), it->hcalIso(), it->pt() );
    if( relIso03 > muon_reliso)  continue;
    if ( fabs( it->vz() - value_ve_z ) > muon_vtxmatch ) continue;

    TLorentzVector p4;
    p4.SetPxPyPzE(it->px (), it->py (), it->pz (), it->energy ());
    sel_muons.push_back(p4);

  }
  std::sort(sel_muons.begin(),sel_muons.end(),HighestPt());


  ///////////////////////////
  // Electron
  ///////////////////////////

  // <s type="Electron" PtThreshold="10." EtaThreshold="2.5" RelIso="0.15" D0Cut="0.04" VertexMatchThr="1." DElectronETSCThr="15" RemuThr="0.1"/>
  // PtThreshold="15." EtaThreshold="2.5" RelIso="0.15" D0Cut="0.04" VertexMatchThr="1." DElectronETSCThr="15" RemuThr="0.1"/>

  double electron_cut_pt = 10.;
  double electron_cut_eta = 2.5;
  double electron_reliso = 0.15;
  double electron_vtxmatch = 1.;

  Handle<vector<pat::Electron>> electrons;
  evt.getByLabel(InputTag("selectedPatElectronsPF2PAT"), electrons);
  std::vector < TLorentzVector > sel_electrons;

  for (auto it = electrons->begin(); it != electrons->end(); it++) {

    if (it->pt () < electron_cut_pt || fabs (it->eta ()) > electron_cut_eta) continue;
    if (it->passingCutBasedPreselection() != true) continue;
    double relIso03 = RelIso03(it->trackIso(), it->ecalIso(), it->hcalIso(), it->pt() );
    if( relIso03 > electron_reliso)  continue;
    if ( fabs( it->vz() - value_ve_z ) > electron_vtxmatch ) continue;

    TLorentzVector p4;
    p4.SetPxPyPzE(it->px (), it->py (), it->pz (), it->energy ());
    sel_electrons.push_back(p4);

  }
  std::sort(sel_electrons.begin(),sel_electrons.end(),HighestPt());


  ///////////////////////////
  // Tau
  ///////////////////////////

  // <s type="Tau"      Algo="selectedPatTausPF2PAT" PtThreshold="10." EtaThreshold="2.3" RelIso="0.15" D0Cut="0.04" VertexMatchThr="1." TauLeadTrkPtCut="5."/>
  //selectedPatTausPF2PAT" PtThreshold="45." EtaThreshold="2.3" RelIso="0.15" D0Cut="0.04" VertexMatchThr="1." TauLeadTrkPtCut="10."/>


  // TRIGGER
  double tau_cut_pt = 10.;
  double tau_cut_eta = 2.3;
  double tau_cut_d0 = 0.04;
  double tau_vtxmatch = 1.;
  double tau_cut_leadTrackPt = 5.;

  /*
  // Final selection
  double tau_cut_pt = 45.;
  double tau_cut_eta = 2.3;
  double tau_cut_d0 = 0.04;
  double tau_vtxmatch = 1.;
  double tau_cut_leadTrackPt = 10.;
  */

  Handle<vector<pat::Tau>> taus;
  evt.getByLabel(InputTag("selectedPatTausPF2PAT"), taus);

  std::vector < TLorentzVector > sel_taus;
  int iTau = 0;
  int nTau = 0;

  for (auto it = taus->begin(); it != taus->end(); it++) {

    /*
    const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( taus, iTau,
                                            "tauMatchHLTTaus45PF", evt, pTrigEvt ) );
    */
    iTau++;

    //cout<< " Found tau " << endl;
    double tau_leadTrackPt;
    double tau_dxy;
    if (it->pt () < tau_cut_pt || fabs (it->eta ()) > tau_cut_eta) continue;

    auto tauIDs = it->tauIDs();
    int tau_againstElectronTight = -99;
    int tau_againstMuonTight = -99;
    int tau_byLooseCombinedIsolationDeltaBetaCorr = -99;
    int tau_byMediumCombinedIsolationDeltaBetaCorr = -99;
    int tau_byTightCombinedIsolationDeltaBetaCorr = -99;
    for (unsigned int i = 0; i < tauIDs.size (); i++) {
      //std::cout << "tau discr " << tauIDs[i].first << " value " << tauIDs[i].second << std::endl;
      if(tauIDs[i].first == "againstElectronTight") tau_againstElectronTight = tauIDs[i].second;
      if(tauIDs[i].first == "againstMuonTight") tau_againstMuonTight = tauIDs[i].second;
      if(tauIDs[i].first == "byLooseCombinedIsolationDeltaBetaCorr") tau_byLooseCombinedIsolationDeltaBetaCorr = tauIDs[i].second;
      if(tauIDs[i].first == "byMediumCombinedIsolationDeltaBetaCorr") tau_byMediumCombinedIsolationDeltaBetaCorr = tauIDs[i].second;
      if(tauIDs[i].first == "byTightCombinedIsolationDeltaBetaCorr") tau_byTightCombinedIsolationDeltaBetaCorr = tauIDs[i].second;
    }

    if(fabs(it->eta())<1.566 && fabs(it->eta())>1.4442)continue;
    if( it->leadPFChargedHadrCand().isAvailable()) {
      if (it->leadPFChargedHadrCand()->trackRef().isNonnull()){
        tau_leadTrackPt = it->leadPFChargedHadrCand()->trackRef()->pt();
        tau_dxy = it->leadPFChargedHadrCand()->trackRef()->dxy(bspot);
      }else if  (it->leadPFChargedHadrCand()->gsfTrackRef().isNonnull()){
        tau_leadTrackPt = it->leadPFChargedHadrCand()->gsfTrackRef()->pt();
        tau_dxy = it->leadPFChargedHadrCand()->gsfTrackRef()->dxy(bspot);
      }else{
       tau_leadTrackPt = -999.;
       tau_dxy = -999.;
      }
    }else {
      tau_leadTrackPt = -999.;
      tau_dxy = -999.;
    }

    if ( tau_againstElectronTight != 1)   continue;
    if ( tau_againstMuonTight != 1)   continue;
    // Trigger -> Medium
    //if ( tau_byMediumCombinedIsolationDeltaBetaCorr != 1)   continue;
    // Final -> Loose
    if ( tau_byLooseCombinedIsolationDeltaBetaCorr != 1)   continue;
    if ( tau_leadTrackPt <= tau_cut_leadTrackPt ) continue;
    if( fabs( tau_dxy)  >= tau_cut_d0 )                 continue;
    if ( fabs( it->vz() - value_ve_z ) > tau_vtxmatch ) continue;

    TLorentzVector p4;
    p4.SetPxPyPzE(it->px (), it->py (), it->pz (), it->energy ());

    double deltaRmu = 10000;
    double deltaRel = 10000;

    for(unsigned int imu=0; imu< sel_muons.size(); imu++){
      double deltaR = p4.DeltaR(sel_muons[imu]);
      if(deltaR < deltaRmu) deltaRmu = deltaR;
    }

    for(unsigned int iel=0; iel< sel_electrons.size(); iel++){
      double deltaR = p4.DeltaR(sel_electrons[iel]);
      if(deltaR < deltaRel) deltaRel = deltaR;
    }

    if( !(deltaRmu > 0.4  && deltaRel > 0.4) ) continue;

    tau_all_pt->Fill(it->pt ());
    tau_all_eta->Fill(it->eta ());
    tau_all_loose->Fill(tau_byLooseCombinedIsolationDeltaBetaCorr);
    tau_all_medium->Fill(tau_byMediumCombinedIsolationDeltaBetaCorr);
    tau_all_tight->Fill(tau_byTightCombinedIsolationDeltaBetaCorr);
    tau_all_againste->Fill(tau_againstElectronTight);
    tau_all_againstmu->Fill(tau_againstMuonTight);
    if( HLT_QuadJet45_IsoPFTau45 == 1){
      tau_trigger_pt->Fill(it->pt ());
      tau_trigger_eta->Fill(it->eta ());
      tau_trigger_loose->Fill(tau_byLooseCombinedIsolationDeltaBetaCorr);
      tau_trigger_medium->Fill(tau_byMediumCombinedIsolationDeltaBetaCorr);
      tau_trigger_tight->Fill(tau_byTightCombinedIsolationDeltaBetaCorr);
      tau_trigger_againste->Fill(tau_againstElectronTight);
      tau_trigger_againstmu->Fill(tau_againstMuonTight);
    }

    sel_taus.push_back(p4);
    nTau++;
  }

  std::sort(sel_taus.begin(),sel_taus.end(),HighestPt());

  tau_all_mult->Fill(nTau);
  if( HLT_QuadJet45_IsoPFTau45 == 1){
    tau_trigger_mult->Fill(nTau);
  }


  ///////////////////////////
  // Jets
  ///////////////////////////

  // <s type="Jet"      Algo="pf" PtThreshold="10." EtaThreshold="3" />
  // type="Jet"      Algo="pf" PtThreshold="20." EtaThreshold="2.4" />


  // TRIGGER
  double jet_cut_pt = 10.;
  double jet_cut_eta = 2.4;
  /*
  // Final Selection
  double jet_cut_pt = 20.;
  double jet_cut_eta = 2.4;
  */
  Handle<vector<pat::Jet>> jets;
  evt.getByLabel(InputTag("selectedPatJetsPF2PAT"), jets);

  std::vector < TLorentzVector > sel_jets_hltmatch;
  int iJet = 0;
  int nJet = 0;
  //std::cout << std::endl << jets->size() << std::endl;
  for (auto it = jets->begin(); it != jets->end(); it++) {

    const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( jets, iJet,
                                            "jetMatchHLTJets45PF2PAT", evt, pTrigEvt ) );
    iJet++;

    if (it->pt () < jet_cut_pt || fabs (it->eta ()) > jet_cut_eta) continue;

    nJet++;

    jet_all_pt->Fill(it->pt());
    jet_all_eta->Fill(it->eta());
    if( nJet == 0) jet_all_pt1->Fill(it->pt());
    if( nJet == 1) jet_all_pt2->Fill(it->pt());
    if( nJet == 2) jet_all_pt3->Fill(it->pt());
    if( nJet == 3) jet_all_pt4->Fill(it->pt());

    if( HLT_QuadJet45_IsoPFTau45 == 1){

      jet_trigger_pt->Fill(it->pt());
      jet_trigger_eta->Fill(it->eta());
      if( nJet == 0) jet_trigger_pt1->Fill(it->pt());
      if( nJet == 1) jet_trigger_pt2->Fill(it->pt());
      if( nJet == 2) jet_trigger_pt3->Fill(it->pt());
      if( nJet == 3) jet_trigger_pt4->Fill(it->pt());

    }

    if ( trigRef45.isAvailable() ) {
      TLorentzVector p4;
      p4.SetPxPyPzE(it->px (), it->py (), it->pz (), it->energy ());
      TLorentzVector p4HLT;
      p4HLT.SetPxPyPzE (trigRef45->px (), trigRef45->py (), trigRef45->pz (), trigRef45->energy ());
      if ( p4.DeltaR( p4HLT ) < 0.4 ){
        sel_jets_hltmatch.push_back(p4);
      }
    }
  }
  std::sort(sel_jets_hltmatch.begin(),sel_jets_hltmatch.end(),HighestPt());
  jet_all_mult->Fill(nJet);

  if( HLT_QuadJet45_IsoPFTau45 == 1){
    jet_trigger_mult->Fill(nJet);
  }

  /*
  cout << std::endl;
  for (unsigned int s=0; s<sel_jets_hltmatch.size(); s++)
  {
    cout<< "JET " << s << " pt " << sel_jets_hltmatch[s].Pt() << std::endl;
  }
  */


  ///////////////////////////
  // Tau Trigger calculation
  ///////////////////////////

  fill_tau_trigger(runNumber, sel_jets_hltmatch, sel_taus, filter_status, filter_status4, sel_hlttaus, sel_hltjets);


}


void TauTrigger::fill_tau_trigger(int runNumber, std::vector<TLorentzVector> thejets, std::vector<TLorentzVector> theTaus,
                                  int filterStatus, int filterStatus2, std::vector<TLorentzVector> tauObjTrig,
                                  std::vector<TLorentzVector> jetObjTrig){

  // Restrict runs
  if (runNumber<165970  || (runNumber>166782 && runNumber<171050) ) return;
  if (runNumber > 178420) return;
  // Four HLT matched jets and exactly one tau
  if (thejets.size() < 4) return;
  if (theTaus.size() != 1) return;

  ///////////Tau Leg/////////////////////////////////////////////////////////////////

  float deltaRMin = 99;
  double deltaR = 99;

  for (unsigned int s=0; s<thejets.size(); s++)
  {
    deltaR = thejets[s].DeltaR(theTaus[0]);
    if (deltaR < deltaRMin) deltaRMin = deltaR;
  }

  // Require tau / jet match
  if (deltaRMin > 0.4) return;

  // Require the first filter bit to have first filter bit to have fired
  if (filterStatus !=1 ) return;

  // Fill the reference tau
  reference_tau->Fill(theTaus[0].Pt());

  // Match tau with HLT objects
  float deltaRMinTrig = 99.;
  float deltaRTrig =99.;
  int idx = -1;

  for (unsigned int i=0; i<tauObjTrig.size(); i++){

    deltaRTrig = theTaus[0].DeltaR(tauObjTrig[i]);
    if (deltaRTrig<deltaRMinTrig){
      deltaRMinTrig = deltaRTrig;
      idx = i;
    }
  }

  // Require second filter bit to be true a
  if( (filterStatus2==1) && (deltaRMinTrig<0.4)){
    if (fabs(tauObjTrig[idx].Eta())<2.3){
      HLTtau_pt->Fill(tauObjTrig[idx].Pt());
      HLTtau_eta->Fill(tauObjTrig[idx].Eta());
      HLTtau_pt_diff->Fill(theTaus[0].Pt()-tauObjTrig[idx].Pt());
      HLTtau_eta_diff->Fill(theTaus[0].Eta()-tauObjTrig[idx].Eta());
      HLTtau_DR_diff->Fill(theTaus[0].Eta()-tauObjTrig[idx].Eta());
      triggered_tau->Fill(theTaus[0].Pt());
    }
  }

  eff_tau->Divide(triggered_tau,reference_tau,1,1,"B");

}


double TauTrigger::RelIso03(double TrkIso03, double ECaloIso03, double HCaloIso03, double pt ){
    if(pt > 0.)  return( ((TrkIso03+ECaloIso03+HCaloIso03)/pt));
    return (-999.);
}


// ------------ method called when starting to processes a luminosity block  ------------
void TauTrigger::beginLuminosityBlock(const edm::LuminosityBlock & lumi, const edm::EventSetup & setup)
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

void TauTrigger::endLuminosityBlock(const edm::LuminosityBlock & lumi, const edm::EventSetup & setup)
{

  /*

  edm::Handle<edm::MergeableCounter> nEventsTotalCounter;
  lumi.getByLabel(edm::InputTag("nEventsTotal"), nEventsTotalCounter);
  //std::cout << "*****************************" << std::endl;
  //std::cout << "NEVENTS " << nEventsTotalCounter->value << std::endl;
  //std::cout << "*****************************" << std::endl;
  nEventsTotal += nEventsTotalCounter->value;

  edm::Handle<edm::MergeableCounter> nEventsFilteredCounter;
  lumi.getByLabel("nEventsFiltered", nEventsFilteredCounter);
  nEventsFiltered += nEventsFilteredCounter->value;
  */

}

/*
void TauTrigger::beginRun(edm::Run& run, edm::EventSetup const& setup)
{
}
*/
void TauTrigger::endRun(edm::Run& run, edm::EventSetup const& setup)
{
  /*
  edm::Handle<GenRunInfoProduct> genRunInfo;
  run.getByLabel( "generator", genRunInfo );

  double crossSection = genRunInfo->crossSection();

  std::cout << "CrossSection!: " << crossSection << std::endl;
  */
}


void TauTrigger::beginJob()
{
}

void TauTrigger::endJob()
{

  /*
  // Fill information about the analysis of the file
  info->Fill();

  std::cout<< std::endl << "Number of events stored: " << tree->GetEntries() << " / " << nEventsTotal << std::endl;
  */
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TauTrigger);
                                                        
    
