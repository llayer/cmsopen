1	#include "../interface/GeneralMMEstimation.h"
2	                
3	GeneralMMEstimation::GeneralMMEstimation(vector<Dataset> datasets, float isoLoose, float isoTight1, float isoTight2, unsigned int n_bins, float min, float max, string Channel){
4	
5	      theNBins = n_bins;
6	      theMinBin = min;
7	      theMaxBin = max;
8	
9	      theIsolations.iso1[0] = isoLoose;
10	      theIsolations.iso2[0] = isoLoose;
11	
12	      theIsolations.iso1[1] = isoLoose;
13	      theIsolations.iso2[1] = isoTight2;
14	
15	      theIsolations.iso1[2] = isoTight1;
16	      theIsolations.iso2[2] = isoLoose;
17	
18	      theIsolations.iso1[3] = isoTight1;
19	      theIsolations.iso2[3] = isoTight2;
20	
21	      theChannel = Channel;
22	      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin and each isolation
23	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
24	       locNSelected[iso_index] = 0;
25	      }
26	
27	      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin
28	      EpsilonEFake = 0;
29	      EpsilonEFakeErr = 0;
30	      EpsilonESignal = 0;
31	      EpsilonESignalErr = 0;
32	
33	      EpsilonMuFake = 0;
34	      EpsilonMuFakeErr = 0;
35	      EpsilonMuSignal = 0;
36	      EpsilonMuSignalErr = 0;
37	
38	      //Reinitialized to a value, for each bin, at each IncrementNSetBin() call
39	      //Then passed bin by bin to RunTheMatrixMethod() call
40	      theNSelected.reserve(100000);
41	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
42	       struct NSelected theNSelElement;
43	       for(unsigned int iso_index=0; iso_index<4; iso_index++){
44	         theNSelElement.NSel[iso_index] = 0.;
45	       }
46	       theNSelected.push_back(theNSelElement);
47	      }
48	
49	      IsoNames[0] = "LL";
50	      IsoNames[1] = "LT";
51	      IsoNames[2] = "TL";
52	      IsoNames[3] = "TT";
53	
54	      //Reinitialized to a value, during RunTheMatrixMethod() call, for each bin, each isolation and for each iteration a new value is filled
55	      theDistributions.reserve(100000);
56	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
57	       std::stringstream ss;
58	       ss << bin_index;
59	       struct Distribution theDistributionElement;
60	       for(unsigned int iso_index=0; iso_index<4; iso_index++){
61	        theDistributionElement.NMMEstimatedTTDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"TTDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"TTDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
62	        theDistributionElement.NMMEstimatedTFDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"TFDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"TFDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
63	        theDistributionElement.NMMEstimatedFTDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"FTDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"FTDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
64	        theDistributionElement.NMMEstimatedFFDistribution[iso_index] = new TH1F(("NMMEstimated"+IsoNames[iso_index]+theChannel+"FFDistribution_"+(ss.str())).c_str(), ("NMMEstimated"+IsoNames[iso_index]+theChannel+"FFDistribution_"+(ss.str())).c_str(),  2000, -1000, 1000);
65	       }
66	       theDistributions.push_back(theDistributionElement);
67	      }
68	
69	
70	      //Initialized only here, once! Then filled elsewhere
71	      theMMEstimatedValues.reserve(100000);
72	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
73	       struct MMEstimated theMMEstimatedValuesElement;        
74	       for(unsigned int iso_index=0; iso_index<4; iso_index++){
75	        theMMEstimatedValuesElement.NofMMEstimatedTT[iso_index]= 0.;
76	        theMMEstimatedValuesElement.MMEstimatedTTErr[iso_index]= 0.;
77	        theMMEstimatedValuesElement.NofMMEstimatedTF[iso_index]= 0.;
78	        theMMEstimatedValuesElement.MMEstimatedTFErr[iso_index]= 0.;
79	        theMMEstimatedValuesElement.NofMMEstimatedFT[iso_index]= 0.;
80	        theMMEstimatedValuesElement.MMEstimatedFTErr[iso_index]= 0.;
81	        theMMEstimatedValuesElement.NofMMEstimatedFF[iso_index]= 0.;
82	        theMMEstimatedValuesElement.MMEstimatedFFErr[iso_index]= 0.;
83	       }
84	       theMMEstimatedValues.push_back(theMMEstimatedValuesElement);
85	      }
86	
87	      //Initialized only here, once! Then filled elsewhere
88	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
89	       theMMEstimatedPlots.MMEstimated_TT[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TT").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TT").c_str(), theNBins, theMinBin, theMaxBin);
90	       theMMEstimatedPlots.MMEstimated_TF[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TF").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_TF").c_str(), theNBins, theMinBin, theMaxBin);
91	       theMMEstimatedPlots.MMEstimated_FT[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FT").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FT").c_str(), theNBins, theMinBin, theMaxBin);
92	       theMMEstimatedPlots.MMEstimated_FF[iso_index] = new TH1F(("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FF").c_str(), ("MMEstimated_"+IsoNames[iso_index]+theChannel+"_FF").c_str(), theNBins, theMinBin, theMaxBin);
93	      }
94	
95	
96	      //Initialized only here, once! Then filled elsewhere
97	      theMMExpectedPlots.reserve(100000);
98	      {
99	       struct MMExpectedPlots tmpMMExpectedPlot;
100	       for(unsigned int iso_index=0; iso_index<4; iso_index++){
101	         tmpMMExpectedPlot.Name[iso_index] = "TTbarSignal";
102	         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str(), theNBins, theMinBin, theMaxBin); 
103	         h->Sumw2();
104	         tmpMMExpectedPlot.MMExpected[iso_index] = h;
105	         h = 0;
106	         delete h;        
107	       }
108	       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
109	      }
110	
111	
112	      {
113	       struct MMExpectedPlots tmpMMExpectedPlot;
114	       for(unsigned int iso_index=0; iso_index<4; iso_index++){
115	         tmpMMExpectedPlot.Name[iso_index] = "TTbarSemileptonic";
116	         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str(), theNBins, theMinBin, theMaxBin); 
117	         h->Sumw2();
118	         tmpMMExpectedPlot.MMExpected[iso_index] = h;
119	         h = 0;
120	         delete h;        
121	       }
122	       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
123	      }
124	
125	      for(unsigned int dataset_index=0; dataset_index < datasets.size(); dataset_index++){
126	       struct MMExpectedPlots tmpMMExpectedPlot;
127	       for(unsigned int iso_index=0; iso_index<4; iso_index++){
128	         tmpMMExpectedPlot.Name[iso_index] = datasets[dataset_index].Name();
129	         TH1F *h = new TH1F(("MMExpected_"+IsoNames[iso_index]+theChannel+"_"+datasets[dataset_index].Name()).c_str(),("MMExpected_"+IsoNames[iso_index]+theChannel+"_"+datasets[dataset_index].Name()).c_str(), theNBins, theMinBin, theMaxBin); 
130	         h->Sumw2();
131	         tmpMMExpectedPlot.MMExpected[iso_index] = h;
132	         h = 0;
133	         delete h;        
134	       }
135	       theMMExpectedPlots.push_back(tmpMMExpectedPlot);
136	      }
137	
138	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
139	       histoN[iso_index] = new TH1F(("histoN"+IsoNames[iso_index]+theChannel).c_str(), ("histoN"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
140	       histoTT[iso_index] = new TH1F(("histoTT"+IsoNames[iso_index]+theChannel).c_str(), ("histoTT"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
141	       histoTF[iso_index] = new TH1F(("histoTF"+IsoNames[iso_index]+theChannel).c_str(), ("histoTF"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
142	       histoFT[iso_index] = new TH1F(("histoFT"+IsoNames[iso_index]+theChannel).c_str(), ("histoFT"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
143	       histoFF[iso_index] = new TH1F(("histoFF"+IsoNames[iso_index]+theChannel).c_str(), ("histoFF"+IsoNames[iso_index]+theChannel).c_str(), theNBins, theMinBin, theMaxBin);
144	      }
145	
146	
147	}
148	
149	GeneralMMEstimation::~GeneralMMEstimation(){
150	
151	      for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
152	        for(unsigned int iso_index=0; iso_index<4; iso_index++){
153	         delete theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index];
154	         delete theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index];
155	         delete theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index];
156	         delete theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index];
157	       }
158	      }
159	
160	
161	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
162	       delete theMMEstimatedPlots.MMEstimated_TT[iso_index];
163	       delete theMMEstimatedPlots.MMEstimated_TF[iso_index];
164	       delete theMMEstimatedPlots.MMEstimated_FT[iso_index];
165	       delete theMMEstimatedPlots.MMEstimated_FF[iso_index];
166	      }
167	
168	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
169	       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
170	         delete theMMExpectedPlots[i].MMExpected[iso_index];
171	       }
172	      }
173	
174	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
175	       delete histoN[iso_index];
176	       delete histoTT[iso_index];
177	       delete histoTF[iso_index];
178	       delete histoFT[iso_index];
179	       delete histoFF[iso_index];
180	      }
181	
182	
183	}
184	
185	
186	struct Isolations GeneralMMEstimation::GetIsolations(){
187	  return theIsolations;
188	}
189	
190	
191	void GeneralMMEstimation::IncrementNSelBin(unsigned int iso_index, unsigned int bin_index, float weight){
192	         theNSelected[bin_index].NSel[iso_index] += weight;
193	}
194	
195	void GeneralMMEstimation::CountNSel(const DiLeptonSelection & sel, Dataset dataset, SelectionTable selTable, unsigned int d, float weight, string CandTypeRequested, int selStepCut){
196	
197	    int selStepMM[4]= {-1, -1, -1, -1};
198	    string CandTypeForMM[4]= {"", "", "", ""};
199	    unsigned int njets[4] = {-1, -1, -1, -1};
200	
201	
202	    for (unsigned int iso_index = 0; iso_index<4; iso_index++) {
203	        vector<NTJet> SelectedJetsForMM; bool LepPairForMM[4]; vector<NTElectron> candElecForMM; vector<NTMuon> candMuonForMM;
204	        DiLeptonSelection sel_aux(sel);
205	        selStepMM[iso_index] = sel_aux.FillTableForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], selTable, &(dataset), d, weight);         
206	        // Be careful if inverted or not (at the moment pairing is applied before isolation)
207	        LepPairForMM[iso_index] = sel_aux.GetLeptonPairForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], candMuonForMM, candElecForMM, CandTypeForMM[iso_index]);
208	        SelectedJetsForMM = sel_aux.GetSelectedJets(candMuonForMM, candElecForMM);
209	        njets[iso_index] = SelectedJetsForMM.size(); //if(iso_index == 3){njets[3]=SelectedJetsForMM.size(); njets[2]=SelectedJetsForMM.size(); njets[1]=SelectedJetsForMM.size(); njets[0]=SelectedJetsForMM.size();}
210	    }
211	    //    for (unsigned int iso_index = 0; iso_index<3; iso_index++) {
212	    //        CandTypeForMM[iso_index] = CandTypeForMM[3];
213	    //    }
214	
215	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
216	        //Here, njets[] is the variable vs. whih the plots of Matrix Method will be done. It could be any other variable!     
217	       if((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut)  && ((njets[3] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[3] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
218	         IncrementNSelBin(3, bin_index, weight);
219	       }
220	       if((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)  && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
221	         IncrementNSelBin(2, bin_index, weight);
222	       }
223	       if((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
224	         IncrementNSelBin(1, bin_index, weight);
225	       }
226	       if((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut) && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1))))){
227	         IncrementNSelBin(0, bin_index, weight);
228	       }
229	
230	
231	
232	
233	        if(    ((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut) && ((njets[3] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[3] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))  && 
234	               (
235	                !((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut) && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 ||
236	                !((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 ||
237	                !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
238	               )
239	          ){ std::cout << "WARNING in CountNSel: Passing Selection TT, but not passing selection TL or LT or LL!" << std::endl; }
240	
241	
242	
243	        if(    ((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut) && ((njets[2] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[2] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 &&
244	               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
245	          ){ std::cout << "WARNING in CountNSel: Passing Selection TL, but not passing selection LL!" << std::endl;  }
246	
247	
248	
249	        if(    ((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut) && ((njets[1] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[1] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))                 &&
250	               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)  && ((njets[0] >= (((theMaxBin-theMinBin)/theNBins)*bin_index)) && (njets[0] < (((theMaxBin-theMinBin)/theNBins)*(bin_index+1)))))
251	          ){ std::cout << "WARNING in CountNSel: Passing Selection LT, but not passing selection LL!" << std::endl;  }
252	
253	
254	
255	
256	
257	     }
258	
259	
260	}
261	
262	
263	
264	void GeneralMMEstimation::FillMMExpected(const DiLeptonSelection & sel, Dataset dataset, SelectionTable selTable, unsigned int d, float weight, string CandTypeRequested, int selStepCut, const NTEvent* event){
265	
266	
267	       int selStepMM[4] = {-1, -1, -1, -1};
268	       string CandTypeForMM[4]= {"", "", "", ""};
269	       unsigned int njets[4] = {-1, -1, -1, -1};
270	
271	
272	       for (unsigned int iso_index = 0; iso_index<4; iso_index++) {
273	        vector<NTJet> SelectedJetsForMM; bool LepPairForMM[4]; vector<NTElectron> candElecForMM; vector<NTMuon> candMuonForMM;
274	        DiLeptonSelection sel_aux(sel);
275	        selStepMM[iso_index] = sel_aux.FillTableForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], selTable, &(dataset), d, weight);         
276	        // Be careful if inverted or not
277	        LepPairForMM[iso_index] = sel_aux.GetLeptonPairForMM(GetIsolations().iso1[iso_index],GetIsolations().iso2[iso_index], candMuonForMM, candElecForMM, CandTypeForMM[iso_index]);
278	        SelectedJetsForMM = sel_aux.GetSelectedJets(candMuonForMM, candElecForMM);
279	        njets[iso_index] = SelectedJetsForMM.size(); //if(iso_index == 3){njets[3]=SelectedJetsForMM.size(); njets[2]=SelectedJetsForMM.size(); njets[1]=SelectedJetsForMM.size(); njets[0]=SelectedJetsForMM.size();}
280	       }
281	       //       for (unsigned int iso_index = 0; iso_index<3; iso_index++) {
282	       //           CandTypeForMM[iso_index] = CandTypeForMM[3];
283	       //       }
284	
285	       //Here, njets[] is the variable vs. whih the plots of Matrix Method will be done. It could be any other variable!     
286	
287	
288	     if((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut)){
289	
290	       if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) && dataset.Name() == "TTbar" // emu
291	       ){
292	         FillMMExpectedPlot("TTbarSignal",3,njets[3],weight);
293	       }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
294	                event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
295	                event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu                
296	                event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
297	                event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
298	       ){
299	         FillMMExpectedPlot("TTbarSemileptonic",3,njets[3],weight);
300	       }else{
301	         FillMMExpectedPlot(dataset.Name(),3,njets[3],weight);
302	       }
303	
304	     }
305	
306	     if((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)){
307	
308	       if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) && dataset.Name() == "TTbar" // emu
309	       ){
310	         FillMMExpectedPlot("TTbarSignal",2,njets[2],weight);
311	       }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
312	                event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
313	                event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
314	                event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
315	                event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
316	       ){
317	         FillMMExpectedPlot("TTbarSemileptonic",2,njets[2],weight);
318	       }else{
319	         FillMMExpectedPlot(dataset.Name(),2,njets[2],weight);
320	       }
321	
322	     }
323	
324	    if((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut)){
325	
326	        if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100)  && dataset.Name() == "TTbar" // emu
327	        ){
328	          FillMMExpectedPlot("TTbarSignal",1,njets[1],weight);
329	        }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
330	                 event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
331	                 event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
332	                 event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
333	                 event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
334	        ){
335	          FillMMExpectedPlot("TTbarSemileptonic",1,njets[1],weight);
336	        }else{
337	          FillMMExpectedPlot(dataset.Name(),1,njets[1],weight);
338	        }
339	    }
340	        
341	    if((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut)){
342	
343	         if((event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) && dataset.Name() == "TTbar" // emu
344	         ){
345	           FillMMExpectedPlot("TTbarSignal",0,njets[0],weight);
346	         }else if((event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
347	                  event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
348	                  event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
349	                  event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
350	                  event->TMEME == 10100 || event->TMEME == 11000) && dataset.Name() == "TTbar" // tau(->e/mu)jets
351	         ){
352	           FillMMExpectedPlot("TTbarSemileptonic",0,njets[0],weight);
353	         }else{
354	           FillMMExpectedPlot(dataset.Name(),0,njets[0],weight);
355	         }
356	    }      
357	
358	      
359	
360	
361	
362	
363	
364	
365	
366	        if(    ((CandTypeForMM[3] == CandTypeRequested) && (selStepMM[3] >= selStepCut))  && 
367	               (
368	                !((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut)) ||
369	                !((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut)) ||
370	                !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
371	               )
372	          ){ std::cout << "WARNING in FillMMExpected: Passing Selection TT, but not passing selection TL or LT or LL!" << std::endl;  }
373	
374	
375	
376	        if(    ((CandTypeForMM[2] == CandTypeRequested) && (selStepMM[2] >= selStepCut))  &&
377	               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
378	          ){ std::cout << "WARNING in FillMMExpected: Passing Selection TL, but not passing selection LL!" << std::endl;  }
379	
380	
381	
382	        if(    ((CandTypeForMM[1] == CandTypeRequested) && (selStepMM[1] >= selStepCut))  &&
383	               !((CandTypeForMM[0] == CandTypeRequested) && (selStepMM[0] >= selStepCut))
384	          ){ std::cout << "WARNING in FillMMExpected: Passing Selection LT, but not passing selection LL!" << std::endl;  }
385	
386	
387	
388	
389	
390	
391	
392	
393	}
394	
395	
396	
397	
398	
399	void GeneralMMEstimation::IncludeSystematicError(){
400	 float EpsilonEFake0 = EpsilonEFake;
401	 float EpsilonESignal0  = EpsilonESignal;
402	 EpsilonEFake = ranEpsEFake.Gaus(EpsilonEFake0, EpsilonEFakeErr);
403	 EpsilonESignal  = ranEpsESignal.Gaus(EpsilonESignal0,  EpsilonESignalErr);
404	 float EpsilonMuFake0 = EpsilonMuFake;
405	 float EpsilonMuSignal0  = EpsilonMuSignal;
406	 EpsilonMuFake = ranEpsMuFake.Gaus(EpsilonMuFake0, EpsilonMuFakeErr);
407	 EpsilonMuSignal  = ranEpsMuSignal.Gaus(EpsilonMuSignal0,  EpsilonMuSignalErr);
408	}
409	
410	
411	void GeneralMMEstimation::IncludeStatisticalError(){
412	 float N4 = locNSelected[3];
413	 float N3;
414	 float N2;
415	 if(locNSelected[2] <= locNSelected[1]){
416	  N3 = locNSelected[2] - N4;
417	  N2 = locNSelected[1] - N3 - N4;
418	 }else{
419	  N3 = locNSelected[1] - N4;
420	  N2 = locNSelected[2] - N3 - N4;
421	 }
422	 float N1 = locNSelected[0] - N2 - N3 -N4;
423	 N1 = ranN1.Poisson(N1);
424	 N2 = ranN2.Poisson(N2);
425	 N3 = ranN3.Poisson(N3);
426	 N4 = ranN4.Poisson(N4);
427	 locNSelected[3] = N4;
428	 if(locNSelected[2] <= locNSelected[1]){
429	  locNSelected[2] = N3+N4;
430	  locNSelected[1] = N2+N3+N4;
431	 }else{
432	  locNSelected[1] = N3+N4;
433	  locNSelected[2] = N2+N3+N4;
434	 }
435	 locNSelected[0] = N1+N2+N3+N4;
436	}
437	
438	
439	void GeneralMMEstimation::SolveTheSystem(bool doCorrections){
440	
441	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
442	        NMMEstimatedTT[iso_index] = 0;
443	        NMMEstimatedTF[iso_index] = 0;
444	        NMMEstimatedFT[iso_index] = 0;
445	        NMMEstimatedFF[iso_index] = 0;
446	      }
447	
448	
449	      TMatrixF theEffmatrix(4,4); // A
450	      TMatrixF solutions(4,1);    // x
451	      TMatrixF mesured(4,1);      // b
452	
453	      float eff[16] ;  // A
454	
455	      float Eff_llttTT   = EpsilonESignal * EpsilonMuSignal;
456	      float Eff_llttFT   = EpsilonEFake * EpsilonMuSignal;
457	      float Eff_llttTF   = EpsilonESignal * EpsilonMuFake;
458	      float Eff_llttFF   = EpsilonEFake * EpsilonMuFake;
459	
460	      float Eff_lltlTT   = EpsilonESignal;
461	      float Eff_lltlFT   = EpsilonEFake;
462	      float Eff_lltlTF   = EpsilonESignal;
463	      float Eff_lltlFF   = EpsilonEFake;
464	
465	      float Eff_llltTT   = EpsilonMuSignal;
466	      float Eff_llltFT   = EpsilonMuSignal;
467	      float Eff_llltTF   = EpsilonMuFake;
468	      float Eff_llltFF   = EpsilonMuFake;
469	
470	      eff[0] = 1.;
471	      eff[1] = 1.;
472	      eff[2] = 1.;
473	      eff[3] = 1.;
474	
475	      eff[4] = Eff_llltFF;
476	      eff[5] = Eff_llltTF;
477	      eff[6] = Eff_llltFT;
478	      eff[7] = Eff_llltTT;
479	
480	      eff[8] = Eff_lltlFF;
481	      eff[9] = Eff_lltlTF;
482	      eff[10] = Eff_lltlFT;
483	      eff[11] = Eff_lltlTT;
484	
485	      eff[12] = Eff_llttFF;
486	      eff[13] = Eff_llttTF;
487	      eff[14] = Eff_llttFT;
488	      eff[15] = Eff_llttTT;
489	      
490	      theEffmatrix.SetMatrixArray(eff);
491	      theEffmatrix.Invert();
492	
493	      float ArrayMes[4] ; // b
494	      ArrayMes[0] = locNSelected[0];
495	      ArrayMes[1] = locNSelected[1];
496	      ArrayMes[2] = locNSelected[2];
497	      ArrayMes[3] = locNSelected[3];
498	
499	      mesured.SetMatrixArray(ArrayMes);
500	      solutions = theEffmatrix*mesured; 
501	
502	      NMMEstimatedFF[3] = solutions(0,0)*Eff_llttFF;
503	      NMMEstimatedTF[3] = solutions(1,0)*Eff_llttTF;
504	      NMMEstimatedFT[3] = solutions(2,0)*Eff_llttFT;
505	      NMMEstimatedTT[3] = solutions(3,0)*Eff_llttTT;
506	
507	      NMMEstimatedFF[2] = solutions(0,0)*Eff_lltlFF;
508	      NMMEstimatedTF[2] = solutions(1,0)*Eff_lltlTF;
509	      NMMEstimatedFT[2] = solutions(2,0)*Eff_lltlFT;
510	      NMMEstimatedTT[2] = solutions(3,0)*Eff_lltlTT;
511	
512	      NMMEstimatedFF[1] = solutions(0,0)*Eff_llltFF;
513	      NMMEstimatedTF[1] = solutions(1,0)*Eff_llltTF;
514	      NMMEstimatedFT[1] = solutions(2,0)*Eff_llltFT;
515	      NMMEstimatedTT[1] = solutions(3,0)*Eff_llltTT;
516	
517	      NMMEstimatedFF[0] = solutions(0,0);
518	      NMMEstimatedTF[0] = solutions(1,0);
519	      NMMEstimatedFT[0] = solutions(2,0);
520	      NMMEstimatedTT[0] = solutions(3,0);
521	
522	      if(doCorrections){
523	
524	        float EpsilonESignal_bar = 1 - EpsilonESignal;
525	        float EpsilonMuSignal_bar = 1 - EpsilonMuSignal;
526	
527	        NMMEstimatedTT[3] = NMMEstimatedTT[3]
528	                          + NMMEstimatedTT[0] * EpsilonESignal * EpsilonMuSignal_bar * EpsilonMuFake
529	                          + NMMEstimatedTT[0] * EpsilonMuSignal * EpsilonESignal_bar * EpsilonEFake
530	                          + NMMEstimatedTT[0] * EpsilonESignal_bar * EpsilonMuSignal_bar * EpsilonEFake * EpsilonMuFake;
531	
532	        NMMEstimatedFT[3] = NMMEstimatedFT[3]
533	                          - NMMEstimatedTT[0] * EpsilonESignal * EpsilonMuSignal_bar * EpsilonMuFake
534	                          + NMMEstimatedFT[0] * EpsilonMuFake * EpsilonESignal_bar * EpsilonEFake;
535	
536	        NMMEstimatedTF[3] = NMMEstimatedTF[3]
537	                          - NMMEstimatedTT[0] * EpsilonMuSignal * EpsilonESignal_bar * EpsilonEFake
538	                          + NMMEstimatedTF[0] * EpsilonEFake * EpsilonMuSignal_bar * EpsilonMuFake;
539	
540	        NMMEstimatedFF[3] = NMMEstimatedFF[3]
541	                          - NMMEstimatedTT[0] * EpsilonESignal_bar * EpsilonMuSignal_bar * EpsilonEFake * EpsilonMuFake
542	                          - NMMEstimatedFT[0] * EpsilonMuFake * EpsilonESignal_bar * EpsilonEFake
543	                          - NMMEstimatedTF[0] * EpsilonEFake * EpsilonMuSignal_bar * EpsilonMuFake;
544	      }
545	
546	
547	}
548	
549	void GeneralMMEstimation::FillDistributions(unsigned int bin_index){
550	
551	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
552	          theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Fill(NMMEstimatedTT[iso_index]);
553	          theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Fill(NMMEstimatedTF[iso_index]);
554	          theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Fill(NMMEstimatedFT[iso_index]);
555	          theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Fill(NMMEstimatedFF[iso_index]);
556	          theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index] += NMMEstimatedTT[iso_index];
557	          theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index] += NMMEstimatedTF[iso_index];
558	          theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index] += NMMEstimatedFT[iso_index];
559	          theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index] += NMMEstimatedFF[iso_index];
560	    }
561	}
562	
563	
564	void GeneralMMEstimation::CalculateRms(unsigned int bin_index, unsigned int NbIterations){
565	
566	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
567	          theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index]/NbIterations)-NMMEstimatedTT[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index]/NbIterations)-NMMEstimatedTT[iso_index]);
568	          theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index]/NbIterations)-NMMEstimatedTF[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index]/NbIterations)-NMMEstimatedTF[iso_index]);
569	          theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index]/NbIterations)-NMMEstimatedFT[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index]/NbIterations)-NMMEstimatedFT[iso_index]);
570	          theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index] += ((theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index]/NbIterations)-NMMEstimatedFF[iso_index])*((theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index]/NbIterations)-NMMEstimatedFF[iso_index]);
571	    }
572	}
573	
574	
575	
576	void GeneralMMEstimation::SetMMEstimated(unsigned int bin_index, unsigned int NbIterations){
577	
578	     for(unsigned int iso_index=0; iso_index<4; iso_index++){
579	      theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index])/NbIterations;
580	      theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index])/(NbIterations-1));
581	      theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index])/NbIterations;
582	      theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index])/(NbIterations-1));
583	      theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index])/NbIterations;
584	      theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index])/(NbIterations-1));
585	      theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index] = (theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index])/NbIterations;
586	      theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index] = sqrt((theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index])/(NbIterations-1));
587	     }
588	
589	}
590	
591	
592	void GeneralMMEstimation::ReadMMFile(string fileName){
593	
594	  TFile* file_MM   = new TFile((fileName).c_str());
595	  file_MM->cd();
596	  
597	  TH1F* tmpMMNSelected[4];
598	  for(unsigned int iso_index=0; iso_index<4; iso_index++){
599	   tmpMMNSelected[iso_index]  = (TH1F*)gDirectory->Get(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str());
600	  }
601	  for(unsigned int iso_index=0; iso_index<4; iso_index++){
602	   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
603	    theNSelected[bin_index].NSel[iso_index] = tmpMMNSelected[iso_index]->GetBinContent(bin_index+1) ;
604	   }
605	  }
606	  for(unsigned int iso_index=0; iso_index<4; iso_index++){
607	   tmpMMNSelected[iso_index] = 0;
608	   delete tmpMMNSelected[iso_index];
609	  }
610	
611	  file_MM->Close();
612	  delete file_MM;
613	
614	}
615	
616	
617	
618	
619	void GeneralMMEstimation::ReadMMFileForPullTest(string fileName, float epsilon_s_e, float epsilon_f_e, float epsilon_s_mu, float epsilon_f_mu){
620	
621	  TFile* file_MM   = new TFile((fileName).c_str());
622	  file_MM->cd();
623	
624	  TH1F* histoTmpTT[4];
625	  TH1F* histoTmpTF[4];
626	  TH1F* histoTmpFT[4];
627	  TH1F* histoTmpFF[4];
628	  
629	  for(unsigned int iso_index=0; iso_index<4; iso_index++){
630	   histoTmpTT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str());
631	   histoTmpTF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
632	   histoTmpFT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
633	   histoTmpFF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbar").c_str());
634	   histoTmpTF[iso_index]->Add(histoTmpTF[iso_index], histoTmpTF[iso_index], 0, 0.5);
635	   histoTmpFT[iso_index]->Add(histoTmpFT[iso_index], histoTmpFT[iso_index], 0, 0.5);
636	  }  
637	
638	  for(unsigned int iso_index=0; iso_index<4; iso_index++){
639	   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
640	     histoTT[iso_index]->SetBinContent(bin_index+1, histoTmpTT[iso_index]->GetBinContent(bin_index+1));
641	     histoTF[iso_index]->SetBinContent(bin_index+1, histoTmpTF[iso_index]->GetBinContent(bin_index+1));
642	     histoFT[iso_index]->SetBinContent(bin_index+1, histoTmpFT[iso_index]->GetBinContent(bin_index+1));
643	     histoFF[iso_index]->SetBinContent(bin_index+1, histoTmpFF[iso_index]->GetBinContent(bin_index+1));
644	   }
645	  }  
646	
647	  //LT
648	  histoTT[1]->Add(histoTT[0], histoTT[0], 0, epsilon_s_mu);
649	  histoTF[1]->Add(histoTF[0], histoTF[0], 0, epsilon_f_mu);
650	  histoFT[1]->Add(histoFT[0], histoFT[0], 0, epsilon_s_mu);
651	  histoFF[1]->Add(histoFF[0], histoFF[0], 0, epsilon_f_mu);
652	
653	  //TL
654	  histoTT[2]->Add(histoTT[0], histoTT[0], 0, epsilon_s_e);
655	  histoTF[2]->Add(histoTF[0], histoTF[0], 0, epsilon_s_e);
656	  histoFT[2]->Add(histoFT[0], histoFT[0], 0, epsilon_f_e);
657	  histoFF[2]->Add(histoFF[0], histoFF[0], 0, epsilon_f_e);
658	
659	  //TT
660	  histoTT[3]->Add(histoTT[0], histoTT[0], 0, epsilon_s_e * epsilon_s_mu);
661	  histoTF[3]->Add(histoTF[0], histoTF[0], 0, epsilon_s_e * epsilon_f_mu);
662	  histoFT[3]->Add(histoFT[0], histoFT[0], 0, epsilon_f_e * epsilon_s_mu);
663	  histoFF[3]->Add(histoFF[0], histoFF[0], 0, epsilon_f_e * epsilon_f_mu);
664	
665	
666	  histoN[0]->Add(histoTT[0], histoTF[0], 1, 1);
667	  histoN[0]->Add(histoN[0], histoFT[0], 1, 1);
668	  histoN[0]->Add(histoN[0], histoFF[0], 1, 1);
669	
670	  histoN[1]->Add(histoTT[1], histoTF[1], 1, 1);
671	  histoN[1]->Add(histoN[1], histoFT[1], 1, 1);
672	  histoN[1]->Add(histoN[1], histoFF[1], 1, 1);
673	
674	  histoN[2]->Add(histoTT[2], histoTF[2], 1, 1);
675	  histoN[2]->Add(histoN[2], histoFT[2], 1, 1);
676	  histoN[2]->Add(histoN[2], histoFF[2], 1, 1);
677	
678	  histoN[3]->Add(histoTT[3], histoTF[3], 1, 1);
679	  histoN[3]->Add(histoN[3], histoFT[3], 1, 1);
680	  histoN[3]->Add(histoN[3], histoFF[3], 1, 1);
681	
682	  for(unsigned int iso_index=0; iso_index<4; iso_index++){
683	   for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
684	    theNSelected[bin_index].NSel[iso_index] = histoN[iso_index]->GetBinContent(bin_index+1) ;
685	   }
686	  }
687	
688	 
689	  for(unsigned int iso_index=0; iso_index<4; iso_index++){
690	   histoTmpTT[iso_index] = 0;
691	   delete histoTmpTT[iso_index];
692	   histoTmpTF[iso_index] = 0;
693	   delete histoTmpTF[iso_index];
694	   histoTmpFT[iso_index] = 0;
695	   delete histoTmpFT[iso_index];
696	   histoTmpFF[iso_index] = 0;
697	   delete histoTmpFF[iso_index];
698	  }
699	
700	  file_MM->Close();
701	  delete file_MM;
702	
703	}
704	
705	
706	
707	
708	
709	
710	
711	
712	
713	
714	void GeneralMMEstimation::RunTheMatrixMethod(vector<struct MMEpsilons> valMMEpsilons, unsigned int NbIterations, bool doStatistical, bool doSystematic, bool doCorrections){
715	
716	
717	  for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
718	
719	   for(unsigned int iso_index=0; iso_index<4; iso_index++){
720	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
721	     theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->SetBinContent(i+1, 0.);
722	    }
723	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
724	     theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->SetBinContent(i+1, 0.);
725	    }
726	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
727	     theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->SetBinContent(i+1, 0.);
728	    }
729	    for(unsigned int i=0; i< (unsigned int) theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->GetXaxis()->GetNbins(); i++){
730	     theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->SetBinContent(i+1, 0.);
731	    }
732	   }
733	
734	
735	
736	   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
737	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
738	       locNSelected[iso_index] = theNSelected[bin_index].NSel[iso_index];
739	      }
740	      EpsilonEFake = valMMEpsilons[bin_index].EpsilonEFake;
741	      EpsilonEFakeErr = valMMEpsilons[bin_index].EpsilonEFakeErr;
742	      EpsilonESignal = valMMEpsilons[bin_index].EpsilonESignal;
743	      EpsilonESignalErr = valMMEpsilons[bin_index].EpsilonESignalErr;
744	
745	      EpsilonMuFake = valMMEpsilons[bin_index].EpsilonMuFake;
746	      EpsilonMuFakeErr = valMMEpsilons[bin_index].EpsilonMuFakeErr;
747	      EpsilonMuSignal = valMMEpsilons[bin_index].EpsilonMuSignal;
748	      EpsilonMuSignalErr = valMMEpsilons[bin_index].EpsilonMuSignalErr;
749	
750	      if(doStatistical) IncludeStatisticalError();
751	      if(doSystematic) IncludeSystematicError();
752	      SolveTheSystem(doCorrections);
753	      FillDistributions(bin_index);
754	   }
755	
756	
757	   for (unsigned int i=0 ; i<NbIterations ; i++ ) {   // pseudoexperiments
758	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
759	       locNSelected[iso_index] = theNSelected[bin_index].NSel[iso_index];
760	      }
761	      EpsilonEFake = valMMEpsilons[bin_index].EpsilonEFake;
762	      EpsilonEFakeErr = valMMEpsilons[bin_index].EpsilonEFakeErr;
763	      EpsilonESignal = valMMEpsilons[bin_index].EpsilonESignal;
764	      EpsilonESignalErr = valMMEpsilons[bin_index].EpsilonESignalErr;
765	
766	      EpsilonMuFake = valMMEpsilons[bin_index].EpsilonMuFake;
767	      EpsilonMuFakeErr = valMMEpsilons[bin_index].EpsilonMuFakeErr;
768	      EpsilonMuSignal = valMMEpsilons[bin_index].EpsilonMuSignal;
769	      EpsilonMuSignalErr = valMMEpsilons[bin_index].EpsilonMuSignalErr;
770	
771	      if(doStatistical) IncludeStatisticalError();
772	      if(doSystematic) IncludeSystematicError();
773	      SolveTheSystem(doCorrections);
774	      CalculateRms(bin_index, NbIterations); 
775	   }
776	
777	 
778	   SetMMEstimated(bin_index, NbIterations);
779	
780	   for(unsigned int iso_index=0; iso_index<4; iso_index++){
781	    theMMEstimatedPlots.MMEstimated_TT[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedTT[iso_index] );
782	    theMMEstimatedPlots.MMEstimated_TT[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedTTErr[iso_index] );
783	    theMMEstimatedPlots.MMEstimated_TF[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedTF[iso_index] );
784	    theMMEstimatedPlots.MMEstimated_TF[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedTFErr[iso_index] );
785	    theMMEstimatedPlots.MMEstimated_FT[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedFT[iso_index] );
786	    theMMEstimatedPlots.MMEstimated_FT[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedFTErr[iso_index] );
787	    theMMEstimatedPlots.MMEstimated_FF[iso_index]->SetBinContent(bin_index+1, theMMEstimatedValues[bin_index].NofMMEstimatedFF[iso_index] );
788	    theMMEstimatedPlots.MMEstimated_FF[iso_index]->SetBinError(bin_index+1, theMMEstimatedValues[bin_index].MMEstimatedFFErr[iso_index] );
789	   }
790	
791	  }
792	
793	
794	
795	}
796	
797	
798	vector<struct MMEstimated> GeneralMMEstimation::GetMMEstimated(){
799	return theMMEstimatedValues;
800	}
801	
802	
803	void GeneralMMEstimation::PrintMMEstimated(){
804	
805	   std::cout << "\\begin{table}" << std::endl;
806	   std::cout << "\\begin{center}" << std::endl;
807	   std::cout << "\\begin{tabular}{|";
808	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
809	    std::cout << " c |";
810	   }
811	   std::cout << "}" << std::endl;
812	   std::cout << "\\hline" << std::endl;
813	
814	
815	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
816	    std::cout << " & Njets = " << bin_index;
817	   }
818	   std::cout << "\\\\ \\hline" << std::endl;
819	
820	   std::cout << "N of TT-like";
821	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
822	    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedTT[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedTTErr[3];
823	   }
824	   std::cout << "\\\\ \\hline" << std::endl;
825	
826	   std::cout << "N of TF-like";
827	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
828	    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedTF[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedTFErr[3];
829	   }
830	   std::cout << "\\\\ \\hline" << std::endl;
831	
832	   std::cout << "N of FT-like";
833	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
834	    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedFT[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedFTErr[3];
835	   }
836	   std::cout << "\\\\ \\hline" << std::endl;
837	
838	   std::cout << "N of FF-like";
839	   for(unsigned int bin_index=2; bin_index<theNBins; bin_index++){
840	    std::cout << " & " << theMMEstimatedValues[bin_index].NofMMEstimatedFF[3] << "$\\pm$" << theMMEstimatedValues[bin_index].MMEstimatedFFErr[3];
841	   }
842	   std::cout << "\\\\ \\hline" << std::endl;
843	
844	   std::cout << "\\end{tabular}" << std::endl;
845	   std::cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
846	   std::cout << "\\end{center}" << std::endl;
847	   std::cout << "\\end{table}" << std::endl;
848	
849	   float NofMMEstimatedTTTOTAL = 0;
850	   float MMEstimatedTTErrTOTAL = 0;
851	   float NofMMEstimatedTFTOTAL = 0;
852	   float MMEstimatedTFErrTOTAL = 0;
853	   float NofMMEstimatedFTTOTAL = 0;
854	   float MMEstimatedFTErrTOTAL = 0;
855	   float NofMMEstimatedFFTOTAL = 0;
856	   float MMEstimatedFFErrTOTAL = 0;
857	
858	   for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
859	    NofMMEstimatedTTTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedTT[3];
860	    MMEstimatedTTErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedTTErr[3]*theMMEstimatedValues[bin_index].MMEstimatedTTErr[3];
861	    NofMMEstimatedTFTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedTF[3];
862	    MMEstimatedTFErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedTFErr[3]*theMMEstimatedValues[bin_index].MMEstimatedTFErr[3];
863	    NofMMEstimatedFTTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedFT[3];
864	    MMEstimatedFTErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedFTErr[3]*theMMEstimatedValues[bin_index].MMEstimatedFTErr[3];
865	    NofMMEstimatedFFTOTAL += theMMEstimatedValues[bin_index].NofMMEstimatedFF[3];
866	    MMEstimatedFFErrTOTAL += theMMEstimatedValues[bin_index].MMEstimatedFFErr[3]*theMMEstimatedValues[bin_index].MMEstimatedFFErr[3];
867	   }
868	
869	   std::cout << "NofMMEstimatedTTTOTAL: " << NofMMEstimatedTTTOTAL << std::endl;
870	   std::cout << "MMEstimatedTTErrTOTAL: " << sqrt(MMEstimatedTTErrTOTAL) << std::endl;
871	   std::cout << "NofMMEstimatedTFTOTAL: " << NofMMEstimatedTFTOTAL << std::endl;
872	   std::cout << "MMEstimatedTFErrTOTAL: " << sqrt(MMEstimatedTFErrTOTAL) << std::endl;
873	   std::cout << "NofMMEstimatedFTTOTAL: " << NofMMEstimatedFTTOTAL << std::endl;
874	   std::cout << "MMEstimatedFTErrTOTAL: " << sqrt(MMEstimatedFTErrTOTAL) << std::endl;
875	   std::cout << "NofMMEstimatedFFTOTAL: " << NofMMEstimatedFFTOTAL << std::endl;
876	   std::cout << "MMEstimatedFFErrTOTAL: " << sqrt(MMEstimatedFFErrTOTAL) << std::endl;
877	
878	}
879	
880	
881	struct MMEstimatedPlots GeneralMMEstimation::GetMMEstimatedPlots(){
882	 return theMMEstimatedPlots;
883	}
884	
885	
886	void GeneralMMEstimation::FillMMExpectedPlot(string datasetname, unsigned int iso_index, float val, float weight){
887	
888	       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
889	         if(datasetname == theMMExpectedPlots[i].Name[iso_index]) (theMMExpectedPlots[i].MMExpected[iso_index])->Fill(val, weight);
890	       }
891	
892	}
893	
894	vector <struct MMExpectedPlots> GeneralMMEstimation::GetMMExpectedPlots(){
895	  return theMMExpectedPlots;
896	}
897	
898	
899	
900	
901	void GeneralMMEstimation::WriteMMFile(string fileName){
902	
903	    TFile* file_MM = new TFile((fileName).c_str(),"RECREATE");
904	    file_MM->cd();
905	
906	    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
907	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
908	        theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Write();
909	        theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Write();
910	        theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Write();
911	        theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Write();
912	      }
913	    }
914	
915	
916	
917	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
918	       for(unsigned int i=0;i< theMMExpectedPlots.size(); i++){
919	         (theMMExpectedPlots[i].MMExpected[iso_index])->Write();
920	       }
921	    }
922	
923	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
924	      theMMEstimatedPlots.MMEstimated_TT[iso_index]->Write();
925	      theMMEstimatedPlots.MMEstimated_TF[iso_index]->Write();
926	      theMMEstimatedPlots.MMEstimated_FT[iso_index]->Write();
927	      theMMEstimatedPlots.MMEstimated_FF[iso_index]->Write();
928	    }
929	
930	    // Writing histos contaning all information concerning NSelected
931	    TH1F* MMNSelected[4];
932	
933	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
934	      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
935	    }
936	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
937	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
938	       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
939	     }
940	     MMNSelected[iso_index]->Write();
941	    }
942	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
943	     MMNSelected[iso_index] = 0;
944	     delete MMNSelected[iso_index];
945	    }
946	
947	    file_MM->Write();
948	    file_MM->Close();
949	    delete file_MM;
950	
951	
952	}
953	
954	
955	
956	
957	
958	void GeneralMMEstimation::WriteMMFileFast(string fileName){
959	
960	    TFile* file_MM = new TFile((fileName).c_str(),"RECREATE");
961	    file_MM->cd();
962	
963	    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
964	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
965	        theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Write();
966	        theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Write();
967	        theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Write();
968	        theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Write();
969	      }
970	    }
971	
972	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
973	      theMMEstimatedPlots.MMEstimated_TT[iso_index]->Write();
974	      theMMEstimatedPlots.MMEstimated_TF[iso_index]->Write();
975	      theMMEstimatedPlots.MMEstimated_FT[iso_index]->Write();
976	      theMMEstimatedPlots.MMEstimated_FF[iso_index]->Write();
977	    }
978	
979	    // Writing histos contaning all information concerning NSelected
980	    TH1F* MMNSelected[4];
981	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
982	      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
983	    }
984	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
985	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
986	       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
987	     }
988	     MMNSelected[iso_index]->Write();
989	    }
990	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
991	     MMNSelected[iso_index] = 0;
992	     delete MMNSelected[iso_index];
993	    }
994	
995	    file_MM->Write();
996	    file_MM->Close();
997	    delete file_MM;
998	
999	
1000	}
1001	
1002	
1003	
1004	
1005	
1006	
1007	void GeneralMMEstimation::WriteMMFileFastForPullTest(string fileINPUT, string fileOUTPUT, float epsilon_s_e, float epsilon_f_e, float epsilon_s_mu, float epsilon_f_mu ){
1008	
1009	 
1010	    TFile* file_MM_INPUT   = new TFile((fileINPUT).c_str());
1011	    file_MM_INPUT->cd();
1012	     
1013	    TH1F* tmpTT[4];
1014	    TH1F* tmpTF[4];
1015	    TH1F* tmpFT[4];
1016	    TH1F* tmpFF[4];
1017	
1018	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
1019	     tmpTT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSignal").c_str());
1020	     tmpTF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
1021	     tmpFT[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic").c_str());
1022	     tmpFF[iso_index] = (TH1F*)gDirectory->Get(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbar").c_str());
1023	     tmpTF[iso_index]->Add(tmpTF[iso_index], tmpTF[iso_index], 0, 0.5);
1024	     tmpFT[iso_index]->Add(tmpFT[iso_index], tmpFT[iso_index], 0, 0.5);
1025	    }
1026	  
1027	    //LT
1028	    tmpTT[1]->Add(tmpTT[0], tmpTT[0], 0, epsilon_s_mu);
1029	    tmpTF[1]->Add(tmpTF[0], tmpTF[0], 0, epsilon_f_mu);
1030	    tmpFT[1]->Add(tmpFT[0], tmpFT[0], 0, epsilon_s_mu);
1031	    tmpFF[1]->Add(tmpFF[0], tmpFF[0], 0, epsilon_f_mu);
1032	
1033	    //TL
1034	    tmpTT[2]->Add(tmpTT[0], tmpTT[0], 0, epsilon_s_e);
1035	    tmpTF[2]->Add(tmpTF[0], tmpTF[0], 0, epsilon_s_e);
1036	    tmpFT[2]->Add(tmpFT[0], tmpFT[0], 0, epsilon_f_e);
1037	    tmpFF[2]->Add(tmpFF[0], tmpFF[0], 0, epsilon_f_e);
1038	
1039	    //TT
1040	    tmpTT[3]->Add(tmpTT[0], tmpTT[0], 0, epsilon_s_e * epsilon_s_mu);
1041	    tmpTF[3]->Add(tmpTF[0], tmpTF[0], 0, epsilon_s_e * epsilon_f_mu);
1042	    tmpFT[3]->Add(tmpFT[0], tmpFT[0], 0, epsilon_f_e * epsilon_s_mu);
1043	    tmpFF[3]->Add(tmpFF[0], tmpFF[0], 0, epsilon_f_e * epsilon_f_mu);
1044	
1045	    TFile* file_MM_OUTPUT = new TFile((fileOUTPUT).c_str(),"RECREATE");
1046	    file_MM_OUTPUT->cd();
1047	   
1048	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
1049	     tmpTT[iso_index]->Write();
1050	     tmpTF[iso_index]->Write(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic_TF").c_str());
1051	     tmpFT[iso_index]->Write(("MMExpected_"+IsoNames[iso_index]+theChannel+"_TTbarSemileptonic_FT").c_str());
1052	     tmpFF[iso_index]->Write();
1053	     tmpTT[iso_index] = 0;
1054	     tmpTF[iso_index] = 0;
1055	     tmpFT[iso_index] = 0;
1056	     tmpFF[iso_index] = 0;
1057	     delete tmpTT[iso_index];
1058	     delete tmpTF[iso_index];
1059	     delete tmpFT[iso_index];
1060	     delete tmpFF[iso_index];
1061	    }
1062	    
1063	    file_MM_INPUT->Close();
1064	    delete file_MM_INPUT;
1065	
1066	
1067	    for(unsigned int bin_index=0; bin_index<theNBins; bin_index++){
1068	      for(unsigned int iso_index=0; iso_index<4; iso_index++){
1069	        theDistributions[bin_index].NMMEstimatedTTDistribution[iso_index]->Write();
1070	        theDistributions[bin_index].NMMEstimatedTFDistribution[iso_index]->Write();
1071	        theDistributions[bin_index].NMMEstimatedFTDistribution[iso_index]->Write();
1072	        theDistributions[bin_index].NMMEstimatedFFDistribution[iso_index]->Write();
1073	      }
1074	    }
1075	
1076	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
1077	      theMMEstimatedPlots.MMEstimated_TT[iso_index]->Write();
1078	      theMMEstimatedPlots.MMEstimated_TF[iso_index]->Write();
1079	      theMMEstimatedPlots.MMEstimated_FT[iso_index]->Write();
1080	      theMMEstimatedPlots.MMEstimated_FF[iso_index]->Write();
1081	    }
1082	
1083	    // Writing histos contaning all information concerning NSelected
1084	    TH1F* MMNSelected[4];
1085	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
1086	      MMNSelected[iso_index] = new TH1F(("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(), ("MMNSelected_"+IsoNames[iso_index]+theChannel).c_str(),  theNBins, theMinBin, theMaxBin);
1087	    }
1088	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
1089	     for(unsigned int bin_index = 0; bin_index < theNBins; bin_index++){
1090	       MMNSelected[iso_index]->SetBinContent(bin_index+1, theNSelected[bin_index].NSel[iso_index]);
1091	     }
1092	     MMNSelected[iso_index]->Write();
1093	    }
1094	    for(unsigned int iso_index=0; iso_index<4; iso_index++){
1095	     MMNSelected[iso_index] = 0;
1096	     delete MMNSelected[iso_index];
1097	    }
1098	
1099	
1100	    file_MM_OUTPUT->Write();
1101	    file_MM_OUTPUT->Close();
1102	    delete file_MM_OUTPUT;
1103	
1104	
1105	}
