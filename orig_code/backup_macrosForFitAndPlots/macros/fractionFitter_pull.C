1	#include <iostream>
2	#include <TROOT.h>
3	#include <TPaveText.h>
4	
5	void fractionFitter_pull()
6	{ 
7	  ofstream ofile;
8	  ofile.open ("pull_results_Haryo_fractionFitter.txt",ios::app); 
9	 
10	// *****************************************************************************
11	  
12	  TFile *f0 = new TFile("~/SemiLeptonicTauXsectionMeas_data_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
13	  TFile *f1 = new TFile("~/SemiLeptonicTauXsectionMeas_multijet_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
14	  TFile *f2 = new TFile("~/SemiLeptonicTauXsectionMeas_WZT_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
15	  TFile *f3 = new TFile("~/SemiLeptonicTauXsectionMeas_singletop_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
16	  TFile *fttbarPE = new TFile("~/scratch0/SemiLeptonicTauXsectionMeas_PE_ttbar_168.root");//TTbartobeModified
17	  TFile *fmultijetPE = new TFile("~/SemiLeptonicTauXsectionMeas_PE_multijet_49.root");//MultijettobeModified
18	 
19	// *****************************************************************************
20	
21	 Int_t stati=0;
22	 Bool_t  fit=0;
23	 Bool_t logy=1;
24	
25	// *****************************************************************************
26	
27	 TCanvas *c1 = new TCanvas("c1", "plots",200,10,700,730);
28	 c1->SetFillColor(10);
29	 c1->SetFillStyle(4000);
30	 c1->SetBorderSize(2);
31	
32	// *****************************************************************************
33	
34	pad1 = new TPad("pad1","This is pad1",0.01,0.01,0.99,0.99,21);
35	
36	pad1->SetFillColor(0);
37	pad1->SetBorderMode(0);
38	pad1->SetFrameFillColor(10);
39	pad1->Draw();
40	//if (logy) pad1->SetLogy();
41	
42	//$$ gStyle->SetOptDate(1);
43	gStyle->SetOptDate(0);
44	gStyle->SetStatColor(0);
45	gStyle->SetTitleColor(29);
46	gStyle->SetTitleW(0.5);
47	gStyle->SetTitleH(0.05);
48	gStyle->SetOptStat(stati);
49	
50	// *****************************************************************************
51	
52	       //Multijet
53	       f1->cd();
54	       TH1F*  g1 = (TH1F*)f1->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
55	       g1->Sumw2();
56	       
57	       fmultijetPE->cd();
58	       TH1F*  h0 = (TH1F*)fmultijetPE->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
59	       h0->Sumw2();
60	       
61	       //TTbar signal
62	       f2->cd();
63	       TH1F*  g2 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
64	       g2->Sumw2();
65	     
66	       fttbarPE->cd();
67	       TH1F*  h1 = (TH1F*)fttbarPE->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
68	       h1->Sumw2();
69	       
70	       //Pseudo-data
71	       h0->Add(h1,1);
72	       
73	      /* //
74	       f2->cd();
75	       TH1F*  g3 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/NNOutput");
76	       g3->Sumw2();
77	     
78	       TH1F*  hist2 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/NNOutput");
79	       hist2->Sumw2();
80	       TH1F*  hist3 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/NNOutput");
81	       hist3->Sumw2();
82	       TH1F*  hist4 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/NNOutput");
83	       hist4->Sumw2();
84	       TH1F*  hist6 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/NNOutput");
85	       hist6->Sumw2();
86	       TH1F*  hist7 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/NNOutput");
87	       hist7->Sumw2();
88	       TH1F*  hist8 = (TH1F*)f2->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/NNOutput");
89	       hist8->Sumw2();
90	    
91	       f3->cd();
92	       TH1F*  hist9 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/NNOutput");
93	       hist9->Sumw2();
94	       TH1F*  hist10 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/NNOutput");
95	       hist10->Sumw2();
96	       TH1F*  hist11 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/NNOutput");
97	       hist11->Sumw2();
98	       TH1F*  hist12 = (TH1F*)f3->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBart/NNOutput");
99	       hist12->Sumw2();
100	       
101	       g3->Add(hist2);
102	       g3->Add(hist3);
103	       g3->Add(hist4);
104	       g3->Add(hist6);
105	       g3->Add(hist7);
106	       g3->Add(hist8);
107	       g3->Add(hist9);
108	       g3->Add(hist10);
109	       g3->Add(hist11);
110	       g3->Add(hist12);*/
111	   
112	       TObjArray *mc = new TObjArray(2); // MC histograms are put in this array
113	       mc->Add(g1);
114	       mc->Add(g2);
115	       h0->Draw();
116	       
117	       TFractionFitter* fitter = new TFractionFitter(h0, mc); // initialise
118	       fitter->Constrain(1,0.0,1.0);     // constrain fraction 1 to be between 0 and 1
119	       fitter->Constrain(2,0.0,1.0);
120	       //fitter->Constrain(3,0.0,1.0);
121	       fitter->SetRangeX(-1.5,1.5);          // use only the first 15 bins in the fit
122	
123	       Int_t status = fitter->Fit();     // perform the fit
124	       cout << "###############" << endl;
125	       cout << " fit status: " << status << endl;
126	       cout << "###############" << endl;
127	       
128	       
129	       Double_t _c1,_c2,_c3;
130	       Double_t c1_err,c2_err,c3_err;
131	       
132	       //TH1F* result = (TH1F*) fitter->GetPlot();
133	       fitter->GetResult(0, _c1, c1_err);
134	       fitter->GetResult(1, _c2, c2_err);
135	       Chi2 = fitter->GetChisquare();
136	       Ndf = fitter->GetNDF();
137	 
138	 
139	       std::cout << "Chi2 " << Chi2 << std::endl;
140	       std::cout << "Ndf " << Ndf << std::endl;
141	       std::cout << "Chi2/Ndf " << Chi2/Ndf << std::endl;
142	       
143	       std::cout << "TTbar signal fraction " << _c2 <<" "<< c2_err << std::endl;
144	       std::cout << "Multijet signal fraction " << _c1 <<" "<< c1_err << std::endl;
145	       
146	       std::cout << "TTbar signal " << _c2*h0->GetSumOfWeights() <<" "<< c2_err*h0->GetSumOfWeights() << std::endl;
147	       std::cout << "Multijet signal " << _c1*h0->GetSumOfWeights() <<" "<< c1_err*h0->GetSumOfWeights() << std::endl;
148	       
149	       //float SF2 = _c2*h0->GetSumOfWeights()/g2->GetSumOfWeights();
150	       //float SF1 = _c1*h0->GetSumOfWeights()/g1->GetSumOfWeights();
151	      
152	       //std::cout << h1->GetSumOfWeights() << std::endl;
153	       std::cout <<"pull " << (_c2*h0->GetSumOfWeights() - h1->GetSumOfWeights())/(c2_err*h0->GetSumOfWeights()) <<std::endl;
154	       ofile<<"h1->Fill("<< (_c2*h0->GetSumOfWeights() - h1->GetSumOfWeights())/(c2_err*h0->GetSumOfWeights()) <<");" <<endl;
155	  
156	     
157	}
158	
