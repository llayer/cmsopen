1	#ifndef SFBweight_h
2	#define SFBweight_h
3	
4	#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
5	
6	// system include files
7	#include <memory>
8	#include <assert.h>
9	#include <typeinfo>
10	#include <vector>
11	#include <map>
12	#include <sstream>
13	#include <fstream>
14	#include "TROOT.h"
15	#include "TCanvas.h"
16	#include "TH2D.h"
17	#include "TGraph.h"
18	#include "TFile.h"
19	
20	
21	
22	using namespace std;
23	using namespace TopTree;
24	
25	class SFBweight {
26	
27	   public:
28	        SFBweight();
29	        SFBweight(const SFBweight& sfw);
30	        SFBweight(int, float, int );
31	        ~SFBweight();
32	
33	        void SFBinit(int, float, int );
34	        void LoadInfo();
35	        void LoadInfo2();
36	        void InitAlgoAndWP(int algo, float wp);
37	        float GetWeight(int, int, float, float) const;
38	        float GetWeight_BTV003(int info,  int quarkorigin, float pt, float eta, int sys) const;
39	        float GetWeight_JP_BTV003(int info,  int quarkorigin, float pt, float eta, int sys) const;
40	        
41	        vector<float> GetWeigth4BSel(int,  int, const std::vector<NTJet> &  selJets) const;
42	        vector<float> GetWeigth4BSel(int method_b,  int syst_b, const std::vector<NTJet> &  selJets, float sf_val_for_b, float sf_val_for_l) const;
43	        const TH2D* GetHistoSFB() const;
44	        const TH2D* GetHistoEffMCb() const;
45	        const TH2D* GetHistoEffMCc() const;
46	        const TH2D* GetHistoEffMCl() const;
47	
48	        int Test() const {return map_effmcc_.size();};
49	
50	   private:
51	        std::string method_origin1_;
52	        std::string method_origin2_;
53	
54	
55	
56	        TH2D* histo_sfvalb_;
57	        TH2D* histo_sferrb_;
58	        TH2D* histo_sfvall_;
59	        TH2D* histo_sferrl_;
60	
61	        TH2D* histo_effvalb_;
62	        TH2D* histo_efferrb_;
63	        TH2D* histo_effvall_;
64	        TH2D* histo_efferrl_;
65	
66	        //access to the current histo for the algo/wp initialized
67	
68	        TH2D* histo_effmcb_;
69	        TH2D* histo_effmcc_;
70	        TH2D* histo_effmcl_;
71	
72	        TH2D* histo_errmcb_;
73	        TH2D* histo_errmcc_;
74	        TH2D* histo_errmcl_;
75	                
76	        //map containing all the algo/wp
77	
78	        map<string,TH2D*> map_effmcb_;
79	        map<string,TH2D*> map_effmcc_;
80	        map<string,TH2D*> map_effmcl_;
81	
82	        map<string,TH2D*> map_errmcb_;
83	        map<string,TH2D*> map_errmcc_;
84	        map<string,TH2D*> map_errmcl_;
85	        
86	
87	        int btag_algo_;
88	        float btag_discri_;
89	        int n_bjets_;
90	
91	};
92	
93	#endif
94	
95	
