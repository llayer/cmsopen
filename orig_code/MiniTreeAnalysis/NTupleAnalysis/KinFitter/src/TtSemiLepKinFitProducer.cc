1	#include "../interface/TtSemiLepKinFitProducer.h"
2	
3	//template<typename LeptonCollection>
4	TtSemiLepKinFitProducer::TtSemiLepKinFitProducer():
5	  useOnlyMatch_(false),
6	  bTagAlgo_("trackCountingHighEffBJetTags"),    
7	  minBTagValueBJet_(1.0),      
8	  maxBTagValueNonBJet_(3.),   
9	  useBTag_(false),              
10	  maxNJets_(4),              
11	  maxNComb_(1),  //maximum number of jet combinations finally written into the event            
12	  maxNrIter_(500),             
13	  maxDeltaS_(5e-05),             
14	  maxF_(0.0001),             
15	  jetParam_(1),  // select parametrisation : 0: EMom, 1: EtEtaPhi, 2: EtThetaPhi      
16	  lepParam_(1),              
17	  metParam_(1),  
18	  mW_(80.4),               
19	  mTop_(173.),             
20	  jetEnergyResolutionSmearFactor_(1.0)
21	{
22	  constraints_.clear();
23	  constraints_.push_back(1);
24	  constraints_.push_back(3);
25	  //constraints_.push_back(3);
26	  constraints_.push_back(7);
27	  
28	  std::cout <<" " <<std::endl;
29	  std::cout <<"Caution, you are using a hard-coded list of constraints: W(lep) mass, W(had) mass, equal top masses " << std::endl;
30	  std::cout <<"Use the TtSemiLepKinFitProducer(std::vector<unsigned> constr) constructor to change the constraints !"<< std::endl;
31	  std::cout <<" " <<std::endl;
32	  
33	  fitter = new TtSemiLepKinFitter(param(jetParam_), param(lepParam_), param(metParam_), maxNrIter_, maxDeltaS_, maxF_,
34	                                  constraints(constraints_), mW_, mTop_);
35	                                  
36	  
37	  FitResultList.clear();
38	  
39	}
40	
41	//template<typename LeptonCollection>
42	TtSemiLepKinFitProducer::TtSemiLepKinFitProducer(std::vector<unsigned> constr):
43	  useOnlyMatch_(false),
44	  bTagAlgo_("trackCountingHighEffBJetTags"),    
45	  minBTagValueBJet_(1.0),      
46	  maxBTagValueNonBJet_(3.),   
47	  useBTag_(false),              
48	  maxNJets_(4),              
49	  maxNComb_(1),  //maximum number of jet combinations finally written into the event            
50	  maxNrIter_(500),             
51	  maxDeltaS_(5e-05),             
52	  maxF_(0.0001),             
53	  jetParam_(1),  // select parametrisation : 0: EMom, 1: EtEtaPhi, 2: EtThetaPhi      
54	  lepParam_(1),              
55	  metParam_(1),  
56	  mW_(80.4),               
57	  mTop_(173.),             
58	  jetEnergyResolutionSmearFactor_(1.0)
59	 
60	/* AC
61	  match_                   (cfg.getParameter<edm::InputTag>("match")), //??
62	  useOnlyMatch_            (cfg.getParameter<bool>         ("useOnlyMatch"        )),
63	  bTagAlgo_                (cfg.getParameter<std::string>  ("bTagAlgo"            )),
64	  minBTagValueBJet_        (cfg.getParameter<double>       ("minBDiscBJets"       )),
65	  maxBTagValueNonBJet_     (cfg.getParameter<double>       ("maxBDiscLightJets"   )),
66	  useBTag_                 (cfg.getParameter<bool>         ("useBTagging"         )),
67	  maxNJets_                (cfg.getParameter<int>          ("maxNJets"            )),
68	  maxNComb_                (cfg.getParameter<int>          ("maxNComb"            )),
69	  maxNrIter_               (cfg.getParameter<unsigned>     ("maxNrIter"           )),
70	  maxDeltaS_               (cfg.getParameter<double>       ("maxDeltaS"           )),
71	  maxF_                    (cfg.getParameter<double>       ("maxF"                )),
72	  jetParam_                (cfg.getParameter<unsigned>     ("jetParametrisation"  )),
73	  lepParam_                (cfg.getParameter<unsigned>     ("lepParametrisation"  )),
74	  metParam_                (cfg.getParameter<unsigned>     ("metParametrisation"  )),
75	  constraints_             (cfg.getParameter<std::vector<unsigned> >("constraints")),
76	  mW_                      (cfg.getParameter<double>       ("mW"                  )),
77	  mTop_                    (cfg.getParameter<double>       ("mTop"                )),
78	  jetEnergyResolutionSmearFactor_(cfg.getParameter<double> ("jetEnergyResolutionSmearFactor"))
79	  */
80	
81	{
82	  
83	  
84	  constraints_ = constr;
85	  
86	  fitter = new TtSemiLepKinFitter(param(jetParam_), param(lepParam_), param(metParam_), maxNrIter_, maxDeltaS_, maxF_,
87	                                  constraints(constraints_), mW_, mTop_);
88	                                  
89	
90	  FitResultList.clear();
91	
92	  /* AC
93	  produces< std::vector<TLorentzVector> >("PartonsHadP");
94	  produces< std::vector<TLorentzVector> >("PartonsHadQ");
95	  produces< std::vector<TLorentzVector> >("PartonsHadB");
96	  produces< std::vector<TLorentzVector> >("PartonsLepB");
97	  produces< std::vector<TLorentzVector> >("Leptons");
98	  produces< std::vector<TLorentzVector> >("Neutrinos");
99	
100	  produces< std::vector<std::vector<int> > >();
101	  produces< std::vector<double> >("Chi2");
102	  produces< std::vector<double> >("Prob");
103	  produces< std::vector<int> >("Status");*/
104	  
105	}
106	
107	
108	void TtSemiLepKinFitProducer::SetupFitParameters(bool useOnlyMatch,
109	  std::string bTagAlgo, double minBTagValueBJet, double maxBTagValueNonBJet, bool useBTag,              
110	  int maxNJets, int maxNComb, unsigned int  maxNrIter, double maxDeltaS, double maxF,             
111	  unsigned int jetParam, unsigned int lepParam, unsigned int  metParam,              
112	  std::vector<unsigned> constraints, double mW, double mTop, double jetEnergyResolutionSmearFactor)
113	{
114	  useOnlyMatch_        =  useOnlyMatch;
115	  bTagAlgo_            =  bTagAlgo;   
116	  minBTagValueBJet_    =  minBTagValueBJet;     
117	  maxBTagValueNonBJet_ =  maxBTagValueNonBJet;  
118	  useBTag_             =  useBTag;       
119	  maxNJets_            =  maxNJets;             
120	  maxNComb_            =  maxNComb;             
121	  maxNrIter_           =  maxNrIter;            
122	  maxDeltaS_           =  maxDeltaS;            
123	  maxF_                =  maxF;            
124	  jetParam_            =  jetParam;            
125	  lepParam_            =  lepParam;             
126	  metParam_            =  metParam;             
127	  constraints_         =  constraints;                   
128	  mW_                  =  mW;             
129	  mTop_                =  mTop;           
130	 jetEnergyResolutionSmearFactor_  = jetEnergyResolutionSmearFactor;
131	
132	}
133	
134	//template<typename LeptonCollection>
135	TtSemiLepKinFitProducer::~TtSemiLepKinFitProducer()
136	{
137	  delete fitter;
138	}
139	
140	//template<typename LeptonCollection>
141	bool TtSemiLepKinFitProducer::doBTagging(bool& useBTag_, const vector<NTJet>& jets, std::vector<int>& combi,
142	                                                           std::string& bTagAlgo_, double& minBTagValueBJet_, double& maxBTagValueNonBJet_){
143	  
144	  if ( useBTag_ && bTagAlgo_ != "trackCountingHighEffBJetTags") std::cout <<"btagAlgo "<< bTagAlgo_ << " not yet supported " <<std::endl;
145	  
146	  if( !useBTag_ ) {
147	    return true;
148	  }
149	  if( useBTag_ && bTagAlgo_ == "trackCountingHighEffBJetTags" && 
150	      jets[combi[TtSemiLepKinFitter::HadB     ]].TCDiscri >= minBTagValueBJet_ &&
151	      jets[combi[TtSemiLepKinFitter::LepB     ]].TCDiscri >= minBTagValueBJet_ &&
152	      jets[combi[TtSemiLepKinFitter::LightQ   ]].TCDiscri <  maxBTagValueNonBJet_ &&
153	      jets[combi[TtSemiLepKinFitter::LightQBar]].TCDiscri <  maxBTagValueNonBJet_ ) {
154	    return true;
155	  }
156	  else{
157	    return false;
158	  }
159	}
160	
161	//template<typename LeptonCollection>
162	void TtSemiLepKinFitProducer::produce(const vector<NTJet>& jets, const vector<NTLepton>& leps, std::string
163	lepType, const vector<NTMET>& mets)
164	{
165	  /*
166	  std::auto_ptr< std::vector<TLorentzVector> > pPartonsHadP( new std::vector<TLorentzVector> );
167	  std::auto_ptr< std::vector<TLorentzVector> > pPartonsHadQ( new std::vector<TLorentzVector> );
168	  std::auto_ptr< std::vector<TLorentzVector> > pPartonsHadB( new std::vector<TLorentzVector> );
169	  std::auto_ptr< std::vector<TLorentzVector> > pPartonsLepB( new std::vector<TLorentzVector> );
170	  std::auto_ptr< std::vector<TLorentzVector> > pLeptons    ( new std::vector<TLorentzVector> );
171	  std::auto_ptr< std::vector<TLorentzVector> > pNeutrinos  ( new std::vector<TLorentzVector> );
172	
173	  std::auto_ptr< std::vector<std::vector<int> > > pCombi ( new std::vector<std::vector<int> > );
174	  std::auto_ptr< std::vector<double>            > pChi2  ( new std::vector<double> );
175	  std::auto_ptr< std::vector<double>            > pProb  ( new std::vector<double> );
176	  std::auto_ptr< std::vector<int>               > pStatus( new std::vector<int> );
177	
178	  
179	  edm::Handle<std::vector<pat::Jet> > jets;
180	  evt.getByLabel(jets_, jets);
181	
182	  edm::Handle<std::vector<pat::MET> > mets;
183	  evt.getByLabel(mets_, mets);
184	
185	  edm::Handle<LeptonCollection> leps;
186	  evt.getByLabel(leps_, leps);*/
187	
188	  unsigned int nPartons = 4;
189	  
190	  bool invalidMatch = false;
191	  if(useOnlyMatch_) { 
192	    std::cout <<"Original code has not been translated, useOnlyMatch_ not supported !" << std::endl;
193	    }
194	  /* 
195	  std::vector<int> match;
196	  bool invalidMatch = false;
197	  if(useOnlyMatch_) {
198	    edm::Handle<std::vector<std::vector<int> > > matchHandle;
199	    evt.getByLabel(match_, matchHandle);
200	    match = *(matchHandle->begin());
201	    // check if match is valid
202	    if(match.size()!=nPartons) invalidMatch=true;
203	    else {
204	      for(unsigned int idx=0; idx<match.size(); ++idx) {
205	        if(match[idx]<0 || match[idx]>=(int)jets->size()) {
206	          invalidMatch=true;
207	          break;
208	        }
209	      }
210	    }
211	  }*/
212	
213	  // -----------------------------------------------------
214	  // skip events with no appropriate lepton candidate in
215	  // or empty MET or less jets than partons or invalid match
216	  // -----------------------------------------------------
217	
218	  if( leps.empty() || mets.empty() || jets.size()<nPartons || invalidMatch ) {
219	    std::cout <<"skipping events with no appropriate lepton candidate in or empty MET or less jets than partons or invalid match " <<  std::endl;
220	      
221	    /*
222	    // the kinFit getters return empty objects here
223	    pPartonsHadP->push_back( fitter->fittedHadP()     );
224	    pPartonsHadQ->push_back( fitter->fittedHadQ()     );
225	    pPartonsHadB->push_back( fitter->fittedHadB()     );
226	    pPartonsLepB->push_back( fitter->fittedLepB()     );
227	    pLeptons    ->push_back( fitter->fittedLepton()   );
228	    pNeutrinos  ->push_back( fitter->fittedNeutrino() );
229	    // indices referring to the jet combination
230	    std::vector<int> invalidCombi;
231	    for(unsigned int i = 0; i < nPartons; ++i) 
232	      invalidCombi.push_back( -1 );
233	    pCombi->push_back( invalidCombi );
234	    // chi2
235	    pChi2->push_back( -1. );
236	    // chi2 probability
237	    pProb->push_back( -1. );
238	    // status of the fitter
239	    pStatus->push_back( -1 );
240	    // feed out all products
241	    evt.put(pCombi);
242	    evt.put(pPartonsHadP, "PartonsHadP");
243	    evt.put(pPartonsHadQ, "PartonsHadQ");
244	    evt.put(pPartonsHadB, "PartonsHadB");
245	    evt.put(pPartonsLepB, "PartonsLepB");
246	    evt.put(pLeptons    , "Leptons"    );
247	    evt.put(pNeutrinos  , "Neutrinos"  );
248	    evt.put(pChi2       , "Chi2"       );
249	    evt.put(pProb       , "Prob"       );
250	    evt.put(pStatus     , "Status"     );*/
251	    return;
252	  }
253	
254	  // -----------------------------------------------------
255	  // analyze different jet combinations using the KinFitter
256	  // (or only a given jet combination if useOnlyMatch=true)
257	  // -----------------------------------------------------
258	    
259	  std::vector<int> jetIndices;
260	  if(!useOnlyMatch_) {
261	    for(unsigned int i=0; i<jets.size(); ++i){
262	      if(maxNJets_ >= (int) nPartons && maxNJets_ == (int) i) break;
263	      jetIndices.push_back(i);
264	    }
265	  }
266	  
267	  std::vector<int> combi;
268	  for(unsigned int i=0; i<nPartons; ++i) {
269	    //if(useOnlyMatch_) combi.push_back( match[i] );
270	    //else 
271	    combi.push_back(i);
272	  }
273	  
274	  FitResultList.clear();
275	
276	  do{
277	
278	    for(int cnt = 0; cnt < TMath::Factorial( combi.size() ); ++cnt){
279	      // take into account indistinguishability of the two jets from the hadr. W decay,
280	      // reduces combinatorics by a factor of 2
281	      if( (combi[TtSemiLepKinFitter::LightQ] < combi[TtSemiLepKinFitter::LightQBar]
282	         || useOnlyMatch_ ) && doBTagging(useBTag_, jets, combi, bTagAlgo_, minBTagValueBJet_, maxBTagValueNonBJet_) ){
283	        
284	        std::vector<NTJet> jetCombi;
285	        jetCombi.resize(nPartons);
286	        jetCombi[TtSemiLepKinFitter::LightQ   ] = jets[combi[TtSemiLepKinFitter::LightQ   ]];
287	        jetCombi[TtSemiLepKinFitter::LightQBar] = jets[combi[TtSemiLepKinFitter::LightQBar]];
288	        jetCombi[TtSemiLepKinFitter::HadB     ] = jets[combi[TtSemiLepKinFitter::HadB     ]];
289	        jetCombi[TtSemiLepKinFitter::LepB     ] = jets[combi[TtSemiLepKinFitter::LepB     ]];
290	
291	        // do the kinematic fit
292	        int status = fitter->fit(jetCombi, leps[0], lepType, mets[0], jetEnergyResolutionSmearFactor_);
293	
294	        if( status == 0 ) { // only take into account converged fits
295	        
296	        
297	          KinFitResult result;
298	          result.Status = status;
299	          result.Chi2 = fitter->fitS();
300	          result.Prob = fitter->fitProb();
301	          result.HadB = fitter->fittedHadB();
302	          result.HadP = fitter->fittedHadP();
303	          result.HadQ = fitter->fittedHadQ();
304	          result.LepB = fitter->fittedLepB();
305	          result.LepL = fitter->fittedLepton();
306	          result.LepN = fitter->fittedNeutrino();
307	          result.JetCombi = combi;
308	
309	          FitResultList.push_back(result);
310	        }
311	
312	      }
313	      if(useOnlyMatch_) break; // don't go through combinatorics if useOnlyMatch was chosen
314	      next_permutation( combi.begin(), combi.end() );
315	    }
316	    if(useOnlyMatch_) break; // don't go through combinatorics if useOnlyMatch was chosen
317	  }
318	  while(stdcomb::next_combination( jetIndices.begin(), jetIndices.end(), combi.begin(), combi.end() ));
319	 
320	  // sort results w.r.t. chi2 values
321	  FitResultList.sort();
322	  
323	  /* AC : no need to feed out result 
324	  // -----------------------------------------------------
325	  // feed out result
326	  // starting with the JetComb having the smallest chi2
327	  // -----------------------------------------------------
328	
329	  if( FitResultList.size() < 1 ) { // in case no fit results were stored in the list (all fits aborted)
330	    pPartonsHadP->push_back( fitter->fittedHadP()     );
331	    pPartonsHadQ->push_back( fitter->fittedHadQ()     );
332	    pPartonsHadB->push_back( fitter->fittedHadB()     );
333	    pPartonsLepB->push_back( fitter->fittedLepB()     );
334	    pLeptons    ->push_back( fitter->fittedLepton()   );
335	    pNeutrinos  ->push_back( fitter->fittedNeutrino() );
336	    // indices referring to the jet combination
337	    std::vector<int> invalidCombi;
338	    for(unsigned int i = 0; i < nPartons; ++i) 
339	      invalidCombi.push_back( -1 );
340	    pCombi->push_back( invalidCombi );
341	    // chi2
342	    pChi2->push_back( -1. );
343	    // chi2 probability
344	    pProb->push_back( -1. );
345	    // status of the fitter
346	    pStatus->push_back( -1 );
347	  }
348	  else {
349	    unsigned int iComb = 0;
350	    for(typename std::list<KinFitResult>::const_iterator result = FitResultList.begin(); result != FitResultList.end(); ++result) {
351	      if(maxNComb_ >= 1 && iComb == (unsigned int) maxNComb_) break;
352	      iComb++;
353	      // partons
354	      pPartonsHadP->push_back( result->HadP );
355	      pPartonsHadQ->push_back( result->HadQ );
356	      pPartonsHadB->push_back( result->HadB );
357	      pPartonsLepB->push_back( result->LepB );
358	      // lepton
359	      pLeptons->push_back( result->LepL );
360	      // neutrino
361	      pNeutrinos->push_back( result->LepN );
362	      // indices referring to the jet combination
363	      pCombi->push_back( result->JetCombi );
364	      // chi2
365	      pChi2->push_back( result->Chi2 );
366	      // chi2 probability
367	      pProb->push_back( result->Prob );
368	      // status of the fitter
369	      pStatus->push_back( result->Status );
370	    }
371	  }
372	  evt.put(pCombi);
373	  evt.put(pPartonsHadP, "PartonsHadP");
374	  evt.put(pPartonsHadQ, "PartonsHadQ");
375	  evt.put(pPartonsHadB, "PartonsHadB");
376	  evt.put(pPartonsLepB, "PartonsLepB");
377	  evt.put(pLeptons    , "Leptons"    );
378	  evt.put(pNeutrinos  , "Neutrinos"  );
379	  evt.put(pChi2       , "Chi2"       );
380	  evt.put(pProb       , "Prob"       );
381	  evt.put(pStatus     , "Status"     );*/
382	}
383	 
384	//template<typename LeptonCollection>
385	TtSemiLepKinFitter::Param TtSemiLepKinFitProducer::param(unsigned val) 
386	{
387	  TtSemiLepKinFitter::Param result;
388	  switch(val){
389	  case TtSemiLepKinFitter::kEMom       : result=TtSemiLepKinFitter::kEMom;       break;
390	  case TtSemiLepKinFitter::kEtEtaPhi   : result=TtSemiLepKinFitter::kEtEtaPhi;   break;
391	  case TtSemiLepKinFitter::kEtThetaPhi : result=TtSemiLepKinFitter::kEtThetaPhi; break;
392	  default: 
393	    //AC throw cms::Exception("WrongConfig") 
394	      std::cout << "Chosen jet parametrization is not supported: " << val << std::endl;
395	    break;
396	  }
397	  return result;
398	} 
399	
400	//template<typename LeptonCollection>
401	TtSemiLepKinFitter::Constraint TtSemiLepKinFitProducer::constraint(unsigned val) 
402	{
403	  TtSemiLepKinFitter::Constraint result;
404	  switch(val){
405	  case TtSemiLepKinFitter::kWHadMass       : result=TtSemiLepKinFitter::kWHadMass;       break;
406	  case TtSemiLepKinFitter::kWLepMass       : result=TtSemiLepKinFitter::kWLepMass;       break;
407	  case TtSemiLepKinFitter::kTopHadMass     : result=TtSemiLepKinFitter::kTopHadMass;     break;
408	  case TtSemiLepKinFitter::kTopLepMass     : result=TtSemiLepKinFitter::kTopLepMass;     break;
409	  case TtSemiLepKinFitter::kNeutrinoMass   : result=TtSemiLepKinFitter::kNeutrinoMass;   break;
410	  case TtSemiLepKinFitter::kEqualTopMasses : result=TtSemiLepKinFitter::kEqualTopMasses; break;
411	  default: 
412	    //AC throw cms::Exception("WrongConfig") 
413	      std::cout << "Chosen fit constraint is not supported: " << val << std::endl;
414	    break;
415	  }
416	  return result;
417	} 
418	
419	//template<typename LeptonCollection>
420	std::vector<TtSemiLepKinFitter::Constraint> TtSemiLepKinFitProducer::constraints(std::vector<unsigned>& val)
421	{
422	  std::vector<TtSemiLepKinFitter::Constraint> result;
423	  for(unsigned i=0; i<val.size(); ++i){
424	    result.push_back(constraint(val[i]));
425	  }
426	  return result; 
427	}
428	
429	double TtSemiLepKinFitProducer::GetFitChi2(int i)
430	{
431	  if (FitResultList.size()>=i+1) 
432	  {  std::list<KinFitResult>::iterator it = FitResultList.begin();
433	     std::advance(it, i);
434	     return it->Chi2;}
435	     
436	  else 
437	  { std::cout <<"Number of converged fits is "<<  FitResultList.size() << std::endl;
438	    return -999;}
439	
440	}
