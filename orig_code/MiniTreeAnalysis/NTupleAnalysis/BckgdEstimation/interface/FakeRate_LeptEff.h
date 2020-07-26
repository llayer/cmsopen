#ifndef FakeRate_LeptEff_h
#define FakeRate_LeptEff_h

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


class FakeRate_LeptEff {


   public:
      FakeRate_LeptEff();
      ~FakeRate_LeptEff();

     
     void SetLooseMuonCollection(   std::vector<NTMuon>      inmuons)    {muons     = inmuons    ;};
     void SetElectronMuonCollection(std::vector<NTElectron>  inelectrons){electrons = inelectrons;};
     
     void CreateHistos(string TypeSel, string datasetname);
     void ReadHistos(TString inputRootFile);
     void FillHistos(NTMET theMet, std::vector<NTJet>, string TypeSel, string datasetname);
     void CalculateFakeRateLeptonEff();
     
     
     void ShowFit(float pt, float eta, float phi, int njet, int param);
     void Savehistos(TString outputFileName);
     
     std::pair<float, float> GetFakeRateMuonPt(    float pt  );
     std::pair<float, float> GetFakeRateMuonEta(   float eta );
     std::pair<float, float> GetFakeRateMuonPhi(   float phi );
     std::pair<float, float> GetFakeRateMuonNJets( int njet  );
     
     std::pair<float, float> GetFakeRateMuonPtEta(   float pt,  float eta   );
     std::pair<float, float> GetFakeRateMuonPtNJet(  float pt,  int   njets );
     std::pair<float, float> GetFakeRateMuonEtaNJet( float eta, int   njets );
     
     std::pair<float, float> GetLeptEffMuonPt(    float pt  );
     std::pair<float, float> GetLeptEffMuonEta(   float eta );
     std::pair<float, float> GetLeptEffMuonPhi(   float phi );
     std::pair<float, float> GetLeptEffMuonNJets( int njet  );
     
     std::pair<float, float> GetLeptEffMuonPtEta(   float pt,  float eta   );
     std::pair<float, float> GetLeptEffMuonPtNJet(  float pt,  int   njets );
     std::pair<float, float> GetLeptEffMuonEtaNJet( float eta, int   njets );
     
     
     std::pair<float, float> GetFakeRateElectronPt(    float pt  );
     std::pair<float, float> GetFakeRateElectronEta(   float eta );
     std::pair<float, float> GetFakeRateElectronPhi(   float phi );
     std::pair<float, float> GetFakeRateElectronNJets( int njet  );
     
     std::pair<float, float> GetFakeRateElectronPtEta(   float pt,  float eta   );
     std::pair<float, float> GetFakeRateElectronPtNJet(  float pt,  int   njets );
     std::pair<float, float> GetFakeRateElectronEtaNJet( float eta, int   njets );
     
     std::pair<float, float> GetLeptEffElectronPt(    float pt  );
     std::pair<float, float> GetLeptEffElectronEta(   float eta );
     std::pair<float, float> GetLeptEffElectronPhi(   float phi );
     std::pair<float, float> GetLeptEffElectronNJets( int njet  );
     
     std::pair<float, float> GetLeptEffElectronPtEta(   float pt,  float eta   );
     std::pair<float, float> GetLeptEffElectronPtNJet(  float pt,  int   njets );
     std::pair<float, float> GetLeptEffElectronEtaNJet( float eta, int   njets );
     
    
    private:
     
     

      std::vector<TH2F*> hlist_Mu_Loose,    hlist_Mu_Tight; 
      std::vector<TH3F*> hlist_Mu_Loose_2D, hlist_Mu_Tight_2D; 
      
      
      
      std::vector<TH2F*> hlist_Mu_Fake_Data;
      std::vector<TH3F*> hlist_Mu_Fake_Data_2D;
      
      std::vector<TH2F*> hlist_Mu_LeptEff_Data;
      std::vector<TH3F*> hlist_Mu_LeptEff_Data_2D;
      
      

      std::vector<TH2F*> hlist_El_Loose,    hlist_El_Tight; 
      std::vector<TH3F*> hlist_El_Loose_2D, hlist_El_Tight_2D; 
      
      std::vector<TH1F*> hlist_El_Fake_Data;
      std::vector<TH2F*> hlist_El_Fake_Data_2D;
      
      std::vector<TH1F*> hlist_El_LeptEff_Data;
      std::vector<TH2F*> hlist_El_LeptEff_Data_2D;
      
      std::vector<NTMuon>     muons; 
      std::vector<NTElectron> electrons; 
           
      // emu (for bkg)
      THStack *hstack1 ;
      TH1F *all1;
      
   


};


#endif
