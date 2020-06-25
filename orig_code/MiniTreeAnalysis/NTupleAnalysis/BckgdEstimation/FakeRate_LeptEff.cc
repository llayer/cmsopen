1	#include "../interface/FakeRate_LeptEff.h"
2	
3	                
4	FakeRate_LeptEff::FakeRate_LeptEff(){
5	
6	     
7	}
8	
9	
10	FakeRate_LeptEff::~FakeRate_LeptEff(){}
11	
12	
13	
14	
15	
16	void FakeRate_LeptEff::CreateHistos(string TypeSel, string datasetname){
17	
18	  TH2F *h_pt, *h_eta, *h_phi, *h_njet;
19	  TH3F *h_pt_eta, *h_pt_njet, *h_eta_njet;
20	  
21	  
22	  if(TypeSel == "muons"){
23	     
24	     //*******************
25	     //for loose selection
26	     //*******************
27	     h_pt     = new TH2F( (datasetname+TypeSel+"_loose_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
28	     hlist_Mu_Loose.push_back(h_pt);
29	     h_eta    = new TH2F( (datasetname+TypeSel+"_loose_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
30	     hlist_Mu_Loose.push_back(h_eta);
31	     h_phi    = new TH2F( (datasetname+TypeSel+"_loose_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
32	     hlist_Mu_Loose.push_back(h_phi);
33	     h_njet   = new TH2F( (datasetname+TypeSel+"_loose_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
34	     hlist_Mu_Loose.push_back(h_njet);
35	     
36	     
37	     h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_loose_pt_eta").c_str(),  "", 48, 0, 120,12, 10, 70, 10, -2.5, 2.5); 
38	     hlist_Mu_Loose_2D.push_back(h_pt_eta);
39	     h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_loose_pt_njet").c_str(), "", 48, 0, 120,12, 10, 70, 7, -0.5, 6.5); 
40	     hlist_Mu_Loose_2D.push_back(h_pt_njet);
41	     h_eta_njet = new TH3F ( (datasetname+TypeSel+"_loose_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5, 7, -0.5, 6.5); 
42	     hlist_Mu_Loose_2D.push_back(h_eta_njet);
43	     
44	     //*******************
45	     //for tight selection
46	     //*******************
47	     h_pt     = new TH2F( (datasetname+TypeSel+"_tight_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
48	     hlist_Mu_Tight.push_back(h_pt);
49	     h_eta    = new TH2F( (datasetname+TypeSel+"_tight_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
50	     hlist_Mu_Tight.push_back(h_eta);
51	     h_phi    = new TH2F( (datasetname+TypeSel+"_tight_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
52	     hlist_Mu_Tight.push_back(h_phi);
53	     h_njet   = new TH2F( (datasetname+TypeSel+"_tight_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
54	     hlist_Mu_Tight.push_back(h_njet);
55	     
56	     
57	     h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_tight_pt_eta").c_str(),  "", 48, 0, 120, 12,   10,  70, 10, -2.5, 2.5); 
58	     hlist_Mu_Tight_2D.push_back(h_pt_eta);
59	     h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_tight_pt_njet").c_str(), "", 48, 0, 120, 12,   10,  70,  7, -0.5, 6.5); 
60	     hlist_Mu_Tight_2D.push_back(h_pt_njet);
61	     h_eta_njet = new TH3F ( (datasetname+TypeSel+"_tight_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5,  7, -0.5, 6.5); 
62	     hlist_Mu_Tight_2D.push_back(h_eta_njet);
63	    
64	     
65	     if(datasetname == "Data"){
66	     
67	     
68	       h_pt     = new TH2F( (datasetname+TypeSel+"_Fake_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
69	       hlist_Mu_Fake_Data.push_back(h_pt);
70	       h_eta    = new TH2F( (datasetname+TypeSel+"_Fake_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
71	       hlist_Mu_Fake_Data.push_back(h_eta);
72	       h_phi    = new TH2F( (datasetname+TypeSel+"_Fake_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
73	       hlist_Mu_Fake_Data.push_back(h_phi);
74	       h_njet   = new TH2F( (datasetname+TypeSel+"_Fake_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
75	       hlist_Mu_Fake_Data.push_back(h_njet);
76	     
77	     
78	       h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_Fake_pt_eta").c_str(),  "", 48, 0, 120,12, 10, 70, 10, -2.5, 2.5); 
79	       hlist_Mu_Fake_Data_2D.push_back(h_pt_eta);
80	       h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_Fake_pt_njet").c_str(), "", 48, 0, 120,12, 10, 70, 7, -0.5, 6.5); 
81	       hlist_Mu_Fake_Data_2D.push_back(h_pt_njet);
82	       h_eta_njet = new TH3F ( (datasetname+TypeSel+"_Fake_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5, 7, -0.5, 6.5); 
83	       hlist_Mu_Fake_Data_2D.push_back(h_eta_njet);
84	     
85	       h_pt     = new TH2F( (datasetname+TypeSel+"_LeptEff_pt").c_str(),  "", 48, 0, 120, 12, 10, 70); 
86	       hlist_Mu_LeptEff_Data.push_back(h_pt);
87	       h_eta    = new TH2F( (datasetname+TypeSel+"_LeptEff_eta").c_str(), "", 48, 0, 120, 10, -2.5, 2.5); 
88	       hlist_Mu_LeptEff_Data.push_back(h_eta);
89	       h_phi    = new TH2F( (datasetname+TypeSel+"_LeptEff_phi").c_str(), "", 48, 0, 120, 10, -3.14, 3.14); 
90	       hlist_Mu_LeptEff_Data.push_back(h_phi);
91	       h_njet   = new TH2F( (datasetname+TypeSel+"_LeptEff_njet").c_str(),"", 48, 0, 120, 7, -0.5, 6.5); 
92	       hlist_Mu_LeptEff_Data.push_back(h_njet);
93	     
94	     
95	       h_pt_eta   = new TH3F ( (datasetname+TypeSel+"_LeptEff_pt_eta").c_str(),  "", 48, 0, 120,12, 10, 70, 10, -2.5, 2.5); 
96	       hlist_Mu_LeptEff_Data_2D.push_back(h_pt_eta);
97	       h_pt_njet  = new TH3F ( (datasetname+TypeSel+"_LeptEff_pt_njet").c_str(), "", 48, 0, 120,12, 10, 70, 7, -0.5, 6.5); 
98	       hlist_Mu_LeptEff_Data_2D.push_back(h_pt_njet);
99	       h_eta_njet = new TH3F ( (datasetname+TypeSel+"_LeptEff_eta_njet").c_str(),"", 48, 0, 120, 10, -2.5, 2.5, 7, -0.5, 6.5); 
100	       hlist_Mu_LeptEff_Data_2D.push_back(h_eta_njet);
101	     
102	     
103	     
104	     }
105	     
106	     
107	     
108	    
109	  }
110	  
111	  if(TypeSel == "electron"){
112	    std::cout << " not available for electrons " << std::endl;
113	  }
114	
115	}
116	
117	
118	void FakeRate_LeptEff::ReadHistos(TString inputRootFile){
119	   // to read histograms already produced
120	
121	
122	
123	}
124	
125	
126	
127	void FakeRate_LeptEff::FillHistos(NTMET theMet, std::vector<NTJet> thejets, string TypeSel, string datasetname){
128	  //cout << "TypeSel " << TypeSel << endl;
129	  //cout << " muons.size() " <<  muons.size() << endl;
130	   if(TypeSel == "muons" && muons.size() == 1){
131	    
132	    int njet = thejets.size();
133	     
134	    double deltaphiMuMET =  theMet.p4.Phi() - muons[0].p4.Phi();
135	    if(deltaphiMuMET < -1*M_PI ) deltaphiMuMET += 2*M_PI;
136	    if(deltaphiMuMET > M_PI    ) deltaphiMuMET -= 2*M_PI;
137	    double mT = pow(2*(muons[0].p4.Pt())*(theMet.p4.Pt())*(1-cos(deltaphiMuMET))  , 0.5);
138	    
139	    
140	    double relIso = muons[0].RelIso03();
141	    for(unsigned int i=0; i< hlist_Mu_Loose.size(); i++){
142	     if(relIso < 0.5){
143	       if((datasetname+TypeSel+"_loose_pt")   == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, muons[0].p4.Pt());
144	       if((datasetname+TypeSel+"_loose_eta")  == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, muons[0].p4.Eta());
145	       if((datasetname+TypeSel+"_loose_phi")  == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, muons[0].p4.Phi());
146	       if((datasetname+TypeSel+"_loose_njet") == hlist_Mu_Loose[i]->GetName() )  hlist_Mu_Loose[i]->Fill(mT, njet);
147	     
148	     
149	       if( i<3 && (datasetname+TypeSel+"_loose_pt_eta")  == hlist_Mu_Loose_2D[i]->GetName() )  hlist_Mu_Loose_2D[i]->Fill(mT, muons[0].p4.Pt(),  muons[0].p4.Eta() );
150	       if( i<3 && (datasetname+TypeSel+"_loose_pt_njet") == hlist_Mu_Loose_2D[i]->GetName() )  hlist_Mu_Loose_2D[i]->Fill(mT, muons[0].p4.Pt(),  njet);
151	       if( i<3 && (datasetname+TypeSel+"_loose_eta_njet")== hlist_Mu_Loose_2D[i]->GetName() )  hlist_Mu_Loose_2D[i]->Fill(mT, muons[0].p4.Eta(), njet);
152	     }
153	     //*******************
154	     //for tight selection
155	     //*******************
156	     if(relIso < 0.15){
157	       if((datasetname+TypeSel+"_tight_pt")   == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, muons[0].p4.Pt());
158	       if((datasetname+TypeSel+"_tight_eta")  == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, muons[0].p4.Eta());
159	       if((datasetname+TypeSel+"_tight_phi")  == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, muons[0].p4.Phi());
160	       if((datasetname+TypeSel+"_tight_njet") == hlist_Mu_Tight[i]->GetName() ) hlist_Mu_Tight[i]->Fill(mT, njet);
161	     
162	     
163	       if( i<3 && (datasetname+TypeSel+"_tight_pt_eta")   == hlist_Mu_Tight_2D[i]->GetName() ) hlist_Mu_Tight_2D[i]->Fill(mT, muons[0].p4.Pt(),  muons[0].p4.Eta());
164	       if( i<3 && (datasetname+TypeSel+"_tight_pt_njet")  == hlist_Mu_Tight_2D[i]->GetName() ) hlist_Mu_Tight_2D[i]->Fill(mT, muons[0].p4.Pt(),  njet);
165	       if( i<3 && (datasetname+TypeSel+"_tight_eta_njet") == hlist_Mu_Tight_2D[i]->GetName() ) hlist_Mu_Tight_2D[i]->Fill(mT, muons[0].p4.Eta(), njet);
166	     }
167	    }
168	    
169	    
170	    
171	  }
172	  
173	  
174	  
175	  
176	
177	}
178	
179	
180	
181	void FakeRate_LeptEff::CalculateFakeRateLeptonEff(){
182	 
183	 
184	 
185	  
186	  
187	  gStyle->SetPadRightMargin(0.13);
188	  gStyle->SetPadLeftMargin(0.13);
189	  gStyle->SetPadBottomMargin(0.13);
190	  gStyle->SetOptTitle(0);
191	  gStyle->SetOptStat(0);
192	  
193	  gStyle->SetOptDate(0);
194	  gStyle->SetStatColor(0);
195	  gStyle->SetTitleColor(1);
196	  //gStyle->SetTitleW(0.4);
197	  //gStyle->SetTitleH(0.07);
198	  //gStyle->SetOptStat(stati);
199	  gStyle->SetOptStat(0);
200	  // gStyle->SetFrameFillColor(18);
201	  //gStyle->SetPalette(51,0);
202	  gStyle->SetPalette(1);
203	  
204	  
205	  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
206	   
207	  c1->SetFillColor(10);
208	  c1->SetFillStyle(4000);
209	  c1->SetBorderSize(2);
210	  
211	  //c1.Divide(2,2);
212	  c1->SetFillColor(0);
213	  c1->SetBorderMode(0);
214	  c1->SetFrameFillColor(10);
215	
216	  
217	  //RooWorkspace *w_loose = new RooWorkspace("w",kTRUE) ;  
218	
219	  
220	  
221	  
222	  
223	  
224	  
225	
226	
227	}
228	
229	
230	void FakeRate_LeptEff::ShowFit(float pt, float eta, float phi, int njet, int param){
231	
232	
233	
234	
235	
236	}
237	
238	
239	//******************************
240	//get values and errors for muon
241	//******************************
242	
243	std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPt(    float pt  ){
244	
245	   std::pair<float, float> val_err(0, 0);
246	   
247	   
248	   
249	   
250	   return val_err;
251	}
252	
253	
254	
255	std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonEta(   float eta ){
256	
257	
258	   std::pair<float, float> val_err(0, 0);
259	
260	   return val_err;
261	
262	
263	}
264	
265	
266	
267	std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPhi(   float phi ){
268	
269	
270	   std::pair<float, float> val_err(0, 0);
271	
272	   return val_err;
273	
274	
275	}
276	
277	
278	
279	std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonNJets( int njet  ){
280	
281	
282	   std::pair<float, float> val_err(0, 0);
283	
284	   return val_err;
285	
286	
287	}
288	
289	
290	
291	
292	std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPtEta(   float pt,  float eta   ){
293	
294	   std::pair<float, float> val_err(0, 0);
295	
296	   return val_err;
297	
298	
299	
300	}
301	
302	
303	
304	std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonPtNJet(  float pt,  int   njets ){
305	
306	
307	   std::pair<float, float> val_err(0, 0);
308	
309	   return val_err;
310	
311	
312	}
313	
314	
315	
316	std::pair<float, float> FakeRate_LeptEff::GetFakeRateMuonEtaNJet( float eta, int   njets ){
317	
318	
319	   std::pair<float, float> val_err(0, 0);
320	
321	   return val_err;
322	
323	
324	}
325	
326	
327	
328	
329	
330	
331	
332	std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPt(      float pt  ){
333	
334	   std::pair<float, float> val_err(0, 0);
335	
336	   return val_err;
337	
338	
339	
340	}
341	
342	
343	
344	std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonEta(   float eta ){
345	
346	   std::pair<float, float> val_err(0, 0);
347	
348	   return val_err;
349	
350	
351	
352	}
353	
354	
355	
356	std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPhi(   float phi ){
357	
358	
359	   std::pair<float, float> val_err(0, 0);
360	
361	   return val_err;
362	
363	
364	}
365	
366	
367	
368	std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonNJets( int njet  ){
369	
370	
371	   std::pair<float, float> val_err(0, 0);
372	
373	   return val_err;
374	
375	
376	}
377	
378	
379	
380	
381	std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPtEta(   float pt,  float eta   ){
382	
383	
384	   std::pair<float, float> val_err(0, 0);
385	
386	   return val_err;
387	
388	
389	}
390	
391	
392	
393	std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonPtNJet(  float pt,  int   njets ){
394	
395	
396	   std::pair<float, float> val_err(0, 0);
397	
398	   return val_err;
399	
400	
401	}
402	
403	
404	
405	std::pair<float, float> FakeRate_LeptEff::GetLeptEffMuonEtaNJet( float eta, int   njets ){
406	
407	
408	   std::pair<float, float> val_err(0, 0);
409	
410	   return val_err;
411	
412	
413	}
414	
415	
416	// get values for electrons
417	
418	
419	std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPt(    float pt  ){
420	
421	   std::pair<float, float> val_err(0, 0);
422	   
423	   
424	   
425	   return val_err;
426	}
427	
428	
429	
430	std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronEta(   float eta ){
431	
432	
433	   std::pair<float, float> val_err(0, 0);
434	
435	   return val_err;
436	
437	
438	}
439	
440	
441	
442	std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPhi(   float phi ){
443	
444	
445	   std::pair<float, float> val_err(0, 0);
446	
447	   return val_err;
448	
449	
450	}
451	
452	
453	
454	std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronNJets( int njet  ){
455	
456	
457	   std::pair<float, float> val_err(0, 0);
458	
459	   return val_err;
460	
461	
462	}
463	
464	
465	
466	
467	std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPtEta(   float pt,  float eta   ){
468	
469	   std::pair<float, float> val_err(0, 0);
470	
471	   return val_err;
472	
473	
474	
475	}
476	
477	
478	
479	std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronPtNJet(  float pt,  int   njets ){
480	
481	
482	   std::pair<float, float> val_err(0, 0);
483	
484	   return val_err;
485	
486	
487	}
488	
489	
490	
491	std::pair<float, float> FakeRate_LeptEff::GetFakeRateElectronEtaNJet( float eta, int   njets ){
492	
493	
494	   std::pair<float, float> val_err(0, 0);
495	
496	   return val_err;
497	
498	
499	}
500	
501	
502	
503	
504	
505	
506	
507	std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPt(  float pt  ){
508	
509	   std::pair<float, float> val_err(0, 0);
510	
511	   return val_err;
512	
513	
514	
515	}
516	
517	
518	
519	std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronEta(   float eta ){
520	
521	   std::pair<float, float> val_err(0, 0);
522	
523	   return val_err;
524	
525	
526	
527	}
528	
529	
530	
531	std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPhi(   float phi ){
532	
533	
534	   std::pair<float, float> val_err(0, 0);
535	
536	   return val_err;
537	
538	
539	}
540	
541	
542	
543	std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronNJets( int njet  ){
544	
545	
546	   std::pair<float, float> val_err(0, 0);
547	
548	   return val_err;
549	
550	
551	}
552	
553	
554	
555	
556	std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPtEta(   float pt,  float eta   ){
557	
558	
559	   std::pair<float, float> val_err(0, 0);
560	
561	   return val_err;
562	
563	
564	}
565	
566	
567	
568	std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronPtNJet(  float pt,  int   njets ){
569	
570	
571	   std::pair<float, float> val_err(0, 0);
572	
573	   return val_err;
574	
575	
576	}
577	
578	
579	
580	std::pair<float, float> FakeRate_LeptEff::GetLeptEffElectronEtaNJet( float eta, int   njets ){
581	
582	
583	   std::pair<float, float> val_err(0, 0);
584	
585	   return val_err;
586	
587	
588	}
589	void FakeRate_LeptEff::Savehistos(TString outputFileName){
590	    cout << " write histo " << endl;
591	    TFile * myFile = new TFile(outputFileName.Data(), "recreate");
592	    myFile->cd();
593	    for(unsigned int i=0; i< hlist_Mu_Loose.size(); i++){
594	       hlist_Mu_Loose[i]   ->Write();
595	       if(i < 3) hlist_Mu_Loose_2D[i]->Write();
596	       hlist_Mu_Tight[i]   ->Write();
597	       if(i < 3) hlist_Mu_Tight_2D[i]->Write();
598	    }
599	    for(unsigned int i=0; i< hlist_Mu_Fake_Data.size(); i++){
600	       hlist_Mu_Fake_Data[i]      ->Write();
601	       if(i < 3) hlist_Mu_Fake_Data_2D[i]   ->Write();
602	       hlist_Mu_LeptEff_Data[i]   ->Write();
603	       if(i < 3) hlist_Mu_LeptEff_Data_2D[i]->Write();
604	    }
605	    myFile->Close();
606	    myFile = 0;
607	    delete myFile ;
608	}
609	
