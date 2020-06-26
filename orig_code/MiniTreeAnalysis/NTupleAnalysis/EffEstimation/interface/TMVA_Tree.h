#ifndef TMVA_Tree_h
#define TMVA_Tree_h

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
#include "../../Tools/interface/EventShapes.h"

#include "TTree.h"
#include "TMVA/Reader.h"

#include "../../KinFitter/interface/TtSemiLepKinFitProducer.h"

using namespace std;
//using namespace RooFit;
using namespace TopTree;


class TMVA_Tree {


   public:
      TMVA_Tree();
      ~TMVA_Tree();

     
     void CreateHistos(TString inputRootFile);
     void SaveHistos(TString inputRootFile);
     void FillHistos(std::vector<NTJet> theJets, vector<TLorentzVector> trigJet, NTMET met,std::vector<NTMuon> selMuons, std::vector<NTElectron> selElectrons, std::vector<NTTau> theTausLoose, std::vector<NTTau> theTausMedium, vector<TLorentzVector> tauObjTrig, bool PassTriggerQuadJet4045IsoPFTauSelection, const float& weight, float& weight_BTAG);

     void Plots();
     
 
    private:
       
     TTree *T;
     TFile *file;
     
     TMVA::Reader *reader;

     float weight_;
     float weight_BTAG_;

     float aplanarity_;
     float sphericity_;
     float circularity_;
     float Ht_; 
     float Ht3_;
     float Mt_;
     float chargeEta_;
     float Met_;
     float MTauJet_;
     float M3_;
     float M2_;
     float chi2_;
     float C_;
     float D_;
     float DeltaPhiTauMet_;
     float NNOutput_;
     float TauPt_;
     float TauEta_;
     int jetMultiplicity_;
     float jetPt_;
     float jetEta_;
     float kinFitChi2_;
     float kinFitTopMass_;
     
     //vector<NTJet> btagjets;
     //vector<float> btagDiscri;
     
     TtSemiLepKinFitProducer kinFitter;
};


#endif
