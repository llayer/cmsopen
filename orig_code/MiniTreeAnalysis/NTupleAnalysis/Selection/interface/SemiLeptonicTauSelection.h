#ifndef SemiLeptonicTauSelection_h
#define SemiLeptonicTauSelection_h

#include "Selection.h"
#include "SelectionTable.h"
#include "../../Tools/interface/Dataset.h"

// system include files
#include <memory>
#include <vector>

#include "TMVA/Reader.h"
#include "../../KinFitter/interface/TtSemiLepKinFitProducer.h"

#include "TRandom.h"

using namespace std;
using namespace TopTree;

/**


*/

class SemiLeptonicTauSelection: public Selection{


   public:
      SemiLeptonicTauSelection();
      SemiLeptonicTauSelection(const SemiLeptonicTauSelection &);
      ~SemiLeptonicTauSelection();

      void SetParameters(int btagAlgo, float btagDiscriCut, int NofBtagJets, float METCut);
      vector<string> GetCutList(){return cuts_;};
      vector<string> GetChannelList(){return channels_;};
           
      int doFullSelection(Dataset* dataset, string channelName=string(""));/** return a integer which correspond to the last step that the event passes in the selection: [0-4] - possibility to check if the candPair correspond to the correct channel*/
      int FillTable(SelectionTable& selTable, Dataset* dataset, int idataset, float weight, int triggerMenu); /** Fill the selectionTable according to the result of doFullSelection  for an event of weight "weight" of a given dataset idataset - Returns the integer of doFullSelection() */
      std::vector<float> GetNNOutputAndKinFit();
      
      int GetbtagAlgo() const;
      float GetbtagDiscriCut() const;
      int GetNofBtagJetsCut() const; 

      bool passTriggerMuonSelection();
      bool passTriggerQuadJet40Selection();
      bool passTriggerQuadJet40IsoPFTauSelection();
      bool passTriggerQuadJet45IsoPFTauSelection();
      float passTriggerQuadJet40PrescaledSelection();
      float QuadJet40Prescale(); 
      float QuadJet40IsoPFTau40Prescale();
      float QuadJet45IsoPFTau45Prescale();
      
      float GetQuadJet40IsoPFTau40Weight();
      float GetQuadJet40IsoPFTau40JetLegTurnOn(float pT);
      Double_t CrystalBallInt(Double_t pT, Double_t mean, Double_t sigma, Double_t alpha, Double_t n, Double_t norm); 
      Double_t ApproxErf(Double_t arg);      
      float GetQuadJet40IsoPFTau40JetLegTurnOnFermi(float pT);
      Double_t GetQuadJet40IsoPFTau40JetLegTurnOnFermiError(float pT);
      float Fermi(Double_t pT, Double_t a_0, Double_t a_1, Double_t a_2);
      float GetQuadJet40IsoPFTau40WeightFermi();      
     
      float GetQuadJet40IsoPFTau40_HISTO_JET(float jet_Pt);
      float GetQuadJet40IsoPFTau40_HISTO_JET_ERROR(float jet_Pt);
      float GetQuadJet40IsoPFTau40_HISTO_TAU(float tau_Pt);
      float GetQuadJet40IsoPFTau40_HISTO_TAU_ERROR(float tau_Pt);
      float Trigger_QuadJet40_IsoPFTau40();
      float Trigger_QuadJet40_IsoPFTau40_ERROR();
      
      float GetQuadJet45IsoPFTau45_HISTO_JET(float jet_Pt);
      float GetQuadJet45IsoPFTau45_HISTO_JET_ERROR(float jet_Pt);
      float GetQuadJet45IsoPFTau45_HISTO_TAU(float tau_Pt);
      float GetQuadJet45IsoPFTau45_HISTO_TAU_ERROR(float tau_pT);
      float Trigger_QuadJet45_IsoPFTau45();
      float Trigger_QuadJet45_IsoPFTau45_ERROR();
      
      float hltFilter(std::string str);
      
      float getWeightTauLeg(int triggerMenu);
      float getWeightJetLegs(int triggerMenu);
      float getWeightTauLegTMVA(int triggerMenu);
      float getWeightJetLegsTMVA(int triggerMenu);

      float getWeightAtLeast1bjet();
      float getWeightAtLeast1bjetTMVA();
      float getWeightTau();
      
      std::vector<NTJet> GetBJetsForAna() const{return bjetsAna;};
      std::vector<NTJet> GetJetsForAna()  const{return jetsAna;};
      std::vector<NTTau> GetTausForAna()  const{return tausAna;};

    private:
      vector<string> cuts_;
      vector<string> channels_;
      
      int btagAlgo_;
      float btagDiscriCut_;
      int NofBtagJets_;
      float METCut_;
      
      std::vector<NTJet> bjetsAna;
      std::vector<NTJet> jetsAna;
      std::vector<NTTau> tausAna;
    
      TMVA::Reader *reader;
      float aplanarity;
      float circularity;
      float sphericity;  
      float chargeEta; 
      float Met;
      float Mt;
      float Ht;
      float chi2;
      float C;
      float D;
      float MTauJet;
      float M3;
      float M2;
      float Ht3;
      float DeltaPhiTauMet;
      float kinFitChi2;
      float kinFitTopMass;   
      
      TtSemiLepKinFitProducer kinFitter;
      
      TH1F* Jet40_Leg;
      TH1F* Jet45_Leg;
      TH1F* Tau40_Leg;
      TH1F* Tau45_Leg;      
      
      TRandom random;

};

#endif
