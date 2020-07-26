#ifndef TauIdEff_h
#define TauIdEff_h

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
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"


 
using namespace std;
//using namespace RooFit;
using namespace TopTree;


class TauIdEff {


   public:
      TauIdEff();
      ~TauIdEff();

     
     void CreateHistos();
     void SaveHistos(TString inputRootFile);
     void FillHistos(std::vector<NTTau>, std::vector<TLorentzVector>, std::vector<TLorentzVector>);
     void Plots();
     
 
    private:
    
     TH1F* tauId_reference; 
     TH1F* tauId_leadTrack; 
     TH1F* tauId_discr;
     TH1F* tauId_againstElectron;
     TH1F* tauId_againstMuon;
     TH1F* tauId_all;
     TH1F* tauId_eff;
     
     TH1F* tauId_gen; 
     TH1F* tauId_gen_matched; 
     TH1F* tauId_gen_eta; 
     TH1F* tauId_gen_matched_eta; 
     
};


#endif
