1	#ifndef MMEstimation_h
2	#define MMEstimation_h
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
34	  float iso1[3]; 
35	  float iso2[3];
36	};
37	
38	struct NSelected{
39	  float NSel[3]; 
40	};
41	
42	struct MMEstimated{
43	 float NofMMEstimatedQCD[3];
44	 float NofMMEstimatedWJets[3];
45	 float NofMMEstimatedSignal[3];
46	 float MMEstimatedQCDErr[3];
47	 float MMEstimatedWJetsErr[3];
48	 float MMEstimatedSignalErr[3];
49	};
50	
51	struct MMEstimatedPlots{
52	      TH1F * MMEstimated_Signal[3];
53	      TH1F * MMEstimated_QCD[3];
54	      TH1F * MMEstimated_WJets[3];
55	};
56	
57	struct MMExpectedPlots{
58	      string Name[3];
59	      TH1F* MMExpected[3];
60	};
61	
62	/*
63	struct MMStacks{
64	  THStack* stackSignal[3];
65	  THStack* stackWJets[3];
66	  THStack* stackQCD[3];
67	};
68	*/
69	
70	struct MMEpsilons{
71	  float EpsilonSignal;
72	  float EpsilonSignalErr;
73	  float EpsilonFake;
74	  float EpsilonFakeErr;
75	};
76	
77	
78	struct Distribution{
79	   TH1F * NMMEstimatedQCDDistribution[3];
80	   TH1F * NMMEstimatedWJetsDistribution[3];
81	   TH1F * NMMEstimatedSignalDistribution[3];
82	};
83	
84	class MMEstimation {
85	
86	
87	   public:
88	      MMEstimation(vector<Dataset>, float, float, unsigned int, float, float, string);
89	      ~MMEstimation();
90	
91	      void CountNSel(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int);
92	      void FillMMExpected(const DiLeptonSelection&, Dataset, SelectionTable, unsigned int, float, string, int, const NTEvent*);
93	      void RunTheMatrixMethod(vector<struct MMEpsilons>, unsigned int, bool, bool, bool);
94	      void RunTheMatrixMethodForEpsilonsTest(vector<struct MMEpsilons>, unsigned int, bool, bool, float);
95	      void ReadMMFile(string);
96	      void ReadMMFileForPullTest(string, float, float);
97	      vector<struct MMEstimated> GetMMEstimated();
98	      void PrintMMEstimated();
99	      struct MMEstimatedPlots GetMMEstimatedPlots();
100	      vector<struct MMExpectedPlots> GetMMExpectedPlots();
101	      void WriteMMFile(string);
102	      void WriteMMFileFast(string);
103	      void WriteMMFileFastForPullTest(string, string, float, float);
104	
105	    private:
106	
107	      struct Isolations GetIsolations();
108	      void IncrementNSelBin(unsigned int, unsigned int, float);
109	      void FillMMExpectedPlot(string, unsigned int, float, float);
110	      void IncludeStatisticalError();
111	      void IncludeStatisticalErrorForEpsilonsTest(float);
112	      void IncludeSystematicError();
113	      void SolveTheSystem(bool);
114	      void FillDistributions(unsigned int);
115	      void CalculateRms(unsigned int, unsigned int);
116	      void SetMMEstimated(unsigned int, unsigned int);
117	
118	      vector<struct Distribution> theDistributions;
119	      unsigned int theNBins; 
120	      float theMinBin;
121	      float theMaxBin;
122	
123	      string IsoNames[3];
124	      string theChannel;
125	      struct Isolations theIsolations;
126	
127	      TRandom ranEpsFake, ranEpsSignal, ranN1, ranN2, ranN3;
128	      float locNSelected[3];
129	
130	      float locNSelectedSignal[3];
131	      float locNSelectedW[3];
132	      float locNSelectedQCD[3];
133	
134	
135	      float EpsilonFake;
136	      float EpsilonFakeErr;
137	      float EpsilonSignal;
138	      float EpsilonSignalErr;
139	      vector<struct NSelected> theNSelected;
140	      float NMMEstimatedSignal[3];
141	      float NMMEstimatedWJets[3];
142	      float NMMEstimatedQCD[3];
143	
144	
145	      TH1F* histoSignal[3];
146	      TH1F* histoW[3];
147	      TH1F* histoQCD[3];
148	
149	      TH1F* histoN[3];
150	
151	
152	      vector<struct MMEstimated> theMMEstimatedValues;
153	      struct MMEstimatedPlots theMMEstimatedPlots;
154	      vector<struct MMExpectedPlots> theMMExpectedPlots;
155	      //      struct MMStacks theMMStacks;
156	
157	};
158	
159	#endif
