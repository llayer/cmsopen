1	{
2	
3	// Setup a style
4	  using namespace RooFit;
5	
6	  gROOT->SetStyle("Plain");
7	  gStyle->SetOptTitle(0);
8	  gStyle->SetOptStat(1111);
9	  gStyle->SetPadTickX(1);
10	  gStyle->SetPadTickY(1);
11	  gStyle->SetTitleSize(0.07, "XYZ");
12	  gStyle->SetTitleFont(22,"X");
13	  gStyle->SetTitleFont(22,"Y");
14	  gStyle->SetPadBottomMargin(0.13);
15	  gStyle->SetPadLeftMargin(0.15);
16	  gStyle->SetPadRightMargin(0.15);
17	  gStyle->SetHistLineWidth(2);
18	
19	// Grab PDF's from histos
20	  
21	   THStack hs("hs","test stacked histograms");
22	  
23	   TFile *_file_WZT ;
24	  _file_WZT = TFile::Open("~/SemiLeptonicTauXsectionMeas_WZT_3-9fb_438p7_met20_newTauLeg_JP_plotfix2_M3_multibinning_taulegnewbinning.root");
25	  _file_WZT->cd();
26	  
27	  TH1F*  hist0 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
28	  hist0->Sumw2();
29	  TH1F*  hist1 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/NNOutput");
30	  hist1->Sumw2();
31	  TH1F*  hist2 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/NNOutput");
32	  hist2->Sumw2();
33	  TH1F*  hist3 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/NNOutput");
34	  hist3->Sumw2();
35	  TH1F*  hist4 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/NNOutput");
36	  hist4->Sumw2();
37	  TH1F*  hist6 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/NNOutput");
38	  hist6->Sumw2();
39	  TH1F*  hist7 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/NNOutput");
40	  hist7->Sumw2();
41	  TH1F*  hist8 = (TH1F*)_file_WZT->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/NNOutput");
42	  hist8->Sumw2();
43	  
44	   
45	  hist1->Add(hist2);  
46	  hist1->Add(hist3);
47	  hist1->Add(hist4);
48	  hist1->Add(hist6);
49	  hist1->Add(hist7);
50	  hist1->Add(hist8);
51	  
52	  TFile *_file_singleTop ;
53	  _file_singleTop = TFile::Open("~/SemiLeptonicTauXsectionMeas_singletop_3-9fb_438p7_met20_newTauLeg_JP_plotfix2_M3_multibinning_taulegnewbinning.root");
54	  _file_singleTop->cd();
55	  TH1F*  hist9 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/NNOutput");
56	  hist9->Sumw2();
57	  TH1F*  hist10 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/NNOutput");
58	  hist10->Sumw2();
59	  TH1F*  hist11 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/NNOutput");
60	  hist11->Sumw2();
61	  TH1F*  hist12 = (TH1F*)_file_singleTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBart/NNOutput");
62	  hist12->Sumw2();
63	  
64	  hist1->Add(hist9);
65	  hist1->Add(hist10);
66	  hist1->Add(hist11);
67	  hist1->Add(hist12);
68	 
69	  hist1->Sumw2();
70	  hist0->SetFillColor(2);hist0->SetLineColor(2);
71	  hist1->SetFillColor(3);hist1->SetLineColor(3);
72	 
73	 
74	  TFile *_fileMULTIJET ;
75	  _fileMULTIJET = TFile::Open("~/SemiLeptonicTauXsectionMeas_multijet_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
76	  _fileMULTIJET->cd();
77	  TH1F*  hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
78	  hist5->SetFillColor(7);hist5->SetLineColor(7);
79	 
80	  RooRealVar x("x","x",-1.5,1.5); 
81	  RooDataHist * rooHist0 = new RooDataHist("TTbarTauHQQ","TTbarTauHQQ",x, hist0);
82	  RooDataHist * rooHist1 = new RooDataHist("BKG","BKG",x, hist1);
83	  RooDataHist * rooHist5 = new RooDataHist("MultiJet","MultiJet",x, hist5);
84	 
85	  RooHistPdf histpdf0("histpdf0","histpdf0",x,*rooHist0) ;
86	  RooHistPdf histpdf5("histpdf5","histpdf5",x,*rooHist5) ;
87	
88	  // Construct composite pdf
89	  
90	  RooRealVar c0("c0","c0",0.5,0.,1.) ;
91	  //RooAddPdf pdf("pdf","pdf",RooArgList(histpdf0,histpdf1,histpdf5),RooArgList(c0,c1,c5),false) ;
92	  RooAddPdf pdf("pdf","pdf",RooArgSet(histpdf0,histpdf5),c0) ;
93	 
94	  // Get Data histo
95	  
96	  TFile *_fileData ;
97	  _fileData = TFile::Open("~/SemiLeptonicTauXsectionMeas_data_3-7fb_438p7_met20_newTauLeg_JP_plotfix2.root");
98	  _fileData->cd();
99	  TH1F*  histData = (TH1F*)_fileData->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
100	  histData->Sumw2(); 
101	  histData->SetMarkerStyle(20);
102	  RooDataHist* data = new RooDataHist("data","data",x, histData);
103	 
104	  TH1F*  histDataFit = new TH1F("histDataFit","histDataFit",histData->GetNbinsX(),histData->GetXaxis()->GetXmin(),histData->GetXaxis()->GetXmax());
105	  histDataFit->Sumw2(); 
106	  histDataFit->Add(histData,hist1,1,-1);
107	  RooDataHist* dataFit = new RooDataHist("data","data",x, histDataFit);
108	 
109	  
110	  // Fit the stuff
111	  
112	  RooFitResult* fitResult = pdf.fitTo(*dataFit, Save()) ;
113	  std::cout << c0.getVal() << std::endl;
114	  std::cout << c0.Print()  << std::endl;
115	  
116	
117	  
118	  // Plot pdf's
119	  
120	  RooPlot* mframe = x.frame(-1.5,1.5) ; 
121	  dataFit->plotOn(mframe,Name("data")) ;  
122	  pdf->plotOn(mframe,LineColor(kBlack),LineStyle(kDashed));
123	  pdf->plotOn(mframe,Components("histpdf0"),VisualizeError(*fitResult),FillColor(kMagenta)) ; 
124	  //pdf->plotOn(mframe,Components("histpdf1"),VisualizeError(*fitResult),FillColor(kGreen)) ; 
125	  pdf->plotOn(mframe,Components("histpdf5"),VisualizeError(*fitResult),FillColor(kBlue)) ; 
126	  //dataFit->plotOn(mframe) ;  
127	  pdf->plotOn(mframe,Name("pdf")) ;
128	  mframe->Draw() ; 
129	  
130	  
131	  // Calculate the various contributions
132	  
133	  cout << " the TTbar tau(h) QQ " << c0.getVal()*histDataFit->Integral() << " ± " << c0.getError()*histDataFit->Integral() << endl;
134	  cout << " the MultiJet        " << (1.-c0.getVal())*histDataFit->Integral() << " ± " << c0.getError()*histDataFit->Integral() << endl;
135	  cout << " the MC stuff "        << hist1->Integral()    << endl; 
136	  cout << " the Data "            << histData->Integral()    << endl; 
137	  cout << " the MC TTbar tau(h) QQ " << hist0->Integral() << endl; 
138	 
139	  cout << "scale factor TTbar tau(h) QQ " << c0.getVal()*histDataFit->Integral()/hist0->Integral() << " ± " << c0.getError()*histDataFit->Integral()/hist0->Integral()<< endl; 
140	  cout << "scale factor MultiJet "        << (1-c0.getVal())*histDataFit->Integral()/hist5->Integral()    << " ± " << c0.getError()*histDataFit->Integral()/hist5->Integral()<< endl; 
141	  cout <<" " <<endl;
142	  cout << "chisquare " <<mframe->chiSquare("pdf","data",1)<<endl;
143	  cout << "pull " <<(c0.getVal()*histDataFit->Integral()-hist0->Integral())/hist0->Integral() << std::endl;
144	 
145	  // Plot the stuff
146	  
147	  hist5->Scale((1-c0.getVal())*histDataFit->Integral()/hist5->Integral());
148	  hist0->Scale(c0.getVal()*histDataFit->Integral()/hist0->Integral());
149	 
150	  histData->Draw("e");
151	  hs.Add(hist5);
152	  hs.Add(hist1);
153	  hs.Add(hist0);
154	  hs.Draw("histosame");
155	  histData->Draw("esame");
156	  
157	  fitResult->Print("") ;
158	  
159	 std::cout <<"W " << hist3->Integral()<< " " << sqrt(hist3->Integral())<< std::endl;
160	 std::cout <<"Z " << hist4->Integral()<< " " << sqrt(hist4->Integral())<< std::endl;
161	 
162	 std::cout <<"tt " << hist1->Integral()-hist3->Integral()-hist4->Integral()-hist9->Integral()-hist10->Integral()-hist11->Integral()-hist12->Integral()<< " " << std::endl;
163	 std::cout <<"s t " << hist9->Integral()+hist10->Integral()+hist11->Integral()+hist12->Integral()<< " " << sqrt(pow(hist9->Integral(),2)+pow(hist10->Integral(),2)+pow(hist11->Integral(),2)+pow(hist12->Integral(),2))<< std::endl;
164	 
165	}
166	/*
167	  // Construct binned likelihood
168	  RooNLLVar nll("nll","nll",pdf,*dataFit) ;
169	  // Start Minuit session on NLL
170	  RooMinuit m(nll) ;
171	  m.migrad() ;
172	  m.hesse() ;
173	  RooFitResult* r1 = m.save() ; 
174	  
175	  // Construct Chi2
176	  RooChi2Var chi2("chi2","chi2",pdf,*dataFit) ;
177	  // Start Minuit session on Chi2
178	  RooMinuit m2(chi2) ;
179	  m2.migrad() ;
180	  m2.hesse() ;
181	  RooFitResult* r2 = m2.save() ;
182	
183	  // Print results 
184	  cout << "result of binned likelihood fit" << endl ;
185	  r1->Print("v") ;
186	  cout << "result of chi2 fit" << endl ;
187	  r2->Print("v") ;*/
