1	#ifndef GeneralMMEstimation_h
2	#define GeneralMMEstimation_h
3	
4	// system include files
5	#include <memory>
6	#include <vector>
7	#include <iostream>
8	#include <string>
9	#include "TH1F.h"
10	//#include "THStack.h"
11	#include "TF1.h"
12	
13	#include "TRandom.h"
14	#include "TMatrixF.h"
15	#include "TCanvas.h"
16	
17	#include "TLegend.h"
18	
19	#include "TGraphErrors.h"
20	
21	
22	#include "../../Tools/interface/Dataset.h"
23	#include "../../Selection/interface/Selection.h"
24	#include "../../Selection/interface/SelectionTable.h"
25	#include "../../Selection/interface/DiLeptonSelection.h"
26	#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
27	
28	
29	using namespace std;
30	//using namespace TopTree;
31	
32	
33	struct Isolations{
34	  float iso1[4]; 
35	  float iso2[4];
36	};
37	
38	struct NSelected{
39	  float NSel[4]; 
40	};
41	
42	struct MMEstimated{
43	 float NofMMEstimatedTT[4];
44	 float NofMMEstimatedTF[4];
45	 float NofMMEstimatedFT[4];
46	 float NofMMEstimatedFF[4];
47	 float MMEstimatedTTErr[4];
48	 float MMEstimatedTFErr[4];
49	 float MMEstimatedFTErr[4];
50	 float MMEstimatedFFErr[4];
51	};
52	
53	struct MMEstimatedPlots{
54	      TH1F * MMEstimated_TT[4];
55	      TH1F * MMEstimated_TF[4];
56	      TH1F * MMEstimated_FT[4];
57	      TH1F * MMEstimated_FF[4];
58	};
59	
60	struct MMExpectedPlots{
61	      string Name[4];
62	      TH1F* MMExpected[4];
63	};
64	
65	struct MMEpsilons{
66	  float EpsilonESignal;
67	  float EpsilonESignalErr;
68	  float EpsilonEFake;
69	  float EpsilonEFakeErr;
70	  float EpsilonMuSignal;
71	  float EpsilonMuSignalErr;
72	  float EpsilonMuFake;
73	  float EpsilonMuFakeErr;
74	};
75	
76	
77	struct Distribution{
78	   TH1F * NMMEstimatedTTDistribution[4];
79	   TH1F * NMMEstimatedTFDistribution[4];
80	   TH1F * NMMEstimatedFTDistribution[4];
81	   TH1F * NMMEstimatedFFDistribution[4];
82	};
83	
84	class GeneralMMEstimation {
85	
86	
87	   public:
88	      GeneralMMEstimation(vector<Dataset>, float, float, float, unsigned int, float, float, string);
89	      ~GeneralMMEstimation();
90	
91	      void CountNSel(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int);
92	      void FillMMExpected(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int, const NTEvent*);
93	      void RunTheMatrixMethod(vector<struct MMEpsilons>, unsigned int, bool, bool, bool);
94	      void ReadMMFile(string);
95	      void ReadMMFileForPullTest(string, float, float, float, float);
96	      vector<struct MMEstimated> GetMMEstimated();
97	      void PrintMMEstimated();
98	      struct MMEstimatedPlots GetMMEstimatedPlots();
99	      vector<struct MMExpectedPlots> GetMMExpectedPlots();
100	      void WriteMMFile(string);
101	      void WriteMMFileFast(string);
102	      void WriteMMFileFastForPullTest(string, string, float, float, float, float);
103	
104	    private:
105	
106	      struct Isolations GetIsolations();
107	      void IncrementNSelBin(unsigned int, unsigned int, float);
108	      void FillMMExpectedPlot(string, unsigned int, float, float);
109	      void IncludeStatisticalError();
110	      void IncludeSystematicError();
111	      void SolveTheSystem(bool);
112	      void FillDistributions(unsigned int);
113	      void CalculateRms(unsigned int, unsigned int);
114	      void SetMMEstimated(unsigned int, unsigned int);
115	
116	      vector<struct Distribution> theDistributions;
117	      unsigned int theNBins; 
118	      float theMinBin;
119	      float theMaxBin;
120	
121	      string IsoNames[4];
122	      string theChannel;
123	      struct Isolations theIsolations;
124	
125	      TRandom ranEpsEFake, ranEpsESignal, ranEpsMuFake, ranEpsMuSignal, ranN1, ranN2, ranN3, ranN4;
126	      float locNSelected[4];
127	      float EpsilonEFake;
128	      float EpsilonEFakeErr;
129	      float EpsilonESignal;
130	      float EpsilonESignalErr;
131	      float EpsilonMuFake;
132	      float EpsilonMuFakeErr;
133	      float EpsilonMuSignal;
134	      float EpsilonMuSignalErr;
135	
136	      vector<struct NSelected> theNSelected;
137	
138	      float NMMEstimatedTT[4];
139	      float NMMEstimatedTF[4];
140	      float NMMEstimatedFT[4];
141	      float NMMEstimatedFF[4];
142	
143	      TH1F* histoTT[4];
144	      TH1F* histoTF[4];
145	      TH1F* histoFT[4];
146	      TH1F* histoFF[4];
147	
148	      TH1F* histoN[4];
149	
150	
151	      vector<struct MMEstimated> theMMEstimatedValues;
152	      struct MMEstimatedPlots theMMEstimatedPlots;
153	      vector<struct MMExpectedPlots> theMMExpectedPlots;
154	      //      struct MMStacks theMMStacks;
155	
156	};
157	
158	#endif
