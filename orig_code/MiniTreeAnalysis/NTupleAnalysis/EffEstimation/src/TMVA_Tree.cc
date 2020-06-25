1	#include "../interface/TMVA_Tree.h"
2	
3	                
4	TMVA_Tree::TMVA_Tree(){}
5	
6	
7	TMVA_Tree::~TMVA_Tree(){}
8	
9	
10	void TMVA_Tree::CreateHistos(TString inputRootFile){
11	
12	
13	  file = new TFile(inputRootFile.Data(),"RECREATE");
14	  T = new TTree("Ttree","Ttree");
15	    
16	  T->Branch("weight",     &weight_,     "weight/F");
17	  T->Branch("weight_BTAG",     &weight_BTAG_,     "weight_BTAG/F");
18	  T->Branch("aplanarity", &aplanarity_, "aplanarity/F");
19	  T->Branch("circularity",&circularity_,"circularity/F");
20	  T->Branch("sphericity", &sphericity_, "sphericity/F");
21	  T->Branch("Mt",         &Mt_,         "Mt/F");
22	  T->Branch("Ht",         &Ht_,         "Ht/F");
23	  T->Branch("Ht3",        &Ht3_,        "Ht3/F");
24	  T->Branch("chargeEta",  &chargeEta_,  "chargeEta/F");
25	  T->Branch("Met",        &Met_,        "Met/F"); 
26	  T->Branch("MtauJet",    &MTauJet_,    "MTauJet/F"); 
27	  T->Branch("M3",         &M3_,         "M3/F"); 
28	  T->Branch("M2",         &M2_,         "M2/F"); 
29	  T->Branch("chi2",       &chi2_,       "chi2/F"); 
30	  T->Branch("C",          &C_,          "C/F"); 
31	  T->Branch("D",          &D_,          "D/F"); 
32	  T->Branch("DeltaPhiTauMet", &DeltaPhiTauMet_, "DeltaPhiTauMet/F"); 
33	  T->Branch("NNOutput",       &NNOutput_,        "NNOutput/F"); 
34	  T->Branch("TauPt",          &TauPt_,           "TauPt/F");
35	  T->Branch("TauEta",         &TauEta_,          "TauEta/F");
36	  T->Branch("jetMultiplicity",&jetMultiplicity_, "jetMultiplicity/I");
37	  T->Branch("jetPt",          &jetPt_,           "jetPt/F");
38	  T->Branch("jetEta",         &jetEta_,          "jetEta/F");
39	  T->Branch("kinFitChi2",     &kinFitChi2_,      "kinFitChi2/F");
40	  T->Branch("kinFitTopMass",  &kinFitTopMass_,   "kinFitTopMass/F");
41	 
42	  reader = new TMVA::Reader( "" ); 
43	  reader->AddVariable( "aplanarity", &aplanarity_);
44	  reader->AddVariable( "chargeEta" , &chargeEta_ );
45	  reader->AddVariable( "Met",        &Met_);
46	  reader->AddVariable( "MtauJet",    &MTauJet_ ); 
47	  reader->AddVariable( "Ht" ,        &Ht_ ); 
48	  reader->AddVariable( "kinFitChi2", &kinFitChi2_ );
49	  reader->AddVariable( "DeltaPhiTauMet", &DeltaPhiTauMet_ );
50	  reader->AddVariable( "D", &D_);
51	
52	  reader->BookMVA("MLP","/opt/sbg/data/data1/cms/lebihan/official8/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/weights/TMVAClassification_MLP_KinFit_MET15_cris.weights.xml");
53	}
54	
55	
56	void TMVA_Tree::SaveHistos(TString inputRootFile){
57	 
58	  T->Print();
59	  T->Write();
60	  file->Close();
61	}  
62	
63	////////:to run on MC
64	void TMVA_Tree::FillHistos(std::vector<NTJet> theJets, vector<TLorentzVector>jetObjTrig, NTMET Met, std::vector<NTMuon> selMuons, std::vector<NTElectron> selElectrons,
65	                           std::vector<NTTau> theTausLoose, std::vector<NTTau> theTausMedium, vector<TLorentzVector> tauObjTrig, bool PassTriggerQuadJet4045IsoPFTauSelection,  const float& weight, float& weight_BTAG){
66	
67	
68	        vector<NTJet> btagjets;
69	        vector<float> btagDiscri;
70	        int btagAlgo_ = 0 ;
71	        float btagDiscriCut_ = 3.3;
72	        
73	        int njets_45 = 0;
74	        
75	        for(unsigned int j=0;j<theJets.size();j++){
76	                
77	          if (theJets[j].p4.Pt()>45){
78	
79	                
80	        float deltaRMinJetTrig =99.; 
81	            float deltaRJetTrig =99.;
82	            for(unsigned int k=0;k<jetObjTrig.size();k++)
83	              {
84	                deltaRJetTrig = theJets[j].p4.DeltaR(jetObjTrig[k]);
85	                if (deltaRJetTrig<deltaRMinJetTrig) deltaRMinJetTrig = deltaRJetTrig; }
86	            
87	            if(deltaRMinJetTrig<0.4) 
88	                  
89	                  njets_45++;
90	                 
91	          } 
92	                
93	                switch(btagAlgo_){
94	                        case 0 :
95	                                if(theJets[j].TCDiscri>=btagDiscriCut_){
96	                                         btagjets.push_back(theJets[j]);
97	                                         btagDiscri.push_back(theJets[j].TCDiscri);
98	                                }
99	                                break;  
100	                        case 1 :
101	                                if(theJets[j].SVDiscri>=btagDiscriCut_){
102	                                        btagjets.push_back(theJets[j]); 
103	                                        btagDiscri.push_back(theJets[j].SVDiscri);
104	                                }
105	                                break;  
106	                        case 2 :
107	                                if(theJets[j].SMDiscri>=btagDiscriCut_){
108	                                        btagjets.push_back(theJets[j]); 
109	                                        btagDiscri.push_back(theJets[j].SMDiscri);
110	                                }
111	                                break;  
112	                        case 4 :
113	                                //std::cout << "JP discri " << jet_cand[j].GetDiscri("jetProbabilityBJetTags") << std::endl;
114	                                if(theJets[j].GetDiscri("jetProbabilityBJetTags")>=btagDiscriCut_){
115	                                        btagjets.push_back(theJets[j]); 
116	                                        btagDiscri.push_back(theJets[j].GetDiscri("jetProbabilityBJetTags"));
117	                                }
118	                                break;          
119	                                
120	                        default:
121	                                cerr << "btagAlgo doesn't exist !"<<endl;
122	                                break;  
123	                }
124	                
125	        }
126	        
127	        
128	
129	        //// da scommentare quando si gira sui dati!!!!!!!!
130	  if( theTausMedium.size()==1 && tauObjTrig.size()>=1 ){
131	    
132	    float deltaRMinTauTrig = 99.;
133	    float deltaRTauTrig = 99.;
134	    for(unsigned int j=0;j<tauObjTrig.size();j++)
135	      {
136	        deltaRTauTrig = theTausMedium[0].p4.DeltaR(tauObjTrig[j]);
137	        if(deltaRTauTrig<deltaRMinTauTrig) deltaRMinTauTrig = deltaRTauTrig; }
138	    
139	    if(deltaRMinTauTrig<0.4){
140	            
141	            
142	            if ( theJets.size() >= 4 && njets_45>=3 &&  
143	                 theTausMedium.size() == 1 && Met.p4.Et()>=20 &&  selMuons.size()==0 && selElectrons.size()==0 && btagjets.size()==0)
144	              {
145	                if(PassTriggerQuadJet4045IsoPFTauSelection==true){
146	                
147	                weight_ = weight;
148	                weight_BTAG_ = weight_BTAG;
149	                EventShapes evShape(theJets, theTausMedium);
150	                EventShapes evShapeM3(theJets);
151	                
152	                aplanarity_  = evShape.aplanarity();
153	                circularity_ = evShape.circularity();
154	                sphericity_  = evShape.sphericity();
155	                Ht_          = evShape.HT();
156	                Ht3_         = evShapeM3.HT3();
157	                MTauJet_     = evShape.sqrt_s();
158	                chi2_        = evShapeM3.chi2(theJets);
159	                Mt_          = sqrt (pow(theTausMedium[0].p4.Et() + Met.p4.Et(),2) - pow(theTausMedium[0].p4.Px() + Met.p4.Px(),2) - pow(theTausMedium[0].p4.Py() + Met.p4.Py(),2));
160	                chargeEta_   =(theTausMedium[0].Charge)*fabs((theTausMedium[0].p4.Eta()));
161	                Met_         = Met.p4.Et();      
162	                M3_          = evShape.M3chi2(theJets);
163	                M2_          = evShape.M2chi2(theJets);
164	                C_           = evShape.C();
165	                D_           = evShape.D();
166	                DeltaPhiTauMet_ = fabs(theTausMedium[0].p4.DeltaPhi(Met.p4));
167	                
168	                NNOutput_= reader->EvaluateMVA("MLP");
169	                TauPt_ = theTausMedium[0].p4.Et();
170	                TauEta_= theTausMedium[0].p4.Eta();
171	                jetMultiplicity_ = theJets.size();
172	                jetPt_ = theJets[0].p4.Pt();
173	                jetEta_= theJets[0].p4.Eta();
174	                
175	                //------------------------------------ kinFitter stuff
176	                
177	                std::vector<NTMET> mets;
178	                mets.push_back(Met);
179	                
180	                vector<NTLepton> lep;
181	                lep.push_back(dynamic_cast<NTLepton&> (theTausMedium.at(0)));
182	                
183	                std::string lepType = "tau";
184	                kinFitter.produce(theJets, lep,"tau", mets);
185	                
186	                kinFitChi2_ = kinFitter.GetFitChi2();
187	                
188	                TLorentzVector HadB = kinFitter.GetFitHadB();
189	                TLorentzVector HadP = kinFitter.GetFitHadP();
190	                TLorentzVector HadQ = kinFitter.GetFitHadQ();   
191	                TLorentzVector LepB = kinFitter.GetFitLepB();   
192	                TLorentzVector LepL = kinFitter.GetFitLepL();   
193	                TLorentzVector LepN = kinFitter.GetFitLepN();   
194	                
195	                TLorentzVector HadSide = HadB+HadP+HadQ;
196	                //std::cout << "M top had "<< HadSide.M() <<std::endl; 
197	                kinFitTopMass_ = HadSide.M();
198	                TLorentzVector LepSide = LepB+LepL+LepN;
199	                //std::cout << "M top Lep "<< LepSide.M() <<std::endl; 
200	                TLorentzVector WLepSide = LepL+LepN;
201	                //std::cout << "M W lep "<< WLepSide.M() <<std::endl; 
202	                TLorentzVector WHadSide = HadP+HadQ;
203	                //std::cout << "M W  Had"<< WHadSide.M() <<std::endl; 
204	                
205	                
206	                T->Fill();
207	                        }
208	                    }
209	          }
210	        }
211	}
212	void TMVA_Tree::Plots(){
213	  
214	  gStyle->SetPadRightMargin(0.13);
215	  gStyle->SetPadLeftMargin(0.13);
216	  gStyle->SetPadBottomMargin(0.13);
217	  gStyle->SetOptTitle(1);
218	  gStyle->SetOptStat(0);
219	  
220	  gStyle->SetOptDate(0);
221	  gStyle->SetStatColor(0);
222	  gStyle->SetTitleColor(1);
223	  gStyle->SetOptStat(0);
224	  gStyle->SetPalette(1);
225	  
226	  
227	  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
228	  
229	  c1->SetFillColor(10);
230	  c1->SetFillStyle(4000);
231	  c1->SetBorderSize(2);
232	  
233	  //c1.Divide(2,2);
234	  c1->SetFillColor(0);
235	  c1->SetBorderMode(0);
236	  c1->SetFrameFillColor(10);
237	  
238	}
239	
