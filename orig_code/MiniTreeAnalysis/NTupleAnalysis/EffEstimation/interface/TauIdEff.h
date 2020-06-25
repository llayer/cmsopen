1	#ifndef TauIdEff_h
2	#define TauIdEff_h
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
20	
21	
22	 
23	using namespace std;
24	//using namespace RooFit;
25	using namespace TopTree;
26	
27	
28	class TauIdEff {
29	
30	
31	   public:
32	      TauIdEff();
33	      ~TauIdEff();
34	
35	     
36	     void CreateHistos();
37	     void SaveHistos(TString inputRootFile);
38	     void FillHistos(std::vector<NTTau>, std::vector<TLorentzVector>, std::vector<TLorentzVector>);
39	     void Plots();
40	     
41	 
42	    private:
43	    
44	     TH1F* tauId_reference; 
45	     TH1F* tauId_leadTrack; 
46	     TH1F* tauId_discr;
47	     TH1F* tauId_againstElectron;
48	     TH1F* tauId_againstMuon;
49	     TH1F* tauId_all;
50	     TH1F* tauId_eff;
51	     
52	     TH1F* tauId_gen; 
53	     TH1F* tauId_gen_matched; 
54	     TH1F* tauId_gen_eta; 
55	     TH1F* tauId_gen_matched_eta; 
56	     
57	};
58	
59	
60	#endif
