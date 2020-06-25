1	#ifndef TriggerCommissioning_h
2	#define TriggerCommissioning_h
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
19	#include "TLorentzVector.h"
20	#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
21	
22	
23	 
24	using namespace std;
25	//using namespace RooFit;
26	using namespace TopTree;
27	
28	
29	class TriggerCommissioning {
30	
31	
32	   public:
33	      TriggerCommissioning();
34	      ~TriggerCommissioning();
35	  
36	     void CreateHistos();
37	     void SaveHistos(TString inputRootFile);
38	     void FillHistos(std::vector<NTJet>, std::vector<NTTau>, bool, float, float, vector<TLorentzVector>, vector<TLorentzVector>);
39	     void Plots();
40	     
41	 
42	    private:
43	     
44	     //
45	     TH1F* reference_tau;
46	     TH1F* triggered_tau;
47	     TH1F* reference_tau_eta;
48	     TH1F* triggered_tau_eta;
49	     TH1F* eff_tau;
50	     TH1F* eff_tau_eta;
51	     
52	     TH1F* reference_tau_2;
53	     TH1F* triggered_tau_2;
54	     TH1F* eff_tau_2;
55	    
56	     TH1F* HLTtau_pt;
57	     TH1F* HLTtau_pt_diff;
58	     TH1F* HLTtau_eta;
59	     TH1F* HLTtau_eta_diff;
60	     TH1F* HLTtau_DR_diff;
61	   
62	    
63	     //
64	     TH1F* jet1_ref;
65	     TH1F* jet2_ref;
66	     TH1F* jet3_ref;
67	     TH1F* jet4_ref;
68	    
69	     TH1F* jet1_trig;
70	     TH1F* jet2_trig;
71	     TH1F* jet3_trig;
72	     TH1F* jet4_trig;
73	        
74	     TH1F* jet1_eff;
75	     TH1F* jet2_eff;
76	     TH1F* jet3_eff;
77	     TH1F* jet4_eff;
78	    
79	     TH1F* HLTTau_HLTJet_DR;
80	   
81	};
82	
83	
84	#endif
