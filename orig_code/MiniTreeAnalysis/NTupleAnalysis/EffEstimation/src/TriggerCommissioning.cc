1	#include "../interface/TriggerCommissioning.h"
2	
3	                
4	TriggerCommissioning::TriggerCommissioning(){}
5	
6	
7	TriggerCommissioning::~TriggerCommissioning(){}
8	
9	
10	void TriggerCommissioning::CreateHistos(){
11	  
12	  //
13	  jet1_ref = new TH1F("jet1_ref","jet1_ref", 19,10,200);
14	  jet2_ref = new TH1F("jet2_ref","jet2_ref", 19,10,200);
15	  jet3_ref = new TH1F("jet3_ref","jet3_ref", 19,10,200);
16	  jet4_ref = new TH1F("jet4_ref","jet4_ref", 19,10,200);
17	 
18	  jet1_trig = new TH1F("jet1_trig","jet1_trig", 19,10,200);
19	  jet2_trig = new TH1F("jet2_trig","jet2_trig", 19,10,200);
20	  jet3_trig = new TH1F("jet3_trig","jet3_trig", 19,10,200);
21	  jet4_trig = new TH1F("jet4_trig","jet4_trig", 19,10,200);
22	  
23	  jet1_eff = new TH1F("jet1_eff","jet-leg efficiency vs pT", 19,10,200);
24	  jet2_eff = new TH1F("jet2_eff","jet-leg efficiency vs pT", 19,10,200);
25	  jet3_eff = new TH1F("jet3_eff","jet-leg efficiency vs pT", 19,10,200);
26	  jet4_eff = new TH1F("jet4_eff","jet-leg efficiency vs pT", 19,10,200);
27	  
28	  jet1_eff->Sumw2();
29	  jet2_eff->Sumw2();
30	  jet3_eff->Sumw2();
31	  jet4_eff->Sumw2();
32	  
33	  //
34	  //Double_t x2bins[16] = {0,5,10,15,20,25,30,35,40,45,50,60,70,80,90,100};
35	  Double_t x2bins[19] = {0,5,10,15,20,25,30,32.5,35,37.5,40,42.5,45,47.5,50,60,70,80,90};
36	  
37	  reference_tau= new TH1F("reference_tau","reference_tau", 18, x2bins);
38	  triggered_tau= new TH1F("triggered_tau","triggered_tau", 18, x2bins);
39	  reference_tau_eta= new TH1F("reference_tau_eta","reference_tau_eta", 5,0,2.5);
40	  triggered_tau_eta= new TH1F("triggered_tau_eta","triggered_tau_eta", 5,0,2.5);
41	    
42	  eff_tau= new TH1F("eff_tau","eff_tau", 18, x2bins);
43	  eff_tau_eta= new TH1F("eff_tau_eta","eff_tau_eta", 5,0,2.5);
44	  
45	  reference_tau->Sumw2();
46	  triggered_tau->Sumw2();
47	  reference_tau_eta->Sumw2();
48	  triggered_tau_eta->Sumw2();
49	  eff_tau->Sumw2();
50	  eff_tau_eta->Sumw2();
51	  
52	  //
53	  HLTtau_pt       = new TH1F("HLTtau_pt","HLTtau_pt", 50,0,100);
54	  HLTtau_eta      = new TH1F("HLTtau_eta","HLTtau_eta", 100,0,2.5);
55	  HLTtau_pt_diff  = new TH1F("HLTtau_pt_diff","HLTtau_pt_diff", 100,-100,100);
56	  HLTtau_eta_diff = new TH1F("HLTtau_eta_diff","HLTtau_eta_diff", 100,-0.2,0.2);
57	  HLTtau_DR_diff  = new TH1F("HLTtau_DR_diff","HLTtau_DR_diff", 100,0,0.1);
58	 
59	 
60	  HLTTau_HLTJet_DR = new TH1F("HLTTau_HLTJet_DR","HLTTau_HLTJet_DR",100,0,5);
61	  reference_tau_2= new TH1F("reference_tau_2","reference_tau_2", 18, x2bins);
62	  triggered_tau_2= new TH1F("triggered_tau_2","triggered_tau_2", 18, x2bins);
63	  eff_tau_2= new TH1F("eff_tau_2","eff_tau_2", 18, x2bins);
64	  
65	  HLTTau_HLTJet_DR->Sumw2();
66	  reference_tau_2->Sumw2();
67	  triggered_tau_2->Sumw2();
68	  eff_tau_2->Sumw2();    
69	 }
70	
71	
72	void TriggerCommissioning::SaveHistos(TString inputRootFile){
73	
74	  TFile* f = new TFile(inputRootFile,"RECREATE");
75	  f->cd();
76	  
77	  //       
78	  jet1_ref->Write();
79	  jet2_ref->Write();
80	  jet3_ref->Write();
81	  jet4_ref->Write();
82	  
83	  jet1_trig->Write();
84	  jet2_trig->Write();
85	  jet3_trig->Write();
86	  jet4_trig->Write();
87	  
88	  jet1_eff->Divide(jet1_trig,jet1_ref,1,1,"B");
89	  jet2_eff->Divide(jet2_trig,jet2_ref,1,1,"B");
90	  jet3_eff->Divide(jet3_trig,jet3_ref,1,1,"B");
91	  jet4_eff->Divide(jet4_trig,jet4_ref,1,1,"B"); 
92	  
93	  jet1_eff->Write();
94	  jet2_eff->Write();
95	  jet3_eff->Write();
96	  jet4_eff->Write();
97	  
98	  //
99	  reference_tau->Write();
100	  triggered_tau->Write();
101	  eff_tau->Write();
102	  
103	  reference_tau_eta->Write();
104	  triggered_tau_eta->Write();
105	  eff_tau_eta->Write();
106	 
107	  HLTtau_pt->Write();
108	  HLTtau_eta->Write();
109	  HLTtau_pt_diff->Write();
110	  HLTtau_eta_diff->Write();
111	  HLTtau_DR_diff->Write();
112	  
113	  HLTTau_HLTJet_DR->Write();
114	  reference_tau_2->Write();
115	  triggered_tau_2->Write();
116	  eff_tau_2->Write();
117	 
118	}
119	
120	
121	void TriggerCommissioning::FillHistos(std::vector<NTJet> thejets, std::vector<NTTau> theTaus,  
122	bool passQuadJetIsoPFTau, float filterStatus, float filterStatus2, 
123	vector<TLorentzVector> tauObjTrig, vector<TLorentzVector> jetObjTrig){
124	   
125	  // find the jets
126	     
127	  int njets_eta25 = 0;
128	  int ijet0_eta25 =-1;
129	  int ijet1_eta25 =-1;
130	  int ijet2_eta25 =-1;
131	  int ijet3_eta25 =-1;
132	    
133	  
134	  for (unsigned int j=0; j<thejets.size(); j++)
135	  {
136	    if(fabs(thejets[j].p4.Eta())<2.5 && thejets[j].p4.Pt()>10.)
137	    {
138	     njets_eta25++; 
139	    
140	     if(njets_eta25 == 1 && thejets[j].p4.Pt()>70 && thejets[j].p4HLT.Px() !=0) ijet0_eta25 = j;
141	     if(njets_eta25 == 2 && thejets[j].p4.Pt()>70 && thejets[j].p4HLT.Px() !=0) ijet1_eta25 = j;
142	     if(njets_eta25 == 3 && thejets[j].p4.Pt()>70 && thejets[j].p4HLT.Px() !=0) ijet2_eta25 = j;
143	     if(njets_eta25 == 4 && thejets[j].p4.Pt()>10) ijet3_eta25 = j;}
144	   }
145	   
146	    
147	   if(njets_eta25 >= 4 && ijet0_eta25!=-1 && ijet1_eta25!=-1 && ijet2_eta25!=-1 && ijet3_eta25!=-1)
148	   { 
149	    if (njets_eta25>=1) jet1_ref->Fill(thejets[ijet0_eta25].p4.Pt()); 
150	    if (njets_eta25>=2) jet2_ref->Fill(thejets[ijet1_eta25].p4.Pt()); 
151	    if (njets_eta25>=3) jet3_ref->Fill(thejets[ijet2_eta25].p4.Pt()); 
152	    if (njets_eta25>=4) jet4_ref->Fill(thejets[ijet3_eta25].p4.Pt()); 
153	        
154	    if (njets_eta25>=1 && thejets[ijet0_eta25].p4HLT.Px() !=0)  jet1_trig->Fill(thejets[ijet0_eta25].p4.Pt());
155	    if (njets_eta25>=2 && thejets[ijet1_eta25].p4HLT.Px() !=0)  jet2_trig->Fill(thejets[ijet1_eta25].p4.Pt());
156	    if (njets_eta25>=3 && thejets[ijet2_eta25].p4HLT.Px() !=0)  jet3_trig->Fill(thejets[ijet2_eta25].p4.Pt());
157	    if (njets_eta25>=4 && thejets[ijet3_eta25].p4HLT.Px() !=0)  jet4_trig->Fill(thejets[ijet3_eta25].p4.Pt());
158	   }
159	    
160	    
161	   
162	  ///////////Tau Leg/////////////////////////////////////////////////////////////////
163	
164	   int njet = 0;
165	   float deltaRMin = 99;
166	   double deltaR = 99;
167	   
168	   //std::cout << "=====================" <<std::endl;
169	  
170	   for (unsigned int s=0; s<thejets.size(); s++)
171	   {
172	     if (fabs(thejets[s].p4.Eta())<2.4 && thejets[s].p4.Pt()>10 && thejets[s].p4HLT.Px()!=0) 
173	     { 
174	       //if (thejets[s].p4.DeltaR(thejets[s].p4HLT)>0.4) std::cout <<"got it : DeltaR jet" << thejets[s].p4.DeltaR(thejets[s].p4HLT) <<std::endl; }
175	       //else njetMatched++;
176	       //std::cout << "theJet " <<thejets[s].p4.Eta()<<" "<< thejets[s].p4.Phi()<<" "<< thejets[s].p4.Pt()<< std::endl;
177	       //std::cout << "trigJet " <<thejets[s].p4HLT.Eta()<<" "<< thejets[s].p4HLT.Phi()<<" "<< thejets[s].p4HLT.Pt()<< std::endl;
178	       
179	       if (thejets[s].p4.DeltaR(thejets[s].p4HLT)<0.4)
180	       {
181	        njet++;
182	        //std::cout << "theJet DeltaR " <<thejets[s].p4.DeltaR(thejets[s].p4HLT) << std::endl;
183	        if (theTaus.size()==1) 
184	        {
185	         deltaR = thejets[s].p4.DeltaR(theTaus[0].p4);
186	         if(deltaR < deltaRMin) deltaRMin = deltaR;
187	         }
188	       }     
189	     }
190	    }
191	    //std::cout <<"filter  " << filterStatus << " " << njet << std::endl;
192	        
193	    
194	   if (njet>=4 && theTaus.size()==1 && deltaRMin<0.4 && filterStatus==1)
195	   {  
196	        reference_tau->Fill(theTaus[0].p4.Pt());
197	        if (theTaus[0].p4.Pt()>=50) reference_tau_eta->Fill(theTaus[0].p4.Eta());
198	
199	        //if( theTaus[0].p4HLT.Px()!=0 && passQuadJetIsoPFTau == 1)
200	        //std::cout <<"filter2 " << filterStatus2 << std::endl;
201	        
202	        float deltaRMinTrig = 99.;
203	        float deltaRTrig =99.;
204	        float idx = -1;
205	        
206	        for (unsigned int i=0; i<tauObjTrig.size(); i++)
207	        { deltaRTrig = theTaus[0].p4.DeltaR(tauObjTrig[i]);
208	          if (deltaRTrig<deltaRMinTrig)
209	          { 
210	           deltaRMinTrig = deltaRTrig; 
211	           idx = i;
212	           }
213	         }
214	          
215	        if(filterStatus2==1 && deltaRMinTrig<0.4)
216	        {
217	          //std::cout <<"theTau  "<<theTaus[0].p4.Eta()<<" "<<theTaus[0].p4.Phi()<<" "<<theTaus[0].p4.Pt()<<std::endl;
218	          //std::cout <<"trigTau "<<tauObjTrig[idx].Eta()<<" "<<tauObjTrig[idx].Phi()<<" "<<tauObjTrig[idx].Pt()<<std::endl;
219	          //std::cout <<"Delta R " <<deltaRMinTrig<< std::endl;
220	        
221	          if (fabs(tauObjTrig[idx].Eta())<2.3)
222	          {
223	           HLTtau_pt->Fill(tauObjTrig[idx].Pt());
224	           HLTtau_eta->Fill(tauObjTrig[idx].Eta());
225	           HLTtau_pt_diff->Fill(theTaus[0].p4.Pt()-tauObjTrig[idx].Pt());
226	           HLTtau_eta_diff->Fill(theTaus[0].p4.Eta()-tauObjTrig[idx].Eta());
227	           HLTtau_DR_diff->Fill(theTaus[0].p4.Eta()-tauObjTrig[idx].Eta());
228	     
229	           triggered_tau->Fill(theTaus[0].p4.Pt());
230	           if (theTaus[0].p4.Pt()>=50) triggered_tau_eta->Fill(deltaRMinTrig);
231	           }
232	         }
233	      }
234	
235	   eff_tau->Divide(triggered_tau,reference_tau,1,1,"B");
236	   eff_tau_eta->Divide(triggered_tau_eta,reference_tau_eta,1,1,"B");
237	   
238	   
239	   // number of objects in trigger
240	   //std::cout <<"======= "<< passQuadJetIsoPFTau<< std::endl;
241	   if(passQuadJetIsoPFTau)
242	   {
243	   float deltaR=3;
244	   float deltaRmin=3;
245	   if (filterStatus==1 && filterStatus2==1) 
246	   {
247	    for (unsigned int i=0; i<tauObjTrig.size(); i++)
248	    {
249	     //std::cout <<"tau "<< tauObjTrig[i].Eta()<<" " << tauObjTrig[i].Phi()<< std::endl;
250	     for (unsigned int i=0; i<jetObjTrig.size(); i++)
251	     {
252	      //std::cout <<"jet " << jetObjTrig[i].Eta()<<" " << jetObjTrig[i].Phi()<< std::endl;
253	      deltaR = tauObjTrig[0].DeltaR(jetObjTrig[i]);
254	      if(deltaR<deltaRmin)
255	      deltaRmin = deltaR;
256	      }
257	   
258	      }
259	      HLTTau_HLTJet_DR->Fill(deltaRmin);
260	     }//else cout << "problem"<<endl;
261	    }
262	    
263	    // tau-leg 2
264	    
265	    if (theTaus.size()==1 && filterStatus==1 && jetObjTrig.size()==4)
266	    {   reference_tau_2->Fill(theTaus[0].p4.Pt());
267	    
268	        float deltaRMinTrigTau = 99.;
269	        float deltaRTrigTau =99.;
270	        float idxTau = -1;
271	        
272	        for (unsigned int i=0; i<tauObjTrig.size(); i++)
273	        { deltaRTrigTau = theTaus[0].p4.DeltaR(tauObjTrig[i]);
274	          if (deltaRTrigTau<deltaRMinTrigTau)
275	          { 
276	           deltaRMinTrigTau = deltaRTrigTau; 
277	           idxTau = i;
278	           }
279	         }
280	        
281	        float deltaRMinTrigJet = 99.;
282	        float deltaRTrigJet =99.;
283	        float idxJet = -1;
284	        //std::cout <<" jetObjTrig.size() " <<jetObjTrig.size()<< std::endl;
285	        
286	        for (unsigned int j=0; j<jetObjTrig.size(); j++)
287	        { deltaRTrigJet = theTaus[0].p4.DeltaR(jetObjTrig[j]);
288	          if (deltaRTrigJet<deltaRMinTrigJet)
289	          { 
290	           deltaRMinTrigJet = deltaRTrigJet; 
291	           idxJet = j;
292	           }
293	         }
294	        
295	        //if(filterStatus2==1 && deltaRMinTrigTau<0.4)
296	          
297	        if(filterStatus2==1 && deltaRMinTrigTau<0.4 && deltaRMinTrigJet<0.4)
298	        { 
299	         triggered_tau_2->Fill(theTaus[0].p4.Pt());}
300	     }   
301	    eff_tau_2->Divide(triggered_tau_2,reference_tau_2,1,1,"B");
302	 
303	   
304	}
305	
306	void TriggerCommissioning::Plots(){
307	
308	}
309	

