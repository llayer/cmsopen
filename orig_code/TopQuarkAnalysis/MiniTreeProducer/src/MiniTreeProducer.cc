1 #include "TopQuarkAnalysis/MiniTreeProducer/interface/MiniTreeProducer.h"
2	#include "FWCore/ServiceRegistry/interface/Service.h"
3	#include "CommonTools/UtilAlgos/interface/TFileService.h"
4	//#include "DataFormats/BeamSpot/interface/BeamSpot.h"
5	#include "DataFormats/MuonReco/interface/MuonMETCorrectionData.h"
6	#include "DataFormats/Math/interface/Point3D.h"
7	#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
8
9	#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
10	#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
11	#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
12	#include <SimDataFormats/GeneratorProducts/interface/PdfInfo.h>
13	#include <SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h>
14	#include <RecoEgamma/EgammaTools/interface/ConversionFinder.h>
15
16	#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
17
18	//For electron-conversion
19	#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
20	#include "MagneticField/Engine/interface/MagneticField.h"
21	#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
22	#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
23	#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
24	#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
25	#include "DataFormats/TrackReco/interface/Track.h"
26	#include "DataFormats/TrackReco/interface/TrackFwd.h"
27	#include "DataFormats/Scalers/interface/DcsStatus.h"
28
29	#include "DataFormats/PatCandidates/interface/TriggerObject.h"
30
31
32	typedef math::XYZPoint Point;
33	using namespace std;
34
35
36	MiniTreeProducer::MiniTreeProducer (const edm::ParameterSet & iConfig)
37	{
38	  cout << "Constructor of MiniTreeProducer" << endl;
39
40	  // -------------  get cfg file information:
41
42	  verbose         = iConfig.getParameter < int >              ("verbose");
43	  readPDFWeights  = iConfig.getParameter < bool >             ("readPDFWeights");
44	  TriggerMenu     = iConfig.getParameter < InputTag >         ("TriggerMenu");
45	  TriggerInfo     = iConfig.getParameter < bool >             ("TriggerInfo");
46	  saveAllTriggers = iConfig.getParameter < bool >             ("saveAllTriggers");
47	  triggerList     = iConfig.getParameter < vector < string > >("triggerList");
48	  prescaleList    = iConfig.getParameter < vector < string > >("prescaleList");
49	  hltJetsMatcher  = iConfig.getParameter < string >           ("hltJetsMatcher");
50	  hltTausMatcher  = iConfig.getParameter < string >           ("hltTausMatcher");
51	  hltJetsMatcher45= iConfig.getParameter < string >           ("hltJetsMatcher45");
52	  hltTausMatcher45= iConfig.getParameter < string >           ("hltTausMatcher45");
53
54	  electronProducer  = iConfig.getParameter < InputTag > ("electronProducer");
55	  doElectrons       = iConfig.getParameter < bool >     ("doElectrons");
56	  electron_cut_pt   = iConfig.getParameter < double >   ("electron_cut_pt");
57	  electron_cut_eta  = iConfig.getParameter < double >   ("electron_cut_eta");
58
59	  photonProducer  = iConfig.getParameter < InputTag > ("photonProducer");
60	  doPhotons       = iConfig.getParameter < bool >     ("doPhotons");
61	  photon_cut_pt   = iConfig.getParameter < double >   ("photon_cut_pt");
62	  photon_cut_eta  = iConfig.getParameter < double >   ("photon_cut_eta");
63
64	  muonProducer = iConfig.getParameter < InputTag > ("muonProducer");
65	  doMuons      = iConfig.getParameter < bool >     ("doMuons");
66	  muon_cut_pt  = iConfig.getParameter < double >   ("muon_cut_pt");
67	  muon_cut_eta = iConfig.getParameter < double >   ("muon_cut_eta");
68
69	  tauProducer  = iConfig.getParameter < vector < string> >("tauProducer");
70	  doTaus       = iConfig.getParameter < bool >     ("doTaus");
71	  tau_cut_pt   = iConfig.getParameter < double >   ("tau_cut_pt");
72	  tau_cut_eta  = iConfig.getParameter < double >   ("tau_cut_eta");
73
74	  trackProducer    = iConfig.getParameter < InputTag > ("trackProducer");
75	  doTracks         = iConfig.getParameter < bool >     ("doTracks");
76	  track_cut_pt     = iConfig.getParameter < double >   ("track_cut_pt");
77	  track_cut_eta    = iConfig.getParameter < double >   ("track_cut_eta");
78	  beamSpotProducer = iConfig.getParameter < InputTag > ("beamSpotProducer");
79
80	  jetmetProducer = iConfig.getParameter < VParameters > ("jetmetProducer");
81	  jet_cut_pt     = iConfig.getParameter < double >      ("jet_cut_pt");
82	  jet_cut_eta    = iConfig.getParameter < double >      ("jet_cut_eta");
83
84	  rho_PUUE_dens  =  iConfig.getParameter < InputTag >      ("rho_PUUE_dens");
85	  neutralRho_PUUE_dens  =  iConfig.getParameter < InputTag >      ("neutralRho_PUUE_dens");
86
87	  vertexProducer = iConfig.getParameter < InputTag > ("vertexProducer");
88	  saveAllVertex  = iConfig.getParameter < bool >     ("saveAllVertex");
89
90
91	  doGenParticleCollection         = iConfig.getParameter < bool >     ("doGenParticleCollection");
92	  //register product
93	  produces < TopTree::MTEvent > ();
94	  isAOD           = iConfig.getParameter < bool >             ("isAOD");
95	  isData          = iConfig.getParameter < bool >             ("isData");
96	  cout << "End of MiniTreeProducer constructor" << endl;
97	}
98
99
100	MiniTreeProducer::~MiniTreeProducer ()
101	{
102	  //TopTree objects
103	  vertexAlgo = 0;
104	  delete vertexAlgo;
105	  // do anything here that needs to be done at desctruction time
106	  // (e.g. close files, deallocate resources etc.)
107
108	}
109
110
111	//
112	// member functions
113	//
114
115	// ------------ method called to produce the data  ------------
116	void MiniTreeProducer::produce (edm::Event & iEvent, const edm::EventSetup & iSetup)
117	{
118	  using namespace edm;
119	  using namespace IPTools;
120
121	  if (verbose > 0) {
122	    cout << "--------- new event ---------" << endl;
123	  }
124
125
126
127
128
129
130	  if (verbose > 1)
131	    cout << "Initializing TopTree objects" << endl;
132
133	  // --------------------
134	  //    TopTree objects
135	  // --------------------
136	  //TopTreeTrack    *tttrack  = 0;
137	  //TopTreeBS       *bsp      = 0;
138	  //TopTreePVx      *pvx      = 0;
139	  //TopTreeElectron *electron = 0;
140	  //TopTreeMuon *muon         = 0;
141	  //TopTreeTau *tau           = 0;
142	  //TopTreeJetMet *cjetmet    = 0;
143	  //TopTreeJet *caloJet       = 0;
144	  //TopTreeTrack *asstrack    = 0;
145	  //TopTreePU *pu_info        = 0;
146
147	  // --------------------
148	  //    Fill Event info
149	  // --------------------
150	  if (verbose > 1)
151	    cout << "Filling event info" << endl;
152	  TopTree::MTEvent cev;
153	  cev.runNb = iEvent.id ().run ();
154	  cev.eventNb = iEvent.id ().event ();
155	  cev.lumiblock = iEvent.id ().luminosityBlock ();
156	  cev.eventWeight = 1;
157
158	  // --------------------
159	  //    Fill MC Truth
160	  // --------------------
161	  if (verbose > 1)
162	    cout << "Filling MC truth info" << endl;
163	  if ( isData !=  1 ) {
164
165
166
167
168	    //********************************************************
169	    // Calculate PDF weight
170	    //********************************************************
171	    edm::Handle<GenEventInfoProduct> theGenEventInfo;
172	    iEvent.getByType( theGenEventInfo );
173
174
175
176	    cev.partonFlavor1 = theGenEventInfo->pdf()->id.first;
177	    cev.partonFlavor2 = theGenEventInfo->pdf()->id.second;
178	    cev.x1 = theGenEventInfo->pdf()->x.first;
179	    cev.x2 = theGenEventInfo->pdf()->x.second;
180	    cev.Q_scale = theGenEventInfo->pdf()->scalePDF;
181
182
183	    if(readPDFWeights){
184	      edm::InputTag pdfWeightTag("pdfWeights:cteq66"); // or any other PDF set
185	      edm::Handle<std::vector<double> > weightHandle;
186	      iEvent.getByLabel(pdfWeightTag, weightHandle);
187
188	      std::vector<double> weights = (*weightHandle);
189	      std::cout << "Event weight for central PDF:" << weights[0] << std::endl;
190
191	       unsigned int nmembers = weights.size();
192	       for (unsigned int j=0; j<nmembers; j++) {
193	           //std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
194	           //std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
195	           cev.pdfWeights.push_back(float(weights[j]));
196
197	      }
198	      /*
199	      // stupid CWR !!
200	      edm::InputTag pdfWeightTag2("pdfWeights:MRST2006nnlo"); // or any other PDF set
201	      edm::Handle<std::vector<double> > weightHandle2;
202	      iEvent.getByLabel(pdfWeightTag2, weightHandle2);
203
204	      std::vector<double> weights2 = (*weightHandle2);
205	      //std::cout << "Event weight for central PDF:" << weights[0] << std::endl;
206
207	       unsigned int nmembers2 = weights2.size();
208	       for (unsigned int j=0; j<nmembers2; j++) {
209	           //std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
210	           //std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
211	           cev.pdfWeights.push_back(float(weights2[j]));
212
213	      }
214	      */
215	      // stupid CWR !!
216	      edm::InputTag pdfWeightTag3("pdfWeights:NNPDF10"); // or any other PDF set
217	      edm::Handle<std::vector<double> > weightHandle3;
218	      iEvent.getByLabel(pdfWeightTag3, weightHandle3);
219
220	      std::vector<double> weights3 = (*weightHandle3);
221	      std::cout << "Event weight for central PDF MRST:" << weights3[0] << std::endl;
222
223	       unsigned int nmembers3 = weights3.size();
224	       for (unsigned int j=0; j<nmembers3; j++) {
225	           //std::cout << "Event weight for PDF variation +" << (j+1)/2 << ": " << weights[j] << std::endl;
226	           //std::cout << "Event weight for PDF variation -" << (j+1)/2 << ": " << weights[j+1] << std::endl;
227	           cev.pdfWeights.push_back(float(weights3[j]));
228
229	      }
230
231
232
233	    }
234	std::cout << "Pileup"<< std::endl;
235
236	    //********************************************************
237	    // Pileup summary info
238	    //********************************************************
239
240	     edm::InputTag PileupSrc_("addPileupInfo");
241	     Handle<std::vector< PileupSummaryInfo > >  PupInfo;
242	     iEvent.getByLabel(PileupSrc_, PupInfo);
243
244	     std::vector<PileupSummaryInfo>::const_iterator PVI;
245
246	     //std::cout << "pu summary 189 " << PupInfo->size() << std::endl;
247	     // (then, for example, you can do)
248	     for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
249	       int n_bc=PVI->getBunchCrossing();
250	       //std::cout << " Pileup Information: bunchXing, nvtx, size : " << PVI->getBunchCrossing() << " "
251	       //<< PVI->getPU_NumInteractions()  << " "
252	       //<< PVI->getPU_zpositions().size() << std::endl;
253
254	       if (n_bc==0 )  cev.num_pileup_bc0    = PVI->getPU_NumInteractions();
255	       if (n_bc==1 )  cev.num_pileup_bcp1   = PVI->getPU_NumInteractions();
256	       if (n_bc==-1)  cev.num_pileup_bcm1   = PVI->getPU_NumInteractions();
257
258	       cev.runNb = PVI->getTrueNumInteractions();
259
260
261	    }
262
263	     //std::cout << "pu summary 207 " << std::endl;
264	    //********************************************************
265	    // Fill skimmed GenparticleCollection
266	    //********************************************************
267
268	    Handle < reco::GenParticleCollection > genParticles;
269	    iEvent.getByLabel ("genParticles", genParticles);
270
271
272	    if (!doGenParticleCollection)
273	      if (verbose > 1)
274	        cout << "Do not fill skimmed GenParticleCollection " << endl;
275	    if (doGenParticleCollection) {
276
277	      edm::Handle<GenEventInfoProduct> genInfos;
278	      iEvent.getByLabel("generator", genInfos);
279	      if(genInfos->binningValues().size() > 0 ) cev.ptHat = genInfos->binningValues()[0];
280
281	      cev.fillGenInfo(genParticles);
282	      if (verbose > 1)
283	        cout << "Filling skimmed GenParticleCollection" << endl;
284	      std::map < int, std::vector < int > >daughtermap;
285	      std::map < int, std::vector < int > >mothermap;
286	      size_t MaxLim = 100;
287	      bool maxlimfound = false;
288	      for (size_t i = 0; i < genParticles->size (); ++i) {
289	        const GenParticle & paIt = (*genParticles)[i];
290	        if (maxlimfound)
291	          continue;
292	        if (abs (paIt.pdgId ()) == 92 && MaxLim <= 100) {
293	          MaxLim = i;
294	          maxlimfound = true;
295	        }
296	      }
297	      for (size_t i = 0; i < MaxLim; ++i) {
298	        const GenParticle & paIt = (*genParticles)[i];
299	        // daughters
300	        std::vector < int >list_index_dau;
301	        list_index_dau.clear ();
302	        for (unsigned int j = 0; j < paIt.numberOfDaughters (); j++) {
303	          const reco::Candidate * d = paIt.daughter (j);
304	          if(d == NULL) continue;
305
306	          for (size_t k = i + 1; k < genParticles->size (); ++k) {
307	            const GenParticle & p = (*genParticles)[k];
308	            if (p.p4 ().e () == d->p4 ().e () && p.p4 ().pz () == d->p4 ().pz () && p.pdgId () == d->pdgId () && p.status () == d->status ())
309	              list_index_dau.push_back (k);
310	          }
311	          daughtermap[i] = list_index_dau;
312	          d = 0 ;
313	          delete d;
314	        }
315	        cev.GPC.push_back (paIt);
316	        // mothers
317	        std::vector < int >list_index_mot;
318	        list_index_mot.clear ();
319	        for (unsigned int j = 0; j < paIt.numberOfMothers (); j++) {
320	          const reco::Candidate * m = paIt.mother (j);
321	          if(m == NULL) continue;
322
323	          for (size_t k = 0; k < i; ++k) {
324	            const GenParticle & p = (*genParticles)[k];
325	            if (p.p4 ().e () == m->p4 ().e () && p.pdgId () == m->pdgId () && p.status () == m->status ())
326	              list_index_mot.push_back (k);
327	          }
328	          mothermap[i] = list_index_mot;
329	          m = 0 ;
330	          delete m;
331	        }
332
333	      }
334
335
336	      cev.GPC_list_index_dau = daughtermap;
337	      cev.GPC_list_index_mot = mothermap;
338	    }
339	  }
340
341	  //********************************************************
342	  //  trigger query
343	  //********************************************************
344	  TriggerResults tr;
345	  Handle < TriggerResults > h_trigRes;
346	  //iEvent.getByLabel(InputTag("TriggerResults::HLT"), h_trigRes);     // Trigger menu 1E31
347	  iEvent.getByLabel (TriggerMenu, h_trigRes);   //Trigger menu E29
348	  tr = *h_trigRes;
349
350	  if (TriggerInfo) {
351	    vector < string > tList;
352	    Service < service::TriggerNamesService > tns;
353	    bool foundNames = tns->getTrigPaths (tr, tList);
354	    if (!foundNames)
355	      std::cout << "Could not get trigger names!\n";
356	    if (tr.size () != tList.size ())
357	      std::cout << "ERROR: length of names and paths not the same: " << tList.size () << "," << tr.size () << endl;
358	    if (saveAllTriggers) {
359	        for (unsigned int i = 0; i < tr.size (); i++) {
360	        //std::cout <<"tList[i] "<<tList[i]<< std::endl;
361	           if (tr[i].accept () == 1)
362	                cev.triggers.push_back (pair < string, bool > (tList[i], true));
363	           else
364	                cev.triggers.push_back (pair < string, bool > (tList[i], false));
365	        }
366	    }
367	    else {
368	        for (unsigned int i = 0; i < tr.size (); i++) {
369	                for (unsigned int j = 0; j < triggerList.size (); j++) {
370	                        if (tList[i] == triggerList[j]) {
371	                                if (tr[i].accept () == 1)
372	                                        cev.triggers.push_back (pair < string, bool > (tList[i], true));
373	                                else
374	                                        cev.triggers.push_back (pair < string, bool > (tList[i], false));
375	                        }
376	                }
377	        }
378	   }
379	  }
380
381	  //********************************************************
382	  //  trigger matching with offline object
383	  //********************************************************
384
385	  // get Trigger summary from Event
386	  edm::Handle<trigger::TriggerEvent> summary;
387	  std::string T = "";
388
389	  T = TriggerMenu.process();
390
391
392	  edm::InputTag summaryTag_("hltTriggerSummaryAOD","",T);
393	  iEvent.getByLabel(summaryTag_,summary);
394
395	  std::string::size_type n(0);
396	  std::string::size_type index(0);
397
398	  //hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter
399	  edm::InputTag memberTagElectron_("hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter","",T);
400
401	  index=summary->filterIndex(memberTagElectron_);
402	  // get all trigger objects corresponding to this module.
403	  // loop through them and see how many objects match the selection
404	  double match_pTHLTElectron15 = -1;
405	  if (index<summary->sizeFilters()) {
406	    const trigger::Keys& KEYS_el (summary->filterKeys(index));
407	    const std::string::size_type n1_el(KEYS_el.size());
408	    for (std::string::size_type i=0; i!=n1_el; ++i) {
409	      const trigger::TriggerObject& triggerObject_el( summary->getObjects().at(KEYS_el[i]) );
410	      //std::cout<<"je teste electron trigger object "<<std::endl;
411	      if (triggerObject_el.pt() >= 15 && iEvent.id ().run () < 138000 ) {
412	        n++;
413	        match_pTHLTElectron15 = triggerObject_el.pt();
414
415	      }
416	    }
417	  }
418
419	  cev.match_HLT_Ele10_LW_L1R_recoEl = match_pTHLTElectron15;
420
421	  //********************************************************
422	  // Retrieve dynamic prescales for triggers
423	  //********************************************************
424
425	  pat::TriggerEvent pTrigEvt;
426	  //if (isData) {
427
428	    Handle< pat::TriggerEvent > patTriggerEvent;
429	    iEvent.getByLabel("patTriggerEvent", patTriggerEvent );
430	    pTrigEvt = *patTriggerEvent;
431
432	    for (unsigned int j = 0; j < prescaleList.size (); j++) {
433	       if(pTrigEvt.path(prescaleList[j])!=0)
434	        cev.prescales.push_back(pair < string, int > (prescaleList[j], pTrigEvt.path(prescaleList[j])->prescale()));
435	       else
436	        cev.prescales.push_back(pair < string, int > (prescaleList[j], -99));
437	     }
438	  //}
439	  //default initialisation for MC
440	  //else cev.prescales.push_back(pair < string, int > ("empty",-99));
441
442	    bool filter_status = -99;
443	    const pat::TriggerFilter* filter_1 = pTrigEvt.filter("hltQuadJet40IsoPFTau40");
444	    if (filter_1) filter_status = filter_1->status();
445	    cev.prescales.push_back (pair < string, int > ("hltQuadJet40IsoPFTau40", filter_status));
446
447	    filter_status = -99;
448	    const pat::TriggerFilter* filter_2 = pTrigEvt.filter("HLTPFTauTightIsoSequence");
449	    if (filter_2) filter_status = filter_2->status();
450	    cev.prescales.push_back (pair < string, int > ("HLTPFTauTightIsoSequence", filter_status));
451
452	    filter_status = -99;
453	    const pat::TriggerFilter* filter_3 = pTrigEvt.filter("hltPFTau5Track");
454	    if (filter_3) filter_status = filter_3->status();
455	    cev.prescales.push_back (pair < string, int > ("hltPFTau5Track", filter_status));
456
457	    filter_status = -99;
458	    const pat::TriggerFilter* filter_4 = pTrigEvt.filter("hltPFTau5Track5");
459	    if (filter_4) filter_status = filter_4->status();
460	    cev.prescales.push_back (pair < string, int > ("hltPFTau5Track5", filter_status));
461
462	    filter_status = -99;
463	    const pat::TriggerFilter* filter_5 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet20Central");
464	    if (filter_5) filter_status = filter_5->status();
465	    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet20Central", filter_status));
466
467	    filter_status = -99;
468	    const pat::TriggerFilter* filter_6 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40");
469	    if (filter_6) filter_status = filter_6->status();
470	    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40", filter_status));
471
472	    filter_status = -99;
473	    const pat::TriggerFilter* filter_7 = pTrigEvt.filter("hltQuadJet45IsoPFTau45");
474	    if (filter_7) filter_status = filter_7->status();
475	    cev.prescales.push_back (pair < string, int > ("hltQuadJet45IsoPFTau45", filter_status));
476
477	    filter_status = -99;
478	    const pat::TriggerFilter* filter_8 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45");
479	    if (filter_8) filter_status = filter_8->status();
480	    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45", filter_status));
481
482	    filter_status = -99;
483	    const pat::TriggerFilter* filter_9 = pTrigEvt.filter("hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45");
484	    if (filter_9) filter_status = filter_9->status();
485	    cev.prescales.push_back (pair < string, int > ("hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45", filter_status));
486
487
488	  //********************************************************
489	  // PAT trigger helper for trigger matching information
490	  //********************************************************
491	  const pat::helper::TriggerMatchHelper matchHelper;
492
493	  const pat::TriggerObjectRefVector trigRefs( patTriggerEvent->objects( trigger::TriggerJet ) );
494	  for ( pat::TriggerObjectRefVector::const_iterator iTrig = trigRefs.begin(); iTrig != trigRefs.end(); ++iTrig )
495	  {
496	   //std::cout <<"HLT jet "<< (*iTrig)->pt() << " " << (*iTrig)->eta() <<std::endl;
497	   //std::cout << pTrigEvt.objectInFilter( (*iTrig), "hltQuadJet40IsoPFTau40") << std::endl;
498	   if (pTrigEvt.objectInFilter( (*iTrig), "hltQuadJet40IsoPFTau40") ||
499	       pTrigEvt.objectInFilter( (*iTrig), "hltQuadJet45IsoPFTau45"))
500	   {
501	    TLorentzVector p4((*iTrig)->px(),(*iTrig)->py(),(*iTrig)->pz(),(*iTrig)->energy());
502	    cev.jetObjTrig.push_back(p4);
503	    }
504	  }
505
506	  const pat::TriggerObjectRefVector trigRefs2( patTriggerEvent->objects( trigger::TriggerTau ) );
507	  for ( pat::TriggerObjectRefVector::const_iterator iTrig = trigRefs2.begin(); iTrig != trigRefs2.end(); ++iTrig )
508	  {
509	   //std::cout <<"HLT tau "<< (*iTrig)->pt() << " " << (*iTrig)->eta() <<std::endl;
510	   //std::cout << pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40") << std::endl;
511	    if (pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40") ||
512	        pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau45") ||
513	        pTrigEvt.objectInFilter( (*iTrig), "hltFilterPFTauTrack5TightIsoL1QuadJet28CentralPFTau45")
514	       )
515	   {
516	    TLorentzVector p4((*iTrig)->px(),(*iTrig)->py(),(*iTrig)->pz(),(*iTrig)->energy());
517	    cev.tauObjTrig.push_back(p4);
518	    }
519	   }
520
521
522	  // -------------------------
523	  // get the beamspot from the Event:
524	  // -------------------------
525	  if (verbose > 1)
526	    cout << "Working on beamspot related variable" << endl;
527
528	  edm::Handle < reco::BeamSpot > recoBeamSpotHandle;
529	  iEvent.getByType (recoBeamSpotHandle);
530	  const BeamSpot bs = *recoBeamSpotHandle;
531	  cev.p3BS.SetXYZ (bs.x0 (), bs.y0 (), bs.z0 ());
532	  cev.p3BSErr.SetXYZ (bs.x0Error (), bs.y0Error (), bs.z0Error ());
533
534
535	// for compatibilty
536	  reco::BeamSpot beamSpot;
537	  Handle < reco::BeamSpot > beamSpotHa;
538	  iEvent.getByLabel (beamSpotProducer, beamSpotHa);
539	  Point bspot = beamSpotHa.isValid ()? beamSpotHa->position () : Point (0, 0, 0);
540	  // -------------------------
541
542	  Handle < vector < reco::Vertex > >vertices;
543	  iEvent.getByLabel (vertexProducer, vertices);
544	  int dum = 0;
545	  const reco::Vertex * vp1 = 0;
546
547	  for (vector < reco::Vertex >::const_iterator itv = vertices->begin (), edv = vertices->end (); itv != edv; ++itv) {
548	      if (dum == 0) vp1 = &*itv;
549	      TopTreePVx* pvx = cev.NewVertex ();
550	      pvx->chi2 = itv->chi2 ();
551	      pvx->isFake = itv->isFake ();
552	      pvx->isValid = itv->isValid ();
553	      pvx->ndof = itv->ndof ();
554	      pvx->normalizedChi2 = itv->normalizedChi2 ();
555	      pvx->NumberOfTracks = itv->tracksSize ();
556	      pvx->p3.SetXYZ (itv->x (), itv->y (), itv->z ());
557	      pvx->rho = itv->position ().rho ();
558	      pvx->p3Err.SetXYZ (itv->xError (), itv->yError (), itv->zError ());
559	      pvx = 0;
560	      delete pvx;
561	      dum++;
562	      if (dum == 1 && !saveAllVertex)
563	      break;
564	  }
565
566
567
568	  //***************************************************************************
569	  // Photons ----------------------------------------------------------------
570	  //***************************************************************************
571
572
573	  if (!doPhotons)
574	    if (verbose > 1)
575	      cout << "Do not fill the photons" << endl;
576	  if (doPhotons) {
577	    if (verbose > 1)
578	      cout << "Filling photons" << endl;
579
580	      Handle < vector < pat::Photon > >phHa;
581	      iEvent.getByLabel (photonProducer, phHa);
582	      const vector < pat::Photon > *gamma = phHa.product ();
583
584	      for (vector < pat::Photon >::const_iterator it = gamma->begin (); it != gamma->end (); it++) {
585
586	        const pat::Photon * thephoton = &*it;
587	        if (!thephoton)
588	          continue;             // useless ?
589	        // Applying preselection based on pT, eta, ID
590	        if (thephoton->p4().pt () < photon_cut_pt || fabs (thephoton->p4().eta ()) > photon_cut_eta)
591	        continue;
592
593	        TopTreePhoton *photon = cev.NewPhoton ();
594
595	        photon->p4.SetPxPyPzE (thephoton->p4().px (), thephoton->p4().py (), thephoton->p4().pz (), thephoton->p4().energy ());
596
597	        photon->ECaloIso = thephoton->ecalIso();
598	        photon->HCaloIso = thephoton->hcalIso();
599	        photon->TrkIso   = thephoton->trackIso();
600
601	        photon->isEB = thephoton->isEB();
602	        photon->e1x5 = thephoton->e1x5();
603	        photon->e2x5 = thephoton->e2x5();
604	        photon->e3x3 = thephoton->e3x3();
605	        photon->e5x5 = thephoton->e5x5();
606	        thephoton = 0;
607	        delete thephoton;
608	        photon = 0;
609	        delete photon;
610	      }
611	    gamma = 0;
612	    delete gamma;
613	  }
614
615
616
617
618
619
620
621	  // -------------------------
622	  //      Leptons (ele, mu, tau)
623	  // -------------------------
624
625
626
627
628	  //***************************************************************************
629	  // Electrons ----------------------------------------------------------------
630	  //***************************************************************************
631
632	  if (!doElectrons)
633	    if (verbose > 1)
634	      cout << "Do not fill the electrons" << endl;
635	  if (doElectrons) {
636	    if (verbose > 1)
637	      cout << "Filling electrons" << endl;
638
639	    Handle < vector < pat::Electron > >elHa;
640	    iEvent.getByLabel (electronProducer, elHa);
641	    const vector < pat::Electron > *ele = elHa.product ();
642
643	    //edm::Handle < EcalRecHitCollection > recHits;
644	    //iEvent.getByLabel (edm::InputTag ("ecalRecHit", "EcalRecHitsEB"), recHits);
645	    //const EcalRecHitCollection *myRecHits = recHits.product ();
646
647	    for (vector < pat::Electron >::const_iterator it = ele->begin (); it != ele->end (); it++) {
648
649	      const pat::Electron * elec = &*it;
650	      if (!elec)
651	        continue;               // useless ?
652	      // Applying preselection based on pT, eta, ID
653	      if (elec->pt () < electron_cut_pt || fabs (elec->eta ()) > electron_cut_eta)
654	        continue;
655
656	      TopTreeElectron *electron = cev.NewElectron ();
657
658	      electron->p4.SetPxPyPzE (elec->px (), elec->py (), elec->pz (), elec->energy ());
659	      electron->vertex.SetXYZ (elec->vx (), elec->vy (), elec->vz ());
660	      electron->Charge = elec->charge ();
661	      electron->addInfo.DB = elec->dB ();
662	      electron->nlost = elec->gsfTrack ()->trackerExpectedHitsInner ().numberOfLostHits ();
663	      //cout<<"nlost "<<elec->gsfTrack ()->trackerExpectedHitsInner ().numberOfLostHits ()<<endl;
664
665	      /////////////////////////////////
666	      //        Conversion
667	      ////////////////////////////////
668	     float evt_bField=0;
669	     // need the magnetic field
670	     //
671	     // if isData then derive bfield using the
672	     // magnet current from DcsStatus
673	     // otherwise take it from the IdealMagneticFieldRecord
674
675	     if (isData) {
676	     // scale factor = 3.801/18166.0 which are
677	     // average values taken over a stable two
678	     // week period
679
680	    edm::Handle<DcsStatusCollection> dcsHandle;
681	    iEvent.getByLabel("scalersRawToDigi", dcsHandle);
682
683	    if (dcsHandle.isValid())
684	     {
685	       if ((*dcsHandle).size()==0)
686	         {
687	           edm::LogError("DetStatus") << "Error! dcsStatus has size 0, bField set to magneticField->inTesla(GlobalPoint(0.,0.,0.)).z()" ;
688	            ESHandle<MagneticField> magneticField;
689	            iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
690	            evt_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
691
692	         }
693	       else {
694	     float currentToBFieldScaleFactor = 2.09237036221512717e-04;
695	     float current = (*dcsHandle)[0].magnetCurrent();
696	     evt_bField = current*currentToBFieldScaleFactor;
697	        }
698	    }
699	   }
700	   else {
701	    ESHandle<MagneticField> magneticField;
702	    iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
703
704	    evt_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
705	   }
706
707	          //Get the CTF tracks
708	      Handle<reco::TrackCollection> tracks_h;
709	      iEvent.getByLabel("generalTracks", tracks_h);
710
711	      ConversionFinder convFinder;
712	      //returns the best candidate partner (see text below)
713	      ConversionInfo convInfo = convFinder.getConversionInfo(*elec, tracks_h, evt_bField);
714	      electron->deltaDistance = convInfo.dist();
715	      electron->deltaCotTheta = convInfo.dcot();
716	      //cout<<"elec: "<<convInfo.dist()<<" "<<convInfo.dcot()<<endl;
717
718	    //  if(elec->gsfTrack()) electron->isGsfElectron = true;
719	       bool isGsfElectron = true;
720	       if (!elec->gsfTrack()) isGsfElectron = false;
721	        electron->isGsfElectron = isGsfElectron;
722	      // Swiss cross for Ecal spkie cleaning
723	      double myswissCross = -999;
724	      // Cut only on EB, ecal-seeded electrons
725	      //if (elec->ecalDrivenSeed () > 0 && fabs (elec->superCluster ()->eta ()) < 1.4442) {
726	        //const reco::CaloClusterPtr seed = elec->superCluster ()->seed ();     // seed cluster
727	        //const DetId seedId = seed->seed ();
728	        //EcalSeverityLevelAlgo severity;
729	        //myswissCross = severity.swissCross (seedId, *myRecHits);
730	      //}
731	      electron->elspike = myswissCross;
732	      // end swisscross
733
734	      //ECAL - HCAL Energy
735	      electron->EmEnergy = elec->ecalEnergy();
736	      electron->HadEnergy = elec->ecalEnergy()*elec->hcalOverEcal();
737
738	      //handle the electron-ID
739	      electron->elecIdLoose = elec->electronID ("eidLoose");
740	      electron->elecIdTight = elec->electronID ("eidTight");
741	      electron->elecIdRobustLoose = elec->electronID ("eidRobustLoose");
742	      electron->elecIdRobustTight = elec->electronID ("eidRobustTight");
743	      electron->elecIdWP90_r       = elec->electronID("simpleEleId90relIso");
744	      electron->elecIdWP90_c       = elec->electronID("simpleEleId90cIso");
745	      //new e-id
746	      electron->cicVeryLoose     =  elec->electronID("cicVeryLoose");
747	      electron->cicLoose         =  elec->electronID("cicLoose");
748	      electron->cicMedium        =  elec->electronID("cicMedium");
749	      electron->cicTight         =  elec->electronID("cicTight");
750	      electron->cicSuperTight    =  elec->electronID("cicSuperTight");
751	      electron->cicHyperTight1    =  elec->electronID("cicHyperTight1");
752	      electron->cicHyperTight2    =  elec->electronID("cicHyperTight2");
753	      electron->cicHyperTight3    =  elec->electronID("cicHyperTight3");
754	      electron->cicVeryLooseMC     =  elec->electronID("cicVeryLooseMC");
755	      electron->cicLooseMC         =  elec->electronID("cicLooseMC");
756	      electron->cicMediumMC        =  elec->electronID("cicMediumMC");
757	      electron->cicTightMC         =  elec->electronID("cicTightMC");
758	      electron->cicSuperTightMC    =  elec->electronID("cicSuperTightMC");
759	      electron->cicHyperTight1MC    =  elec->electronID("cicHyperTight1MC");
760	      electron->cicHyperTight2MC    =  elec->electronID("cicHyperTight2MC");
761	      electron->cicHyperTight3MC    =  elec->electronID("cicHyperTight3MC");
762
763
764
765
766
767
768	      electron->TrkZ0 = elec->TrackPositionAtVtx ().Z ();
769	      electron->TrkPt = sqrt (elec->trackMomentumAtVtx ().perp2 ());
770	      electron->TrkIso04 = elec->trackIso ();
771	      electron->CaloIso04 = elec->caloIso ();
772	      electron->ECaloIso04 = elec->ecalIso ();
773	      electron->HCaloIso04 = elec->hcalIso ();
774	      electron->TrkIso03 = elec->dr03TkSumPt ();
775
776	      electron->CaloIso03 = elec->dr03EcalRecHitSumEt () + elec->dr03HcalTowerSumEt ();
777
778	      electron->ECaloIso03 = elec->dr03EcalRecHitSumEt ();
779	      electron->HCaloIso03 = elec->dr03HcalTowerSumEt ();
780
781	      //PF isolation
782
783	      electron->PATNeutralHadronIso =  elec->neutralHadronIso ();
784	      electron->PATChargedHadronIso =  elec->chargedHadronIso () ;
785	      electron->PATPhotonIso        =  elec->photonIso () ;
786	      electron->PATTrackIso         =  elec->trackIso ();
787
788
789	      electron->caloEnergy = elec->caloEnergy ();
790	      //electron->eSuperClusterOverP = elec->eSuperClusterOverP ();
791	      //electron->deltaEtaSeedClusterTrackAtCalo = elec->deltaEtaSeedClusterTrackAtCalo ();
792	      //electron->deltaPhiSeedClusterTrackAtCalo = elec->deltaPhiSeedClusterTrackAtCalo ();
793	      //electron->deltaEtaSuperClusterTrackAtVtx = elec->deltaEtaSuperClusterTrackAtVtx ();
794	      //electron->deltaPhiSuperClusterTrackAtVtx = elec->deltaPhiSuperClusterTrackAtVtx ();
795	      //electron->hadronicOverEm = elec->hadronicOverEm ();
796	      //electron->hcalOverEcal = elec->hcalOverEcal ();
797	      //electron->sigmaIetaIeta = elec->sigmaIetaIeta ();
798	      //electron->e2x5Max = elec->e2x5Max ();
799	      //electron->e5x5 = elec->e5x5 ();
800	      electron->fbrem = elec->fbrem ();
801	      electron->classification = elec->classification ();
802	      electron->isEB = elec->isEB();
803	      electron->isEcalDriven = elec->ecalDrivenSeed ();
804	      electron->rawSCEnergy = elec->superCluster ()->rawEnergy ();
805	      double theta = 2*atan(exp(-1*elec->superCluster()->eta()));
806	      electron->ET_SC = elec->superCluster()->energy()*sin(theta);
807
808	      // Information relative to track
809	      electron->D0      = elec->gsfTrack ()->dxy (bspot);
810	      electron->Chi2    = elec->gsfTrack ()->normalizedChi2 ();
811
812	      edm::ESHandle < TransientTrackBuilder > trackBuilder;
813	      iSetup.get < TransientTrackRecord > ().get ("TransientTrackBuilder", trackBuilder);
814	      TransientTrack tt = trackBuilder->build (it->gsfTrack ());
815	      if (vp1) {
816	        double ip = IPTools::absoluteImpactParameter3D (tt, *vp1).second.value ();
817	        electron->IP3D = ip;
818	        double ipe = IPTools::absoluteImpactParameter3D (tt, *vp1).second.error ();
819	        electron->IP3DErr = ipe;
820	      }
821
822	      // MC information
823	      if ( isData != 1 ) {
824	        if ((elec->genLepton () != NULL)) {
825	          electron->addInfo.p4Gen.SetPxPyPzE (elec->genLepton ()->px (), elec->genLepton ()->py (), elec->genLepton ()->pz (), elec->genLepton ()->energy ());
826	        }
827
828	        Handle < reco::GenParticleCollection > genParticles;
829	        iEvent.getByLabel ("genParticles", genParticles);
830
831	        reco::GenParticle genmother;
832	        reco::GenParticle gengrandmother;
833	        reco::GenParticle genggrandmother;
834	        math::XYZTLorentzVector Zero (0, 0, 0, 0);
835
836	        genmother.setCharge (-99);
837	        genmother.setP4 (Zero);
838	        gengrandmother.setCharge (-99);
839	        gengrandmother.setP4 (Zero);
840	        genggrandmother.setCharge (-99);
841	        genggrandmother.setP4 (Zero);
842
843	        electron->LeptonOrigin = getElectronOrigin (genParticles, elec, genmother, gengrandmother, genggrandmother);
844	        electron->addInfo.GenMother = genmother;
845	        electron->addInfo.GenGrandMother = gengrandmother;
846	        electron->addInfo.GenGGrandMother = genggrandmother;
847	      }
848	      // REMARK: pour avoir acces a la trace il faut lors de la creation du patlayer1 mettre a true l'acces a la trace!
849	      electron->TrackEl = *(it->gsfTrack ());
850
851	      elec = 0;
852	      delete elec;
853	      electron = 0;
854	      delete electron;
855	    }                           // end electron loop
856	    ele = 0;
857	    delete ele;
858	  }
859
860	  //***************************************************************************
861	  // Muons --------------------------------------------------------------------
862	  //***************************************************************************
863
864
865	  if (!doMuons)
866	    if (verbose > 1)
867	      cout << "Do not fill the muons" << endl;
868	  if (doMuons) {
869	    if (verbose > 1)
870	      cout << "Filling muons" << endl;
871	    Handle < vector < pat::Muon > >muHa;
872	    iEvent.getByLabel (muonProducer, muHa);
873	    const vector < pat::Muon > *mus = muHa.product ();
874
875
876	    for (vector < pat::Muon >::const_iterator it = mus->begin (); it != mus->end (); it++) {
877	      const pat::Muon * mu = &*it;
878
879	      // Applying a preselectin on muons based on lep pT, eta, iso
880	      if (mu->pt () < muon_cut_pt || fabs (mu->eta ()) > muon_cut_eta || !(mu->isGlobalMuon () || mu->isTrackerMuon ()))
881	        continue;
882
883	      TopTreeMuon* muon = cev.NewMuon ();
884
885	      muon->p4.SetPxPyPzE (mu->px (), mu->py (), mu->pz (), mu->energy ());
886	      muon->vertex.SetXYZ (mu->vx (), mu->vy (), mu->vz ());
887	      muon->Charge = mu->charge ();
888	      muon->MuonType = mu->isGlobalMuon () * 100 + mu->isStandAloneMuon () * 10 + mu->isTrackerMuon ();
889	      muon->GlobalMuonPromptTight = mu->muonID ("GlobalMuonPromptTight");
890	      muon->addInfo.DB = mu->dB ();
891	      muon->TrkIso03 = mu->trackIso ();
892	      muon->CaloIso03 = mu->caloIso ();
893	      muon->ECaloIso03 = mu->ecalIso ();
894	      muon->HCaloIso03 = mu->hcalIso ();
895
896	      muon->PATNeutralHadronIso =  mu->neutralHadronIso ();
897	      muon->PATChargedHadronIso =  mu->chargedHadronIso () ;
898	      muon->PATPhotonIso        =  mu->photonIso () ;
899	      muon->PATTrackIso         =  mu->trackIso ();
900	      // REMARK: pour avoir acces a la trace il faut lors de la creation du patlayer1 mettre a true l'acces a la trace!
901	      // Information related to the tracks
902
903	      Point bspot = beamSpotHa.isValid ()? beamSpotHa->position () : Point (0, 0, 0);
904	      const reco::TrackRef globalTrack = mu->globalTrack ();
905	      if (globalTrack.isNonnull ()) {
906	        muon->D0 = globalTrack->dxy (bspot);
907	        muon->Chi2 = globalTrack->normalizedChi2 ();
908	      }
909	      if (it->innerTrack ().isNonnull ()) {
910	        muon->D0Inner = it->innerTrack ()->dxy (bspot);
911	      }
912
913	      muon->TrackMu = *(it->track ());
914	      if (mu->isStandAloneMuon ())
915	        muon->StaMu = *(it->standAloneMuon ());
916	      if (mu->isGlobalMuon ())
917	        muon->ComMu = *(it->combinedMuon ());
918
919	      muon->NTrValidHits = it->innerTrack()->numberOfValidHits();
920	      muon->NValidHits   = -999;
921	      muon->NOfMatches = it->numberOfMatches();
922	      if (mu->isGlobalMuon ())  muon->NValidHits = it->globalTrack()->hitPattern().numberOfValidMuonHits();
923	      edm::ESHandle < TransientTrackBuilder > trackBuilder;
924	      iSetup.get < TransientTrackRecord > ().get ("TransientTrackBuilder", trackBuilder);
925	      TransientTrack tt = trackBuilder->build (it->innerTrack ());
926	      if (vp1) {
927	        double ip = IPTools::absoluteImpactParameter3D (tt, *vp1).second.value ();
928	        muon->IP3D = ip;
929	        double ipe = IPTools::absoluteImpactParameter3D (tt, *vp1).second.error ();
930	        muon->IP3DErr = ipe;
931	      }
932
933	      //MC-info
934	      if (isData != 1 ) {
935	        if ((mu->genLepton () != NULL)) {
936	          muon->addInfo.p4Gen.SetPxPyPzE (mu->genLepton ()->px (), mu->genLepton ()->py (), mu->genLepton ()->pz (), mu->genLepton ()->energy ());
937	        }
938	        Handle < reco::GenParticleCollection > genParticles;
939	        iEvent.getByLabel ("genParticles", genParticles);
940	        reco::GenParticle genmother;
941	        reco::GenParticle gengrandmother;
942	        reco::GenParticle genggrandmother;
943	        math::XYZTLorentzVector Zero (0, 0, 0, 0);
944	        genmother.setCharge (-99);
945	        genmother.setP4 (Zero);
946	        gengrandmother.setCharge (-99);
947	        gengrandmother.setP4 (Zero);
948	        genggrandmother.setCharge (-99);
949	        genggrandmother.setP4 (Zero);
950	        muon->LeptonOrigin = getMuonOrigin (genParticles, mu, genmother, gengrandmother, genggrandmother);
951	        muon->addInfo.GenMother = genmother;
952	        muon->addInfo.GenGrandMother = gengrandmother;
953	        muon->addInfo.GenGGrandMother = genggrandmother;
954	      }
955	      mu = 0;
956	      delete mu;
957	      muon = 0;
958	      delete muon;
959	    }                           // end muon loop
960	    mus = 0;
961	    delete mus;
962	  }
963
964	  //***************************************************************************
965	  // Taus ----------------------------------------------------------------
966	  //***************************************************************************
967	if (!doTaus)
968	    if (verbose > 1)
969	      cout << "Do not fill the taus" << endl;
970	  if (doTaus) {
971	    if (verbose > 1)
972	      cout << "Filling taus" << endl;
973
974
975	   for (unsigned int m = 0 ; m<tauProducer.size(); ++m )
976	   {
977
978	    Handle < vector < pat::Tau > >tauHa;
979	    iEvent.getByLabel (tauProducer.at(m), tauHa);
980	    const vector < pat::Tau > *taus = tauHa.product ();
981
982	    TopTree::MTCollection<TopTreeTau> *ctau = cev.NewTauColl();
983
984	    ctau->algo = tauProducer.at(m);
985	    int iTau = 0 ;
986	    for (vector < pat::Tau >::const_iterator it = taus->begin (); it != taus->end (); it++) {
987
988	      const pat::Tau * patTau = &*it;
989	      if (!patTau)
990	        continue;               // useless ?
991	      // Applying preselection based  on pT, eta, ID
992	      if (patTau->pt () < tau_cut_pt || fabs (patTau->eta ()) > tau_cut_eta)
993	        continue;
994
995	      TopTreeTau *tau = new TopTreeTau ();
996	      tau->p4.SetPxPyPzE (patTau->px (), patTau->py (), patTau->pz (), patTau->energy ());
997	      tau->vertex.SetXYZ (patTau->vx (), patTau->vy (), patTau->vz ());
998	      tau->Charge = patTau->charge ();
999	      //tau->Dxy_BS = patTau->dB();
1000
1001	      /*const pat::TriggerObjectRef trigRef( matchHelper.triggerMatchObject( tauHa, iTau, hltTausMatcher, iEvent, pTrigEvt ) );
1002	      if ( trigRef.isAvailable() ) {
1003	         tau->p4HLT.SetPxPyPzE (trigRef->px (), trigRef->py (), trigRef->pz (), trigRef->energy ());
1004	      } */
1005
1006
1007	      const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( tauHa, iTau, hltTausMatcher45, iEvent, pTrigEvt ) );
1008	      if ( trigRef45.isAvailable() ) {
1009	         tau->p4HLT.SetPxPyPzE (trigRef45->px (), trigRef45->py (), trigRef45->pz (), trigRef45->energy ());
1010	      }
1011
1012	      if( patTau->leadPFChargedHadrCand().isAvailable()) {
1013	        if (patTau->leadPFChargedHadrCand()->trackRef().isNonnull())
1014	        {
1015	         tau->leadTrackPt = patTau->leadPFChargedHadrCand()->trackRef()->pt();
1016	         tau->D0 = patTau->leadPFChargedHadrCand()->trackRef()->dxy(bspot);}
1017	        else if  (patTau->leadPFChargedHadrCand()->gsfTrackRef().isNonnull())
1018	        {
1019	         tau->leadTrackPt = patTau->leadPFChargedHadrCand()->gsfTrackRef()->pt();
1020	         tau->D0 = patTau->leadPFChargedHadrCand()->gsfTrackRef()->dxy(bspot);}
1021	        else
1022	        {
1023	         tau->leadTrackPt = -999.;
1024	         tau->D0 = -999.;}
1025	      }
1026	      else {
1027	        tau->leadTrackPt = -999.;
1028	        tau->D0 = -999.;
1029	      }
1030
1031	      tau->numSigConeTracks = patTau->signalTracks ().size ();
1032	      tau->numIsoConeTracks = patTau->isolationTracks ().size ();
1033
1034	      tau->isolationPFChargedHadrCandsPtSum = patTau->isolationPFChargedHadrCandsPtSum ();
1035	      tau->isolationPFGammaCandsEtSum = patTau->isolationPFGammaCandsEtSum ();
1036	      tau->maximumHCALPFClusterEt = patTau->maximumHCALPFClusterEt ();
1037	      tau->emFraction = patTau->emFraction ();
1038	      tau->hcalTotOverPLead = patTau->hcalTotOverPLead ();
1039	      tau->hcalMaxOverPLead = patTau->hcalMaxOverPLead ();
1040	      tau->hcal3x3OverPLead = patTau->hcal3x3OverPLead ();
1041	      tau->ecalStripSumEOverPLead = patTau->ecalStripSumEOverPLead ();
1042
1043	      tau->decayMode = patTau->decayMode ();
1044	      tau->tauIDs= patTau->tauIDs();
1045
1046	      //MC-info
1047	      if (isData != 1 ) {
1048	      if ((patTau->genLepton () != NULL)) {
1049	        tau->addInfo.p4Gen.SetPxPyPzE (patTau->genLepton ()->px (), patTau->genLepton ()->py (), patTau->genLepton ()->pz (), patTau->genLepton ()->energy ());
1050	      }
1051
1052	      /*
1053	      std::vector<reco::GenParticleRef> associatedGenParticles = patTau->genParticleRefs();
1054	      for ( std::vector<reco::GenParticleRef>::const_iterator it = associatedGenParticles.begin();
1055	       it != associatedGenParticles.end(); ++it ) {
1056	       if ( it->isAvailable() ) {
1057	       const reco::GenParticleRef& genParticle = (*it);
1058	       std::cout << "genParticle->pdgId() "<< genParticle->pdgId() <<" "<<genParticle->pt()<<" "<<genParticle->eta()<<std::endl;
1059	         }
1060	      }*/
1061
1062	      /*const reco::GenJet * gjet = patTau->genJet();
1063	      if (gjet)
1064	      { // valid only if hadronic decay mode
1065	       //std::vector <const GenParticle*> vgjet = gjet->getGenConstituents ();
1066	       //for (unsigned int i=0; i<vgjet.size(); i++)
1067	       // std::cout <<"gen jet pdgid " << vgjet[i]->pdgId() <<std::endl;
1068	       }
1069	      */
1070
1071	      //
1072	      tau->LeptonOrigin = 0;
1073	      if (isData != 1 ) {
1074	        reco::GenParticleRef tauref = patTau->genParticleById(15,0,false);
1075	        if (tauref.isNonnull()) { tau->LeptonOrigin = 15;   }
1076	        reco::GenParticleRef tauref2 = patTau->genParticleById(-15,0,false);
1077	        if (tauref2.isNonnull()){ tau->LeptonOrigin = -15;  }
1078
1079	        reco::GenParticleRef elref = patTau->genParticleById(11,0,false);
1080	        if (elref.isNonnull()  && elref->pt()>10.)  { tau->LeptonOrigin = 11;  }
1081	        reco::GenParticleRef elref2 = patTau->genParticleById(-11,0,false);
1082	        if (elref2.isNonnull() && elref2->pt()>10.) { tau->LeptonOrigin = -11; }
1083	        reco::GenParticleRef muref = patTau->genParticleById(13,0,false);
1084	        if (muref.isNonnull()  && muref->pt()>10.)  { tau->LeptonOrigin = 13;  }
1085	        reco::GenParticleRef muref2 = patTau->genParticleById(-13,0,false);
1086	        if (muref2.isNonnull() && muref2->pt()>10.) { tau->LeptonOrigin = -13; }
1087	      }
1088
1089	      Handle < reco::GenParticleCollection > genParticles;
1090	      iEvent.getByLabel ("genParticles", genParticles);
1091
1092	      reco::GenParticle genmother;
1093	      reco::GenParticle gengrandmother;
1094	      reco::GenParticle genggrandmother;
1095	      math::XYZTLorentzVector Zero (0, 0, 0, 0);
1096
1097	      genmother.setCharge (-99);
1098	      genmother.setP4 (Zero);
1099	      gengrandmother.setCharge (-99);
1100	      gengrandmother.setP4 (Zero);
1101	      genggrandmother.setCharge (-99);
1102	      genggrandmother.setP4 (Zero);
1103	      //tau->LeptonOrigin = -1;
1104	      tau->decay = getTauDecay(genParticles,patTau);
1105	      tau->addInfo.GenMother = genmother;
1106	      tau->addInfo.GenGrandMother = gengrandmother;
1107	      tau->addInfo.GenGGrandMother = genggrandmother;
1108
1109	      }
1110
1111	      ctau->objects.push_back(*tau);
1112	      patTau = 0;
1113	      iTau++;
1114	      delete patTau;
1115	      tau = 0;
1116	      delete tau;
1117	    }                           // end tau loop
1118	    taus = 0;
1119	    delete taus;
1120	   }
1121	  }
1122
1123
1124	  //****************************************************************************
1125	  // Tracks --------------------------------------------------------------------
1126	  //****************************************************************************
1127
1128
1129	  if (!doTracks)
1130	    if (verbose > 1)
1131	      cout << "Do not fill the tracks" << endl;
1132	  if (doTracks) {
1133	    if (verbose > 1)
1134	      cout << "Filling tracks" << endl;
1135	    Handle < std::vector < reco::Track > >trackHa;
1136	    iEvent.getByLabel (trackProducer, trackHa);
1137	    const std::vector < reco::Track > *tracks = trackHa.product ();
1138
1139	    for (std::vector < reco::Track >::const_iterator it = tracks->begin (); it != tracks->end (); it++) {
1140	      const reco::Track * track = &*it;
1141
1142	      // Applying a preselection based on on pT, eta
1143	      if (track->pt () < track_cut_pt || fabs (track->eta ()) > track_cut_eta)
1144	        continue;
1145
1146	      TopTreeTrack    *tttrack = cev.NewTrack ();
1147	      tttrack->p4.SetPxPyPzE (track->px (), track->py (), track->pz (), track->p ());
1148	      tttrack->Chi2 = track->chi2 () / track->ndof ();
1149	      tttrack->nHitTracker = track->hitPattern ().numberOfValidHits ();
1150	      tttrack->d0 = track->d0 ();
1151	      tttrack->d0Error = track->d0Error ();
1152	      tttrack->dz = track->dz ();
1153	      tttrack->dzError = track->dzError ();
1154	      tttrack->dxy = track->dxy ();
1155	      tttrack->dxyError = track->dxyError ();
1156	      tttrack->charge = track->charge ();
1157	      if (beamSpotHa.isValid ()) {
1158	        beamSpot = *beamSpotHa;
1159	        tttrack->dxy_BS = track->dxy (beamSpot.position ());
1160	      }
1161	      track = 0;
1162	      delete track;
1163	      tttrack = 0;
1164	      delete tttrack;
1165	    }                           // end tracks loop
1166	    tracks = 0;
1167	    delete tracks;
1168	  }
1169
1170
1171
1172	  //****************************************************************************
1173	  // JetMET --------------------------------------------------------------------
1174	  //****************************************************************************
1175
1176
1177	  //prepare residual corrections
1178	  //string JEC_PATH("CondFormats/JetMETObjects/data/");
1179	  /*
1180	  edm::FileInPath fipRes_PF(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5PF.txt");
1181	  JetCorrectorParameters *ResJetCorPar_PF = new JetCorrectorParameters(fipRes_PF.fullPath());
1182	  std::vector<JetCorrectorParameters> vparam_PF;
1183	  vparam_PF.push_back(*ResJetCorPar_PF);
1184	  FactorizedJetCorrector *resJEC_PF = new FactorizedJetCorrector(vparam_PF);
1185
1186
1187	  edm::FileInPath fipRes_JPT(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5JPT.txt");
1188	  JetCorrectorParameters *ResJetCorPar_JPT = new JetCorrectorParameters(fipRes_JPT.fullPath());
1189	  std::vector<JetCorrectorParameters> vparam_JPT;
1190	  vparam_JPT.push_back(*ResJetCorPar_JPT);
1191	  FactorizedJetCorrector *resJEC_JPT = new FactorizedJetCorrector(vparam_JPT);
1192
1193
1194	  edm::FileInPath fipRes_Calo(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5Calo.txt");
1195	  JetCorrectorParameters *ResJetCorPar_Calo = new JetCorrectorParameters(fipRes_Calo.fullPath());
1196	  std::vector<JetCorrectorParameters> vparam_Calo;
1197	  vparam_Calo.push_back(*ResJetCorPar_Calo);
1198	  FactorizedJetCorrector *resJEC_Calo = new FactorizedJetCorrector(vparam_Calo);
1199
1200	  */
1201
1202	  //get Rho (PU/UE densities)
1203	  edm::Handle<double> h_rho;
1204	  iEvent.getByLabel( rho_PUUE_dens, h_rho);
1205	  //iEvent.getByLabel( edm::InputTag("kt6PFJets", "rho"), h_rho);
1206	  //                    edm::InputTag("kt6PFJets", "rho")
1207
1208	  cev.rho_PUUE_dens = (*h_rho);
1209
1210
1211	  //get Rho (PU/UE densities)
1212	  edm::Handle<double> h_neutralRho;
1213	  iEvent.getByLabel( neutralRho_PUUE_dens, h_neutralRho);
1214
1215	  cev.neutralRho_PUUE_dens = (*h_neutralRho);
1216
1217
1218	  if (verbose > 1)
1219	    cout << "Filling JETMET" << endl;
1220	  // Compute correction needed for tcMET
1221
1222	  Handle < edm::ValueMap < reco::MuonMETCorrectionData > >tcMet_ValueMap_Handle;
1223	  iEvent.getByLabel ("muonTCMETValueMapProducer", "muCorrData", tcMet_ValueMap_Handle);
1224
1225	  Handle < vector < reco::Muon > >muon_h;
1226	  iEvent.getByLabel ("muons", muon_h);
1227
1228	  float SumMuMetCorrX = 0.;
1229	  float SumMuMetCorrY = 0.;
1230
1231	  const unsigned int nMuons = muon_h->size ();
1232	  for (unsigned int mus = 0; mus < nMuons; mus++) {
1233	    reco::MuonRef muref (muon_h, mus);
1234	    reco::MuonMETCorrectionData muCorrData = (*tcMet_ValueMap_Handle)[muref];
1235	    if (muCorrData.type () == 0 || muCorrData.type () == 4) {
1236	      SumMuMetCorrX += muCorrData.corrX () - muref->px ();
1237	      SumMuMetCorrY += muCorrData.corrY () - muref->py ();
1238	    }
1239	  }
1240
1241
1242	  VParameters::iterator ijetmet = jetmetProducer.begin ();
1243	  VParameters::iterator ijetmetEnd = jetmetProducer.end ();
1244
1245	  for (; ijetmet != ijetmetEnd; ++ijetmet) {
1246	    std::string jet_ = ijetmet->getUntrackedParameter < std::string > ("jet");
1247	    std::string met_ = ijetmet->getUntrackedParameter < std::string > ("met");
1248	    std::string algo_ = ijetmet->getUntrackedParameter < std::string > ("algo");
1249
1250	    TopTreeJetMet *cjetmet = cev.NewJetMet ();
1251	    cjetmet->algo = algo_;
1252
1253	    edm::Handle < edm::View < pat::MET > >metHandle;
1254	    iEvent.getByLabel (met_, metHandle);
1255	    edm::View < pat::MET > mets = *metHandle;
1256	    if (metHandle->size () != 0) {
1257	      const pat::MET met = mets.front ();
1258
1259
1260	      cjetmet->met.p4.SetPxPyPzE (met.px (), met.py (), met.pz (), met.energy());       // This is global (JES+muon) corrected MET
1261	      cjetmet->met.misEt = met.pt ();
1262	      cjetmet->met.uncmisEt = met.uncorrectedPt ();
1263	      cjetmet->met.sumEt = met.sumEt ();
1264	      // NB: met - globaluncorrection = uncorrectedmet : met.px() - met.corEx(uc0) = uncmisEt.px(), idem for py
1265
1266	      pat::MET::UncorrectionType uc0;   // { uncorrALL, uncorrJES, uncorrMUON, uncorrMAXN }
1267	      uc0 = pat::MET::UncorrectionType (0);
1268	      pat::MET::UncorrectionType uc1;
1269	      uc1 = pat::MET::UncorrectionType (1);
1270	      pat::MET::UncorrectionType uc2;
1271	      uc2 = pat::MET::UncorrectionType (2);
1272
1273	      double cor_0_x = met.corEx (uc0);
1274	      double cor_0_y = met.corEy (uc0);
1275
1276	      // MET global uncorrections (to be added if needed):
1277	      cjetmet->met.dmEx = cor_0_x;
1278	      cjetmet->met.dmEy = cor_0_y;
1279
1280	      if (algo_ == "tc") {
1281	        cjetmet->met.doCorrection (SumMuMetCorrX, SumMuMetCorrY);
1282	        cjetmet->met.p4MuonCorr.SetPxPyPzE (met.px () + SumMuMetCorrX, met.py () + SumMuMetCorrY, 0.0, 0.0);
1283	      }
1284
1285	    }
1286
1287
1288	    //****************************************************************************
1289	    // Jets ----------------------------------------------------------------------
1290	    //****************************************************************************
1291
1292
1293	    if (verbose > 1)
1294	      cout << "Filling jets" << endl;
1295	    Handle < vector < pat::Jet > >jetHa;
1296	    iEvent.getByLabel (jet_, jetHa);
1297	    const vector < pat::Jet > *jets = jetHa.product ();
1298
1299
1300	    JetIDSelectionFunctor jetIDLoose (JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::LOOSE);
1301	    PFJetIDSelectionFunctor PfjetIDLoose (PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::LOOSE);
1302	    JetIDSelectionFunctor jetIDTight (JetIDSelectionFunctor::PURE09, JetIDSelectionFunctor::TIGHT);
1303	    PFJetIDSelectionFunctor PfjetIDTight (PFJetIDSelectionFunctor::FIRSTDATA, PFJetIDSelectionFunctor::TIGHT);
1304
1305	    int iJet = 0;
1306	    for (vector < pat::Jet >::const_iterator it = jets->begin (); it != jets->end (); it++) {
1307
1308	      double scale = -99;
1309	      double jetPT = it->pt ();
1310	//modifdg
1311	      //if (it->corrStep () == "abs" || it->corrStep () == "ABS") {
1312	        scale = it->jecFactor("Uncorrected");      // scale factor from current jet with correction (abs=L1+L2+L3) to raw jet
1313	         //}
1314
1315	      //Applying a preselection based on pT, eta
1316	      if (jetPT < jet_cut_pt || abs (it->eta ()) > jet_cut_eta)
1317	        continue;
1318
1319	      //create Jet --------------
1320	      TopTreeJet *caloJet = new TopTreeJet ();
1321
1322	      //fill jet infos --------------------
1323	      caloJet->p4.SetPxPyPzE (it->px (), it->py (), it->pz (), it->energy ());
1324	       if(!isData && it->genJet() != 0) caloJet->p4Gen.SetPxPyPzE (it->genJet()->px (), it->genJet()->py (), it->genJet()->pz (), it->genJet()->energy ());
1325	       if(!isData && it->genParton() != 0 ) caloJet->p4Parton.SetPxPyPzE (it->genParton()->px (), it->genParton()->py (), it->genParton()->pz (), it->genParton()->energy ());
1326	        caloJet->scale = scale;
1327	      reco::JetID jid = it->jetID ();
1328	      caloJet->n90Hits = jid.n90Hits;
1329	      caloJet->fHPD = jid.fHPD;
1330
1331	      // add uncorr jet
1332
1333	      caloJet->uncorrJet = it->correctedP4(0);
1334	      caloJet->jetArea = it->jetArea();
1335	      caloJet->corrJES = (it->correctedJet("Uncorrected")).pt()/it->pt();
1336
1337
1338
1339
1340	      //caloJet->resCorr = -1;
1341
1342	      pat::strbitset ret = jetIDLoose.getBitTemplate ();
1343	      pat::strbitset retpf = PfjetIDLoose.getBitTemplate ();
1344	      ret.set (false);
1345	      retpf.set (false);
1346	      pat::strbitset rett = jetIDTight.getBitTemplate ();
1347	      pat::strbitset rettpf = PfjetIDTight.getBitTemplate ();
1348	      rett.set (false);
1349	      rettpf.set (false);
1350
1351	      // Tc jet ID not supported
1352	      if (it->isCaloJet () || it->isJPTJet ()) {
1353	        caloJet->isLOOSE = jetIDLoose (*it, ret);
1354	        caloJet->isTIGHT = jetIDTight (*it, rett);
1355	        //get residual corr
1356	        if (it->isCaloJet ()){
1357	          //resJEC_Calo->setJetEta(it->eta());
1358	          //resJEC_Calo->setJetPt(it->pt());
1359	          //caloJet->resCorr = resJEC_Calo->getCorrection();
1360	        }else{
1361	          //resJEC_JPT->setJetEta(it->eta());
1362	          //resJEC_JPT->setJetPt(it->pt());
1363	          //caloJet->resCorr = resJEC_JPT->getCorrection();
1364	        }
1365
1366	      }
1367	      if (it->isPFJet ()) {
1368	        caloJet->isLOOSE = PfjetIDLoose (*it, retpf);
1369	        caloJet->isTIGHT = PfjetIDTight (*it, rettpf);
1370	        //get residual corr
1371	        //resJEC_PF->setJetEta(it->eta());
1372	        ///resJEC_PF->setJetPt(it->pt());
1373	        //caloJet->resCorr = resJEC_PF->getCorrection();
1374	      }
1375
1376	      if (it->isCaloJet () || it->isJPTJet () ) {
1377	        caloJet->emEnergyFraction = it->emEnergyFraction ();
1378	        caloJet->hadEnergyFraction = it->energyFractionHadronic ();
1379	      }
1380	      else if (it->isPFJet ()) {
1381	        caloJet->emEnergyFraction = -99;
1382	        caloJet->hadEnergyFraction = -99;
1383	        caloJet->CHEF = it->chargedHadronEnergyFraction ();
1384	        caloJet->CEEF = it->chargedEmEnergyFraction ();
1385	        caloJet->NHEF = it->neutralHadronEnergyFraction ();
1386	        caloJet->NEEF = it->neutralEmEnergyFraction ();
1387	      }
1388	      caloJet->nTracks = it->associatedTracks().size();
1389	      float sumPtTracks = 0;
1390	      for(unsigned int tr=0;tr<it->associatedTracks().size();tr++) sumPtTracks+=it->associatedTracks()[tr]->pt();
1391	      caloJet->sumPtTracks = sumPtTracks;
1392
1393	      //modifdg
1394	      if (isData != 1 ) {
1395
1396	        caloJet->partonFlavour = it->partonFlavour ();
1397
1398	      }
1399	      //Here
1400	      const reco::TrackRefVector & TrkJet = it->associatedTracks ();
1401	      for (unsigned int itk = 0; itk < TrkJet.size (); itk++) {
1402	        const reco::Track tk = *TrkJet[itk];
1403	      }
1404	      if(!isAOD){
1405
1406	        const reco::TrackIPTagInfo * TIP = it->tagInfoTrackIP ();
1407	        const reco::TrackRefVector & T = TIP->selectedTracks ();
1408
1409	        std::vector < reco::TrackIPTagInfo::TrackIPData > ip = TIP->impactParameterData ();
1410
1411	        // retrieve the "secondary vertex tag infos"
1412	        const reco::SecondaryVertexTagInfo * SVT = it->tagInfoSecondaryVertex ();
1413	        const reco::TrackRefVector & S = SVT->vertexTracks();
1414
1415	        vector < float >prob2d, prob3d;
1416
1417	        prob2d.clear ();
1418	        prob3d.clear ();
1419
1420	        if (TIP->hasProbabilities ()) {
1421	          prob2d = TIP->probabilities (0);
1422	          prob3d = TIP->probabilities (1);
1423	        }
1424	        for (unsigned int itk = 0; itk < T.size (); itk++) {
1425
1426	          const reco::Track tk = *T[itk];
1427	          TopTreeTrack *asstrack = caloJet->NewTrack ();
1428	          asstrack->p4.SetPxPyPzE (tk.px (), tk.py (), tk.pz (), tk.p ());
1429	          asstrack->Chi2 = tk.chi2 () / tk.ndof ();
1430	          asstrack->nHitTracker = tk.hitPattern ().numberOfValidHits ();
1431	          asstrack->nHitPixel = tk.hitPattern ().numberOfValidPixelHits ();
1432	          asstrack->withFirstPixel = tk.hitPattern ().hasValidHitInFirstPixelBarrel ();
1433	          asstrack->nHitPXB = tk.hitPattern ().numberOfValidPixelBarrelHits ();
1434	          asstrack->nHitPXF = tk.hitPattern ().numberOfValidPixelEndcapHits ();
1435	          asstrack->nHitTIB = tk.hitPattern ().numberOfValidStripTIBHits ();
1436	          asstrack->nHitTOB = tk.hitPattern ().numberOfValidStripTOBHits ();
1437	          asstrack->nHitTID = tk.hitPattern ().numberOfValidStripTIDHits ();
1438	          asstrack->nHitTEC = tk.hitPattern ().numberOfValidStripTECHits ();
1439	          asstrack->charge = tk.charge ();
1440	          asstrack->IP3Dsig = ip[itk].ip3d.significance ();
1441	          asstrack->IP3D = ip[itk].ip3d.value ();
1442	          asstrack->IP3Derr = ip[itk].ip3d.error ();
1443	          asstrack->IP2Dsig = ip[itk].ip2d.significance ();
1444	          asstrack->IP2D = ip[itk].ip2d.value ();
1445	          asstrack->IP2Derr = ip[itk].ip2d.error ();
1446	          if (itk < prob3d.size ())
1447	            asstrack->Prob3D = prob3d[itk];
1448	          if (itk < prob2d.size ())
1449	            asstrack->Prob2D = prob2d[itk];
1450	            GlobalPoint pv (TIP->primaryVertex ()->position ().x (), TIP->primaryVertex ()->position ().y (), TIP->primaryVertex ()->position ().z ());
1451	            asstrack->DecayLength = (ip[itk].closestToJetAxis - pv).mag ();
1452	            asstrack = 0;
1453	            delete asstrack;
1454
1455	        }
1456
1457	        std::vector < std::size_t > sortedIndices = TIP->sortedIndexes (reco::TrackIPTagInfo::IP3DSig);
1458
1459	        GlobalPoint pv (TIP->primaryVertex ()->position ().x (), TIP->primaryVertex ()->position ().y (), TIP->primaryVertex ()->position ().z ());
1460	        //const reco::TrackRefVector & TS = TIP-> sortedTracks (sortedIndices);
1461
1462	        caloJet->nTracksSVX = S.size ();
1463	        caloJet->nSVX = SVT->nVertices ();
1464	        if (SVT->nVertices () >= 1) {
1465	          // pick the first secondary vertex (the "best" one)
1466	          const reco::Vertex & sv = SVT->secondaryVertex (0);
1467	          caloJet->chi2SVX = sv.chi2 ();
1468	          caloJet->ndofSVX = sv.ndof ();
1469
1470	          caloJet->flightDistanceSVX = SVT->flightDistance (0, false).value ();
1471	          caloJet->flightDistanceSigSVX = SVT->flightDistance (0, false).significance ();
1472	          caloJet->flightDistanceErrSVX = SVT->flightDistance (0, false).error ();
1473	          // compute the invariant mass from a four-vector sum
1474	          TLorentzVector trackFourVectorSum;
1475
1476	          // loop over all tracks in the vertex
1477	          for (reco::Vertex::trackRef_iterator track = sv.tracks_begin (); track != sv.tracks_end (); ++track) {
1478	            TLorentzVector vec;
1479	            vec.SetXYZM ((*track)->px (), (*track)->py (), (*track)->pz (), 0.13957);   // pion mass
1480	            trackFourVectorSum += vec;
1481	          }
1482	          caloJet->MassSVX = trackFourVectorSum.M ();
1483
1484	        }
1485
1486	        TIP = 0;
1487	        delete TIP;
1488	        SVT = 0;
1489	        delete SVT;
1490	      }
1491
1492
1493	      caloJet->DiscriPair = it->getPairDiscri ();
1494	      caloJet->vertex.SetXYZ (it->vx (), it->vy (), it->vz ());
1495	      caloJet->TCDiscri = it->bDiscriminator ("trackCountingHighEffBJetTags");
1496	      // ATTENTION : Pour certain processing des data reelles il faut utiliser l'une ou lautre de ces 2 lignes
1497	      double SV1 = it->bDiscriminator ("simpleSecondaryVertexBJetTags");
1498	      double SV2 = it->bDiscriminator ("simpleSecondaryVertexHighEffBJetTags");
1499	      if (SV2 == -1000) SV2 = SV1;
1500	      caloJet->SVDiscri = SV2;
1501
1502	      caloJet->SMDiscri = it->bDiscriminator ("softMuonBJetTags");
1503	      caloJet->setTracks (it->associatedTracks ());
1504	      /*
1505	      const pat::TriggerObjectRef trigRef( matchHelper.triggerMatchObject( jetHa, iJet, hltJetsMatcher, iEvent, pTrigEvt ) );
1506	      if ( trigRef.isAvailable() ) {
1507	         caloJet->p4HLT.SetPxPyPzE (trigRef->px (), trigRef->py (), trigRef->pz (), trigRef->energy ());}
1508	      */
1509
1510	      const pat::TriggerObjectRef trigRef45( matchHelper.triggerMatchObject( jetHa, iJet, hltJetsMatcher45, iEvent, pTrigEvt ) );
1511	      if ( trigRef45.isAvailable() ) {
1512	         caloJet->p4HLT.SetPxPyPzE (trigRef45->px (), trigRef45->py (), trigRef45->pz (), trigRef45->energy ());
1513	      }
1514
1515	      cjetmet->objects.push_back (*caloJet);
1516
1517	      cjetmet->sumEtJet += it->et ();
1518
1519	      cjetmet->sumEtJetRaw += it->et () / scale;
1520
1521	      iJet++;
1522
1523	      //TIP = 0;
1524	      //delete TIP;
1525	      //SVT = 0;
1526	      //delete SVT;
1527	      delete caloJet;
1528	    }                           // end jet loop
1529	    cjetmet = 0;// other delete object belonging to cevt
1530	    delete cjetmet;
1531	    jets = 0;// handle on file
1532	    delete jets;
1533	  }
1534
1535
1536
1537	  //ResJetCorPar_PF = 0 ;
1538	  //delete ResJetCorPar_PF ;
1539	  //resJEC_PF = 0  ;
1540	  //delete resJEC_PF ;
1541
1542
1543	 // ResJetCorPar_JPT = 0  ;
1544	 // delete ResJetCorPar_JPT ;
1545	 // resJEC_JPT = 0  ;
1546	 // delete resJEC_JPT ;
1547
1548
1549	 // ResJetCorPar_Calo = 0  ;
1550	  //delete ResJetCorPar_Calo ;
1551	  //resJEC_Calo  = 0 ;
1552	 // delete resJEC_Calo ;
1553
1554
1555
1556
1557
1558	  // is put into the Event
1559	  std::auto_ptr < TopTree::MTEvent > pOut (new TopTree::MTEvent (cev));
1560	  iEvent.put (pOut);
1561	  if (verbose > 0)
1562	    cout << "Event registered in the MiniTree" << endl;
1563
1564	  // --------------------
1565	  //    Deleting some pointers
1566	  // --------------------
1567	  vp1 = 0;
1568	  delete vp1;
1569
1570	}
1571
1572	// ------------ method called once each job just before starting event loop  ------------
1573	void MiniTreeProducer::beginJob ()
1574	{
1575	  cout << "BeginJob for MiniTreeProducer " << endl;
1576	}
1577
1578	// ------------ method called once each job just after ending the event loop  ------------
1579	void MiniTreeProducer::endJob ()
1580	{
1581	  cout << "EndJob for MiniTreeProducer" << endl;
1582	}
1583
1584
1585	bool MiniTreeProducer::MyJetCleaner (const vector < pat::Electron > *electrons, pat::Jet J)
1586	{
1587	  bool passed = true;
1588	  //     for(vector<pat::Electron>::const_iterator it = electrons->begin(); it!=electrons->end(); it++){
1589	  //       const pat::Electron* elec = &*it;
1590	  //       std::cout<<"test MyJetCleaner0 "<<elec->pt()<< " "<<elec->trackIso()<< " "<<elec->caloIso()<<std::endl;
1591	  //     }
1592	  return passed;
1593	}
1594
1595
1596	int
1597	  MiniTreeProducer::getMuonOrigin (edm::Handle < reco::GenParticleCollection > genParticles, const pat::Muon * thePatMuon,
1598	                                   reco::GenParticle & MotherGenMuon, reco::GenParticle & GrandMotherGenMuon, reco::GenParticle & GGrandMotherGenMuon)
1599	{
1600
1601	  int muonOrigin = -99;
1602	  const reco::Candidate * theGenMuon;
1603	  reco::GenParticle test;
1604	  bool matchedGenLepton = false;
1605
1606	  for (reco::GenParticleCollection::const_iterator p = genParticles->begin (); p != genParticles->end (); ++p) {
1607	    const reco::Candidate * aGenMuon = (dynamic_cast < reco::Candidate * >(const_cast < reco::GenParticle * >(&*p)));
1608
1609	    if (abs (p->pdgId ()) == 13 && p->status () == 1) {
1610	      if ((thePatMuon->genLepton () != NULL) && abs (thePatMuon->genLepton ()->pt () - aGenMuon->pt ()) < 0.00001) {
1611	        theGenMuon = aGenMuon;
1612	        matchedGenLepton = true;
1613	      }
1614	    }
1615	  }
1616
1617	  if (matchedGenLepton) {
1618	    muonOrigin = 0;
1619	    bool isFromBDecay = false;
1620	    bool isFromCDecay = false;
1621	    if (theGenMuon->mother () != 0 && abs (theGenMuon->pdgId ()) == 13) {
1622	      const reco::Candidate * aMotherGenMuon1 = theGenMuon->mother ();
1623	      const reco::Candidate * aMotherGenMuon2 = theGenMuon->mother ();
1624	      setGenParticleFromCandidate (MotherGenMuon, theGenMuon->mother ());
1625	      //                        MotherGenMuon = *theGenMuon->mother();
1626	      int gm = 0;
1627	      while (aMotherGenMuon2->mother () != 0) {
1628	        gm++;
1629	        aMotherGenMuon2 = aMotherGenMuon2->mother ();
1630	        if (abs (aMotherGenMuon2->pdgId ()) == 24 && abs (aMotherGenMuon1->pdgId ()) == 13)
1631	          muonOrigin = muonOrigin + 1;  // muon from W
1632	        if (abs (aMotherGenMuon2->pdgId ()) == 24 && abs (aMotherGenMuon1->pdgId ()) == 15)
1633	          muonOrigin = muonOrigin + 1;  // muon from W->tau
1634	        if (abs (aMotherGenMuon2->pdgId ()) == 23 && abs (aMotherGenMuon1->pdgId ()) == 13)
1635	          muonOrigin = muonOrigin + 10;
1636	        if (abs (aMotherGenMuon1->pdgId ()) == 4 || (abs (aMotherGenMuon1->pdgId ()) > 39 && abs (aMotherGenMuon1->pdgId ()) < 50)
1637	            || (abs (aMotherGenMuon1->pdgId ()) > 390 && abs (aMotherGenMuon1->pdgId ()) < 500) || (abs (aMotherGenMuon1->pdgId ()) > 3900 && abs (aMotherGenMuon1->pdgId ()) < 5000))
1638	          isFromCDecay = true;
1639	        if (abs (aMotherGenMuon1->pdgId ()) == 5 || (abs (aMotherGenMuon1->pdgId ()) > 49 && abs (aMotherGenMuon1->pdgId ()) < 60)
1640	            || (abs (aMotherGenMuon1->pdgId ()) > 490 && abs (aMotherGenMuon1->pdgId ()) < 600) || (abs (aMotherGenMuon1->pdgId ()) > 4900 && abs (aMotherGenMuon1->pdgId ()) < 6000))
1641	          isFromBDecay = true;
1642	        //                              if (gm==1) GrandMotherGenMuon = *aMotherGenMuon2;
1643	        //                              if (gm==2) GGrandMotherGenMuon = *aMotherGenMuon2;
1644	        if (gm == 1)
1645	          setGenParticleFromCandidate (GrandMotherGenMuon, aMotherGenMuon2);
1646	        if (gm == 2)
1647	          setGenParticleFromCandidate (GGrandMotherGenMuon, aMotherGenMuon2);
1648	      }
1649	      aMotherGenMuon1 = aMotherGenMuon2;
1650	    }
1651	    if (isFromCDecay)
1652	      muonOrigin = muonOrigin + 1000;
1653	    if (isFromBDecay)
1654	      muonOrigin = muonOrigin + 100;
1655	  }
1656	  return muonOrigin;            //REMARK : cbZW format!
1657	}
1658
1659	int
1660	  MiniTreeProducer::getElectronOrigin (edm::Handle < reco::GenParticleCollection > genParticles, const pat::Electron * thePatElectron, reco::GenParticle & MotherGenElectron,
1661	                                       reco::GenParticle & GrandMotherGenElectron, reco::GenParticle & GGrandMotherGenElectron)
1662	{
1663
1664	  int electronOrigin = -99;
1665	  reco::Candidate * theGenElectron;
1666	  bool matchedGenLepton = false;
1667
1668	  for (reco::GenParticleCollection::const_iterator p = genParticles->begin (); p != genParticles->end (); ++p) {
1669	    reco::Candidate * aGenElectron = (dynamic_cast < reco::Candidate * >(const_cast < reco::GenParticle * >(&*p)));
1670
1671	    if (abs (p->pdgId ()) == 11 && p->status () == 1) {
1672	      if ((thePatElectron->genLepton () != NULL) && abs (thePatElectron->genLepton ()->pt () - aGenElectron->pt ()) < 0.00001) {
1673	        theGenElectron = aGenElectron;
1674	        matchedGenLepton = true;
1675	      }
1676	    }
1677	  }
1678
1679	  if (matchedGenLepton) {
1680	    electronOrigin = 0;
1681	    bool isFromBDecay = false;
1682	    bool isFromCDecay = false;
1683	    if (theGenElectron->mother () != 0 && abs (theGenElectron->pdgId ()) == 11) {
1684	      const reco::Candidate * aMotherGenElectron1 = theGenElectron->mother ();
1685	      const reco::Candidate * aMotherGenElectron2 = theGenElectron->mother ();
1686	      setGenParticleFromCandidate (MotherGenElectron, theGenElectron->mother ());
1687	      //                        MotherGenElectron = *theGenElectron->mother();
1688	      int gm = 0;
1689	      while (aMotherGenElectron2->mother () != 0) {
1690	        gm++;
1691	        aMotherGenElectron2 = aMotherGenElectron2->mother ();
1692	        if (abs (aMotherGenElectron2->pdgId ()) == 24 && abs (aMotherGenElectron1->pdgId ()) == 11)
1693	          electronOrigin = electronOrigin + 1;
1694	        if (abs (aMotherGenElectron2->pdgId ()) == 24 && abs (aMotherGenElectron1->pdgId ()) == 15)
1695	          electronOrigin = electronOrigin + 1;
1696	        if (abs (aMotherGenElectron2->pdgId ()) == 23 && abs (aMotherGenElectron1->pdgId ()) == 11)
1697	          electronOrigin = electronOrigin + 10;
1698	        if (abs (aMotherGenElectron1->pdgId ()) == 4 || (abs (aMotherGenElectron1->pdgId ()) > 39 && abs (aMotherGenElectron1->pdgId ()) < 50)
1699	            || (abs (aMotherGenElectron1->pdgId ()) > 390 && abs (aMotherGenElectron1->pdgId ()) < 500) || (abs (aMotherGenElectron1->pdgId ()) > 3900 && abs (aMotherGenElectron1->pdgId ()) < 5000))
1700	          isFromCDecay = true;
1701	        if (abs (aMotherGenElectron1->pdgId ()) == 5 || (abs (aMotherGenElectron1->pdgId ()) > 49 && abs (aMotherGenElectron1->pdgId ()) < 60)
1702	            || (abs (aMotherGenElectron1->pdgId ()) > 490 && abs (aMotherGenElectron1->pdgId ()) < 600) || (abs (aMotherGenElectron1->pdgId ()) > 4900 && abs (aMotherGenElectron1->pdgId ()) < 6000))
1703	          isFromBDecay = true;
1704	        //                              if (gm==1) GrandMotherGenElectron = *aMotherGenElectron2;
1705	        //                              if (gm==2) GGrandMotherGenElectron = *aMotherGenElectron2;
1706	        if (gm == 1)
1707	          setGenParticleFromCandidate (GrandMotherGenElectron, aMotherGenElectron2);
1708	        if (gm == 2)
1709	          setGenParticleFromCandidate (GGrandMotherGenElectron, aMotherGenElectron2);
1710	      }
1711	      aMotherGenElectron1 = aMotherGenElectron2;
1712	    }
1713
1714	    if (isFromCDecay)
1715	      electronOrigin = electronOrigin + 1000;
1716	    if (isFromBDecay)
1717	      electronOrigin = electronOrigin + 100;
1718	  }
1719	  return electronOrigin;
1720	}
1721
1722	int
1723	  MiniTreeProducer::getTauDecay(edm::Handle<reco::GenParticleCollection> genParticles,
1724	                                const pat::Tau *thePatTau)
1725	{
1726
1727	  int tauDecay = -999;
1728	  bool matchedGenLepton = false;
1729	  reco::GenParticleCollection::const_iterator pMatched;
1730
1731	  for (reco::GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++p){
1732	    reco::Candidate * aGenTau = (dynamic_cast<reco::Candidate *>(const_cast<reco::GenParticle *>(&*p)));
1733
1734
1735	    if (abs(p->pdgId()) == 15 && p->status() == 2){
1736	#if DEBUG
1737	      cout << "getTauOrigin: thePatElectron->genLepton() " << thePatTau->genLepton() <<endl;
1738	#endif
1739	      if ((thePatTau->genLepton() != NULL) && abs(thePatTau->genLepton()->pt()-aGenTau->pt()) < 0.00001){
1740	        matchedGenLepton = true;
1741	        pMatched = p;
1742	      }
1743	    }
1744	  }
1745
1746	#if DEBUG
1747	  cout << "getTauDecay: matchedGenLepton " << matchedGenLepton<<endl;
1748	#endif
1749	  if (matchedGenLepton){
1750	    tauDecay = -99;
1751
1752	    int tau_children_n = pMatched->numberOfDaughters();
1753	    int sumPdgId = 0;
1754
1755	       for (int k=0; k<tau_children_n; k++)
1756	       {
1757	         int dpdgId = abs(pMatched->daughter(k)->pdgId());
1758	         sumPdgId += dpdgId;
1759
1760	         if (dpdgId == 223 || dpdgId == 221 || dpdgId == 213 || dpdgId == 113 || dpdgId == 323)
1761	         {
1762	           if(pMatched->daughter(k)->status() != 1)
1763	           {
1764	           sumPdgId -= dpdgId;
1765
1766	           for (unsigned int ww=0; ww<pMatched->daughter(k)->numberOfDaughters(); ww++)
1767	           {
1768	            sumPdgId += abs(pMatched->daughter(k)->daughter(ww)->pdgId());
1769
1770	            if (abs(pMatched->daughter(k)->daughter(ww)->pdgId())==311 && pMatched->daughter(k)->daughter(ww)->status()!=1)
1771	            {
1772	              sumPdgId -= 311;
1773	              for (unsigned int v=0; v<pMatched->daughter(k)->daughter(ww)->numberOfDaughters(); v++)
1774	              {
1775	               sumPdgId += pMatched->daughter(k)->daughter(ww)->daughter(v)->pdgId();
1776	              }
1777	             }
1778	            }
1779	           }
1780	          }
1781	         }
1782
1783
1784	      if (sumPdgId ==227)                                                                       { tauDecay = 0;  }//pi+nu
1785	      if (sumPdgId ==229)                                                                       { tauDecay = 1;  }//pi+pi0nu
1786	      if (sumPdgId ==449 || sumPdgId ==338 || sumPdgId ==340)                                   { tauDecay = 2;  }//pi+2pi0nu
1787	      if (sumPdgId ==560)                                                                       { tauDecay = 3;  }//pi+3pi0nu
1788	      if (sumPdgId ==671)                                                                       { tauDecay = 4;  }//pi+4pi0nu
1789	      if (sumPdgId ==315)                                                                       { tauDecay = 5;  }//pi+gammanu
1790	      if (sumPdgId ==360 || sumPdgId ==382)                                                     { tauDecay = 6;  }//pi+pi0nugamma(s)
1791	      if (sumPdgId ==537 || sumPdgId ==357 || sumPdgId ==538)                                   { tauDecay = 7;  }//pi+k0nu
1792	      if (sumPdgId ==468 || sumPdgId ==648 || sumPdgId ==487 || sumPdgId==667 || sumPdgId ==847){ tauDecay = 8;  }//pi+2n(w K0)nu
1793	      if (sumPdgId ==760 || sumPdgId ==769 || sumPdgId ==759)                                   { tauDecay = 9;  }//pi+3n(w pi0)nu
1794	      if (sumPdgId ==471)                                                                       { tauDecay = 10; }//pi+2pi0nugamma
1795
1796	      if (sumPdgId ==649)                                                                       { tauDecay = 30; }//3pi+nu
1797	      if (sumPdgId ==760)                                                                       { tauDecay = 31; }//3pi+pi0nu
1798	      if (sumPdgId ==782)                                                                       { tauDecay = 34; }//3pi+pi0nugamma
1799	      if (sumPdgId ==871)                                                                       { tauDecay = 32; }//3pi+2pi0nu
1800	      if (sumPdgId ==982)                                                                       { tauDecay = 33; }//3pi+3pi0nu
1801
1802	      if (sumPdgId ==337)                                                                       { tauDecay = 20; }//k+nu
1803	      if (sumPdgId ==448)                                                                       { tauDecay = 21; }//k+pi0nu
1804	      if (sumPdgId ==467 || sumPdgId ==647)                                                     { tauDecay = 22; }//k+k0nu
1805	      if (sumPdgId ==559)                                                                       { tauDecay = 23; }//k+2pi0nu
1806	      if (sumPdgId ==578 || sumPdgId ==758)                                                     { tauDecay = 24; }//k+pi0k0nu
1807	      if (sumPdgId ==670)                                                                       { tauDecay = 25; }//k+3pi0nu
1808
1809	      if (sumPdgId ==869)                                                                       { tauDecay = 40; }//k+k+pi+nu
1810
1811	      if (sumPdgId ==1071)                                                                      { tauDecay = 50; }//5pinu
1812	      if (sumPdgId ==1182)                                                                      { tauDecay = 51; }//5pipi0nu
1813
1814	      if (sumPdgId ==39)                                                                        { tauDecay = 100;}//enunu
1815	      if (sumPdgId ==43)                                                                        { tauDecay = 200;}//mununu
1816
1817	   }
1818
1819	  return tauDecay;
1820	}
1821
1822	void MiniTreeProducer::setGenParticleFromCandidate (reco::GenParticle & genp, const reco::Candidate * recoc)
1823	{
1824	  genp.setCharge (recoc->charge ());
1825	  genp.setP4 (recoc->p4 ());
1826	  genp.setMass (recoc->mass ());
1827	  genp.setPdgId (recoc->pdgId ());
1828	  genp.setStatus (recoc->status ());
1829	  genp.setVertex (recoc->vertex ());
1830	}
1831
1832	//define this as a plug-in
1833	DEFINE_FWK_MODULE (MiniTreeProducer);
