1	#include "../interface/MMEstimation.h"
2	                
3	MMEstimation::MMEstimation(vector<Dataset> datasets, float isoLoose, float isoTight, unsigned int n_bins, float min, float max, string Channel){
4	
5	      theNBins = n_bins;
6	      theMinBin = min;
7	      theMaxBin = max;
8	
9	      theIsolations.iso1[0] = isoLoose;
10	      theIsolations.iso1[1] = isoTight;
11	      theIsolations.iso1[2] = isoTight;
12	      theIsolations.iso2[0] = isoLoose;
13	      theIsolations.iso2[1] = isoLoose;
14	      theIsolations.iso2[2] = isoTight;
15	
16	      theChannel = Channel;
17	      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin and each isolation
18	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
19	       locNSelected[iso_index] = 0;
20	       locNSelectedSignal[iso_index] = 0;
21	       locNSelectedW[iso_index] = 0;
22	       locNSelectedQCD[iso_index] = 0;
23	      }
24	
25	      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin
26	      EpsilonFake = 0;
27	      EpsilonFakeErr = 0;
28	      EpsilonSignal = 0;
29	      EpsilonSignalErr = 0;
30	
31	      //Reinitialized to a value, for each bin, at each IncrementNSetBin() call
32	      //Then passed bin by bin to RunTheMatrixMethod() call
33	      theNSelected.reserve(100000);
34	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
35	       struct NSelected theNSelElement;
36	       for(unsigned int iso_index=0; iso_index<3; iso_index++){
37	         theNSelElement.NSel[iso_index] = 0.;
38	       }
39	       theNSelected.push_back(theNSelElement);
40	      }
41	
42	      IsoNames[0] = "Loose";
43	      IsoNames[1] = "Medium";
44	      IsoNames[2] = "Tight";
45	
46	      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin, each isolation and for each iteration a new value is filled
47	      theDistributions.reserve(100000);
48	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
49	       std::stringstream ss;
50	       ss << bin_index;
51	       struct Distribution theDistributionElement;
52	       for(unsigned int iso_index=0; iso_index<3; iso_index++){
53	        theDistributionElement.NMMEstimatedQCDDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"QCDDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"QCDDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
54	        theDistributionElement.NMMEstimatedWJetsDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"WJetsDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"WJetsDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
55	        theDistributionElement.NMMEstimatedSignalDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"SignalDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"SignalDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
56	        //        theDistributionElement.NMMEstimatedQCDDistribution[iso_index]->SetBit(TH1::kCanRebin);
57	        //        theDistributionElement.NMMEstimatedWJetsDistribution[iso_index]->SetBit(TH1::kCanRebin);
58	        //        theDistributionElement.NMMEstimatedSignalDistribution[iso_index]->SetBit(TH1::kCanRebin);
59	       }
60	       theDistributions.push_back(theDistributionElement);
61	      }
62	
63	
64	      //Initialized only here, once! Then filled elsewhere
65	      theMMEstimatedValues.reserve(100000);
66	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
67	       struct MMEstimated theMMEstimatedValuesElement;        
68	       for(unsigned int iso_index=0; iso_index<3; iso_index++){
69	        theMMEstimatedValuesElement.NofMMEstimatedQCD[iso_index]= 0.;
70	        theMMEstimatedValuesElement.MMEstimatedQCDErr[iso_index]= 0.;
71	        theMMEstimatedValuesElement.NofMMEstimatedWJets[iso_index]= 0.;
72	        theMMEstimatedValuesElement.MMEstimatedWJetsErr[iso_index]= 0.;
73	        theMMEstimatedValuesElement.NofMMEstimatedSignal[iso_index]= 0.;
74	        theMMEstimatedValuesElement.MMEstimatedSignalErr[iso_index]= 0.;
75	       }
76	       theMMEstimatedValues.push_back(theMMEstimatedValuesElement);
77	      }
78	
79	      //Initialized only here, once! Then filled elsewhere
80	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
81	       theMMEstimatedPlots.MMEstimated_QCD[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_QCD").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_QCD").c_str(), theNBins, theMinBin, theMaxBin);
82	       theMMEstimatedPlots.MMEstimated_WJets[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_WJets").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_WJets").c_str(), theNBins, theMinBin, theMaxBin);
83	       theMMEstimatedPlots.MMEstimated_Signal[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_Signal").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_Signal").c_str(), theNBins, theMinBin, theMaxBin);
84	      }
85	
86	
87	
88	
89	      //Initialized only here, once! Then filled elsewhere
90	      theMMExpectedPlots.reserve(100000);
91	      {
92	       struct MMExpectedPlots tmpMMExpectedPlot;
93	       for(unsigned int iso_index=0; iso_index<3; iso_index++){
94	         tmpMMExpectedPlot.Name[iso_index] = "TTbarSignal";
95	         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str(), theNBins, theMinBin, theMaxBin); 
96	         h->Sumw2();
97	         tmpMMExpectedPlot.MMExpected[iso_index] = h;
98	         h = 0;
99	         delete h;        
100	       }
101	       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
102	      }
103	
104	
105	      {
106	       struct MMExpectedPlots tmpMMExpectedPlot;
107	       for(unsigned int iso_index=0; iso_index<3; iso_index++){
108	         tmpMMExpectedPlot.Name[iso_index] = "TTbarSemileptonic";
109	         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str(), theNBins, theMinBin, theMaxBin); 
110	         h->Sumw2();
111	         tmpMMExpectedPlot.MMExpected[iso_index] = h;
112	         h = 0;
113	         delete h;        
114	       }
115	       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
116	      }
117	
118	      for(unsigned int dataset_index=0; dataset_index < datasets.size(); dataset_index++){
119	       struct MMExpectedPlots tmpMMExpectedPlot;
120	       for(unsigned int iso_index=0; iso_index<3; iso_index++){
121	         tmpMMExpectedPlot.Name[iso_index] = datasets[dataset_index].Name();
122	         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_"+datasets[dataset_index].Name()).c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_"+datasets[dataset_index].Name()).c_str(), theNBins, theMinBin, theMaxBin); 
123	         h->Sumw2();
124	         tmpMMExpectedPlot.MMExpected[iso_index] = h;
125	         h = 0;
126	         delete h;        
127	       }
128	       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
129	      }
130	
131	
132	      /*
133	      //Initialized only here, once! Then filled elsewhere
134	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
135	        theMMStacks.stackSignal[iso_index] = new THStack(("Stack_"+IsoNames[iso_index]+theChannel+"_Signal").c_str(),("Stack_"+IsoNames[iso_index]+theChannel+"_Signal").c_str());
136	        theMMStacks.stackWJets[iso_index] = new THStack(("Stack_"+IsoNames[iso_index]+theChannel+"_WJets").c_str(),("Stack_"+IsoNames[iso_index]+theChannel+"_WJets").c_str());
137	        theMMStacks.stackQCD[iso_index] = new THStack(("Stack_"+IsoNames[iso_index]+theChannel+"_QCD").c_str(),("Stack_"+IsoNames[iso_index]+theChannel+"_QCD").c_str());
138	      }
139	      */
140	
141	
142	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
143	       histoN[iso_index] = new TH1F(("histoN"+IsoNames[iso_index]+theChannel).c_str(), ("histoN"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
144	       histoSignal[iso_index] = new TH1F(("histoSignal"+IsoNames[iso_index]+theChannel).c_str(), ("histoSignal"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
145	       histoW[iso_index] = new TH1F(("histoW"+IsoNames[iso_index]+theChannel).c_str(), ("histoW"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
146	       histoQCD[iso_index] = new TH1F(("histoQCD"+IsoNames[iso_index]+theChannel).c_str(), ("histoQCD"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
147	
148	      }
149	
150	
151	}
152	
153	MMEstimation::~MMEstimation(){
154	
155	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
156	        for(unsigned int iso_index=0; iso_index<3; iso_index++){
157	         delete theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index];
158	         delete theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index];
159	         delete theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index];
160	       }
161	      }
162	
163	
164	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
165	       delete theMMEstimatedPlots.MMEstimated_QCD[iso_index];
166	       delete theMMEstimatedPlots.MMEstimated_WJets[iso_index];
167	       delete theMMEstimatedPlots.MMEstimated_Signal[iso_index];
168	      }
169	
170	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
171	       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
172	         delete theMMExpectedPlots[i].MMExpected[iso_index];
173	       }
174	      }
175	      /*
176	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
177	        delete theMMStacks.stackSignal[iso_index];
178	        delete theMMStacks.stackWJets[iso_index];
179	        delete theMMStacks.stackQCD[iso_index];
180	      }
181	      */
182	
183	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
184	       delete histoN[iso_index];
185	       delete histoSignal[iso_index];
186	       delete histoW[iso_index];
187	       delete histoQCD[iso_index];
188	      }
189	
190	
191	
192	}
193	
194	
195	struct Isolations MMEstimation::GetIsolations(){
196	  return theIsolations;
197	}
198	
199	
200	void MMEstimation::IncrementNSelBin(unsigned int iso_index, unsigned int bin_index, float weight){
201	         theNSelected[bin_index].NSel[iso_index] += weight;
202	}
203	
204	void MMEstimation::CountNSel(const DiLeptonSelection & sel, Dataset dataset, SelectionTable selTable, unsigned int d, float weight, string CandTypeRequested, int selStepCut){
205	
206	    int selStepMM[3]= {-1, -1, -1};
207	    string CandTypeForMM[3]= {"", "", ""};
208	    unsigned int njets[3] = {-1, -1, -1};
209	    
210	
211	    for (unsigned int iso_index = 0; iso_index < 3; iso_index++) {
212	        vector<NTJet> SelectedJetsForMM; bool LepPairForMM[3]; vector<NTElectron> candElecForMM; vector<NTMuon> candMuonForMM;
213	        DiLeptonSelection sel_aux(sel);
214	        selStepMM[iso_index] = sel_aux.FillTableForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], selTable, &(dataset), d, weight);         
215	        // Be careful if inverted or not (at the moment pairing is applied before isolation)
216	        LepPairForMM[iso_index] = sel_aux.GetLeptonPairForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], candMuonForMM, candElecForMM, CandTypeForMM[iso_index]);
217	        SelectedJetsForMM = sel_aux.GetSelectedJets(candMuonForMM, candElecForMM);
218	        njets[iso_index] = SelectedJetsForMM.size();//  if(iso_index == 2){njets[2]=SelectedJetsForMM.size(); njets[1]=SelectedJetsForMM.size(); njets[0]=SelectedJetsForMM.size();}
219	    }
220	    //    for (unsigned int iso_index = 0; iso_index<2; iso_index++) {
221	    //        CandTypeForMM[iso_index] = CandTypeForMM[2];
222	    //    }
223	
224	
225	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
226	        //Here, njets[] is the variable vs. whih the plots of Matrix Method will be done. It could be any other variable!     
227	        if((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)  && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
228	         IncrementNSelBin(2, bin_index, weight);
229	        }
230	        if((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
231	         IncrementNSelBin(1, bin_index, weight);
232	        }
233	        if((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut) && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
234	         IncrementNSelBin(0, bin_index, weight);
235	        }
236	
237	    
238	        
239	        if(    ((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut) && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))  && 
240	               (
241	                !((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 ||
242	                !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
243	               )
244	          ){ std::cout << "WARNING in CountNSel: Passing Selection Tight, but not passing selection Medium or Loose!" << std::endl;  }
245	
246	
247	        if(    ((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 &&
248	               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
249	          ){ std::cout << "WARNING in CountNSel: Passing Selection Medium, but not passing selection Loose!" << std::endl;  }
250	
251	
252	
253	        
254	
255	
256	
257	     }
258	
259	
260	
261	
262	}
263	
264	
265	
266	void MMEstimation::FillMMExpected(const DiLeptonSelection & sel, Dataset dataset, SelectionTable selTable, unsigned int d, float weight, string CandTypeRequested, int selStepCut, const NTEvent* event){
267	
268	
269	       int selStepMM[3] = {-1, -1, -1};
270	       string CandTypeForMM[3]= {"", "", ""};
271	       unsigned int njets[3] = {-1, -1, -1};
272	
273	
274	       for (unsigned int iso_index = 0; iso_index < 3; iso_index++) {
275	        vector<NTJet> SelectedJetsForMM; bool LepPairForMM[3]; vector<NTElectron> candElecForMM; vector<NTMuon> candMuonForMM;
276	        DiLeptonSelection sel_aux(sel);
277	        selStepMM[iso_index] = sel_aux.FillTableForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], selTable, &(dataset), d, weight);         
278	        // Be careful if inverted or not
279	        LepPairForMM[iso_index] = sel_aux.GetLeptonPairForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], candMuonForMM, candElecForMM, CandTypeForMM[iso_index]);
280	        SelectedJetsForMM = sel_aux.GetSelectedJets(candMuonForMM, candElecForMM);
281	        njets[iso_index] = SelectedJetsForMM.size(); //if(iso_index == 2){njets[2]=SelectedJetsForMM.size(); njets[1]=SelectedJetsForMM.size(); njets[0]=SelectedJetsForMM.size();}
282	       }
283	       //       for (unsigned int iso_index = 0; iso_index<2; iso_index++) {
284	       //           CandTypeForMM[iso_index] = CandTypeForMM[2];
285	       //       }
286	
287	
288	       //Here, njets[] is the variable vs. whih the plots of Matrix Method will be done. It could be any other variable!     
289	       if((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)){
290	
291	         if(((CandTypeRequested == "ee" && (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200)) || // ee
292	             (CandTypeRequested == "mumu" && (event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000)) // mumu
293	            ) && dataset.Name() == "TTbar"
294	         ){
295	           FillMMExpectedPlot("TTbarSignal",2,njets[2],weight);
296	         }else if(((CandTypeRequested == "ee" && (event->TMEME == 1 || // l(=e)jets
297	                   event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
298	                   event->TMEME == 10001 || // l(=e)tau(->had)
299	                   event->TMEME == 10100)) || // tau(->e)jets
300	                   (CandTypeRequested == "mumu" && (event->TMEME == 10 || // l(=mu)jets
301	                   event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
302	                   event->TMEME == 10010 || // l(=mu)tau(->had)
303	                   event->TMEME == 11000)) // tau(->mu)jets
304	                   ) && dataset.Name() == "TTbar"
305	         ){
306	           FillMMExpectedPlot("TTbarSemileptonic",2,njets[2],weight);
307	         }else{
308	           FillMMExpectedPlot(dataset.Name(),2,njets[2],weight);
309	         }
310	
311	       }
312	
313	       if((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut)){
314	
315	         if(((CandTypeRequested == "ee" && (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200)) || // ee
316	             (CandTypeRequested == "mumu" && (event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000)) // mumu
317	            ) && dataset.Name() == "TTbar"
318	         ){
319	           FillMMExpectedPlot("TTbarSignal",1,njets[1],weight);
320	         }else if(((CandTypeRequested == "ee" && (event->TMEME == 1 || // l(=e)jets
321	                   event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
322	                   event->TMEME == 10001 || // l(=e)tau(->had)
323	                   event->TMEME == 10100)) || // tau(->e)jets
324	                   (CandTypeRequested == "mumu" && (event->TMEME == 10 || // l(=mu)jets
325	                   event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
326	                   event->TMEME == 10010 || // l(=mu)tau(->had)
327	                   event->TMEME == 11000)) // tau(->mu)jets
328	                   ) && dataset.Name() == "TTbar"
329	         ){
330	           FillMMExpectedPlot("TTbarSemileptonic",1,njets[1],weight);
331	         }else{
332	           FillMMExpectedPlot(dataset.Name(),1,njets[1],weight);
333	         }
334	       }
335	
336	       if((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)){
337	
338	         if(((CandTypeRequested == "ee" && (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200)) || // ee
339	             (CandTypeRequested == "mumu" && (event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000)) // mumu
340	            ) && dataset.Name() == "TTbar"
341	         ){
342	           FillMMExpectedPlot("TTbarSignal",0,njets[0],weight);
343	         }else if(((CandTypeRequested == "ee" && (event->TMEME == 1 || // l(=e)jets
344	                   event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
345	                   event->TMEME == 10001 || // l(=e)tau(->had)
346	                   event->TMEME == 10100)) || // tau(->e)jets
347	                   (CandTypeRequested == "mumu" && (event->TMEME == 10 || // l(=mu)jets
348	                   event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
349	                   event->TMEME == 10010 || // l(=mu)tau(->had)
350	                   event->TMEME == 11000)) // tau(->mu)jets
351	                   ) && dataset.Name() == "TTbar"
352	         ){
353	           FillMMExpectedPlot("TTbarSemileptonic",0,njets[0],weight);
354	         }else{
355	           FillMMExpectedPlot(dataset.Name(),0,njets[0],weight);
356	         }
357	       }
358	
359	
360	
361	
362	        if(   ((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut))       && 
363	               (
364	                !((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut))    ||
365	                !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
366	               )
367	          ){ std::cout << "WARNING in FillMMExpected: Passing Selection Tight, but not passing selection Medium or Loose!" << std::endl;  }
368	
369	
370	        if(    ((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut))      &&
371	               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
372	          ){ std::cout << "WARNING in FillMMExpected: Passing Selection Medium, but not passing selection Loose!" << std::endl;  }
373	
374	
375	
376	
377	
378	
379	}
380	
381	
382	
383	
384	
385	void MMEstimation::IncludeSystematicError(){
386	 float EpsilonFake0 = EpsilonFake;
387	 float EpsilonSignal0  = EpsilonSignal;
388	 EpsilonFake = ranEpsFake.Gaus(EpsilonFake0, EpsilonFakeErr);
389	 EpsilonSignal  = ranEpsSignal.Gaus(EpsilonSignal0,  EpsilonSignalErr);
390	}
391	
392	
393	void MMEstimation::IncludeStatisticalError(){
394	 float N3 = locNSelected[2];
395	 float N2 = locNSelected[1] - N3;
396	 float N1 = locNSelected[0] - N2 - N3;
397	 N1 = ranN1.Poisson(N1);
398	 N2 = ranN2.Poisson(N2);
399	 N3 = ranN3.Poisson(N3);
400	 locNSelected[2] = N3;
401	 locNSelected[1] = N2+N3;
402	 locNSelected[0] = N1+N2+N3;
403	}
404	
405	
406	void MMEstimation::IncludeStatisticalErrorForEpsilonsTest(float weight){
407	
408	 float N3Signal = locNSelectedSignal[2];
409	 float N2Signal = locNSelectedSignal[1] - N3Signal;
410	 float N1Signal = locNSelectedSignal[0] - N2Signal - N3Signal;
411	 N1Signal = weight * ranN1.Poisson(N1Signal);
412	 N2Signal = weight * ranN2.Poisson(N2Signal);
413	 N3Signal = weight * ranN3.Poisson(N3Signal);
414	 locNSelectedSignal[2] = N3Signal;
415	 locNSelectedSignal[1] = N2Signal+N3Signal;
416	 locNSelectedSignal[0] = N1Signal+N2Signal+N3Signal;
417	
418	
419	 float N3W = locNSelectedW[2];
420	 float N2W = locNSelectedW[1] - N3W;
421	 float N1W = locNSelectedW[0] - N2W - N3W;
422	 N1W = weight * ranN1.Poisson(N1W);
423	 N2W = weight * ranN2.Poisson(N2W);
424	 N3W = weight * ranN3.Poisson(N3W);
425	 locNSelectedW[2] = N3W;
426	 locNSelectedW[1] = N2W+N3W;
427	 locNSelectedW[0] = N1W+N2W+N3W;
428	
429	
430	 float N3QCD = locNSelectedQCD[2];
431	 float N2QCD = locNSelectedQCD[1] - N3QCD;
432	 float N1QCD = locNSelectedQCD[0] - N2QCD - N3QCD;
433	 N1QCD = weight * ranN1.Poisson(N1QCD);
434	 N2QCD = weight * ranN2.Poisson(N2QCD);
435	 N3QCD = weight * ranN3.Poisson(N3QCD);
436	 locNSelectedQCD[2] = N3QCD;
437	 locNSelectedQCD[1] = N2QCD+N3QCD;
438	 locNSelectedQCD[0] = N1QCD+N2QCD+N3QCD;
439	
440	
441	
442	
443	}
444	
445	
446	
447	
448	void MMEstimation::SolveTheSystem(bool doCorrections){
449	
450	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
451	        NMMEstimatedSignal[iso_index] = 0;
452	        NMMEstimatedWJets[iso_index] = 0;
453	        NMMEstimatedQCD[iso_index] = 0;
454	      }
455	
456	
457	      TMatrixF theEffmatrix(3,3); // A
458	      TMatrixF solutions(3,1);    // x
459	      TMatrixF mesured(3,1);      // b
460	
461	      float eff[9] ;  // A
462	
463	
464	      float Eff_ltS   = EpsilonSignal*EpsilonSignal;
465	      float Eff_ltW   = EpsilonSignal*EpsilonFake;
466	      float Eff_ltQCD = EpsilonFake*EpsilonFake;
467	      
468	      float Eff_lmS   = 2.*EpsilonSignal - EpsilonSignal*EpsilonSignal;
469	      float Eff_lmW   = EpsilonSignal + EpsilonFake - EpsilonSignal*EpsilonFake ;
470	      float Eff_lmQCD = 2.*EpsilonFake - EpsilonFake*EpsilonFake;
471	
472	      eff[0] = Eff_ltS  ;
473	      eff[1] = Eff_ltW  ;
474	      eff[2] = Eff_ltQCD ;
475	      //  eff[2] = EpsilonFake0*EpsilonFake0;
476	      
477	      eff[3] = Eff_lmS  ;
478	      eff[4] = Eff_lmW  ;
479	      eff[5] = Eff_lmQCD ;
480	      //  eff[5] = 2.*EpsilonFake0 - EpsilonFake0*EpsilonFake0;
481	      
482	      eff[6] = 1.;
483	      eff[7] = 1.;
484	      eff[8] = 1.; //1 
485	      
486	      theEffmatrix.SetMatrixArray(eff);
487	      theEffmatrix.Invert();
488	
489	      float ArrayMes[3] ; // b
490	      ArrayMes[0] = locNSelected[2];
491	      ArrayMes[1] = locNSelected[1];
492	      ArrayMes[2] = locNSelected[0];
493	
494	      mesured.SetMatrixArray(ArrayMes);
495	      solutions = theEffmatrix*mesured; 
496	
497	      NMMEstimatedSignal[2] = solutions(0,0)*Eff_ltS;
498	      NMMEstimatedWJets[2] = solutions(1,0)*Eff_ltW;
499	      NMMEstimatedQCD[2] = solutions(2,0)*Eff_ltQCD;
500	     
501	      NMMEstimatedSignal[1] = solutions(0,0)*Eff_lmS;
502	      NMMEstimatedWJets[1] = solutions(1,0)*Eff_lmW;
503	      NMMEstimatedQCD[1] = solutions(2,0)*Eff_lmQCD;
504	      
505	      NMMEstimatedSignal[0] = solutions(0,0);
506	      NMMEstimatedWJets[0] = solutions(1,0);
507	      NMMEstimatedQCD[0] = solutions(2,0);      
508	
509	      if(doCorrections){
510	        float EpsilonSignal_bar = 1 - EpsilonSignal;
511	
512	        NMMEstimatedSignal[2] = NMMEstimatedSignal[2] 
513	                              + NMMEstimatedSignal[0] * (2 * EpsilonSignal_bar - 2 * EpsilonSignal_bar * EpsilonSignal_bar) * EpsilonFake
514	                              + NMMEstimatedSignal[0] * EpsilonSignal_bar * EpsilonSignal_bar * EpsilonFake * EpsilonFake;
515	
516	        NMMEstimatedWJets[2] = NMMEstimatedWJets[2] 
517	                             - NMMEstimatedSignal[0] * (2 * EpsilonSignal_bar - 2 * EpsilonSignal_bar * EpsilonSignal_bar) * EpsilonFake
518	                             + NMMEstimatedWJets[0] * EpsilonSignal_bar * EpsilonFake * EpsilonFake;
519	
520	
521	        NMMEstimatedQCD[2] = NMMEstimatedQCD[2]
522	                           - NMMEstimatedSignal[0] * EpsilonSignal_bar * EpsilonSignal_bar * EpsilonFake * EpsilonFake
523	                           - NMMEstimatedWJets[0] * EpsilonSignal_bar * EpsilonFake * EpsilonFake;
524	      }
525	
526	
527	
528	}
529	
530	void MMEstimation::FillDistributions(unsigned int bin_index){
531	
532	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
533	          theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->Fill(NMMEstimatedSignal[iso_index]);
534	          theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->Fill(NMMEstimatedWJets[iso_index]);
535	          theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->Fill(NMMEstimatedQCD[iso_index]);
536	
537	          theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index] += NMMEstimatedSignal[iso_index];
538	          theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index] += NMMEstimatedQCD[iso_index];
539	          theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index] += NMMEstimatedWJets[iso_index];
540	
541	          /*
542	          theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index] += NMMEstimatedSignal[iso_index]*NMMEstimatedSignal[iso_index];
543	          theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index] += NMMEstimatedQCD[iso_index]*NMMEstimatedQCD[iso_index];
544	          theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index] += NMMEstimatedWJets[iso_index]*NMMEstimatedWJets[iso_index];
545	          */
546	    }
547	}
548	
549	
550	void MMEstimation::CalculateRms(unsigned int bin_index, unsigned int NbIterations){
551	
552	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
553	          theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index]/NbIterations)-NMMEstimatedSignal[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index]/NbIterations)-NMMEstimatedSignal[iso_index]);
554	          theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index]/NbIterations)-NMMEstimatedQCD[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index]/NbIterations)-NMMEstimatedQCD[iso_index]);
555	          theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index]/NbIterations)-NMMEstimatedWJets[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index]/NbIterations)-NMMEstimatedWJets[iso_index]);
556	
557	    }
558	}
559	
560	
561	
562	void MMEstimation::SetMMEstimated(unsigned int bin_index, unsigned int NbIterations){
563	
564	     for(unsigned int iso_index=0; iso_index<3; iso_index++){
565	       /*
566	      theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index] = theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetMean();
567	      theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index] = theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetRMS();
568	      theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index] = theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetMean();
569	      theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index] = theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetRMS();
570	      theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index] = theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetMean();
571	      theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index] = theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetRMS();
572	      */
573	
574	
575	      theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index])/NbIterations;
576	      theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index])/NbIterations;
577	      theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index])/NbIterations;
578	      theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index])/(NbIterations-1));
579	      theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index])/(NbIterations-1));
580	      theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index])/(NbIterations-1));
581	
582	     }
583	
584	}
585	
586	
587	void MMEstimation::ReadMMFile(string fileName){
588	
589	  TFile* file_MM   = new TFile((fileName).c_str());
590	  file_MM->cd();
591	  
592	  TH1F* tmpMMNSelected[3];
593	  for(unsigned int iso_index=0; iso_index<3; iso_index++){
594	   tmpMMNSelected[iso_index]  = (TH1F*)gDirectory->Get(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str());
595	  }
596	  for(unsigned int iso_index=0; iso_index<3; iso_index++){
597	   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
598	    theNSelected[bin_index].NSel[iso_index] = tmpMMNSelected[iso_index]->GetBinContent(bin_index+1) ;
599	   }
600	  }
601	  for(unsigned int iso_index=0; iso_index<3; iso_index++){
602	   tmpMMNSelected[iso_index] = 0;
603	   delete tmpMMNSelected[iso_index];
604	  }
605	
606	  file_MM->Close();
607	  delete file_MM;
608	
609	}
610	
611	
612	
613	void MMEstimation::ReadMMFileForPullTest(string fileName, float epsilon_s, float epsilon_f){
614	
615	  TFile* file_MM   = new TFile((fileName).c_str());
616	  file_MM->cd();
617	
618	  TH1F* histoTmpSignal[3];
619	  TH1F* histoTmpW[3];
620	  TH1F* histoTmpQCD[3];
621	  
622	  for(unsigned int iso_index=0; iso_index<3; iso_index++){
623	   histoTmpSignal[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str());
624	   histoTmpW[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
625	   histoTmpQCD[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbar").c_str());
626	  }  
627	
628	  for(unsigned int iso_index=0; iso_index<3; iso_index++){
629	   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
630	     histoSignal[iso_index]->SetBinContent(bin_index+1, histoTmpSignal[iso_index]->GetBinContent(bin_index+1));
631	     histoW[iso_index]->SetBinContent(bin_index+1, histoTmpW[iso_index]->GetBinContent(bin_index+1));
632	     histoQCD[iso_index]->SetBinContent(bin_index+1, histoTmpQCD[iso_index]->GetBinContent(bin_index+1));
633	   }
634	  }  
635	
636	  histoSignal[1]->Add(histoSignal[0], histoSignal[0], 0, 2*epsilon_s - epsilon_s * epsilon_s);
637	  histoW[1]->Add(histoW[0], histoW[0], 0, epsilon_s + epsilon_f - epsilon_s * epsilon_f);
638	  histoQCD[1]->Add(histoQCD[0], histoQCD[0], 0, 2*epsilon_f - epsilon_f * epsilon_f);
639	
640	  histoSignal[2]->Add(histoSignal[0], histoSignal[0], 0, epsilon_s * epsilon_s);
641	  histoW[2]->Add(histoW[0], histoW[0], 0, epsilon_s * epsilon_f);
642	  histoQCD[2]->Add(histoQCD[0], histoQCD[0], 0, epsilon_f * epsilon_f);
643	
644	  histoN[0]->Add(histoW[0], histoSignal[0], 1, 1);
645	  histoN[0]->Add(histoN[0], histoQCD[0], 1, 1);
646	
647	  histoN[1]->Add(histoW[0], histoSignal[0], epsilon_s + epsilon_f - epsilon_s * epsilon_f, 2*epsilon_s - epsilon_s * epsilon_s);
648	  histoN[1]->Add(histoN[1], histoQCD[0], 1, 2*epsilon_f - epsilon_f * epsilon_f);
649	
650	  histoN[2]->Add(histoW[0], histoSignal[0], epsilon_s * epsilon_f, epsilon_s * epsilon_s);
651	  histoN[2]->Add(histoN[2], histoQCD[0], 1, epsilon_f * epsilon_f);
652	
653	
654	  for(unsigned int iso_index=0; iso_index<3; iso_index++){
655	   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
656	    theNSelected[bin_index].NSel[iso_index] = histoN[iso_index]->GetBinContent(bin_index+1) ;
657	   }
658	  }
659	
660	 
661	  for(unsigned int iso_index=0; iso_index<3; iso_index++){
662	   histoTmpSignal[iso_index] = 0;
663	   delete histoTmpSignal[iso_index];
664	   histoTmpW[iso_index] = 0;
665	   delete histoTmpW[iso_index];
666	   histoTmpQCD[iso_index] = 0;
667	   delete histoTmpQCD[iso_index];
668	  }
669	
670	  file_MM->Close();
671	  delete file_MM;
672	
673	}
674	
675	
676	
677	
678	
679	void MMEstimation::RunTheMatrixMethod(vector<struct MMEpsilons> valMMEpsilons, unsigned int NbIterations, bool doStatistical, bool doSystematic, bool doCorrections){
680	
681	
682	  for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
683	
684	   for(unsigned int iso_index=0; iso_index<3; iso_index++){
685	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
686	     theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->SetBinContent(i+1, 0.);
687	    }
688	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
689	     theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->SetBinContent(i+1, 0.);
690	    }
691	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
692	     theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->SetBinContent(i+1, 0.);
693	    }
694	   }
695	
696	
697	
698	   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
699	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
700	       locNSelected[iso_index] = theNSelected[bin_index].NSel[iso_index];
701	      }
702	      EpsilonFake = valMMEpsilons[bin_index].EpsilonFake;
703	      EpsilonFakeErr = valMMEpsilons[bin_index].EpsilonFakeErr;
704	      EpsilonSignal = valMMEpsilons[bin_index].EpsilonSignal;
705	      EpsilonSignalErr = valMMEpsilons[bin_index].EpsilonSignalErr;
706	
707	      if(doStatistical) IncludeStatisticalError();
708	      if(doSystematic) IncludeSystematicError();
709	      SolveTheSystem(doCorrections);
710	      FillDistributions(bin_index);
711	   }
712	
713	
714	   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
715	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
716	       locNSelected[iso_index] = theNSelected[bin_index].NSel[iso_index];
717	      }
718	      EpsilonFake = valMMEpsilons[bin_index].EpsilonFake;
719	      EpsilonFakeErr = valMMEpsilons[bin_index].EpsilonFakeErr;
720	      EpsilonSignal = valMMEpsilons[bin_index].EpsilonSignal;
721	      EpsilonSignalErr = valMMEpsilons[bin_index].EpsilonSignalErr;
722	
723	      if(doStatistical) IncludeStatisticalError();
724	      if(doSystematic) IncludeSystematicError();
725	      SolveTheSystem(doCorrections);
726	      CalculateRms(bin_index, NbIterations); 
727	   }
728	
729	 
730	   SetMMEstimated(bin_index, NbIterations);
731	
732	   for(unsigned int iso_index=0; iso_index<3; iso_index++){
733	     /*
734	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinContent(bin_index+1, theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetMean());
735	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinContent(bin_index+1, theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetMean());
736	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinContent(bin_index+1, theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetMean());
737	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinError(bin_index+1, theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetRMS());
738	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinError(bin_index+1, theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetRMS());
739	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinError(bin_index+1, theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetRMS());
740	     */
741	
742	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index] );
743	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index] );
744	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index] );
745	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index] );
746	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index] );
747	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index] );
748	
749	
750	   }
751	
752	  }
753	
754	
755	
756	}
757	
758	
759	
760	
761	
762	
763	
764	
765	
766	void MMEstimation::RunTheMatrixMethodForEpsilonsTest(vector<struct MMEpsilons> valMMEpsilons, unsigned int NbIterations, bool doStatistical, bool doSystematic, float weight){
767	
768	
769	  for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
770	
771	   for(unsigned int iso_index=0; iso_index<3; iso_index++){
772	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
773	     theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->SetBinContent(i+1, 0.);
774	    }
775	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
776	     theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->SetBinContent(i+1, 0.);
777	    }
778	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
779	     theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->SetBinContent(i+1, 0.);
780	    }
781	   }
782	
783	
784	
785	   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
786	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
787	        locNSelectedSignal[iso_index] = histoSignal[iso_index]->GetBinContent(bin_index+1);
788	        locNSelectedW[iso_index] = histoW[iso_index]->GetBinContent(bin_index+1);
789	        locNSelectedQCD[iso_index] = histoQCD[iso_index]->GetBinContent(bin_index+1);
790	      }
791	
792	      if(doStatistical) IncludeStatisticalErrorForEpsilonsTest(weight);
793	      if(doSystematic) IncludeSystematicError();
794	
795	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
796	        locNSelected[iso_index] = locNSelectedSignal[iso_index]
797	                                + locNSelectedW[iso_index]
798	                                + locNSelectedQCD[iso_index];
799	      }
800	
801	      if (locNSelectedSignal[0] != 0) {
802	        EpsilonSignal = sqrt(locNSelectedSignal[2]/locNSelectedSignal[0]);
803	      }else{
804	        EpsilonSignal = 0;
805	      }
806	      EpsilonSignalErr = 0;
807	      if (locNSelectedW[0] != 0 && EpsilonSignal != 0) {
808	       EpsilonFake = (locNSelectedW[2]/locNSelectedW[0])/(EpsilonSignal);
809	      }else{
810	        EpsilonFake = 0;
811	      }
812	      EpsilonFakeErr = 0;
813	
814	      SolveTheSystem(false);
815	      FillDistributions(bin_index);
816	   }
817	      
818	
819	
820	   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
821	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
822	        locNSelectedSignal[iso_index] = histoSignal[iso_index]->GetBinContent(bin_index+1);
823	        locNSelectedW[iso_index] = histoW[iso_index]->GetBinContent(bin_index+1);
824	        locNSelectedQCD[iso_index] = histoQCD[iso_index]->GetBinContent(bin_index+1);
825	      }
826	
827	      if(doStatistical) IncludeStatisticalErrorForEpsilonsTest(weight);
828	      if(doSystematic) IncludeSystematicError();
829	
830	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
831	        locNSelected[iso_index] = locNSelectedSignal[iso_index]
832	                                + locNSelectedW[iso_index]
833	                                + locNSelectedQCD[iso_index];
834	      }
835	
836	      if (locNSelectedSignal[0] != 0) {
837	        EpsilonSignal = sqrt(locNSelectedSignal[2]/locNSelectedSignal[0]);
838	      }else{
839	        EpsilonSignal = 0;
840	      }
841	      EpsilonSignalErr = 0;
842	      if (locNSelectedW[0] != 0 && EpsilonSignal != 0) {
843	       EpsilonFake = (locNSelectedW[2]/locNSelectedW[0])/(EpsilonSignal);
844	      }else{
845	        EpsilonFake = 0;
846	      }
847	      EpsilonFakeErr = 0;
848	
849	      SolveTheSystem(false);
850	      CalculateRms(bin_index, NbIterations); 
851	   }
852	
853	 
854	   SetMMEstimated(bin_index, NbIterations);
855	
856	   for(unsigned int iso_index=0; iso_index<3; iso_index++){
857	     /*
858	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinContent(bin_index+1, theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetMean());
859	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinContent(bin_index+1, theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetMean());
860	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinContent(bin_index+1, theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetMean());
861	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinError(bin_index+1, theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->GetRMS());
862	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinError(bin_index+1, theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->GetRMS());
863	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinError(bin_index+1, theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->GetRMS());
864	     */
865	
866	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index] );
867	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index] );
868	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index] );
869	    theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index] );
870	    theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index] );
871	    theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index] );
872	
873	
874	   }
875	
876	  }
877	
878	
879	
880	}
881	
882	
883	
884	
885	
886	
887	
888	
889	
890	
891	
892	
893	
894	
895	
896	
897	
898	vector<struct MMEstimated> MMEstimation::GetMMEstimated(){
899	return theMMEstimatedValues;
900	}
901	
902	
903	void MMEstimation::PrintMMEstimated(){
904	
905	/*
906	   for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
907	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
908	      std::cout <<"theMMEstimatedValue of NofQDC for bin_index: "<<bin_index <<" and iso_index: "<<iso_index <<" is: "<<theMMEstimatedValues[bin_index].NofMMEstimatedQCD[iso_index] << std::endl;
909	      std::cout <<"theMMEstimatedValue of QCDErr for bin_index: "<<bin_index <<" and iso_index: "<<iso_index <<" is: "<<theMMEstimatedValues[bin_index].MMEstimatedQCDErr[iso_index] << std::endl;
910	      std::cout <<"theMMEstimatedValue of NofWJets for bin_index: "<<bin_index <<" and iso_index: "<<iso_index <<" is: "<<theMMEstimatedValues[bin_index].NofMMEstimatedWJets[iso_index] << std::endl;
911	      std::cout <<"theMMEstimatedValue of WJetsErr for bin_index: "<<bin_index <<" and iso_index: "<<iso_index <<" is: "<<theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[iso_index] << std::endl;
912	      std::cout <<"theMMEstimatedValue of NofSignal for bin_index: "<<bin_index <<" and iso_index: "<<iso_index <<" is: "<<theMMEstimatedValues[bin_index].NofMMEstimatedSignal[iso_index] << std::endl;
913	      std::cout <<"theMMEstimatedValue of SignalErr for bin_index: "<<bin_index <<" and iso_index: "<<iso_index <<" is: "<<theMMEstimatedValues[bin_index].MMEstimatedSignalErr[iso_index] << std::endl;
914	    }
915	   }
916	*/
917	
918	
919	   std::cout << "\\begin{table}" << std::endl;
920	   std::cout << "\\begin{center}" << std::endl;
921	   std::cout << "\\begin{tabular}{|";
922	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
923	    std::cout << " c |";
924	   }
925	   std::cout << "}" << std::endl;
926	   std::cout << "\\hline" << std::endl;
927	
928	
929	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
930	    std::cout << " & Njets = " << bin_index;
931	   }
932	   std::cout << "\\\\ \\hline" << std::endl;
933	
934	
935	   std::cout << "N of Signal-like";
936	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
937	    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedSignal[2] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedSignalErr[2];
938	   }
939	   std::cout << "\\\\ \\hline" << std::endl;
940	
941	
942	   std::cout << "N of W-like";
943	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
944	    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedWJets[2] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[2];
945	   }
946	   std::cout << "\\\\ \\hline" << std::endl;
947	
948	
949	   std::cout << "N of QCD-like";
950	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
951	    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedQCD[2] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedQCDErr[2];
952	   }
953	   std::cout << "\\\\ \\hline" << std::endl;
954	
955	   std::cout << "\\end{tabular}" << std::endl;
956	   std::cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
957	   std::cout << "\\end{center}" << std::endl;
958	   std::cout << "\\end{table}" << std::endl;
959	
960	
961	
962	
963	   float NofMMEstimatedSignalTOTAL = 0;
964	   float NofMMEstimatedWJetsTOTAL = 0;
965	   float NofMMEstimatedQCDTOTAL = 0;
966	
967	   float MMEstimatedSignalErrTOTAL = 0;
968	   float MMEstimatedWJetsErrTOTAL = 0;
969	   float MMEstimatedQCDErrTOTAL = 0;
970	
971	   for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
972	
973	    NofMMEstimatedSignalTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedSignal[2];
974	    NofMMEstimatedQCDTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedQCD[2];
975	    NofMMEstimatedWJetsTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedWJets[2];
976	    MMEstimatedSignalErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedSignalErr[2]*theMMEstimatedValues[bin_index].MMEstimatedSignalErr[2];
977	    MMEstimatedQCDErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedQCDErr[2]*theMMEstimatedValues[bin_index].MMEstimatedQCDErr[2];
978	    MMEstimatedWJetsErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[2]*theMMEstimatedValues[bin_index].MMEstimatedWJetsErr[2];
979	
980	   }
981	
982	   std::cout << "NofMMEstimatedSignalTOTAL: " << NofMMEstimatedSignalTOTAL << std::endl;
983	   std::cout << "MMEstimatedSignalErrTOTAL: " << sqrt(MMEstimatedSignalErrTOTAL) << std::endl;
984	   std::cout << "NofMMEstimatedQCDTOTAL: " << NofMMEstimatedQCDTOTAL << std::endl;
985	   std::cout << "MMEstimatedQCDErrTOTAL: " << sqrt(MMEstimatedQCDErrTOTAL) << std::endl;
986	   std::cout << "NofMMEstimatedWJetsTOTAL: " << NofMMEstimatedWJetsTOTAL << std::endl;
987	   std::cout << "MMEstimatedWJetsErrTOTAL: " << sqrt(MMEstimatedWJetsErrTOTAL) << std::endl;
988	
989	
990	
991	
992	
993	
994	
995	
996	}
997	
998	
999	struct MMEstimatedPlots MMEstimation::GetMMEstimatedPlots(){
1000	 return theMMEstimatedPlots;
1001	}
1002	
1003	
1004	void MMEstimation::FillMMExpectedPlot(string datasetname, unsigned int iso_index, float val, float weight){
1005	
1006	       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1007	         if(datasetname == theMMExpectedPlots[i].Name[iso_index]) (theMMExpectedPlots[i].MMExpected[iso_index])->Fill(val, weight);
1008	       }
1009	
1010	}
1011	
1012	vector <struct MMExpectedPlots> MMEstimation::GetMMExpectedPlots(){
1013	  return theMMExpectedPlots;
1014	}
1015	
1016	
1017	
1018	
1019	void MMEstimation::WriteMMFile(string fileName){
1020	    
1021	  /*
1022	    TLegend* legSignal[3] = {new TLegend(0.67,0.45,0.888,0.70), new TLegend(0.67,0.45,0.888,0.70), new TLegend(0.67,0.45,0.888,0.70)};
1023	    TLegend* legQCD[3] = {new TLegend(0.67,0.45,0.888,0.70), new TLegend(0.67,0.45,0.888,0.70), new TLegend(0.67,0.45,0.888,0.70)};
1024	    TLegend* legWJets[3] = {new TLegend(0.67,0.45,0.888,0.70), new TLegend(0.67,0.45,0.888,0.70), new TLegend(0.67,0.45,0.888,0.70)};
1025	    TCanvas *cSignal[3];
1026	    TCanvas *cQCD[3];
1027	    TCanvas *cWJets[3];
1028	    TH1F * tmpSignal[3];
1029	    TH1F * tmpQCD[3];
1030	    TH1F * tmpWJets[3];
1031	    TGraphErrors *tmpGraphSignal[3];
1032	    TGraphErrors *tmpGraphQCD[3];
1033	    TGraphErrors *tmpGraphWJets[3];
1034	
1035	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1036	       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1037	         if  ((theMMExpectedPlots[i].Name[iso_index]) == "TTbarSignal") theMMStacks.stackSignal[iso_index]->Add((theMMExpectedPlots[i].MMExpected[iso_index])); (theMMExpectedPlots[i].MMExpected[iso_index])->SetFillColor(2);
1038	         if  ((theMMExpectedPlots[i].Name[iso_index]) == "SingleToptW") theMMStacks.stackSignal[iso_index]->Add((theMMExpectedPlots[i].MMExpected[iso_index])); (theMMExpectedPlots[i].MMExpected[iso_index])->SetFillColor(4);
1039	         if  ((theMMExpectedPlots[i].Name[iso_index]) == "Zjets") theMMStacks.stackSignal[iso_index]->Add((theMMExpectedPlots[i].MMExpected[iso_index])); (theMMExpectedPlots[i].MMExpected[iso_index])->SetFillColor(6);
1040	         if  ((theMMExpectedPlots[i].Name[iso_index]) == "Wjets") theMMStacks.stackWJets[iso_index]->Add((theMMExpectedPlots[i].MMExpected[iso_index])); (theMMExpectedPlots[i].MMExpected[iso_index])->SetFillColor(9);
1041	         if  ((theMMExpectedPlots[i].Name[iso_index]) == "TTbar") theMMStacks.stackQCD[iso_index]->Add((theMMExpectedPlots[i].MMExpected[iso_index])); (theMMExpectedPlots[i].MMExpected[iso_index])->SetFillColor(41);
1042	         if  ((theMMExpectedPlots[i].Name[iso_index]) == "QCDPt15") theMMStacks.stackQCD[iso_index]->Add((theMMExpectedPlots[i].MMExpected[iso_index])); (theMMExpectedPlots[i].MMExpected[iso_index])->SetFillColor(39);
1043	       }
1044	
1045	    }
1046	
1047	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1048	
1049	      cSignal[iso_index] = new TCanvas(("cSignal_"+IsoNames[iso_index]+theChannel).c_str(), ("cSignal_"+IsoNames[iso_index]+theChannel).c_str(), 600, 800);
1050	      cSignal[iso_index]->cd();
1051	      theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetMarkerStyle(20);
1052	      //theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetMinimum(-3);
1053	      //theMMEstimatedPlots.MMEstimated_Signal[iso_index]->SetMaximum(3);
1054	      theMMEstimatedPlots.MMEstimated_Signal[iso_index]->Draw();
1055	      theMMStacks.stackSignal[iso_index]->Draw("hesame");
1056	      theMMEstimatedPlots.MMEstimated_Signal[iso_index]->Draw("same");
1057	      tmpSignal[iso_index]  = new TH1F(("tmpSignal"+IsoNames[iso_index]+theChannel).c_str(), ("tmpSignal"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
1058	      for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1059	        if((theMMExpectedPlots[i].Name[iso_index]) == "TTbarSignal") tmpSignal[iso_index]->Add(tmpSignal[iso_index], (theMMExpectedPlots[i].MMExpected[iso_index]));
1060	        if((theMMExpectedPlots[i].Name[iso_index]) == "SingleToptW") tmpSignal[iso_index]->Add(tmpSignal[iso_index], (theMMExpectedPlots[i].MMExpected[iso_index]));
1061	        if((theMMExpectedPlots[i].Name[iso_index]) == "Zjets") tmpSignal[iso_index]->Add(tmpSignal[iso_index], (theMMExpectedPlots[i].MMExpected[iso_index]));
1062	      }
1063	      tmpGraphSignal[iso_index] = new TGraphErrors(tmpSignal[iso_index]);
1064	      tmpGraphSignal[iso_index]->SetFillStyle(3005);
1065	      tmpGraphSignal[iso_index]->SetFillColor(1);
1066	      tmpGraphSignal[iso_index]->Draw("e2same");
1067	      for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1068	        if((theMMExpectedPlots[i].Name[iso_index]) == "TTbarSignal") legSignal[iso_index]->AddEntry((theMMExpectedPlots[i].MMExpected[iso_index]), ((theMMExpectedPlots[i].Name[iso_index])).c_str(),"f");
1069	        if((theMMExpectedPlots[i].Name[iso_index]) == "SingleToptW") legSignal[iso_index]->AddEntry((theMMExpectedPlots[i].MMExpected[iso_index]), ((theMMExpectedPlots[i].Name[iso_index])).c_str(),"f");
1070	        if((theMMExpectedPlots[i].Name[iso_index]) == "Zjets") legSignal[iso_index]->AddEntry((theMMExpectedPlots[i].MMExpected[iso_index]), ((theMMExpectedPlots[i].Name[iso_index])).c_str(),"f");
1071	      }
1072	      legSignal[iso_index]->AddEntry(theMMEstimatedPlots.MMEstimated_Signal[iso_index], "Matrix Method","p");
1073	      legSignal[iso_index]->Draw("same");    
1074	
1075	
1076	      cWJets[iso_index] = new TCanvas(("cWJets_"+IsoNames[iso_index]+theChannel).c_str(), ("cWJets_"+IsoNames[iso_index]+theChannel).c_str(), 600, 800);
1077	      cWJets[iso_index]->cd();
1078	      theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetMarkerStyle(20);
1079	      //theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetMinimum(-3);
1080	      //theMMEstimatedPlots.MMEstimated_WJets[iso_index]->SetMaximum(3);
1081	      theMMEstimatedPlots.MMEstimated_WJets[iso_index]->Draw();
1082	      theMMStacks.stackWJets[iso_index]->Draw("hesame");
1083	      theMMEstimatedPlots.MMEstimated_WJets[iso_index]->Draw("same");
1084	      tmpWJets[iso_index]  = new TH1F(("tmpWJets"+IsoNames[iso_index]+theChannel).c_str(), ("tmpWJets"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
1085	      for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1086	        if((theMMExpectedPlots[i].Name[iso_index]) == "Wjets") tmpWJets[iso_index]->Add(tmpWJets[iso_index], (theMMExpectedPlots[i].MMExpected[iso_index]));
1087	      }
1088	      tmpGraphWJets[iso_index] = new TGraphErrors(tmpWJets[iso_index]);
1089	      tmpGraphWJets[iso_index]->SetFillStyle(3005);
1090	      tmpGraphWJets[iso_index]->SetFillColor(1);
1091	      tmpGraphWJets[iso_index]->Draw("e2same");
1092	      for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1093	        if((theMMExpectedPlots[i].Name[iso_index]) == "Wjets") legWJets[iso_index]->AddEntry((theMMExpectedPlots[i].MMExpected[iso_index]), ((theMMExpectedPlots[i].Name[iso_index])).c_str(),"f");
1094	      }
1095	      legWJets[iso_index]->AddEntry(theMMEstimatedPlots.MMEstimated_WJets[iso_index], "Matrix Method","p");
1096	      legWJets[iso_index]->Draw("same");    
1097	
1098	
1099	      cQCD[iso_index] = new TCanvas(("cQCD_"+IsoNames[iso_index]+theChannel).c_str(), ("cQCD_"+IsoNames[iso_index]+theChannel).c_str(), 600, 800);
1100	      cQCD[iso_index]->cd();
1101	      theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetMarkerStyle(20);
1102	      //theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetMinimum(-3);
1103	      //theMMEstimatedPlots.MMEstimated_QCD[iso_index]->SetMaximum(3);
1104	      theMMEstimatedPlots.MMEstimated_QCD[iso_index]->Draw();
1105	      theMMStacks.stackQCD[iso_index]->Draw("hesame");
1106	      theMMEstimatedPlots.MMEstimated_QCD[iso_index]->Draw("same");
1107	      tmpQCD[iso_index]  = new TH1F(("tmpQCD"+IsoNames[iso_index]+theChannel).c_str(), ("tmpQCD"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
1108	      for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1109	        if((theMMExpectedPlots[i].Name[iso_index]) == "TTbar") tmpQCD[iso_index]->Add(tmpQCD[iso_index], (theMMExpectedPlots[i].MMExpected[iso_index]));
1110	        if((theMMExpectedPlots[i].Name[iso_index]) == "QCDPt15") tmpQCD[iso_index]->Add(tmpQCD[iso_index], (theMMExpectedPlots[i].MMExpected[iso_index]));
1111	      }
1112	      tmpGraphQCD[iso_index] = new TGraphErrors(tmpQCD[iso_index]);
1113	      tmpGraphQCD[iso_index]->SetFillStyle(3005);
1114	      tmpGraphQCD[iso_index]->SetFillColor(1);
1115	      tmpGraphQCD[iso_index]->Draw("e2same");
1116	      for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1117	        if((theMMExpectedPlots[i].Name[iso_index]) == "TTbar") legQCD[iso_index]->AddEntry((theMMExpectedPlots[i].MMExpected[iso_index]), ((theMMExpectedPlots[i].Name[iso_index])).c_str(),"f");
1118	        if((theMMExpectedPlots[i].Name[iso_index]) == "QCDPt15") legQCD[iso_index]->AddEntry((theMMExpectedPlots[i].MMExpected[iso_index]), ((theMMExpectedPlots[i].Name[iso_index])).c_str(),"f");
1119	      }
1120	      legQCD[iso_index]->AddEntry(theMMEstimatedPlots.MMEstimated_QCD[iso_index], "Matrix Method","p");
1121	      legQCD[iso_index]->Draw("same");    
1122	
1123	    }
1124	
1125	  */
1126	
1127	
1128	
1129	
1130	
1131	
1132	    TFile* file_MM = new TFile((fileName).c_str(),"RECREATE");
1133	    file_MM->cd();
1134	
1135	
1136	
1137	    /*
1138	
1139	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1140	     theMMStacks.stackSignal[iso_index]->Write();
1141	     theMMStacks.stackQCD[iso_index]->Write();
1142	     theMMStacks.stackWJets[iso_index]->Write();
1143	    }
1144	    */
1145	
1146	
1147	    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
1148	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
1149	        theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->Write();
1150	        theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->Write();
1151	        theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->Write();
1152	      }
1153	    }
1154	
1155	
1156	
1157	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1158	       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
1159	         (theMMExpectedPlots[i].MMExpected[iso_index])->Write();
1160	       }
1161	    }
1162	
1163	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1164	      theMMEstimatedPlots.MMEstimated_Signal[iso_index]->Write();
1165	      theMMEstimatedPlots.MMEstimated_QCD[iso_index]->Write();
1166	      theMMEstimatedPlots.MMEstimated_WJets[iso_index]->Write();
1167	    }
1168	
1169	    // Writing histos contaning all information concerning NSelected
1170	    TH1F* MMNSelected[3];
1171	
1172	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1173	      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
1174	    }
1175	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1176	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
1177	       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
1178	     }
1179	     MMNSelected[iso_index]->Write();
1180	    }
1181	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1182	     MMNSelected[iso_index] = 0;
1183	     delete MMNSelected[iso_index];
1184	    }
1185	
1186	    /*
1187	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1188	     cSignal[iso_index]->Write();
1189	     cQCD[iso_index]->Write();
1190	     cWJets[iso_index]->Write();
1191	
1192	     cSignal[iso_index] = 0;
1193	     cQCD[iso_index] = 0;
1194	     cWJets[iso_index] = 0;
1195	     tmpSignal[iso_index] = 0;
1196	     tmpGraphSignal[iso_index] = 0;
1197	     tmpQCD[iso_index] = 0;
1198	     tmpGraphQCD[iso_index] = 0;
1199	     tmpWJets[iso_index] = 0;
1200	     tmpGraphWJets[iso_index] = 0;
1201	     delete cSignal[iso_index];
1202	     delete cQCD[iso_index];
1203	     delete cWJets[iso_index];
1204	     delete tmpSignal[iso_index];
1205	     delete tmpGraphSignal[iso_index];
1206	     delete tmpQCD[iso_index];
1207	     delete tmpGraphQCD[iso_index];
1208	     delete tmpWJets[iso_index];
1209	     delete tmpGraphWJets[iso_index];
1210	     delete legSignal[iso_index];
1211	     delete legQCD[iso_index];
1212	     delete legWJets[iso_index];
1213	    }
1214	    */
1215	
1216	    
1217	
1218	    file_MM->Write();
1219	    file_MM->Close();
1220	    delete file_MM;
1221	
1222	
1223	}
1224	
1225	
1226	
1227	
1228	
1229	void MMEstimation::WriteMMFileFast(string fileName){
1230	
1231	    TFile* file_MM = new TFile((fileName).c_str(),"RECREATE");
1232	    file_MM->cd();
1233	
1234	    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
1235	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
1236	        theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->Write();
1237	        theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->Write();
1238	        theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->Write();
1239	      }
1240	    }
1241	
1242	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1243	      theMMEstimatedPlots.MMEstimated_Signal[iso_index]->Write();
1244	      theMMEstimatedPlots.MMEstimated_QCD[iso_index]->Write();
1245	      theMMEstimatedPlots.MMEstimated_WJets[iso_index]->Write();
1246	    }
1247	
1248	    // Writing histos contaning all information concerning NSelected
1249	    TH1F* MMNSelected[3];
1250	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1251	      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
1252	    }
1253	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1254	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
1255	       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
1256	     }
1257	     MMNSelected[iso_index]->Write();
1258	    }
1259	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1260	     MMNSelected[iso_index] = 0;
1261	     delete MMNSelected[iso_index];
1262	    }
1263	
1264	    file_MM->Write();
1265	    file_MM->Close();
1266	    delete file_MM;
1267	
1268	
1269	}
1270	
1271	
1272	
1273	
1274	
1275	
1276	
1277	
1278	
1279	
1280	
1281	void MMEstimation::WriteMMFileFastForPullTest(string fileINPUT, string fileOUTPUT, float epsilon_s, float epsilon_f){
1282	
1283	 
1284	    TFile* file_MM_INPUT   = new TFile((fileINPUT).c_str());
1285	    file_MM_INPUT->cd();
1286	     
1287	    TH1F* tmpSignal[3];
1288	    TH1F* tmpW[3];
1289	    TH1F* tmpQCD[3];
1290	
1291	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1292	     tmpSignal[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str());
1293	     tmpW[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
1294	     tmpQCD[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbar").c_str());
1295	    }
1296	
1297	  
1298	    tmpSignal[1]->Add(tmpSignal[1], tmpSignal[0], 0, 2*epsilon_s - epsilon_s * epsilon_s);
1299	    tmpW[1]->Add(tmpW[1], tmpW[0], 0, epsilon_s + epsilon_f - epsilon_s * epsilon_f);
1300	    tmpQCD[1]->Add(tmpQCD[1], tmpQCD[0], 0, 2*epsilon_f - epsilon_f * epsilon_f);
1301	
1302	    tmpSignal[2]->Add(tmpSignal[2], tmpSignal[0], 0, epsilon_s * epsilon_s);
1303	    tmpW[2]->Add(tmpW[2], tmpW[0], 0, epsilon_s * epsilon_f);
1304	    tmpQCD[2]->Add(tmpQCD[2], tmpQCD[0], 0, epsilon_f * epsilon_f);
1305	
1306	    
1307	   
1308	    TFile* file_MM_OUTPUT = new TFile((fileOUTPUT).c_str(),"RECREATE");
1309	    file_MM_OUTPUT->cd();
1310	
1311	   
1312	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1313	     tmpSignal[iso_index]->Write();
1314	     tmpW[iso_index]->Write();
1315	     tmpQCD[iso_index]->Write();
1316	    }
1317	   
1318	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1319	     tmpSignal[iso_index] = 0;
1320	     tmpW[iso_index] = 0;
1321	     tmpQCD[iso_index] = 0;
1322	     delete tmpSignal[iso_index];
1323	     delete tmpW[iso_index];
1324	     delete tmpQCD[iso_index];
1325	    }
1326	    
1327	    file_MM_INPUT->Close();
1328	    delete file_MM_INPUT;
1329	
1330	
1331	
1332	    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
1333	      for(unsigned int iso_index=0; iso_index<3; iso_index++){
1334	        theDistributions[bin_index].NMMEstimatedQCDDistribution[iso_index]->Write();
1335	        theDistributions[bin_index].NMMEstimatedWJetsDistribution[iso_index]->Write();
1336	        theDistributions[bin_index].NMMEstimatedSignalDistribution[iso_index]->Write();
1337	      }
1338	    }
1339	
1340	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1341	      theMMEstimatedPlots.MMEstimated_Signal[iso_index]->Write();
1342	      theMMEstimatedPlots.MMEstimated_QCD[iso_index]->Write();
1343	      theMMEstimatedPlots.MMEstimated_WJets[iso_index]->Write();
1344	    }
1345	
1346	    // Writing histos contaning all information concerning NSelected
1347	    TH1F* MMNSelected[3];
1348	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1349	      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
1350	    }
1351	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1352	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
1353	       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
1354	     }
1355	     MMNSelected[iso_index]->Write();
1356	    }
1357	    for(unsigned int iso_index=0; iso_index<3; iso_index++){
1358	     MMNSelected[iso_index] = 0;
1359	     delete MMNSelected[iso_index];
1360	    }
1361	
1362	
1363	
1364	
1365	    file_MM_OUTPUT->Write();
1366	    file_MM_OUTPUT->Close();
1367	    delete file_MM_OUTPUT;
1368	
1369	
1370	}
