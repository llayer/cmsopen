#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../EffEstimation/interface/TMVA_Tree.h"
#include "../../Selection/interface/SemiLeptonicTauSelection.h"
#include "../../Tools/interface/PUWeighting.h"
#include "../../Tools/interface/LumiReweightingStandAlone.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TRandom.h>

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
  float Luminosity = 0;
  float LumiError = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0;
  int verbosity = -1;

  ///GV to rewieght the MC data////////
  string PUWeightFileName;
 
  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/TMVA_Tree.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSemiLeptonicTauSelection (sel); // now the parameters for the selection are given to the selection
  //Load for PU:
 
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );

  
 anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  ///Load for PU old version////////////////////////////////
  //sel.GeneratePUWeight(PUWeightFileName);
  //anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  
  TopTree::NTEvent * event = 0;
 
  //
  bool IReweight = true;
  if (DataType == 1) IReweight = false;
  
  // 3D reweighting
  bool  IReweight_puUp	 = false;
  bool  IReweight_puDown = false;
 
  string datafile = "";
  string mcfile   = "rootFiles/PU3DMC.root";
  
  if(!IReweight_puUp && !IReweight_puDown) datafile = "rootFiles/MyDataPileupHistogram_observed_73500.root";
  if( IReweight_puDown  		 ) datafile = "rootFiles/MyDataPileupHistogram_observed_67620.root";
  if( IReweight_puUp			 ) datafile = "rootFiles/MyDataPileupHistogram_observed_79380.root";

     
  reweight::LumiReWeighting *LumiWeights = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
            
  if(!IReweight_puUp && !IReweight_puDown)  LumiWeights->weight3D_init( 1. );
  if( IReweight_puDown  		 )  LumiWeights->weight3D_init( 1. );
  if( IReweight_puUp			 )  LumiWeights->weight3D_init( 1. );
  
  
  /////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Loop over the datasets  "<<endl;
	cout<<"#########################"<<endl;
  }

  for (unsigned int d = 0; d < datasets.size (); d++) {

 cout << "0000 " <<  endl;   
   
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
 cout << "0001 " <<  endl;   

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;   
    
    TMVA_Tree TMVA;
    if(datasets[d].isData() == true) TMVA.CreateHistos("TMVA_bkg_newLumi.root");
    else                             TMVA.CreateHistos("TMVA_sig_newLumi.root");
 
    TRandom random;
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {

      float weight = 1.;
   
  
     // weight *= Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
      
     //triggerMenu
      
     int triggerMenu = 0;
     if(datasets[d].isData() == false)
     {
       float trigRange = random.Uniform(1);
       if(trigRange<0.218)//0.208 new lumi	
       triggerMenu = 40;
       else triggerMenu = 45;}

      //PU
      if (datasets[d].isData() == false && IReweight==true) weight *=LumiWeights->weight3D(event->runNb ,event->num_pileup_bc0,event->runNb); 
      if (datasets[d].isData() == false) weight *= sel.getWeightTauLegTMVA(triggerMenu)*sel.getWeightJetLegsTMVA(triggerMenu);
 
     float weight_BTAG = sel.getWeightAtLeast1bjetTMVA();
         
     datasets[d].eventTree ()->GetEntry (ievt);
     if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
     
     //Load event for the selection
     sel.LoadEvent(event);
     
     //Collection of selected objects
     //vector<NTMuon> selMuons         = sel.GetSelectedMuonsNoIso();
     //vector<NTElectron> selElectrons = sel.GetSelectedElectronsNoIso();
      vector<NTMuon> selMuons      = sel.GetSelectedMuonsLooseIDLooseIso();
      vector<NTElectron> selElectrons  = sel.GetSelectedElectronsLooseIDLooseIso();
  
     vector<NTTau>  selTausLoose     = sel.GetSelectedTaus(selMuons,selElectrons,false,1,0,true);////last false for no discri against electron and muon,  0 for Loose Iso/////
     vector<NTTau>  selTausMedium    = sel.GetSelectedTaus(selMuons,selElectrons,false,1,1,true);
     vector<TLorentzVector> tauObjTrig =sel.GetTauTrigObj();
     vector<NTJet>  selJets          = sel.GetSelectedJets(selMuons,selElectrons,selTausMedium,false,1,false,1);
     bool PassTriggerQuadJet40IsoPFTauSelection = sel.passTriggerQuadJet40IsoPFTauSelection();
     bool PassTriggerQuadJet45IsoPFTauSelection = sel.passTriggerQuadJet45IsoPFTauSelection(); 
     NTMET met = sel.GetMET();
     vector<TLorentzVector> jetObjTrig = sel.GetJetTrigObj();
	
     
     bool  PassTriggerQuadJet4045IsoPFTauSelection; 
     if (datasets[d].isData()==true) 
     {
         
       if ( ((PassTriggerQuadJet40IsoPFTauSelection && (event->runNb<165970  || (event->runNb>166782 && event->runNb<171050) ) ) ||
            (PassTriggerQuadJet45IsoPFTauSelection && ((event->runNb>=165970 && event->runNb<=166782) || (event->runNb>=171050 &&  event->runNb<178421)))   )) PassTriggerQuadJet4045IsoPFTauSelection = true; 
       else PassTriggerQuadJet4045IsoPFTauSelection = false; 
      }
     else PassTriggerQuadJet4045IsoPFTauSelection = true;
     //   std::cout<<"weight"<<weight<<std::endl;
     TMVA.FillHistos(selJets,jetObjTrig, met, selMuons, selElectrons, selTausLoose, selTausMedium, tauObjTrig, PassTriggerQuadJet4045IsoPFTauSelection, weight, weight_BTAG );
     
    }  // end of loop over evts
    
    
    if(datasets[d].isData() == true) TMVA.SaveHistos("TMVA_bkg_newLumi.root");
    else                             TMVA.SaveHistos("TMVA_sig_newLumi.root");
  }				
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  return (0);
}
