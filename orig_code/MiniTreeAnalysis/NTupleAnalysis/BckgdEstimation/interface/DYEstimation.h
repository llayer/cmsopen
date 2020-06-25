1	#ifndef DYEstimation_h
2	#define DYEstimation_h
3	
4	//#include ".h"
5	
6	// system include files
7	#include <memory>
8	#include <vector>
9	#include <iostream>
10	#include <string>
11	#include "TH1F.h"
12	#include "THStack.h"
13	#include "TF1.h"
14	#include "TFile.h"
15	#include "../../Tools/interface/FitFunctions.h"
16	
17	
18	using namespace std;
19	//using namespace TopTree;
20	
21	/**
22	        Steps of the selection: (integer returned by doFullSelection() or FillTable(...))
23	        - Step 1        Dilepton pair choice
24	        - Step 2        Z mass veto 
25	        - Step 3        Minimal jet multiplicity 
26	        - Step 4        MET cuts
27	        - Step 5        btagging cuts
28	
29	*/
30	
31	class DYEstimation {
32	
33	
34	   public:
35	      DYEstimation();
36	      ~DYEstimation();
37	
38	      void WriteHisto();
39	      void FillNbofEventsInSRandCR(bool, bool, float, string,float);
40	      std::vector<TH1F*> CreateDYHisto(string, string, int, float, float);
41	      void FillDYHistos(string, std::vector<TH1F*>, string, int, float, float);
42	      void MakeDYHistos(string, std::vector<TH1F*>, TH1F*);
43	      void MakeDataHistos(string, std::vector<TH1F*>, TH1F*);
44	      void ComputeDYEstimatedEvents_MC();
45	      float GetDYEvents_MC(); 
46	      float GetDYEstimatedEvents_MC(); 
47	      void MakeDYFits();
48	      vector<double> MakeDYFits_emu(TH1F*);
49	      void MakeDYFits_SR(TH1F*,int,float,float,vector<double>,double,double,string,float);
50	      void MakeDYFits_SR_asym(TH1F*,int,float,float,vector<double>,double,double,string,float);
51	      void MakeDYFits_CR(TH1F*,int,float,float,double,double,string);
52	      void MakeDYFits_CR_asym_plusCE(TH1F*,int,float,float,double,double,string);
53	      void MakeDYFits_CR_WithBkgSubtraction(TH1F*,int,float,float,vector<double>,double,double,string,float);
54	      void MakeDYFits_CR_WithBkgSubtraction_asym(TH1F*,int,float,float,vector<double>,double,double,string,float);
55	      void SetNFittedDYMCinMz_CR(double);
56	      float GetNDYMCinMz_CR();
57	      float GetNFittedDYMCinMz_CR();
58	      void SetNFittedDYMCinMz_SR(double);
59	      void SetNFittedDATAinMz_SR(double);
60	      void SetNFittedDATAinMz_CR(double);
61	      void SetNFittedDYMC_CR(double);
62	      void SetDYVerbose(bool);
63	      float GetNDYMCinMz_SR();
64	      float GetNFittedDYMCinMz_SR();
65	      float GetNDYMC_CR();
66	      float GetNFittedDYMC_CR();
67	      float GetNDYMC_SR();
68	      float GetNDATAinMz_SR();
69	      float GetNDATAinMz_CR();
70	      float GetNDATA_CR();
71	      float GetNDATA_BkgSubtracted_CR();
72	      float GetNDATA_SR();
73	      float GetNDYEstimatedMC();
74	      void ComputeDYEstimatedEvents_DATA(string,string);
75	      float GetNDYEstimatedDATA();
76	      float GetNDYEstimatedStatErrorDATA();
77	      float GetNDYEstimatedSysErrorDATA();
78	      float GetNDYEstimatedDATACounting();
79	      float GetNDYEstimatedStatErrorDATACounting();
80	      float GetNDYEstimatedSysErrorDATACounting();
81	      float GetNFittedDATA_CR();
82	      float GetNFittedDATAinMz_SR();
83	      float GetNFittedDATAinMz_CR();
84	      float GetContinuumInZdata();
85	      
86	      
87	      
88	    private:
89	      float f1_;
90	      float NDYMCinMz_SR;        // # of Zjets events with Mz_inf<Mll<Mz_sup in SR
91	      float NDYMCinMz_CR;        // # of Zjets events with Mz_inf<Mll<Mz_sup in CR
92	      float NDYMC_CR;            // # of Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
93	      float NDYMC_SR;            // # of Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in SR
94	      float NFittedDYMCinMz_SR;        // # of fitted Zjets events with Mz_inf<Mll<Mz_sup in SR
95	      float NFittedDYMCinMz_CR;        // # of fitted Zjets events with Mz_inf<Mll<Mz_sup in CR
96	      float NFittedDYMC_CR;            // # of fitted Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
97	      float NunnormDYMCinMz_SR;        // Unnormalized # of Zjets events with Mz_inf<Mll<Mz_sup in SR
98	      float NunnormDYMCinMz_CR;        // Unnormalized # of Zjets events with Mz_inf<Mll<Mz_sup in CR
99	      float NunnormDYMC_CR;            // Unnormalized # of Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
100	      float NMCinMz_SR;        // # of events with Mz_inf<Mll<Mz_sup in SR
101	      float NMCinMz_CR;        // # of events with Mz_inf<Mll<Mz_sup in CR
102	      float NMC_CR;            // # of events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV) in CR
103	      float NDYEstimatedMC;      // # of estimated MC Zjets events with Mll<Mz_inf(76GeV) || Mll>Mz_sup(106GeV)  
104	      float NDYMCinMzWithScaledPlusContinuum_SR;        // # of Zjets + fraction of continuum  events with Mz_inf<Mll<Mz_sup in SR 
105	      float NDYMCinMzWithScaledMinusContinuum_SR;        // # of Zjets - fraction of continuum  events with Mz_inf<Mll<Mz_sup in SR 
106	
107	      float NDATAinMz_SR;        // # of DATA events with Mz_inf<Mll<Mz_sup in SR
108	      float NDATAinMz_CR;        // # of DATA events with Mz_inf<Mll<Mz_sup in CR
109	      float NDATA_CR;            // # of DATA events with Mll<Mz_inf || Mll>Mz_sup in CR
110	      float NDATA_SR;            // # of DATA events with Mll<Mz_inf || Mll>Mz_sup in SR
111	      float NFittedDATA_CR;      // # of fitted DATA events with Mll<Mz_inf || Mll>Mz_sup in CR
112	      float NFittedDATAinMz_SR;  // # of fitted DATA events with Mz_inf<Mll<Mz_sup in SR
113	      float NFittedDATAinMz_CR;  // # of fitted DATA events with Mz_inf<Mll<Mz_sup in CR
114	      float NDYEstimatedDATA;    // # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR
115	      float NDYEstimatedDATACounting;    // # of estimated DY events with Mz_inf<Mll<Mz_sup in SR with counting method
116	      float NDYEstimatedStatErrorDATA;    // Stat. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR
117	      float NDYEstimatedStatErrorDATACounting;    // Stat. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR with counting method
118	      float NDYEstimatedSysErrorDATA;     // Sys. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in SR
119	      float NDYEstimatedSysErrorDATACounting;     // Sys. error of the # of estimated DY events with Mll<Mz_inf || Mll>Mz_sup in      SR with counting method
120	      float NFittedDATAinMzWithScaledPlusContinuum_SR;  // # of fitted DATA + fraction of continuum  events with Mll<Mz_inf || Mll>Mz_sup in SR
121	      float NFittedDATAinMzWithScaledMinusContinuum_SR;  // # of fitted DATA - fraction of continuum  events with Mll<Mz_inf || Mll>Mz_sup in SR
122	      
123	      float ContinuumInZdata;
124	      
125	      bool DYVerbose;
126	      
127	      
128	      std::vector<TH1F*> hlistSR; 
129	      std::vector<TH1F*> hlistCR; 
130	      std::vector<TH1F*> hlistemu; 
131	      std::vector<TH1F*> hlistContCR; 
132	           
133	      // emu (for bkg)
134	      THStack *hstack1 ;
135	      
136	      // ll (for SR)
137	      THStack *hstackSR ;
138	      TH1F *allSR;
139	
140	      // ll (for CR)
141	      THStack *hstackCR ;
142	      TH1F *allCR;
143	
144	      // Continuum in CR (for bkg)
145	      THStack *hstackContCR ;
146	      TH1F *allContCR;
147	
148	};
149	
150	
151	#endif
