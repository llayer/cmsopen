1	#ifndef FakeRate_LeptEff_h
2	#define FakeRate_LeptEff_h
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
20	//#include "RooGlobalFunc.h"
21	
22	
23	 
24	using namespace std;
25	//using namespace RooFit;
26	using namespace TopTree;
27	
28	
29	class FakeRate_LeptEff {
30	
31	
32	   public:
33	      FakeRate_LeptEff();
34	      ~FakeRate_LeptEff();
35	
36	     
37	     void SetLooseMuonCollection(   std::vector<NTMuon>      inmuons)    {muons     = inmuons    ;};
38	     void SetElectronMuonCollection(std::vector<NTElectron>  inelectrons){electrons = inelectrons;};
39	     
40	     void CreateHistos(string TypeSel, string datasetname);
41	     void ReadHistos(TString inputRootFile);
42	     void FillHistos(NTMET theMet, std::vector<NTJet>, string TypeSel, string datasetname);
43	     void CalculateFakeRateLeptonEff();
44	     
45	     
46	     void ShowFit(float pt, float eta, float phi, int njet, int param);
47	     void Savehistos(TString outputFileName);
48	     
49	     std::pair<float, float> GetFakeRateMuonPt(    float pt  );
50	     std::pair<float, float> GetFakeRateMuonEta(   float eta );
51	     std::pair<float, float> GetFakeRateMuonPhi(   float phi );
52	     std::pair<float, float> GetFakeRateMuonNJets( int njet  );
53	     
54	     std::pair<float, float> GetFakeRateMuonPtEta(   float pt,  float eta   );
55	     std::pair<float, float> GetFakeRateMuonPtNJet(  float pt,  int   njets );
56	     std::pair<float, float> GetFakeRateMuonEtaNJet( float eta, int   njets );
57	     
58	     std::pair<float, float> GetLeptEffMuonPt(    float pt  );
59	     std::pair<float, float> GetLeptEffMuonEta(   float eta );
60	     std::pair<float, float> GetLeptEffMuonPhi(   float phi );
61	     std::pair<float, float> GetLeptEffMuonNJets( int njet  );
62	     
63	     std::pair<float, float> GetLeptEffMuonPtEta(   float pt,  float eta   );
64	     std::pair<float, float> GetLeptEffMuonPtNJet(  float pt,  int   njets );
65	     std::pair<float, float> GetLeptEffMuonEtaNJet( float eta, int   njets );
66	     
67	     
68	     std::pair<float, float> GetFakeRateElectronPt(    float pt  );
69	     std::pair<float, float> GetFakeRateElectronEta(   float eta );
70	     std::pair<float, float> GetFakeRateElectronPhi(   float phi );
71	     std::pair<float, float> GetFakeRateElectronNJets( int njet  );
72	     
73	     std::pair<float, float> GetFakeRateElectronPtEta(   float pt,  float eta   );
74	     std::pair<float, float> GetFakeRateElectronPtNJet(  float pt,  int   njets );
75	     std::pair<float, float> GetFakeRateElectronEtaNJet( float eta, int   njets );
76	     
77	     std::pair<float, float> GetLeptEffElectronPt(    float pt  );
78	     std::pair<float, float> GetLeptEffElectronEta(   float eta );
79	     std::pair<float, float> GetLeptEffElectronPhi(   float phi );
80	     std::pair<float, float> GetLeptEffElectronNJets( int njet  );
81	     
82	     std::pair<float, float> GetLeptEffElectronPtEta(   float pt,  float eta   );
83	     std::pair<float, float> GetLeptEffElectronPtNJet(  float pt,  int   njets );
84	     std::pair<float, float> GetLeptEffElectronEtaNJet( float eta, int   njets );
85	     
86	    
87	    private:
88	     
89	     
90	
91	      std::vector<TH2F*> hlist_Mu_Loose,    hlist_Mu_Tight; 
92	      std::vector<TH3F*> hlist_Mu_Loose_2D, hlist_Mu_Tight_2D; 
93	      
94	      
95	      
96	      std::vector<TH2F*> hlist_Mu_Fake_Data;
97	      std::vector<TH3F*> hlist_Mu_Fake_Data_2D;
98	      
99	      std::vector<TH2F*> hlist_Mu_LeptEff_Data;
100	      std::vector<TH3F*> hlist_Mu_LeptEff_Data_2D;
101	      
102	      
103	
104	      std::vector<TH2F*> hlist_El_Loose,    hlist_El_Tight; 
105	      std::vector<TH3F*> hlist_El_Loose_2D, hlist_El_Tight_2D; 
106	      
107	      std::vector<TH1F*> hlist_El_Fake_Data;
108	      std::vector<TH2F*> hlist_El_Fake_Data_2D;
109	      
110	      std::vector<TH1F*> hlist_El_LeptEff_Data;
111	      std::vector<TH2F*> hlist_El_LeptEff_Data_2D;
112	      
113	      std::vector<NTMuon>     muons; 
114	      std::vector<NTElectron> electrons; 
115	           
116	      // emu (for bkg)
117	      THStack *hstack1 ;
118	      TH1F *all1;
119	      
120	   
121	
122	
123	};
124	
125	
126	#endif
