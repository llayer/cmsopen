#ifndef Selection_h
#define Selection_h

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
#include "../../EffEstimation/interface/SFBweight.h"



// system include files
#include <memory>
#include <vector>
#include "TH2F.h"


using namespace std;
using namespace TopTree;


struct HighestPt{
    bool operator()( NTJet j1, NTJet j2 ) const{
        return j1.p4.Pt() > j2.p4.Pt() ;
    }
    bool operator()( NTLepton j1, NTLepton j2 ) const{
        return j1.p4.Pt() > j2.p4.Pt() ;
    }
};


class Selection{


   public:
      Selection();
      Selection(const Selection &);
      ~Selection();
   
      //Reset & load
      void Reset();/** collections of objects are cleared but the parameters are kepted*/
      void LoadEvent(const NTEvent*event); /** collections of objects are updated */
  
      //Add objects collection
      void AddCollection(std::vector<NTElectron> electrons);/** will also remove old collection */
      void AddCollection(std::vector<NTMuon> muons);/** will also remove old collection */
      void AddCollection(std::vector<NTCollection<NTTau> > tausVec);/** will also remove old collection */
      void AddCollection(std::vector<NTTau> taus);/** will also remove old collection */
      void AddCollection(std::vector<NTJetMet> jetmets);/** will also remove old collection */
      void AddCollection(std::vector<NTJet> jets);/** will also remove old collection */
      void AddCollection(std::vector<NTVertex> vertex);/** will also remove old collection */
      void AddCollection(std::vector<std::pair<string, bool> >  triggerList);/** will also remove old collection */
      void AddCollection(std::vector<std::pair<string, int> >  triggerPrescaleList);/** will also remove old collection */
      void AddMET(NTMET met);/** will replace actual MET */

      //Parameters
      bool SetDefaultJetCollection(string jetCollName) ; /** Returns true if a JetMet object of type jetCollName is found */
      bool SetDefaultTauCollection(string tauCollName) ; /** Returns true if a Tau object of type tauCollName is found */
      void SetElectronRequirements(float PtThr, float EtaThr, float RelIsoThr, float D0Thr, float VertexMatchThr_, float ElectronETSCThr, float DRemuThr);
      void SetMuonRequirements(float PtThr, float EtaThr, float RelIsoThr, float D0Thr, float VertexMatchThr_, float NValidHitsThr, float NValidTkHitsThr, float Chi2Thr);
      void SetTauRequirements(float PtThr, float EtaThr, float VertexMatchThr_, float LeadTrkPtThr);
      void SetJetRequirements(float PtThr, float EtaThr);
      void SetMETRequirements(float PtThr);     
      void SetVertexRequirements(float VertexNdofThr, float VertexZThr, float VertexRhoThr);


      //Get default objects
      std::vector<NTVertex>     GetVertex() const {return vertex;};
      std::vector<NTJet>        GetJets() const {return jets;};
      std::vector<NTElectron>   GetElectrons() const {return electrons;};
      std::vector<NTMuon>       GetMuons() const {return muons;};
      std::vector<NTTau>        GetTaus() const {return taus;};
      std::vector<NTTau>        GetTaus(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand) const;
      std::vector<TLorentzVector> GetGenTaus() const {return genTaus;};
      std::vector<TLorentzVector> GetGenATaus() const {return genATaus;};
      std::vector<TLorentzVector> GetTauTrigObj() const {return tauObjTrig;};
      std::vector<TLorentzVector> GetJetTrigObj() const {return jetObjTrig;};

      //Get selected objects
      std::vector<NTVertex>   GetSelectedVertex() const; 
      std::vector<NTJet>      GetScaledJets(float factor = 1.) const; 
      std::vector<NTJet>      GetSmearedJets(vector<NTJet> injets, float jetResFactor = 1.) const; 
      std::vector<NTJet>      GetSelectedJets(bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;
      std::vector<NTJet>      GetSelectedJets(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;
      std::vector<NTJet>      GetSelectedJets(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, std::vector<NTTau> tau_cand, bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;      
      std::vector<NTJet>      GetSelectedJets(std::vector<NTTau> tau_cand, bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;           
      std::vector<NTJet>      GetSelectedJets(float PtThr, float EtaThr, bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;
      std::vector<NTJet>      GetSelectedBJets(const std::vector<NTJet>& SelectedJets, const int& algo, const float & discricut) const;
      
      
      //Get selected electrons
      std::vector<NTElectron> GetScaledElectrons(float scale = 1.)const;
      std::vector<NTElectron> GetSmearedElectrons(float resol = 1.)const;
      std::vector<NTElectron> GetSelectedElectronsNoIso(bool applyLES = false, float scale = 1., bool applyLER = false, float resol = 1.) const;
      std::vector<NTElectron> GetSelectedElectronsLooseIDLooseIso(float PtThr, float EtaThr, bool applyLES = false, float scale = 1.) const;
      std::vector<NTElectron> GetSelectedElectronsLooseIDLooseIso(bool applyLES = false, float scale = 1.) const;      
      std::vector<NTElectron> GetSelectedElectrons(bool applyLES = false, float scale = 1., bool applyLER = false, float resol = 1.) const;
      std::vector<NTElectron> GetSelectedElectronsNoIso(float PtThr, float EtaThr, bool applyLES = false, float scale = 1., bool applyLER = false, float resol = 1.) const;
      std::vector<NTElectron> GetSelectedElectrons(float PtThr, float EtaThr, float ElectronRelIso, bool applyLES = false, float scale = 1., bool applyLER = false, float resol = 1.) const;   
      std::vector<NTElectron> GetSelectedElectronsIsoNonID(bool applyLES = false, float scale = 1., bool applyLER = false, float resol = 1.) const;
      std::vector<NTElectron> GetSelectedElectronsNonIsoNonID(bool applyLES = false, float scale = 1., bool applyLER = false, float resol = 1.) const;      
      
      //Get selected muons
      std::vector<NTMuon>     GetScaledMuons(float scale = 1.) const;
      std::vector<NTMuon>     GetSelectedMuonsNoIso(bool applyLES = false, float scale = 1.) const;
      std::vector<NTMuon>     GetSelectedMuons(bool applyLES = false, float scale = 1.) const;
      std::vector<NTMuon>     GetSelectedMuonsLooseIDLooseIso(float PtThr, float EtaThr, bool applyLES = false, float scale= 1.) const;
      std::vector<NTMuon>     GetSelectedMuonsLooseIDLooseIso(bool applyLES = false, float scale= 1.) const;
      std::vector<NTMuon>     GetSelectedMuonsNoIso(float PtThr, float EtaThr, bool applyLES = false, float scale = 1.) const;
      std::vector<NTMuon>     GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso, bool applyLES = false, float scale = 1.) const;
      std::vector<NTTau>      GetScaledTaus(float scale = 1.) const;
      
      std::vector<NTTau>      GetSelectedTaus(float PtThr, float EtaThr, bool applyLES = false, float scale = 1., int isoLevel = 0., bool antiLep = true) const;
      std::vector<NTTau>      GetSelectedTaus(bool applyLES = false, float scale = 1., int isoLevel = 0., bool antiLep = true) const;
      std::vector<NTTau>      GetSelectedTaus(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, float PtThr, float EtaThr, bool applyLES = false , float scale = 1., int isoLevel = 0., bool antiLep = true) const;
      std::vector<NTTau>      GetSelectedTaus(std::vector<NTMuon> muon_cand, std::vector<NTElectron> elec_cand, bool applyLES = false , float scale = 1., int isoLevel = 0., bool antiLep = true) const;   
      std::vector<NTMuon>     GetSelectedMuonIsoNonID(bool applyLES = false, float scale = 1.) const;
      
      //for muon+jets selection
      std::vector<NTMuon>     GetSelectedMuonsForMuJets(bool applyLES = false, float scale = 1.) const;
      std::vector<NTMuon>     GetSelectedMuonsNoIsoForMuJets(float PtThr, float EtaThr, bool applyLES = false, float scale = 1.) const;
      std::vector<NTMuon>     GetSelectedMuonsForMuJets(float PtThr, float EtaThr, float MuonRelIso, bool applyLES = false, float scale = 1.) const;
      std::vector<NTMuon>     GetSelectedLooseMuonsForMuJets(bool applyLES = false, float scale = 1.) const;
      std::vector<NTElectron> GetSelectedLooseElectronsForMuJets(bool applyLES = false, float scale = 1.) const;
      
      //btag weight
//      void InitSFBWeight(int flagb, int methodb, int systb);
      void InitSFBWeight (int flagb, int methodb, int systb, int btagAlgo, float btagDiscriCut, int btagNjets);
      void ReInitSFBWeight (int flagb, int methodb, int systb, int btagAlgo, float btagDiscriCut, int btagNjets);
      void ResetParameters4Bweight(int flagb, int methodb, int systb);
      int GetFlagb() const;
      int GetMethodb() const;
      int GetSystb() const;
      const SFBweight& GetSFBweight() const;

      
      NTMET GetScaledMET(float scale = 1.) const; 
      NTMET GetSmearedMET(vector<NTJet> injets, float jetResFactor = 1.) const;
      NTMET GetUnclusScaledMET(bool applyUnclusScale, float scale) const; /** It's the clustered MET with a scale factor on the unclustered energy !! */
      NTMET GetMET(bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;
      
      NTMET GetScaledType1METWithJER(vector<NTJet> injets, bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;
      NTMET GetType1MET(             vector<NTJet> injets, bool applyJES = false, float scale = 1., bool applyJER = false, float ResFactor = 0.) const;  
      NTMET GetScaledType1MET(NTMET &themet, float scale = 1.) const; 
      NTMET GetSmearedType1MET(NTMET &themet, vector<NTJet> injets, float jetResFactor = 1.) const;
   
      NTMET GetGenMET(std::vector<WDecaysMC> &wAndDecays);
   
      std::vector<std::pair<string, bool> >  getTriggerList(){return triggerList;};
      std::vector<std::pair<string, int> >   getTriggerPrescaleList(){return triggerPrescaleList;};
      double getEleHLTMatch(){return eleHLTMatch;};
      int getRunNumber(){return runNumber;};
      int getEventNumber(){return eventNumber;};
      bool isAnEventSelected(unsigned int nElectrons, unsigned int nMuons, unsigned int nTaus, unsigned int nJets);

      void GeneratePUWeight(string PUWeightFileName); //** to use at initialisation **/
      float GetPUWeight(); //** use event by event **/
      vector<double> generate_flat10_weights(TH1D* data_npu_estimated); /** internal method **/
      
      std::vector<std::pair<string, bool> >  triggerList;
      std::vector<std::pair<string, int> >   triggerPrescaleList;
      double eleHLTMatch;
      int eventNumber;
      int runNumber;
      int tmeme;
      
      
      void LoadElScaleFactors(); /** collections of objects are updated */
      void LoadMuScaleFactors(); /** collections of objects are updated */
      
      TH2F * getScaleFactEl(){return scaleFactEl;};
      TH2F * getScaleFactMu(){return scaleFactMu;};

       //Treatment of the JES Uncertainty
       void InitJESUnc ();
       TH2F* histo_jesunc_;

      
   private:
      
      vector<double> PUWeights;
      
      TH2F * scaleFactEl;
      TH2F * scaleFactMu;
      
      
      
      int npu; 
      std::vector<NTJetMet> jetmets;
      string JetType; /** correspond to the jet algo of the NTJetMT object chosen as default*/
      std::vector<NTJet> jets; //chosen collection
      NTMET met;
      std::vector<NTElectron> electrons;
      std::vector<NTMuon>     muons;
      std::vector<NTCollection<NTTau> > tausVec;
      string TauType; /** correspond to the tau algo of the NTCollection<NTTau> object chosen as default*/
      std::vector<NTTau>      taus;
      std::vector<NTVertex>   vertex; 
      std::vector<TLorentzVector> genTaus;
      std::vector<TLorentzVector> genATaus;
      std::vector<TLorentzVector> tauObjTrig;
      std::vector<TLorentzVector> jetObjTrig;
   
      std::vector<WDecaysMC>   wAndDecays;
      std::vector<ZDecaysMC>   zAndDecays;
      
      
      //jets
      float JetPtThreshold_;
      float JetEtaThreshold_;
      //muon
      float MuonPtThreshold_;
      float MuonEtaThreshold_;
      float MuonRelIso_;
      //
      float MuonNofValidTrHits_;
      float MuonNofValidHits_;
      float MuonD0Cut_;
      float MuonNormChi2_;
      float MuonVertexMatchThr_;
      //electron
      float ElectronPtThreshold_;
      float ElectronEtaThreshold_;
      float ElectronRelIso_;
      float ElectronD0Cut_;
      float ElectronVertexMatchThr_;
      float ElectronETSCThr_;
      float DRemuThr_;
      //tau
      float TauPtThreshold_;
      float TauEtaThreshold_;
      float TauLeadTrkPtCut_;
      float TauVertexMatchThr_;
      //vertex
      float VertexNdofThr_;
      float VertexZThr_;
      float VertexRhoThr_;
      //met
      float METThreshold_;
      // specific to btag weight;
      int flag_btagweight_;
      SFBweight sfb_;
      int methodb_;
      int systb_;

};

void InitVectorOfWeight(vector<float>& weightb);

void LoadBWeight(Selection& sel, vector<float>& weightb, const vector<NTJet>& theselJets);      
#endif
