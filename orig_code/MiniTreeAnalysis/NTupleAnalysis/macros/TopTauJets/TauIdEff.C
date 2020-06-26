#include <iomanip>
#include <iostream>
#include "../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../Tools/interface/Dataset.h"
#include "../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../EffEstimation/interface/TauIdEff.h"
#include "../Selection/interface/SemiLeptonicTauSelection.h"


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
  string xmlFileName = string ("../config/TauIdEff.xml");
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

    
    TauIdEff tauIdEff;
    tauIdEff.CreateHistos();
    
    
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
      //if (verbosity > 3)
        //cout << "event " << ievt << endl;
     // if (verbosity > 3)
        //cout << "event number=" << event->eventNb << endl;
      //if (verbosity > 3)
     //cout << "run number=" << event->runNb << endl;
     
     //cout << "event number=" << event->eventNb << endl;
     if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
     //Load event for the selection
     sel.LoadEvent(event);
     //cout << "loaded "<<std::endl;
     
     //Collection of selected objects
     vector<NTElectron>  selElectrons = sel.GetSelectedElectrons();
     vector<NTMuon>      selMuons     = sel.GetSelectedMuons();
     //vector<NTTau>       noSelTaus    = sel.GetTaus(selMuons,selElectrons);
     vector<NTTau>       noSelTaus    = sel.GetTaus();
     //std::cout << noSelTaus.size()<<" "<<selElectrons.size()<<" " << selMuons.size()<<std::endl;
     vector<TLorentzVector> genTaus  = sel.GetGenTaus();
     vector<TLorentzVector> agenTaus = sel.GetGenATaus(); 
     tauIdEff.FillHistos(noSelTaus, genTaus, agenTaus);

    }  // end of loop over evts
    
    TString outputRootFile = "TauIdEff_"+datasets[d].Name()+".root";
    tauIdEff.SaveHistos(outputRootFile);
    
  }                             
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  return (0);
}
