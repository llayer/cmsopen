#include "../interface/DYEstimation.h"
2	
3	                
4	DYEstimation::DYEstimation(){
5	     NDYMC_CR          = 0;
6	     NDYMCinMz_SR      = 0;   
7	     NDYMCinMz_CR      = 0;   
8	     NFittedDYMC_CR          = 0;
9	     NFittedDYMCinMz_SR      = 0;   
10	     NFittedDYMCinMz_CR      = 0;   
11	     NunnormDYMC_CR          = 0;
12	     NunnormDYMCinMz_SR      = 0;   
13	     NunnormDYMCinMz_CR      = 0;   
14	     NDYEstimatedMC    = 0;  
15	     NDYMC_SR          = 0;          
16	     NMC_CR = 0.; 
17	     NMCinMz_CR = 0.;
18	     NMCinMz_SR = 0.;
19	
20	     NDATAinMz_SR      = 0;   
21	     NDATAinMz_CR      = 0;   
22	     NDATA_CR          = 0;          
23	     NDATA_SR          = 0;
24	     NDYEstimatedDATA  = 0;     
25	     NDYEstimatedDATACounting  = 0;     
26	     NDYEstimatedStatErrorDATA = 0.;
27	     NDYEstimatedStatErrorDATACounting = 0.;
28	     NFittedDATA_CR          = 0;            
29	     NDYMCinMzWithScaledPlusContinuum_SR = 0.;
30	     NDYMCinMzWithScaledMinusContinuum_SR = 0.;
31	     
32	     ContinuumInZdata  = 0.; 
33	     DYVerbose  = true;   
34	}
35	
36	DYEstimation::~DYEstimation(){}
37	
38	void DYEstimation::FillNbofEventsInSRandCR (bool isInCR, bool isInSR,float DilM, string datasetname,float w){
39	 
40	    bool isZjetslike = false;
41	    isZjetslike = ( datasetname=="Zjets" || datasetname=="DYToMuMu_M-20" || datasetname=="DYToEE_M-20" || datasetname=="DYToTauTau_M-20" );
42	    if ( isInCR && isZjetslike && (DilM<76 || DilM>106) ) { NDYMC_CR += w; NunnormDYMC_CR +=1;}
43	    if ( isInCR && isZjetslike && (DilM>76 && DilM<106) ) { NDYMCinMz_CR += w; NunnormDYMCinMz_CR += 1;}
44	    if ( isInSR && isZjetslike && (DilM<76 || DilM>106) )  NDYMC_SR += w;
45	    if ( isInSR && isZjetslike && (DilM>76 && DilM<106) )  { NDYMCinMz_SR += w; NunnormDYMCinMz_SR += 1; }
46	    bool isMC = false;
47	    isMC = (datasetname!="DATA" && datasetname!="Data" && 
48	           datasetname!="DataMu" && datasetname!="DATAMu" &&
49	           datasetname!="DataEG" && datasetname!="DATAEG" &&
50	           datasetname!="DataMuEG" && datasetname!="DATAMuEG" );
51	    if ( isInCR && isMC && (DilM<76 || DilM>106) )  NMC_CR += w; 
52	    if ( isInCR && isMC && (DilM>76 && DilM<106) )  NMCinMz_CR += w;
53	    if ( isInSR && isMC && (DilM>76 && DilM<106) )  NMCinMz_SR += w;
54	    bool isd = false;
55	    isd = (datasetname=="DATA" || datasetname=="Data" || 
56	           datasetname=="DataMu" || datasetname=="DATAMu" ||
57	           datasetname=="DataEG" || datasetname=="DATAEG");
58	    if ( isInCR && isd && (DilM<76 || DilM>106) )  NDATA_CR++; 
59	    if ( isInCR && isd && (DilM>76 && DilM<106) )  NDATAinMz_CR++;
60	    if ( isInSR && isd && (DilM<76 || DilM>106) )  NDATA_SR++;
61	    if ( isInSR && isd && (DilM>76 && DilM<106) )  NDATAinMz_SR++;
62	 
63	}
64	
65	std::vector<TH1F*> DYEstimation::CreateDYHisto(string TypeSel, string datasetname, int bin, float min, float max){
66	 
67	    TH1F *h;
68	    if ( datasetname=="TTbar" ) {
69	       h = new TH1F((datasetname+"sig"+TypeSel).c_str(),"",bin,min,max); 
70	//       hlist.push_back(h);
71	       if (TypeSel=="SR" )   hlistSR.push_back(h);
72	       if (TypeSel=="CR" )   hlistCR.push_back(h);
73	       if (TypeSel=="emu" )  hlistemu.push_back(h);
74	       if (TypeSel=="ContCR" )  hlistContCR.push_back(h);
75	       h = new TH1F((datasetname+"bkg"+TypeSel).c_str(),"",bin,min,max); 
76	//       hlist.push_back(h);
77	       if (TypeSel=="SR" )   hlistSR.push_back(h);
78	       if (TypeSel=="CR" )   hlistCR.push_back(h);
79	       if (TypeSel=="emu" )  hlistemu.push_back(h);
80	       if (TypeSel=="ContCR" )  hlistContCR.push_back(h);
81	    } else {
82	       h = new TH1F((datasetname+TypeSel).c_str(),"",bin,min,max); 
83	//       hlist.push_back(h);
84	       if (TypeSel=="SR" )   hlistSR.push_back(h);
85	       if (TypeSel=="CR" )   hlistCR.push_back(h);
86	       if (TypeSel=="emu" )  hlistemu.push_back(h);
87	       bool isZjetslike = false;
88	       isZjetslike = ( datasetname=="Zjets" || datasetname=="DYToMuMu_M-20" || datasetname=="DYToEE_M-20" || datasetname=="DYToTauTau_M-20" );
89	       if (TypeSel=="ContCR" && !isZjetslike )  hlistContCR.push_back(h);
90	    } 
91	    
92	    if ( TypeSel=="emu" ) {
93	       hstack1 = new THStack("hstack1","hstack1");
94	//       all1    = new TH1F("all1","",bin,min,max); 
95	    }   
96	    if ( TypeSel=="SR" ) {
97	       hstackSR = new THStack("hstackSR","hstackSR");
98	//       allSR    = new TH1F("allSR","",bin,min,max); 
99	    }   
100	    if ( TypeSel=="CR" ) {
101	       hstackCR = new THStack("hstackCR","hstackCR");
102	//       allCR    = new TH1F("allCR","",bin,min,max); 
103	    }   
104	    if ( TypeSel=="ContCR" ) {
105	       hstackContCR = new THStack("hstackContCR","hstackContCR");
106	    }   
107	    
108	//    return hlist;
109	    if (TypeSel=="SR") return hlistSR;
110	    if (TypeSel=="CR") return hlistCR;
111	    if (TypeSel=="emu") return hlistemu;
112	    if (TypeSel=="ContCR") return hlistContCR;
113	
114	
115	}
116	
117	
118	void DYEstimation::FillDYHistos(string TypeSel, std::vector<TH1F*>hlist, string datasetname, int tmeme, float DilM, float weight){
119	 
120	   //std::cout<<"tmeme,DilM "<<tmeme<<" "<<DilM<<std::endl;
121	
122	  for (unsigned int i=0; i < hlist.size(); i++)
123	//     cout<<"hlist[i]->GetName() "<<hlist[i]->GetName()<<endl;
124	    {
125	       if ( datasetname=="TTbar" ) {
126	          if (TypeSel == "emu" ) { 
127	             if ( ((tmeme==11)||(tmeme==21100)||(tmeme==11001)||(tmeme==10110)) && string(hlist[i]->GetName())==("TTbarsig"+TypeSel)   ) {
128	                 hlist[i]->Fill(DilM, weight);
129	             }
130	             if ( !((tmeme==11)||(tmeme==21100)||(tmeme==11001)||(tmeme==10110)) && string(hlist[i]->GetName())==("TTbarbkg"+TypeSel)   ) {
131	                 hlist[i]->Fill(DilM, weight);
132	             } 
133	          }// TypeSel == "emu"   
134	          if ( TypeSel == "SR" || TypeSel == "CR" || TypeSel == "ContCR") { 
135	            // for mumu
136	             if ( ((tmeme==20)||(tmeme==11010)||(tmeme==22000)) && string(hlist[i]->GetName())==("TTbarsig"+TypeSel)   ) {
137	                 hlist[i]->Fill(DilM, weight);
138	             }
139	            if ( !((tmeme==20)||(tmeme==11010)||(tmeme==22000)) && string(hlist[i]->GetName())==("TTbarbkg"+TypeSel)   ) {
140	                 hlist[i]->Fill(DilM, weight);
141	             } 
142	            // for ee
143	             if ( ((tmeme==2)||(tmeme==10101)||(tmeme==20200)) && string(hlist[i]->GetName())==("TTbarsig"+TypeSel)   ) {
144	                 hlist[i]->Fill(DilM, weight);
145	             }
146	            if ( !((tmeme==2)||(tmeme==10101)||(tmeme==20200)) && string(hlist[i]->GetName())==("TTbarbkg"+TypeSel)   ) {
147	                 hlist[i]->Fill(DilM, weight);
148	             } 
149	          }// TypeSel == "SR"  || TypeSel == "CR" || TypeSel == "ContCR"
150	       } else {
151	          if ( datasetname=="DATA" || datasetname=="Data" || datasetname=="DataMu" || datasetname=="DataEG") {
152	             if ( datasetname+TypeSel == hlist[i]->GetName()  )  hlist[i]->Fill(DilM, 1 );
153	          }else{
154	             if ( datasetname+TypeSel == hlist[i]->GetName()  )  hlist[i]->Fill(DilM, weight);
155	          }
156	       }  
157	    }
158	 
159	}
160	
161	void DYEstimation::MakeDYHistos(string TypeSel, std::vector<TH1F*>hlist, TH1F* all1){
162	
163	// modifdg
164	// merging of Wlnu->Wjets
165	   int IWCR = -1;
166	   int IWSR = -1;
167	   int IWemu = -1;
168	   int IWContCR = -1;
169	   for (unsigned int j=0; j < hlist.size(); j++) {
170	      if ( hlist[j]->GetName()==string("WjetsCR") ) IWCR = j;
171	      if ( hlist[j]->GetName()==string("WjetsSR") ) IWSR = j;
172	      if ( hlist[j]->GetName()==string("Wjetsemu") ) IWemu = j;
173	      if ( hlist[j]->GetName()==string("WjetsContCR") ) IWContCR = j;
174	   }
175	   for (unsigned int j=0; j < hlist.size(); j++) {
176	          if ( hlist[j]->GetName()==string("WjetsMuCR") || hlist[j]->GetName()==string("WjetsECR") ||
177	          hlist[j]->GetName()==string("WjetsTauCR") ) {  
178	             if ( IWCR>=0 ) hlist[IWCR]->Add(hlist[j]);  
179	          }         
180	          if ( hlist[j]->GetName()==string("WjetsMuSR") || hlist[j]->GetName()==string("WjetsESR") ||
181	          hlist[j]->GetName()==string("WjetsTauSR")) {  
182	             if ( IWSR>=0 ) hlist[IWSR]->Add(hlist[j]);  
183	          }
184	          if ( hlist[j]->GetName()==string("WjetsMuemu") || hlist[j]->GetName()==string("WjetsEemu") ||
185	          hlist[j]->GetName()==string("WjetsTauemu")) {  
186	             if ( IWemu>=0 ) hlist[IWemu]->Add(hlist[j]);  
187	          }          
188	          if ( hlist[j]->GetName()==string("WjetsMuContCR") || hlist[j]->GetName()==string("WjetsEContCR") ||
189	          hlist[j]->GetName()==string("WjetsTauContCR") ) {  
190	             if ( IWContCR>=0 ) hlist[IWContCR]->Add(hlist[j]);  
191	          }         
192	   } 
193	// merging of WW,WZ,ZZ -> VV
194	   int IVVSR = -1;
195	   int IVVCR = -1;
196	   int IVVemu = -1;
197	   int IVVContCR = -1;
198	   for (unsigned int j=0; j < hlist.size(); j++) {
199	      if ( hlist[j]->GetName()==string("VVCR") ) IVVCR = j;
200	      if ( hlist[j]->GetName()==string("VVSR") ) IVVSR = j;
201	      if ( hlist[j]->GetName()==string("VVemu") ) IVVemu = j;
202	      if ( hlist[j]->GetName()==string("VVContCR") ) IVVContCR = j;
203	   }
204	   for (unsigned int j=0; j < hlist.size(); j++) {
205	          if ( hlist[j]->GetName()==string("WWCR") || hlist[j]->GetName()==string("WZCR") ||
206	          hlist[j]->GetName()==string("ZZCR") ) {  
207	             if ( IVVCR>=0 ) hlist[IVVCR]->Add(hlist[j]);  
208	          }         
209	          if ( hlist[j]->GetName()==string("WWSR") || hlist[j]->GetName()==string("WZSR") ||
210	          hlist[j]->GetName()==string("ZZSR") ) {  
211	             if ( IVVSR>=0 ) hlist[IVVSR]->Add(hlist[j]);  
212	          }         
213	          if ( hlist[j]->GetName()==string("WWemu") || hlist[j]->GetName()==string("WZemu") ||
214	          hlist[j]->GetName()==string("ZZemu") ) {  
215	             if ( IVVemu>=0 ) hlist[IVVemu]->Add(hlist[j]);  
216	          }         
217	          if ( hlist[j]->GetName()==string("WWContCR") || hlist[j]->GetName()==string("WZContCR") ||
218	          hlist[j]->GetName()==string("ZZContCR") ) {  
219	             if ( IVVContCR>=0 ) hlist[IVVContCR]->Add(hlist[j]);  
220	          }         
221	   } 
222	   
223	// merging of T (singletoptW), Tbar (singleantitopW)  -> SingleToptW 
224	   int ISingleToptWSR = -1;
225	   int ISingleToptWCR = -1;
226	   int ISingleToptWemu = -1;
227	   int ISingleToptWContCR = -1;
228	   for (unsigned int j=0; j < hlist.size(); j++) {
229	      if ( hlist[j]->GetName()==string("SingleToptWCR") ) ISingleToptWCR = j;
230	      if ( hlist[j]->GetName()==string("SingleToptWSR") ) ISingleToptWSR = j;
231	      if ( hlist[j]->GetName()==string("SingleToptWemu") ) ISingleToptWemu = j;
232	      if ( hlist[j]->GetName()==string("SingleToptWContCR") ) ISingleToptWContCR = j;
233	   }
234	   for (unsigned int j=0; j < hlist.size(); j++) {
235	          if ( hlist[j]->GetName()==string("TtWCR") || hlist[j]->GetName()==string("TbartWCR") ) {  
236	             if ( ISingleToptWCR>=0 ) hlist[ISingleToptWCR]->Add(hlist[j]);  
237	          }         
238	          if ( hlist[j]->GetName()==string("TtWSR") || hlist[j]->GetName()==string("TbartWSR") ) {  
239	             if ( ISingleToptWSR>=0 ) hlist[ISingleToptWSR]->Add(hlist[j]);  
240	          }         
241	          if ( hlist[j]->GetName()==string("TtWemu") || hlist[j]->GetName()==string("TbartWemu") ) {  
242	             if ( IVVemu>=0 ) hlist[IVVemu]->Add(hlist[j]);  
243	          }         
244	          if ( hlist[j]->GetName()==string("TtWContCR") || hlist[j]->GetName()==string("TbartWContCR")  ) {  
245	             if ( ISingleToptWContCR>=0 ) hlist[ISingleToptWContCR]->Add(hlist[j]);  
246	          }         
247	   } 
248	// merging of low DY events -> DY
249	   int IZjSR = -1;
250	   int IZjCR = -1;
251	   int IZjemu = -1;
252	   for (unsigned int j=0; j < hlist.size(); j++) {
253	      if ( hlist[j]->GetName()==string("ZjetsCR") ) IZjCR = j;
254	      if ( hlist[j]->GetName()==string("ZjetsSR") ) IZjSR = j;
255	      if ( hlist[j]->GetName()==string("Zjetsemu") ) IZjemu = j;
256	   }
257	   for (unsigned int j=0; j < hlist.size(); j++) {
258	          if ( hlist[j]->GetName()==string("DYToMuMu_M-20CR") || hlist[j]->GetName()==string("DYToEE_M-20CR") ||
259	          hlist[j]->GetName()==string("DYToTauTau_M-20CR") ) {  
260	             if ( IZjCR>=0 ) hlist[IZjCR]->Add(hlist[j]);  
261	          }         
262	          if ( hlist[j]->GetName()==string("DYToMuMu_M-20SR") || hlist[j]->GetName()==string("DYToEE_M-20SR") ||
263	          hlist[j]->GetName()==string("DYToTauTau_M-20SR") ) {  
264	             if ( IZjSR>=0 ) hlist[IZjSR]->Add(hlist[j]);  
265	          }         
266	          if ( hlist[j]->GetName()==string("DYToMuMu_M-20emu") || hlist[j]->GetName()==string("DYToEE_M-20emu") ||
267	          hlist[j]->GetName()==string("DYToTauTau_M-20emu") ) {  
268	             if ( IZjemu>=0 ) hlist[IZjemu]->Add(hlist[j]);  
269	          }         
270	   } 
271	   
272	// endmodifdg
273	
274	  std::vector<string> ordre; // to be verified
275	//   ordre.push_back("Zjets");
276	//   ordre.push_back("VV");
277	//   ordre.push_back("TTbarsig");
278	//   ordre.push_back("SingleToptW");
279	//   ordre.push_back("Wjets");
280	//   ordre.push_back("TTbarbkg");
281	  ordre.push_back("TTbarbkg");
282	  ordre.push_back("Wjets");
283	  ordre.push_back("SingleToptW");
284	  ordre.push_back("TTbarsig");
285	  ordre.push_back("VV");
286	  ordre.push_back("Zjets");
287	    
288	 
289	  for (unsigned int i=0; i < ordre.size(); i++)
290	    {
291	      for (unsigned int j=0; j < hlist.size(); j++)
292	       {
293	             if ( hlist[j]->GetName()==(ordre[i]+TypeSel) ) {  
294	                hstack1->Add(hlist[j]);        
295	                all1->Add(hlist[j]); 
296	             }  
297	       }  
298	    }
299	 
300	//   return all1;
301	}
302	
303	void DYEstimation::MakeDataHistos(string TypeSel, std::vector<TH1F*>hlist, TH1F* all1){
304	
305	      for (unsigned int j=0; j < hlist.size(); j++)
306	       {
307	          if ( ( hlist[j]->GetName()==("Data"+TypeSel)) || ( hlist[j]->GetName()==("DATA"+TypeSel)) ||
308	               ( hlist[j]->GetName()==("DataMu"+TypeSel)) || ( hlist[j]->GetName()==("DataEG"+TypeSel)) 
309	          ) {  all1->Add(hlist[j]); }
310	       }  
311	
312	}
313	
314	void DYEstimation::ComputeDYEstimatedEvents_MC(){
315	
316	  NDYEstimatedMC = NFittedDYMC_CR * NFittedDYMCinMz_SR / NFittedDYMCinMz_CR;
317	  if ( DYVerbose ) { 
318	     std::cout<<"passage dsComputeDYEstimatedEvents_MC "<<NDYEstimatedMC<<" "<<NFittedDYMC_CR<<" "<<NFittedDYMCinMz_SR<<" "<<NFittedDYMCinMz_CR<<std::endl; 
319	     std::cout<<"passage dsComputeDYEstimatedEvents_MC NMC_CR"<<NMC_CR <<std::endl; 
320	  } 
321	  float stat_uncert = sqrt(( ((NunnormDYMC_CR+NunnormDYMCinMz_CR)/NunnormDYMCinMz_CR)*((NunnormDYMC_CR+NunnormDYMCinMz_CR)/NunnormDYMCinMz_CR) )
322	                                 *(1./(NunnormDYMC_CR))
323	                                 + 1./(NunnormDYMCinMz_SR) );
324	  if ( DYVerbose ) { 
325	     std::cout<<"statistical error on the number of ESTIMATED DY "<<NDYEstimatedMC*stat_uncert<<std::endl; 
326	  }
327	  
328	}
329	
330	float DYEstimation::GetNDYEstimatedMC(){
331	   return  NDYEstimatedMC;
332	 
333	} 
334	
335	
336	float DYEstimation::GetDYEvents_MC(){
337	   return  NDYMC_SR;
338	 
339	} 
340	
341	
342	float DYEstimation::GetDYEstimatedEvents_MC(){
343	   return  NDYEstimatedMC;
344	 
345	} 
346	
347	void DYEstimation::MakeDYFits(){
348	//   MakeDYFits_emu();
349	}
350	
351	vector<double> DYEstimation::MakeDYFits_emu(TH1F* all){
352	  
353	// Fit of the emu continuum   
354	//     double * param;
355	     vector<double> pa;
356	     TF1* myf = new TF1("CF",FitFunctions::Continuum, 10., 350., 6); 
357	     myf->SetParameters(1.7256, -4.19201, .0000207, .849781, 125., 0.005);
358	     myf->SetLineWidth(3);
359	     all->Fit("CF", "M","same");
360	     double I = myf->Integral(76.,106.);
361	//     std::cout<<"Integrale = "<<I <<std::endl;
362	//      myf->GetParameters(param);
363	     for (unsigned int j=0; j<6; ++j){
364	//       param[j] = myf->GetParameter(j);
365	       pa.push_back(myf->GetParameter(j));
366	         if ( DYVerbose ) { 
367	            std::cout<<"parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
368	         }   
369	     }
370	     return pa;
371	}
372	
373	
374	void DYEstimation::MakeDYFits_SR(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
375	  
376	//    int nb = 200;
377	//    float minx = 0.;
378	//    float maxx = 350;
379	// conversion factor for the integral computation
380	    float convfactor = (maxx-minx)/nb;
381	    
382	     vector<double> pa;
383	// Fit of the DY in the Signal Region 
384	     if ( minval!=maxval ) {
385	        int minbin = all->FindBin(minval) - 1 ;
386	        int maxbin = all->FindBin(maxval) + 1 ;
387	        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
388	     }   
389	     TF1* myf = new TF1("BWplusCF",FitFunctions::BreitWignerPlusFixedContinuum, 10., 300., 11); 
390	     if ( minval!=maxval ) {
391	//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
392	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
393	     } else {  
394	//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 100., 10., 0.1);
395	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 1000., 10., 0.1);
396	     }
397	     for (int j=0; j<6; ++j){
398	       myf->SetParLimits(j,param[j],param[j]);
399	     }  
400	
401	
402	     myf->SetLineWidth(3);
403	     all->Fit("BWplusCF", "M","same");
404	     double I = myf->Integral(76.,106.);
405	     I = I/convfactor; 
406	     if ( DYVerbose ) { 
407	        std::cout<<"Integrale totale= "<<I <<std::endl;
408	        std::cout<<"Integrale [0;350]= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
409	     }  
410	
411	     for (unsigned int j=0; j<11; ++j){
412	       pa.push_back(myf->GetParameter(j));
413	       if ( DYVerbose ) { 
414	          std::cout<<"MakeDYFits_SR parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
415	       }          
416	     }
417	
418	     myf->SetParameter(10,0.);
419	     double IBW = myf->Integral(76.,106.);
420	     IBW = IBW/convfactor; 
421	     if ( DYVerbose ) { 
422	        std::cout<<"IBW "<<IBW<<std::endl;
423	        std::cout<<"IntegraleIBW [0;350]= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
424	     }  
425	
426	     myf->SetParameter(10,pa[10]);
427	     myf->SetParameter(9,0.);
428	     
429	     for (unsigned int j=0; j<11; ++j){
430	       if ( DYVerbose ) { 
431	          std::cout<<"MakeDYFits_SR parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
432	       }          
433	     }
434	     
435	     double IContinuum = myf->Integral(76.,106.);
436	     IContinuum = IContinuum/convfactor; 
437	     if ( DYVerbose ) { 
438	        std::cout<<"Icontinuum "<< IContinuum<<std::endl;
439	        std::cout<<"Icontinuum[0;350] "<< myf->Integral(0.,350.)/convfactor<<std::endl;
440	     }  
441	
442	     if ( datatype=="MC" ) { 
443	        SetNFittedDYMCinMz_SR(IBW);
444	        NDYMCinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
445	        NDYMCinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
446	     }  
447	     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
448	        ContinuumInZdata = IContinuum;
449	        SetNFittedDATAinMz_SR(IBW);
450	        NFittedDATAinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
451	        NFittedDATAinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
452	     }
453	    
454	}
455	
456	void DYEstimation::MakeDYFits_SR_asym(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
457	  
458	//    int nb = 200;
459	//    float minx = 0.;
460	//    float maxx = 350;
461	// conversion factor for the integral computation
462	    float convfactor = (maxx-minx)/nb;
463	    
464	     vector<double> pa;
465	// Fit of the DY in the Signal Region 
466	     if ( minval!=maxval ) {
467	        int minbin = all->FindBin(minval) - 1 ;
468	        int maxbin = all->FindBin(maxval) + 1 ;
469	        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
470	     }   
471	     TF1* myf = new TF1("AsymBWplusCF",FitFunctions::AsymBreitWignerPlusFixedContinuum, 10., 300., 13); 
472	     if ( minval!=maxval ) {
473	//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
474	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
475	        myf->SetParameter(11,1.);
476	        myf->SetParameter(12,1.);
477	     } else {  
478	//        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 100., 10., 0.1);
479	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 1000., 10., 0.1);
480	        myf->SetParameter(11,1.);
481	        myf->SetParameter(12,1.);
482	     }
483	     for (int j=0; j<6; ++j){
484	       myf->SetParLimits(j,param[j],param[j]);
485	     }  
486	
487	
488	     myf->SetLineWidth(3);
489	     all->Fit("AsymBWplusCF", "M","same");
490	     double I = myf->Integral(76.,106.);
491	     I = I/convfactor; 
492	     if ( DYVerbose ) { 
493	        std::cout<<"Integrale totale asym= "<<I <<std::endl;
494	        std::cout<<"Integrale [0;350] asym= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
495	     }  
496	
497	     for (unsigned int j=0; j<13; ++j){
498	       pa.push_back(myf->GetParameter(j));
499	       if ( DYVerbose ) { 
500	          std::cout<<"MakeDYFits_SR asym parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
501	       }          
502	     }
503	
504	     myf->SetParameter(10,0.);
505	     double IBW = myf->Integral(76.,106.);
506	     IBW = IBW/convfactor; 
507	     if ( DYVerbose ) { 
508	        std::cout<<"IBW asym "<<IBW<<std::endl;
509	        std::cout<<"IntegraleIBW asym [0;350]= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
510	     }  
511	
512	     myf->SetParameter(10,pa[10]);
513	     myf->SetParameter(9,0.);
514	     
515	     for (unsigned int j=0; j<13; ++j){
516	       if ( DYVerbose ) { 
517	          std::cout<<"MakeDYFits_SR asym parametre["<<j<<"] pour calcul contimuum "<< myf->GetParameter(j)<<std::endl;
518	       }  
519	     }
520	     
521	     double IContinuum = myf->Integral(76.,106.);
522	     IContinuum = IContinuum/convfactor; 
523	     if ( DYVerbose ) { 
524	        std::cout<<"Icontinuum asym "<< IContinuum<<std::endl;
525	        std::cout<<"Icontinuum asym[0;350] "<< myf->Integral(0.,350.)/convfactor<<std::endl;
526	     }  
527	
528	     if ( datatype=="MC" ) { 
529	        SetNFittedDYMCinMz_SR(IBW);
530	        NDYMCinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
531	        NDYMCinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
532	     }  
533	     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
534	        ContinuumInZdata = IContinuum;
535	        SetNFittedDATAinMz_SR(IBW);
536	        NFittedDATAinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
537	        NFittedDATAinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
538	     }
539	    
540	}
541	
542	void DYEstimation::MakeDYFits_CR(TH1F* all,int nb, float minx, float maxx, double minval, double maxval,string datatype){
543	  
544	//    int nb = 200;
545	//    float minx = 0;
546	//    float maxx = 350;
547	// conversion factor for the integral computation
548	    float convfactor = (maxx-minx)/nb;
549	    
550	// Fit of the DY in the Control Region 
551	     if ( minval!=maxval ) {
552	        // for zoom in the peak mass
553	        int minbin = all->FindBin(minval) - 1 ;
554	        int maxbin = all->FindBin(maxval) + 1 ;
555	        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
556	     }   
557	//     TF1* myf = new TF1("BWF",FitFunctions::BreitWigner, 10., 350., 3); 
558	     TF1* myf = new TF1("BWF",FitFunctions::BreitWigner, 70., 150., 3); 
559	     myf->SetNpx(1000);
560	//     myf->SetParameters(3., 90., 100.);  //  ok   pour mumu
561	     myf->SetParameters(5., 88., 100.);  // semble meilleur pour ee (shape deformee)
562	     myf->SetLineWidth(3);
563	     all->Fit("BWF", "RM","same");
564	     double I = myf->Integral(76.,106.);
565	     I = I/convfactor; 
566	     double Itot = myf->Integral(20.,350.);
567	     Itot = Itot/convfactor; 
568	     
569	     if ( DYVerbose ) { 
570	        std::cout<<"Integrale BreitWigner= "<<I <<std::endl;
571	        std::cout<<"Integrale BreitWigner[20;350]= "<<Itot <<std::endl;
572	     }  
573	
574	//          if ( NormalizedToZpeakPosition ) {
575	//            IntMz_LowMet = myf->Eval(myf->GetParameter(1));
576	//      } else {
577	//            IntMz_LowMet = I;
578	//      }   
579	
580	     if ( datatype=="MC" )  {SetNFittedDYMCinMz_CR(I) ;SetNFittedDYMC_CR(Itot-I) ;}
581	     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") {SetNFittedDATAinMz_CR(I); NFittedDATA_CR = Itot-I;}
582	   
583	}
584	
585	void DYEstimation::MakeDYFits_CR_asym_plusCE(TH1F* all,int nb, float minx, float maxx, double minval, double maxval,string datatype){
586	  
587	//    int nb = 200;
588	//    float minx = 0;
589	//    float maxx = 350;
590	// conversion factor for the integral computation
591	    float convfactor = (maxx-minx)/nb;
592	    
593	// Fit of the DY in the Control Region 
594	     if ( minval!=maxval ) {
595	        // for zoom in the peak mass
596	        int minbin = all->FindBin(minval) - 1 ;
597	        int maxbin = all->FindBin(maxval) + 1 ;
598	        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
599	     }   
600	     TF1* myf = new TF1("BWFPlusCE",FitFunctions::AsymBreitWignerPlusCutEffect, 10., 300., 10); 
601	//       myf->SetParameter(0,2.6);
602	//       myf->SetParameter(1,90.);
603	//       myf->SetParameter(2,30000.);
604	//       myf->SetParameter(3,3.);
605	//       myf->SetParameter(4,0.);
606	//           myf->SetParLimits(4,0.,0.);
607	//       myf->SetParameter(5,1.);
608	//       myf->SetParameter(6,1.);
609	// //          myf->SetParLimits(5,1.,1.);
610	// //          myf->SetParLimits(6,1.,1.);
611	//       myf->SetParameter(7,10.);
612	//       myf->SetParameter(8,40.);
613	//       myf->SetParameter(9,25.);
614	      myf->SetParameter(0,2.6);
615	      myf->SetParameter(1,90.);
616	      myf->SetParameter(2,30000.);
617	      myf->SetParameter(3,3.);
618	      myf->SetParameter(4,0.);
619	          myf->SetParLimits(4,0.,0.);
620	      myf->SetParameter(5,1.);
621	      myf->SetParameter(6,1.);
622	//          myf->SetParLimits(5,1.,1.);
623	//          myf->SetParLimits(6,1.,1.);
624	      myf->SetParameter(7,50.);
625	      myf->SetParameter(8,40.);
626	      myf->SetParameter(9,25.);
627	
628	
629	     myf->SetLineWidth(3);
630	     all->Fit("BWFPlusCE", "RM","same");
631	     double I = myf->Integral(76.,106.);
632	     I = I/convfactor; 
633	     double Itot = myf->Integral(0.,350.);
634	     Itot = Itot/convfactor; 
635	     
636	     if ( DYVerbose ) { 
637	        std::cout<<"Integrale BreitWigner asym plus CE = "<<I <<std::endl;
638	        std::cout<<"Integrale BreitWigner[0;350] asym plus CE = "<<Itot <<std::endl;
639	     }  
640	
641	
642	     if ( datatype=="MC" )  {
643	        SetNFittedDYMCinMz_CR(I) ;
644	        SetNFittedDYMC_CR(Itot-I) ;
645	     }
646	     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") {
647	        SetNFittedDATAinMz_CR(I); 
648	        NFittedDATA_CR = Itot-I;
649	     }
650	   
651	}
652	
653	void DYEstimation::MakeDYFits_CR_WithBkgSubtraction(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
654	  
655	//    int nb = 200;
656	//    float minx = 0.;
657	//    float maxx = 350;
658	// conversion factor for the integral computation
659	    float convfactor = (maxx-minx)/nb;
660	    
661	     vector<double> pa;
662	// Fit of the DY in the Control Region 
663	     if ( minval!=maxval ) {
664	        int minbin = all->FindBin(minval) - 1 ;
665	        int maxbin = all->FindBin(maxval) + 1 ;
666	        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
667	     }   
668	     TF1* myf = new TF1("BWplusCF",FitFunctions::BreitWignerPlusFixedContinuum, 10., 300., 11); 
669	     if ( minval!=maxval ) {
670	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
671	     } else {  
672	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 10., 90., 100., 10., 0.1);
673	     }
674	     for (int j=0; j<6; ++j){
675	       myf->SetParLimits(j,param[j],param[j]);
676	     }  
677	
678	
679	     myf->SetLineWidth(3);
680	     all->Fit("BWplusCF", "M","same");
681	     double I = myf->Integral(76.,106.);
682	     double Itot = myf->Integral(0.,350.);
683	     int numbin66 = all->FindBin(66.);
684	     double N66 = all->Integral(0.,numbin66);
685	     I = I/convfactor; 
686	     Itot = Itot/convfactor; 
687	     if ( DYVerbose ) { 
688	        std::cout<<"Integrale totale CR = "<<I <<std::endl;
689	        std::cout<<"Integrale [0;350] CR = "<< myf->Integral(0.,350.)/convfactor<<std::endl;
690	        std::cout<<"N66 CR= "<<N66 <<std::endl;
691	     }  
692	
693	     for (unsigned int j=0; j<11; ++j){
694	       pa.push_back(myf->GetParameter(j));
695	       if ( DYVerbose ) { 
696	          std::cout<<"MakeDYFits_CR parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
697	       }          
698	     }
699	
700	     myf->SetParameter(10,0.);
701	     double IBW = myf->Integral(76.,106.);
702	     double IBW66 = myf->Integral(66.,350.);
703	     double ItotIBW = myf->Integral(0.,350.);
704	     IBW = IBW/convfactor; 
705	     ItotIBW = ItotIBW/convfactor; 
706	     IBW66 = IBW66/convfactor; 
707	     if ( DYVerbose ) { 
708	        std::cout<<"IBW CR "<<IBW<<std::endl;
709	        std::cout<<"IntegraleIBW [0;350] CR= "<< ItotIBW <<std::endl;
710	        std::cout<<"IntegraleIBW [66;350] CR = "<< IBW66 <<std::endl;
711	     }  
712	
713	     myf->SetParameter(10,pa[10]);
714	     myf->SetParameter(9,0.);
715	     
716	     for (unsigned int j=0; j<11; ++j){
717	       if ( DYVerbose ) { 
718	          std::cout<<"MakeDYFits_CR parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
719	        }  
720	     }
721	     
722	     double IContinuum = myf->Integral(76.,106.);
723	     double IContinuum66 = myf->Integral(0.,66.);
724	     IContinuum = IContinuum/convfactor; 
725	     IContinuum66 = IContinuum66/convfactor; 
726	     if ( DYVerbose ) { 
727	        std::cout<<"Icontinuum CR "<< IContinuum<<std::endl;
728	        std::cout<<"Icontinuum[0;350] CR "<< myf->Integral(0.,350.)/convfactor<<std::endl;
729	        std::cout<<"Icontinuum[0;66] CR  "<< IContinuum66<<std::endl;
730	     }  
731	
732	
733	     if ( datatype=="MC" ) { 
734	        SetNFittedDYMCinMz_CR(IBW) ;
735	//        SetNFittedDYMC_CR(ItotIBW-IBW) ;
736	        SetNFittedDYMC_CR(IBW66-IBW+ N66-IContinuum66) ;
737	     }  
738	     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
739	         SetNFittedDATAinMz_CR(IBW);
740	//       NFittedDATA_CR = ItotIBW-IBW;
741	         NFittedDATA_CR = IBW66-IBW+ N66-IContinuum66;
742	         if ( DYVerbose ) { 
743	            std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
744	         }   
745	     }
746	    
747	}
748	
749	// void DYEstimation::MakeDYFits_CR_WithBkgSubtraction(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
750	//   
751	// //    int nb = 200;
752	// //    float minx = 0.;
753	// //    float maxx = 350;
754	// // conversion factor for the integral computation
755	//     float convfactor = (maxx-minx)/nb;
756	//     
757	//      vector<double> pa;
758	// // Fit of the DY in the Control Region 
759	//      if ( minval!=maxval ) {
760	//         int minbin = all->FindBin(minval) - 1 ;
761	//         int maxbin = all->FindBin(maxval) + 1 ;
762	//         all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
763	//      }   
764	//      TF1* myf = new TF1("BWplusCFplusCE",FitFunctions::BreitWignerPlusFixedContinuumPlusCutEffect, 10., 300., 14); 
765	//      if ( minval!=maxval ) {
766	//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 5., 90., 100., 1., 0.1);
767	//         myf->SetParameter(11,40.);
768	//         myf->SetParameter(12,15.);
769	//         myf->SetParameter(13,300.);
770	//      } else {  
771	//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 5., 90., 1000., 60., 0.1);
772	//         myf->SetParameter(11,40.);
773	//         myf->SetParameter(12,15.);
774	//         myf->SetParameter(13,300.);
775	//      }
776	//      for (int j=0; j<6; ++j){
777	//        myf->SetParLimits(j,param[j],param[j]);
778	//      }       
779	//      myf->SetParLimits(10,0.,99999999.);
780	// 
781	// 
782	//      myf->SetLineWidth(3);
783	//      all->Fit("BWplusCFplusCE", "M","same");
784	//      double I = myf->Integral(76.,106.);
785	//      double Itot = myf->Integral(0.,350.);
786	//      I = I/convfactor; 
787	//      Itot = Itot/convfactor; 
788	//      std::cout<<"Integrale totale CR= "<<I <<std::endl;
789	//      std::cout<<"Integrale [0;350] CR= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
790	// 
791	//      for (unsigned int j=0; j<14; ++j){
792	//        pa.push_back(myf->GetParameter(j));
793	//        std::cout<<"MakeDYFits_CR parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
794	//      }
795	// 
796	//      myf->SetParameter(10,0.);
797	//      double IBW = myf->Integral(76.,106.);
798	//      double ItotIBW = myf->Integral(0.,350.);
799	//      IBW = IBW/convfactor; 
800	//      ItotIBW = ItotIBW/convfactor; 
801	//      std::cout<<"IBW CR "<<IBW<<std::endl;
802	//      std::cout<<"IntegraleIBW [0;350] CR= "<< ItotIBW <<std::endl;
803	// 
804	//      myf->SetParameter(10,pa[10]);
805	//      myf->SetParameter(9,0.);
806	//      myf->SetParameter(13,0.);
807	//      
808	//      for (unsigned int j=0; j<14; ++j){
809	//        std::cout<<"MakeDYFits_CR parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
810	//      }
811	//      
812	//      double IContinuum = myf->Integral(76.,106.);
813	//      IContinuum = IContinuum/convfactor; 
814	//      std::cout<<"Icontinuum CR "<< IContinuum<<std::endl;
815	//      std::cout<<"Icontinuum[0;350] CR "<< myf->Integral(0.,350.)/convfactor<<std::endl;
816	// 
817	// 
818	//      if ( datatype=="MC" ) { 
819	//         SetNFittedDYMCinMz_CR(IBW) ;
820	//         SetNFittedDYMC_CR(ItotIBW-IBW) ;
821	//      }       
822	//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
823	//          SetNFittedDATAinMz_CR(IBW);
824	//       NFittedDATA_CR = ItotIBW-IBW;
825	//       std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
826	//      }
827	//     
828	// }
829	
830	void DYEstimation::MakeDYFits_CR_WithBkgSubtraction_asym(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
831	  
832	//    int nb = 200;
833	//    float minx = 0.;
834	//    float maxx = 350;
835	// conversion factor for the integral computation
836	    float convfactor = (maxx-minx)/nb;
837	    
838	     vector<double> pa;
839	// Fit of the DY in the Control Region 
840	     if ( minval!=maxval ) {
841	        int minbin = all->FindBin(minval) - 1 ;
842	        int maxbin = all->FindBin(maxval) + 1 ;
843	        all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
844	     }   
845	     TF1* myf = new TF1("AsymBWplusCF",FitFunctions::AsymBreitWignerPlusFixedContinuum, 10., 300., 13); 
846	     if ( minval!=maxval ) {
847	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
848	        myf->SetParameter(11,0.1);
849	        myf->SetParameter(12,0.1);
850	     } else {  
851	        myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 1000., 1., 0.1);
852	        myf->SetParameter(11,0.1);
853	        myf->SetParameter(12,0.1);
854	     }
855	     for (int j=0; j<6; ++j){
856	       myf->SetParLimits(j,param[j],param[j]);
857	     }  
858	
859	
860	     myf->SetLineWidth(3);
861	     all->Fit("AsymBWplusCF", "M","same");
862	     double I = myf->Integral(76.,106.);
863	     double Itot = myf->Integral(0.,350.);
864	     int numbin66 = all->FindBin(66.);
865	     double N66 = all->Integral(0.,numbin66);
866	     I = I/convfactor; 
867	     Itot = Itot/convfactor; 
868	     if ( DYVerbose ) { 
869	        std::cout<<"Integrale totale CR asym= "<<I <<std::endl;
870	        std::cout<<"Integrale [0;350] CR asym= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
871	        std::cout<<"N66 CR= "<<N66 <<std::endl;
872	     }  
873	
874	     for (unsigned int j=0; j<13; ++j){
875	       pa.push_back(myf->GetParameter(j));
876	       if ( DYVerbose ) { 
877	          std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
878	       }          
879	     }
880	
881	     myf->SetParameter(10,0.);
882	     double IBW = myf->Integral(76.,106.);
883	     double IBW66 = myf->Integral(66.,350.);
884	     double ItotIBW = myf->Integral(0.,350.);
885	     IBW = IBW/convfactor; 
886	     ItotIBW = ItotIBW/convfactor; 
887	     IBW66 = IBW66/convfactor; 
888	     if ( DYVerbose ) { 
889	        std::cout<<"IBW CR asym "<<IBW<<std::endl;
890	        std::cout<<"IntegraleIBW [0;350] CR asym= "<< ItotIBW <<std::endl;
891	        std::cout<<"IntegraleIBW [66;350] CR asym= "<< IBW66 <<std::endl;
892	     }  
893	
894	     myf->SetParameter(10,pa[10]);
895	     myf->SetParameter(9,0.);
896	     
897	     for (unsigned int j=0; j<13; ++j){
898	        if ( DYVerbose ) { 
899	          std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
900	        }  
901	     }
902	     
903	     double IContinuum = myf->Integral(76.,106.);
904	     double IContinuum66 = myf->Integral(0.,66.);
905	     IContinuum = IContinuum/convfactor; 
906	     IContinuum66 = IContinuum66/convfactor; 
907	     if ( DYVerbose ) { 
908	        std::cout<<"Icontinuum CR asym  "<< IContinuum<<std::endl;
909	        std::cout<<"Icontinuum[0;350] CR asym "<< myf->Integral(0.,350.)/convfactor<<std::endl;
910	        std::cout<<"Icontinuum[0;66] CR asym "<< IContinuum66<<std::endl;
911	     }  
912	
913	
914	     if ( datatype=="MC" ) { 
915	        SetNFittedDYMCinMz_CR(IBW) ;
916	//        SetNFittedDYMC_CR(ItotIBW-IBW) ;
917	        SetNFittedDYMC_CR(IBW66-IBW+ N66-IContinuum66) ;
918	     }  
919	     if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
920	         SetNFittedDATAinMz_CR(IBW);
921	//       NFittedDATA_CR = ItotIBW-IBW;
922	         NFittedDATA_CR = IBW66-IBW+ N66-IContinuum66;
923	         if ( DYVerbose ) { 
924	            std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
925	         }   
926	     }
927	    
928	}
929	 
930	// void DYEstimation::MakeDYFits_CR_WithBkgSubtraction_asym(TH1F* all, int nb, float minx ,float maxx,  vector<double> param, double minval, double maxval,string datatype, float FracBkgd){
931	//   
932	// //    int nb = 200;
933	// //    float minx = 0.;
934	// //    float maxx = 350;
935	// // conversion factor for the integral computation
936	//     float convfactor = (maxx-minx)/nb;
937	//     
938	//      vector<double> pa;
939	// // Fit of the DY in the Control Region 
940	//      if ( minval!=maxval ) {
941	//         int minbin = all->FindBin(minval) - 1 ;
942	//         int maxbin = all->FindBin(maxval) + 1 ;
943	//         all->GetXaxis()->SetRange(minbin,maxbin); // en numero de bins (66,91)
944	//      }   
945	//      TF1* myf = new TF1("AsymBWplusCFplusCE",FitFunctions::AsymBreitWignerPlusFixedContinuumPlusCutEffect, 10., 300., 16); 
946	//      if ( minval!=maxval ) {
947	//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
948	//         myf->SetParameter(11,0.1);
949	//         myf->SetParameter(12,0.1);
950	//         myf->SetParameter(13,66.);
951	//         myf->SetParameter(14,20.);
952	//         myf->SetParameter(15,200.);
953	//      } else {  
954	//         myf->SetParameters(param[0], param[1], param[2] ,param[3] ,param[4] , param[5], 1., 90., 100., 1., 0.1);
955	//         myf->SetParameter(11,0.1);
956	//         myf->SetParameter(12,0.1);
957	//         myf->SetParameter(13,66.);
958	//         myf->SetParameter(14,20.);
959	//         myf->SetParameter(15,200.);
960	//      }
961	//      for (int j=0; j<6; ++j){
962	//        myf->SetParLimits(j,param[j],param[j]);
963	//      }       
964	//      myf->SetParLimits(10,0.,99999999.);
965	// 
966	// 
967	//      myf->SetLineWidth(3);
968	//      all->Fit("AsymBWplusCFplusCE", "M","same");
969	//      double I = myf->Integral(76.,106.);
970	//      double Itot = myf->Integral(0.,350.);
971	//      int numbin66 = all->FindBin(66.);
972	//      double N66 = all->Integral(0.,numbin66);
973	//      I = I/convfactor; 
974	//      Itot = Itot/convfactor; 
975	//      std::cout<<"Integrale totale CR asym= "<<I <<std::endl;
976	//      std::cout<<"Integrale [0;350] CR asym= "<< myf->Integral(0.,350.)/convfactor<<std::endl;
977	//      std::cout<<"N66 CR= "<<N66 <<std::endl;
978	// 
979	//      for (unsigned int j=0; j<16; ++j){
980	//        pa.push_back(myf->GetParameter(j));
981	//        std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
982	//      }
983	// 
984	//      myf->SetParameter(10,0.);
985	//      double IBW = myf->Integral(76.,106.);
986	//      double IBW66 = myf->Integral(66.,350.);
987	//      double ItotIBW = myf->Integral(0.,350.);
988	//      IBW = IBW/convfactor; 
989	//      ItotIBW = ItotIBW/convfactor; 
990	//      IBW66 = IBW66/convfactor; 
991	//      std::cout<<"IBW CR asym "<<IBW<<std::endl;
992	//      std::cout<<"IntegraleIBW [0;350] CR asym= "<< ItotIBW <<std::endl;
993	//      std::cout<<"IntegraleIBW [66;350] CR asym= "<< IBW66 <<std::endl;
994	// 
995	//      myf->SetParameter(10,pa[10]);
996	//      myf->SetParameter(9,0.);
997	//      myf->SetParameter(15,0.);
998	//      
999	//      for (unsigned int j=0; j<16; ++j){
1000	//        std::cout<<"MakeDYFits_CR asym  parametre["<<j<<"] pour calcule contimuum "<< myf->GetParameter(j)<<std::endl;
1001	//      }
1002	//      
1003	//      double IContinuum = myf->Integral(76.,106.);
1004	//      double IContinuum66 = myf->Integral(0.,66.);
1005	//      IContinuum = IContinuum/convfactor; 
1006	//      IContinuum66 = IContinuum66/convfactor; 
1007	//      std::cout<<"Icontinuum CR asym  "<< IContinuum<<std::endl;
1008	//      std::cout<<"Icontinuum[0;350] CR asym "<< myf->Integral(0.,350.)/convfactor<<std::endl;
1009	//      std::cout<<"Icontinuum[0;66] CR asym "<< IContinuum66<<std::endl;
1010	// 
1011	// 
1012	//      if ( datatype=="MC" ) { 
1013	//         SetNFittedDYMCinMz_CR(IBW) ;
1014	//         SetNFittedDYMC_CR(ItotIBW-IBW) ;
1015	// //        SetNFittedDYMC_CR(IBW66-IBW+ N66-IContinuum66) ;
1016	//      }       
1017	//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
1018	//          SetNFittedDATAinMz_CR(IBW);
1019	//       NFittedDATA_CR = ItotIBW-IBW;
1020	// //    NFittedDATA_CR = IBW66-IBW+ N66-IContinuum66;
1021	//       std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
1022	//      }
1023	//     
1024	// }
1025	
1026	void DYEstimation::SetNFittedDYMCinMz_CR( double intval){
1027	   NFittedDYMCinMz_CR = float(intval);
1028	}
1029	
1030	void DYEstimation::SetNFittedDYMC_CR( double intval){
1031	   NFittedDYMC_CR = float(intval);
1032	}
1033	
1034	void DYEstimation::SetNFittedDATAinMz_CR( double intval){
1035	  NFittedDATAinMz_CR  = float(intval);
1036	}
1037	
1038	float DYEstimation::GetNDYMCinMz_CR(){
1039	   return NDYMCinMz_CR ;
1040	}
1041	
1042	float DYEstimation::GetNFittedDYMCinMz_CR(){
1043	   return NFittedDYMCinMz_CR ;
1044	}
1045	
1046	float DYEstimation::GetNDYMC_CR(){
1047	   return NDYMC_CR ;
1048	}
1049	
1050	float DYEstimation::GetNFittedDYMC_CR(){
1051	   return NFittedDYMC_CR ;
1052	}
1053	
1054	float DYEstimation::GetNDYMC_SR(){
1055	   return NDYMC_SR ;
1056	}
1057	
1058	void DYEstimation::SetDYVerbose( bool v){
1059	  DYVerbose  = v;
1060	}
1061	
1062	void DYEstimation::SetNFittedDYMCinMz_SR( double intval){
1063	   NFittedDYMCinMz_SR = float(intval);
1064	}
1065	
1066	void DYEstimation::SetNFittedDATAinMz_SR( double intval){
1067	   NFittedDATAinMz_SR = float(intval);
1068	}
1069	
1070	float DYEstimation::GetNDYMCinMz_SR(){
1071	   return NDYMCinMz_SR ;
1072	}
1073	
1074	float DYEstimation::GetNFittedDYMCinMz_SR(){
1075	   return NFittedDYMCinMz_SR ;
1076	}
1077	
1078	float DYEstimation::GetNDATAinMz_SR(){
1079	   return  NDATAinMz_SR;
1080	}
1081	
1082	float DYEstimation::GetNDATAinMz_CR(){
1083	   return  NDATAinMz_CR;
1084	}
1085	
1086	float DYEstimation::GetNDATA_CR(){
1087	   return  NDATA_CR;
1088	}
1089	
1090	float DYEstimation::GetNDATA_BkgSubtracted_CR(){
1091	//   return  NDATA_CR * (NDYMC_CR/NMC_CR);  // not very good
1092	   return  NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR );
1093	}
1094	
1095	float DYEstimation::GetNFittedDATA_CR(){
1096	   return  NFittedDATA_CR;
1097	}
1098	
1099	float DYEstimation::GetNFittedDATAinMz_SR(){
1100	   return  NFittedDATAinMz_SR;
1101	}
1102	
1103	float DYEstimation::GetNFittedDATAinMz_CR(){
1104	   return  NFittedDATAinMz_CR;
1105	}
1106	
1107	float DYEstimation::GetNDATA_SR(){
1108	   return  NDATA_SR;
1109	}
1110	
1111	float DYEstimation::GetContinuumInZdata(){
1112	   return  ContinuumInZdata;
1113	}
1114	
1115	void DYEstimation::ComputeDYEstimatedEvents_DATA(string typecomputation,string useMCorDATAforScaleFactor ){
1116	
1117	  if ( typecomputation=="Counting" ) {
1118	     if ( NDATAinMz_SR-ContinuumInZdata >0.) {
1119	//        NDYEstimatedDATACounting = NDATA_CR * (NDYMC_CR/NMC_CR) * (NDATAinMz_SR-ContinuumInZdata) / NDATAinMz_CR;
1120	        NDYEstimatedDATACounting = GetNDATA_BkgSubtracted_CR() * (NDATAinMz_SR-ContinuumInZdata) / NDATAinMz_CR;
1121	     } else {
1122	//        NDYEstimatedDATACounting = NDATA_CR * (NDYMC_CR/NMC_CR) * NDATAinMz_SR / NDATAinMz_CR;
1123	        NDYEstimatedDATACounting = GetNDATA_BkgSubtracted_CR() * NDATAinMz_SR / NDATAinMz_CR;
1124	     }
1125	     if ( DYVerbose ) { 
1126	        std::cout<<"passage Counting ComputeDYEstimatedEvents_DATA "<< NDYEstimatedDATACounting<<" "<< NDATA_CR<<" "<< NDATAinMz_SR<<" "<<ContinuumInZdata<<" "<<NDATAinMz_CR<<std::endl; 
1127	     } 
1128	     if ( useMCorDATAforScaleFactor == "MC" ) {
1129	        if ( NDYMCinMz_SR<=0 ) std::cout<<"Problem with NDYMCinMz_SR==0 "<<std::endl;
1130	        if ( NDYMCinMz_CR<=0 ) std::cout<<"Problem with NDYMCinMz_CR==0 "<<std::endl;
1131	//        NDYEstimatedDATACounting = NDATA_CR * (NDYMC_CR/NMC_CR) * NDYMCinMz_SR / NDYMCinMz_CR;
1132	        NDYEstimatedDATACounting = GetNDATA_BkgSubtracted_CR() * NDYMCinMz_SR / NDYMCinMz_CR;
1133	     }
1134	  }
1135	
1136	  if ( typecomputation=="Fit" ) {
1137	     NDYEstimatedDATA = NFittedDATA_CR * NFittedDATAinMz_SR / NFittedDATAinMz_CR;
1138	     if ( DYVerbose ) { 
1139	        std::cout<<"passage Fit ComputeDYEstimatedEvents_DATA "<< NDYEstimatedDATA<<" "<<NFittedDATA_CR <<" "<< NFittedDATAinMz_SR<<" "<<NFittedDATAinMz_CR<<std::endl; 
1140	     }
1141	     if ( useMCorDATAforScaleFactor == "MC" ) {
1142	        if ( NDYMCinMz_SR<=0 ) std::cout<<"Problem with NDYMCinMz_SR==0 "<<std::endl;
1143	        if ( NDYMCinMz_CR<=0 ) std::cout<<"Problem with NDYMCinMz_CR==0 "<<std::endl;
1144	        NDYEstimatedDATA = NFittedDATA_CR * NDYMCinMz_SR / NDYMCinMz_CR;
1145	//        NDYEstimatedDATA = NDATA_CR * NDYMCinMz_SR / NDYMCinMz_CR;
1146	     }
1147	  }
1148	
1149	  double stat_uncert_data = 0;
1150	  double stat_uncert_data_counting = 0;
1151	  if ( typecomputation=="Counting" ) {
1152	    if ( NDATAinMz_SR-ContinuumInZdata> 0 ) {
1153	//        stat_uncert_data_counting = (NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR*1./sqrt(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR ))
1154	//                                  + 1./sqrt(NDATAinMz_SR-ContinuumInZdata);
1155	       stat_uncert_data_counting = sqrt( ( ((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR)*((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR))
1156	                                 * (1./(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )))
1157	                                 + 1./(NDATAinMz_SR-ContinuumInZdata) );
1158	     } else {
1159	//        stat_uncert_data_counting = (NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR*1./sqrt(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR ))
1160	//                                  + 1./sqrt(NDATAinMz_SR);
1161	       stat_uncert_data_counting = sqrt( ( ((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR)*((NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )+NDATAinMz_CR)/NDATAinMz_CR) )
1162	                                 * (1./(NDATAinMz_CR * (NDYMC_CR /NDYMCinMz_CR )))
1163	                                 + 1./(NDATAinMz_SR) );
1164	     }                        
1165	  }                           
1166	// Not the right computation but a good approximation:
1167	  if ( typecomputation=="Fit" ) {
1168	//     stat_uncert_data = (NFittedDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR*1./sqrt(NFittedDATA_CR)
1169	//                               + 1./sqrt(NFittedDATAinMz_SR);
1170	// //     stat_uncert_data = (NDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR*1./sqrt(NDATA_CR)
1171	// //                               + 1./sqrt(NFittedDATAinMz_SR);
1172	    stat_uncert_data = sqrt( ( ((NFittedDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR)*((NFittedDATA_CR+NFittedDATAinMz_CR)/NFittedDATAinMz_CR) )
1173	                              * (1./(NFittedDATA_CR))
1174	                              + 1./(NFittedDATAinMz_SR) );
1175	  }                           
1176	
1177	// Systematical uncertainty related to the difference of the shape in the Control and Signal Regions
1178	// This estimated with DY MC 
1179	// (code /opt/sbg/data/data1/cms/gele/PAT/423/V2/CMSSW_4_2_3/src/MiniTreeAnalysis/NTupleAnalysis/macros/results_v1/test.C)
1180	//  by comparing the absolute difference of the normalized shape in the
1181	// SR and CR region: found to be 11% (mumu) and 4% (ee) -> mean = 8%
1182	  double sys_shape = 0.08;    
1183	    
1184	// Systematical uncertainty related to the presence of residual background
1185	  double sys_bckg = 0.;
1186	  if ( typecomputation=="Fit" ) {
1187	    sys_bckg = fabs(( NFittedDATAinMzWithScaledMinusContinuum_SR - NFittedDATAinMz_SR )/NFittedDATAinMz_SR);
1188	    if ( sys_bckg>fabs(( NFittedDATAinMzWithScaledPlusContinuum_SR - NFittedDATAinMz_SR )/NFittedDATAinMz_SR) ) sys_bckg = fabs(( NFittedDATAinMzWithScaledPlusContinuum_SR - NFittedDATAinMz_SR )/NFittedDATAinMz_SR);
1189	    std::cout<<"Relative Sys_Bckg "<<sys_bckg<<std::endl;
1190	  }
1191	
1192	
1193	  double sys_tot_1 = 0.;
1194	  sys_tot_1 = sys_shape + sys_bckg;
1195	
1196	    
1197	  NDYEstimatedStatErrorDATA = NDYEstimatedDATA * stat_uncert_data;
1198	  NDYEstimatedSysErrorDATA  = NDYEstimatedDATA * sys_tot_1;
1199	  NDYEstimatedStatErrorDATACounting = NDYEstimatedDATACounting * stat_uncert_data_counting;
1200	  NDYEstimatedSysErrorDATACounting  = NDYEstimatedDATACounting * sys_tot_1;
1201	  
1202	}
1203	
1204	float DYEstimation::GetNDYEstimatedDATA(){
1205	   return  NDYEstimatedDATA;
1206	 
1207	} 
1208	
1209	float DYEstimation::GetNDYEstimatedStatErrorDATA(){
1210	   return  NDYEstimatedStatErrorDATA;
1211	 
1212	} 
1213	
1214	float DYEstimation::GetNDYEstimatedDATACounting(){
1215	   return  NDYEstimatedDATACounting;
1216	 
1217	} 
1218	
1219	float DYEstimation::GetNDYEstimatedStatErrorDATACounting(){
1220	   return  NDYEstimatedStatErrorDATACounting;
1221	 
1222	} 
1223	
1224	float DYEstimation::GetNDYEstimatedSysErrorDATA(){
1225	   return  NDYEstimatedSysErrorDATA;
1226	 
1227	} 
1228	float DYEstimation::GetNDYEstimatedSysErrorDATACounting(){
1229	   return  NDYEstimatedSysErrorDATACounting;
1230	 
1231	} 
1232	
