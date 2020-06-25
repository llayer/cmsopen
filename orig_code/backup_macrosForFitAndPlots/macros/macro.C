1	{
2	#ifdef __CINT__
3	#include "RooGlobalFunc.h"
4	#endif
5	#include "TFile.h"
6	#include "TH1.h"
7	#include "TF1.h"
8	#include "TCanvas.h"
9	#include "TPad.h"
10	#include "TStyle.h"
11	#include <TROOT.h>
12	#include <TMath.h>
13	#include <TText.h>
14	#include <TPaveLabel.h>
15	#include "TROOT.h"
16	#include "TSystem.h"
17	#include "TFrame.h"
18	#include "iostream"
19	#include "TTree.h"
20	#include "TLegend.h"
21	#include "TPaveText.h"
22	#include "TGraphErrors.h"
23	#include <vector>
24	#include <string>
25	#include <fstream>
26	
27	using namespace RooFit;
28	
29	  gROOT->SetStyle("Plain");
30	  gStyle->SetOptTitle(0);
31	  gStyle->SetOptStat(0);
32	  //gStyle->SetOptFit(0111);
33	  gStyle->SetPadTickX(1);
34	  gStyle->SetPadTickY(1);
35	  gStyle->SetTitleXOffset(1.2);
36	  gStyle->SetTitleYOffset(0.01);
37	  gStyle->SetLabelOffset(0.005, "XYZ");
38	  //  gStyle->SetLabelOffset(0.1, "XYZ");
39	  gStyle->SetTitleSize(0.07, "XYZ");
40	  gStyle->SetTitleFont(22,"X");
41	  gStyle->SetTitleFont(22,"Y");
42	  gStyle->SetPadBottomMargin(0.13);
43	  gStyle->SetPadLeftMargin(0.15);
44	  gStyle->SetPadRightMargin(0.15);
45	  gStyle->SetHistLineWidth(2);
46	
47	
48	  //const int nbins = 20;
49	  //Double_t bins[nbins] = {2.0, 4.0, 6.0, 8.0, 10., 12., 14., 16., 18., 20., 25., 30., 40., 50., 60., 70., 80, 90, 100, 150.}; 
50	  double thres = 8 ; // GeV
51	   
52	  int pause ;
53	  gROOT->ForceStyle();
54	  gSystem->Load("libRooFit.so");
55	  
56	  double fit_cuts_min = 20-1.5;//thres-1.5 ;
57	  double fit_cuts_max = 200 ;
58	  
59	  TString name_image="efficiencyTnP" ;  
60	 
61	  int colors[5] = {kRed,kBlack,kBlue,kRed,kBlack};
62	  int styles[5] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare, kOpenStar};
63	   
64	  TFile *_file0 ;
65	  //_file0 = TFile::Open("TriggerEffHisto_Data_OK3.root");
66	  //_file0 = TFile::Open("TriggerEffHisto_Singlemu_v4_pfjets.root");
67	  //_file0 = TFile::Open("TriggerEffHisto_Data_singleMu_735pb-1_jetleg_pf.root");
68	  _file0 = TFile::Open("TriggerEffHisto_JetLeg45_15binSingleMuon_Full.root");
69	  _file0->cd();
70	  
71	  
72	  //TTree* mytree = (TTree*) gDirectory->Get("jet4_eff") ;
73	  TH1F*  jet4_eff = (TH1F*)_file0->Get("jet4_eff");
74	  
75	#ifdef __CINT__
76	  gROOT->ProcessLineSync(".x FuncCB.cxx+") ;
77	#endif
78	  
79	  
80	  // Variables from tree
81	  RooRealVar xaxis("pt","P_{T} [GeV]",20,200) ;
82	  //RooRealVar xaxis("et","E_{T} [GeV]",0,150) ;
83	  RooCategory cut("match","trigger efficieny") ; // discrete variables
84	  cut.defineType("accept",1) ;
85	  cut.defineType("reject",0) ;
86	  RooCategory cutL1("L1match","L1 trigger efficieny") ; // discrete variables
87	  cutL1.defineType("accept",1) ;
88	  cutL1.defineType("reject",0) ;
89	  cutL1.defineType("notset",-1) ;
90	  RooRealVar mass("mass","mass",20,200) ;// consider only this mass range when importing data
91	  //RooRealVar mass("mass","mass",0,280) ;// consider only this mass range when importing data
92	  RooRealVar eta("eta","eta",-3., 3) ;
93	  RooDataHist * dataSet ;
94	  dataSet = new RooDataHist("data","data",xaxis, jet4_eff);
95	  
96	  // PARAMETRES ROOFIT CRYSTAL BALL
97	 /*
98	  RooRealVar norm("norm","N",0.95,0.92,0.98);
99	  RooRealVar alpha("alpha","#alpha",0.25,0.00001,8);
100	  RooRealVar n("n","n",2,1.1,35);
101	  RooRealVar mean("mean","mean",40.5,10,70);
102	  RooRealVar sigma("sigma","#sigma",0.23,0.01,10);
103	  //mean.setVal(42);*/
104	  
105	  
106	  RooRealVar norm("norm","N",0.95,0.9,0.98);
107	  RooRealVar alpha("alpha","#alpha",0.25,0.00001,8);
108	  RooRealVar n("n","n",6.5,1.1,35);
109	  RooRealVar mean("mean","mean",35,20,50);
110	  RooRealVar sigma("sigma","#sigma",0.45,0.01,10);
111	  
112	  //FONCTION CRYSTALL BALL
113	  FuncCB cb("cb","Crystal Ball Integree",xaxis,mean,sigma,alpha,n,norm) ;
114	     
115	  //////////////////////////////////////:: Construct efficiency p.d.f eff(cut|x)/////////////////////////
116	  RooEfficiency eff("eff","efficiency",cb, cut,"accept");
117	     
118	  //RooBinning binning;
119	  //binning = RooBinning(nbins - 1, bins, "binning");
120	    
121	  //RooPlot* frame = xaxis.frame(Bins(18000),Title("Fitted efficiency")) ;
122	  RooPlot* frame =  xaxis.frame();
123	  int color=colors[0];
124	  int style=styles[0];
125	  
126	  //dataSet->plotOn(frame,Binning(binning),Efficiency(cut),MarkerColor(1),LineColor(1), MarkerStyle(style) );
127	  dataSet->plotOn(frame);
128	  
129	  ///////////////////////////////////////////// FITTING /////////////////////////////
130	  xaxis.setRange("interesting",fit_cuts_min,fit_cuts_max);
131	  cout<<endl<<endl<<endl<<endl<<endl ;
132	  cout<<"======================================================================="<<endl ;
133	  //RooFitResult * fit = eff.fitTo(*dataSet); //asymmetric errors
134	  RooFitResult * fit = eff.fitTo(*dataSet,ConditionalObservables(xaxis),Range("interesting"),Minos(kFALSE), Save(kTRUE)); //symmetric errors
135	  
136	  //draw
137	  cb.plotOn(frame,LineColor(1),LineWidth(2));
138	  
139	 
140	  ////////////////////////////  DRAWING PLOTS AND LEGENDS /////////////////////////////////
141	  // _____________________________________________________________________________________
142	
143	  TCanvas* ca = new TCanvas("ca","Trigger Efficiency") ;
144	
145	  ca->SetGridx();
146	  ca->SetGridy();
147	  ca->cd();
148	  
149	  //gPad->SetLogx();
150	  //gPad->SetObjectStat(1);
151	
152	  //frame->GetYaxis()->SetRangeUser(0,1.05);
153	  //frame->GetXaxis()->SetRangeUser(0.5,150.);
154	  //frame->GetYaxis()->SetTitle("Efficiency");
155	  frame->Draw() ;
156	   
157	 /* TPaveText *paveText = new TPaveText(0.162752,0.573357,0.380872,0.673007,"brNDC");
158	  paveText->SetLineColor(1);
159	  paveText->SetTextColor(1);
160	  paveText->SetTextFont(42);
161	  paveText->SetTextSize(0.0227273);
162	  paveText->SetFillColor(kWhite);
163	  paveText->SetShadowColor(kWhite);
164	  //paveText->AddText("HLT_El15 Trigger");
165	  //paveText->AddText("Electrons from Z");
166	  paveText->Draw();*/
167	
168	
169	  ca->Print(name_image+".pdf");
170	  ca->Print(name_image+".root");
171	
172	  return fit ;
173	
174	}
175	
