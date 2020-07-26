#ifndef TriggerCommissioning_h
#define TriggerCommissioning_h

//#include ".h"

// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "THStack.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"


 
using namespace std;
//using namespace RooFit;
using namespace TopTree;


class TriggerCommissioning {


   public:
      TriggerCommissioning();
      ~TriggerCommissioning();
  
     void CreateHistos();
     void SaveHistos(TString inputRootFile);
     void FillHistos(std::vector<NTJet>, std::vector<NTTau>, bool, float, float, vector<TLorentzVector>, vector<TLorentzVector>);
     void Plots();
     
 
    private:
     
     //
     TH1F* reference_tau;
     TH1F* triggered_tau;
     TH1F* reference_tau_eta;
     TH1F* triggered_tau_eta;
     TH1F* eff_tau;
     TH1F* eff_tau_eta;
     
     TH1F* reference_tau_2;
     TH1F* triggered_tau_2;
     TH1F* eff_tau_2;
    
     TH1F* HLTtau_pt;
     TH1F* HLTtau_pt_diff;
     TH1F* HLTtau_eta;
     TH1F* HLTtau_eta_diff;
     TH1F* HLTtau_DR_diff;
   
    
     //
     TH1F* jet1_ref;
     TH1F* jet2_ref;
     TH1F* jet3_ref;
     TH1F* jet4_ref;
    
     TH1F* jet1_trig;
     TH1F* jet2_trig;
     TH1F* jet3_trig;
     TH1F* jet4_trig;
        
     TH1F* jet1_eff;
     TH1F* jet2_eff;
     TH1F* jet3_eff;
     TH1F* jet4_eff;
    
     TH1F* HLTTau_HLTJet_DR;
   
};


#endif
