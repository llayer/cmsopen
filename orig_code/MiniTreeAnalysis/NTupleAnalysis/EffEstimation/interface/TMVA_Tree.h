1	#ifndef TMVA_Tree_h
2	#define TMVA_Tree_h
3	
4	//#include ".h"
5	
6	// system include files
7	#include <memory>
8	#include <vector>
9	#include <iostream>
10	#include <string>
11	#include "TH1F.h"
12	#include "TH2F.h"
13	#include "TH3F.h"
14	#include "THStack.h"
15	#include "TF1.h"
16	#include "TCanvas.h"
17	#include "TStyle.h"
18	#include "TFile.h"
19	#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
20	#include "../../Tools/interface/EventShapes.h"
21	
22	#include "TTree.h"
23	#include "TMVA/Reader.h"
24	
25	#include "../../KinFitter/interface/TtSemiLepKinFitProducer.h"
26	
27	using namespace std;
28	//using namespace RooFit;
29	using namespace TopTree;
30	
31	
32	class TMVA_Tree {
33	
34	
35	   public:
36	      TMVA_Tree();
37	      ~TMVA_Tree();
38	
39	     
40	     void CreateHistos(TString inputRootFile);
41	     void SaveHistos(TString inputRootFile);
42	     void FillHistos(std::vector<NTJet> theJets, vector<TLorentzVector> trigJet, NTMET met,std::vector<NTMuon> selMuons, std::vector<NTElectron> selElectrons, std::vector<NTTau> theTausLoose, std::vector<NTTau> theTausMedium, vector<TLorentzVector> tauObjTrig, bool PassTriggerQuadJet4045IsoPFTauSelection, const float& weight, float& weight_BTAG);
43	
44	     void Plots();
45	     
46	 
47	    private:
48	       
49	     TTree *T;
50	     TFile *file;
51	     
52	     TMVA::Reader *reader;
53	
54	     float weight_;
55	     float weight_BTAG_;
56	
57	     float aplanarity_;
58	     float sphericity_;
59	     float circularity_;
60	     float Ht_; 
61	     float Ht3_;
62	     float Mt_;
63	     float chargeEta_;
64	     float Met_;
65	     float MTauJet_;
66	     float M3_;
67	     float M2_;
68	     float chi2_;
69	     float C_;
70	     float D_;
71	     float DeltaPhiTauMet_;
72	     float NNOutput_;
73	     float TauPt_;
74	     float TauEta_;
75	     int jetMultiplicity_;
76	     float jetPt_;
77	     float jetEta_;
78	     float kinFitChi2_;
79	     float kinFitTopMass_;
80	     
81	     //vector<NTJet> btagjets;
82	     //vector<float> btagDiscri;
83	     
84	     TtSemiLepKinFitProducer kinFitter;
85	};
86	
87	
88	#endif
