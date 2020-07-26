#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../EffEstimation/interface/TriggerCommissioning.h"
#include "../../Selection/interface/SemiLeptonicTauSelection.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

using namespace TopTree;
using namespace std;

int main ()
{
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;
  
  //////////////////////
  //Global variables
  //////////////////////
  vector < Dataset > datasets;
  SemiLeptonicTauSelection sel; 
  //float Luminosity = 0;
  // 0: MC - 1: Data - 2 Data & MC
  //int DataType = 0; 
  int verbosity = -1;

  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/TriggerCommissioning.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSemiLeptonicTauSelection (sel); // now the parameters for the selection are given to the selection
 
  //anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  TopTree::NTEvent * event = 0;
  
 
 //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Loop over the datasets  "<<endl;
	cout<<"#########################"<<endl;
  }

  for (unsigned int d = 0; d < datasets.size (); d++) {


    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    
   
    
    TriggerCommissioning trigComm;
    trigComm.CreateHistos();
    
    
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
     
     float weight = 1.;
     datasets[d].eventTree ()->GetEntry (ievt);
     
     if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
     
     //Load event for the selection
     sel.LoadEvent(event); 
     int runNumber = sel.getRunNumber();
     
     //QuadJet40_IsoPFTau40
     //if ((runNumber>=165970 && runNumber<=166782) || runNumber>=171050) continue;

     //QuadJet45_IsoPFTau45
     
      if (runNumber<165970  || (runNumber>166782 && runNumber<171050) ) continue;
	   
     //Collection of selected objects
    //   vector<NTMuon> selMuons         = sel.GetSelectedMuonsNoIso();
//       vector<NTElectron> selElectrons = sel.GetSelectedElectronsNoIso(); 
     vector<NTMuon> selMuons      = sel.GetSelectedMuonsLooseIDLooseIso();
     vector<NTElectron> selElectrons  = sel.GetSelectedElectronsLooseIDLooseIso();
      
     vector<NTTau>  selTaus          = sel.GetSelectedTaus(selMuons,selElectrons,false,1,1,true);
     //vector<NTTau>  selTaus          = sel.GetSelectedTaus(false,1,1,false);
     //vector<NTTau>  noSelTaus        = sel.GetTaus(selMuons,selElectrons);
     
     //std::cout << sel.getRunNumber()<< std::endl;
     
     //vector<NTJet>  selJets = sel.GetSelectedJets(selMuons,selElectrons, selTaus);
     //vector<NTJet>  selJets = sel.GetSelectedJets(selMuons,selElectrons);
     vector<NTJet>  selJets_noClean = sel.GetSelectedJets();
     //NTMET met = sel.GetMET();
     
     
     //bool  PassTriggerMuonSelection     = sel.passTriggerMuonSelection();
     //bool  PassTriggerQuadJet40         = sel.passTriggerQuadJet40Selection();
     //float PassTriggerQuadJet40Prescaled= sel.passTriggerQuadJet40PrescaledSelection();
     bool  PassTriggerQuadJet40IsoPFTau = sel.passTriggerQuadJet40IsoPFTauSelection();
     bool  PassTriggerQuadJet45IsoPFTau = sel.passTriggerQuadJet45IsoPFTauSelection();
    
     //float QuadJet40Prescale=sel.QuadJet40Prescale();
     //float QuadJet40IsoPFTau40Prescale=sel.QuadJet40IsoPFTau40Prescale();  
     //float QuadJet45IsoPFTau45Prescale=sel.QuadJet45IsoPFTau45Prescale(); 
      
     float filterStatus  = sel.hltFilter("hltQuadJet45IsoPFTau45");
     float filterStatus2 = sel.hltFilter("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45");
     float filterStatus3 = sel.hltFilter("hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45");
     float filterStatus4 = 0;
     if ((filterStatus2 == 1) || (filterStatus3 == 1)) filterStatus4 = 1;

     vector<TLorentzVector> tauObjTrig = sel.GetTauTrigObj();
     vector<TLorentzVector> jetObjTrig = sel.GetJetTrigObj();
     
     trigComm.FillHistos(selJets_noClean, selTaus, PassTriggerQuadJet45IsoPFTau,filterStatus,filterStatus4,tauObjTrig,jetObjTrig);

    }// end of loop over evts
    
    TString outputRootFile = "TriggerEffHisto_"+datasets[d].Name()+".root";
    trigComm.SaveHistos(outputRootFile);
    
  }				
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  return (0);
}
