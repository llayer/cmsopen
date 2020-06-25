1	#include "../interface/TauIdEff.h"
2	
3	                
4	TauIdEff::TauIdEff(){}
5	
6	
7	TauIdEff::~TauIdEff(){}
8	
9	
10	void TauIdEff::CreateHistos(){
11	 
12	  tauId_reference       = new TH1F ("tauId_reference      ","",50,0,100); 
13	  tauId_leadTrack       = new TH1F ("tauId_leadTrack      ","",50,0,100); 
14	  tauId_discr           = new TH1F ("tauId_discr          ","",50,0,100);
15	  tauId_againstElectron = new TH1F ("tauId_againstElectron","",50,0,100);
16	  tauId_againstMuon     = new TH1F ("tauId_againstMuon    ","",50,0,100);
17	  tauId_all             = new TH1F ("tauId_all            ","",50,0,100);
18	  tauId_eff             = new TH1F ("tauId_eff            ","",50,0,100);
19	  
20	  tauId_gen             = new TH1F ("tauId_gen            ","",50,0,100);
21	  tauId_gen_matched     = new TH1F ("tauId_gen_matched    ","",50,0,100);
22	  tauId_gen_eta         = new TH1F ("tauId_gen_eta","",25,-2.5,2.5);
23	  tauId_gen_matched_eta = new TH1F ("tauId_gen_matched_eta","",25,-2.5,2.5);
24	}
25	
26	
27	void TauIdEff::SaveHistos(TString inputRootFile){
28	
29	  TFile* f = new TFile(inputRootFile,"RECREATE");
30	  f->cd();
31	  
32	  
33	  tauId_eff->Divide(tauId_all,tauId_reference);
34	  
35	  tauId_reference      ->Write();    
36	  tauId_leadTrack      ->Write();   
37	  tauId_discr          ->Write();   
38	  tauId_againstElectron->Write();
39	  tauId_againstMuon    ->Write();
40	  tauId_all            ->Write();
41	  tauId_eff            ->Write();
42	  
43	  
44	  tauId_gen            ->Write();
45	  tauId_gen_matched    ->Write();
46	  tauId_gen_eta        ->Write();
47	  tauId_gen_matched_eta->Write();
48	}
49	
50	
51	void TauIdEff::FillHistos(std::vector<NTTau> theTaus, std::vector<TLorentzVector> genTaus, std::vector<TLorentzVector> agenTaus){
52	
53	  float DeltaRmin = 999.;
54	  float DeltaR    = 999.;
55	  
56	  //std::cout <<"----- " << theTaus.size()<< " " <<genTaus.size() <<" " <<agenTaus.size()<< std::endl;
57	   
58	  for (unsigned int j=0; j<theTaus.size(); j++)
59	  {
60	    //std::cout <<"reco " << theTaus[j].p4.Phi()<<" "<<theTaus[j].p4.Eta() <<std::endl;
61	    
62	    for (unsigned int k=0; k<genTaus.size(); k++)
63	    { 
64	       DeltaR = theTaus[j].p4.DeltaR(genTaus[k]);
65	       if (DeltaR < DeltaRmin) DeltaRmin = DeltaR;   
66	     }
67	    
68	    for (unsigned int l=0; l<agenTaus.size(); l++)
69	    { 
70	       DeltaR = theTaus[j].p4.DeltaR(agenTaus[l]);
71	       if (DeltaR < DeltaRmin) DeltaRmin = DeltaR;   
72	     }
73	     
74	     if (DeltaRmin<0.4)
75	     {
76	       tauId_reference->Fill(theTaus[j].p4.Pt());
77	       
78	       if (theTaus[j].leadTrackPt>5.)                              tauId_leadTrack->Fill(theTaus[j].p4.Pt());
79	       if (theTaus[j].GetDiscriminator("byTightIsolation")    ==1) tauId_discr->Fill(theTaus[j].p4.Pt());
80	       if (theTaus[j].GetDiscriminator("againstElectronLoose")==1) tauId_againstElectron->Fill(theTaus[j].p4.Pt());
81	       if (theTaus[j].GetDiscriminator("againstMuonLoose")    ==1) tauId_againstMuon->Fill(theTaus[j].p4.Pt());
82	       if (theTaus[j].leadTrackPt>5.&& theTaus[j].GetDiscriminator("byTightIsolation") ==1
83	                                    && theTaus[j].GetDiscriminator("againstElectronLoose")==1
84	                                    && theTaus[j].GetDiscriminator("againstMuonLoose")    ==1) tauId_all->Fill(theTaus[j].p4.Pt());
85	      }
86	    }
87	    
88	    
89	   
90	    // Matching generated taus to reco taus
91	    
92	    
93	    float DeltaRmin2 = 999.;
94	    float DeltaR2    = 999.;
95	  
96	    
97	    //for (unsigned int l=0; l<agenTaus.size(); l++)
98	    //{ 
99	    if(agenTaus.size()>0)
100	    {
101	     if(fabs(agenTaus[0].Eta())<2.5)
102	     {
103	     tauId_gen->Fill(agenTaus[0].Pt());
104	     tauId_gen_eta->Fill(agenTaus[0].Eta());
105	     for (unsigned int j=0; j<theTaus.size(); j++)
106	     {
107	      DeltaR2 = theTaus[j].p4.DeltaR(agenTaus[0]);
108	      if (DeltaR2 < DeltaRmin2) DeltaRmin2 = DeltaR2;   
109	     }
110	     
111	     if (DeltaRmin2<0.4) {tauId_gen_matched->Fill(agenTaus[0].Pt());tauId_gen_matched_eta->Fill(agenTaus[0].Eta());}
112	     }
113	     }
114	     //}
115	    
116	    DeltaRmin2 = 999.;
117	    DeltaR2    = 999.;
118	  
119	    //for (unsigned int l=0; l<genTaus.size(); l++)
120	    //{ 
121	     
122	    if(genTaus.size()>0)
123	    {
124	  
125	    if(fabs(genTaus[0].Eta())<2.5)
126	     {
127	    
128	     tauId_gen->Fill(genTaus[0].Pt());
129	     tauId_gen_eta->Fill(genTaus[0].Eta());
130	     for (unsigned int j=0; j<theTaus.size(); j++)
131	     {
132	      DeltaR2 = theTaus[j].p4.DeltaR(genTaus[0]);
133	      if (DeltaR2 < DeltaRmin2) DeltaRmin2 = DeltaR2;   
134	     }
135	     
136	     if (DeltaRmin2<0.4) {tauId_gen_matched->Fill(genTaus[0].Pt());tauId_gen_matched_eta->Fill(genTaus[0].Eta());}
137	     }
138	    }
139	   
140	}
141	
142	void TauIdEff::Plots(){
143	 
144	  gStyle->SetPadRightMargin(0.13);
145	  gStyle->SetPadLeftMargin(0.13);
146	  gStyle->SetPadBottomMargin(0.13);
147	  gStyle->SetOptTitle(1);
148	  gStyle->SetOptStat(0);
149	  
150	  gStyle->SetOptDate(0);
151	  gStyle->SetStatColor(0);
152	  gStyle->SetTitleColor(1);
153	  //gStyle->SetTitleW(0.4);
154	  //gStyle->SetTitleH(0.07);
155	  //gStyle->SetOptStat(stati);
156	  gStyle->SetOptStat(0);
157	  // gStyle->SetFrameFillColor(18);
158	  //gStyle->SetPalette(51,0);
159	  gStyle->SetPalette(1);
160	  
161	  
162	  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
163	   
164	  c1->SetFillColor(10);
165	  c1->SetFillStyle(4000);
166	  c1->SetBorderSize(2);
167	  
168	  //c1.Divide(2,2);
169	  c1->SetFillColor(0);
170	  c1->SetBorderMode(0);
171	  c1->SetFrameFillColor(10);
172	
173	}
174	
