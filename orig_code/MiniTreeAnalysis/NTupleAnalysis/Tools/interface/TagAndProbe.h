#ifndef TagAndProbe_h
#define TagAndProbe_h

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
//#include "RooGlobalFunc.h"


 
using namespace std;
//using namespace RooFit;
using namespace TopTree;


class TagAndProbe {


   public:
      TagAndProbe();
      ~TagAndProbe();

     
     void SetLooseIDMuonCollection(      std::vector<NTMuon>      inmuons)    {looseIsoMuons     = inmuons    ;};
     void SetLooseIDElectronCollection(  std::vector<NTElectron>  inelectrons){looseIsoElectrons = inelectrons;};
     void SetLooseIsoMuonCollection(     std::vector<NTMuon>      inmuons)    {looseIDMuons      = inmuons    ;};
     void SetLooseIsoElectronCollection( std::vector<NTElectron>  inelectrons){looseIDElectrons  = inelectrons;};
     
     void SetTightMuonCollection(     std::vector<NTMuon>      inmuons)    {tightMuons     = inmuons    ;};
     void SetTightElectronCollection( std::vector<NTElectron>  inelectrons){tightElectrons = inelectrons;};
     
     void CreateHistos(string TypeSel, string datasetname);
     void ReadHistos(TString inputRootFile);
     void FillHistos(std::vector<NTJet> thejets, string TypeSel, string datasetname, double);
     void CalculateFakeRateLeptonEff();
     
     
     void Savehistos(TString outputFileName);
     
     
     
    
    private:
     
     

      std::vector<TH1F*> hlist_Mu_LooseID; 
      std::vector<TH1F*> hlist_Mu_LooseIso; 
      std::vector<TH1F*> hlist_Mu_Tight; 
      
      std::vector<TH1F*> hlist_El_LooseID; 
      std::vector<TH1F*> hlist_El_LooseIso; 
      std::vector<TH1F*> hlist_El_Tight; 
      
      
      

      
      
      std::vector<NTMuon>     tightMuons; 
      std::vector<NTElectron> tightElectrons; 
      std::vector<NTMuon>     looseIsoMuons; 
      std::vector<NTElectron> looseIsoElectrons; 
      std::vector<NTMuon>     looseIDMuons; 
      std::vector<NTElectron> looseIDElectrons; 
           
      // emu (for bkg)
      THStack *hstack1 ;
      TH1F *all1;
      
   


};


#endif
