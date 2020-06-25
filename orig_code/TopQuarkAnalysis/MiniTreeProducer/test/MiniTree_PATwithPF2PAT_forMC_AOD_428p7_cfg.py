1	## import skeleton process
2	#from PhysicsTools.PatAlgos.patTemplate_cfg import *
3	
4	
5	import FWCore.ParameterSet.Config as cms
6	
7	process = cms.Process("PAT")
8	
9	
10	## MessageLogger
11	process.load("FWCore.MessageLogger.MessageLogger_cfi")
12	
13	
14	
15	
16	## Geometry and Detector Conditions (needed for a few patTuple production steps)
17	process.load("Configuration.StandardSequences.Geometry_cff")
18	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
19	
20	from Configuration.PyReleaseValidation.autoCond import autoCond
21	
22	# modifdg
23	#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
24	process.GlobalTag.globaltag = cms.string('START42_V17::All')
25	
26	process.GlobalTag.toGet = cms.VPSet(
27	  cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
28	       tag = cms.string("TrackProbabilityCalibration_2D_2011_v1_mc"),
29	       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU")),
30	  cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
31	       tag = cms.string("TrackProbabilityCalibration_3D_2011_v1_mc"),
32	       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU"))
33	)
34	
35	process.load("Configuration.StandardSequences.MagneticField_cff")
36	
37	## Standard PAT Configuration File
38	#process.load("PhysicsTools.PatAlgos.patSequences_cff")
39	
40	
41	## Output Module Configuration (expects a path 'p')
42	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
43	process.out = cms.OutputModule("PoolOutputModule",
44	                               fileName = cms.untracked.string('patTuple.root'),
45	                               # save only events passing the full path
46	                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
47	                               # save PAT Layer 1 output; you need a '*' to
48	                               # unpack the list of commands 'patEventContent'
49	                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
50	                               )
51	
52	process.outpath = cms.EndPath(process.out)
53	
54	
55	
56	
57	
58	
59	
60	# PAT Layer 0+1
61	process.load("PhysicsTools.PatAlgos.patSequences_cff")
62	process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
63	
64	#trigger matching & dynamic prescales stuff
65	from PhysicsTools.PatAlgos.tools.trigTools import *
66	switchOnTrigger( process )
67	
68	
69	
70	################################################################################################################################################
71	
72	
73	####################################
74	#  For jet matching at HLC level
75	#  Used by tau+jets analysis
76	####################################
77	process.jetMatchHLTJets = cms.EDProducer(
78	  # matching in DeltaR, sorting by best DeltaR
79	  "PATTriggerMatcherDRLessByR"
80	  # matcher input collections
81	, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
82	#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
83	, matched = cms.InputTag( 'patTrigger' )
84	  # selections of trigger objects
85	#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
86	, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
87	  # selection of matches
88	, maxDPtRel   = cms.double( 0.5 ) # no effect here
89	, maxDeltaR   = cms.double( 0.5 )
90	, maxDeltaEta = cms.double( 0.2 ) # no effect here
91	  # definition of matcher output
92	, resolveAmbiguities    = cms.bool( True )
93	, resolveByMatchQuality = cms.bool( True )
94	)
95	switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets' ] )
96	################################################################################################################################################
97	
98	####################################
99	#  For tau matching at HLC level
100	#  Used by tau+jets analysis
101	####################################
102	process.tauMatchHLTTaus = cms.EDProducer(
103	  # matching in DeltaR, sorting by best DeltaR
104	  "PATTriggerMatcherDRLessByR"
105	  # matcher input collections
106	#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
107	, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
108	, matched = cms.InputTag( 'patTrigger' )
109	  # selections of trigger objects
110	, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*",1 , 0 )')#&& filter( "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40" )')
111	#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
112	  # selection of matches
113	, maxDPtRel   = cms.double( 0.5 ) # no effect here
114	, maxDeltaR   = cms.double( 0.5 )
115	, maxDeltaEta = cms.double( 0.2 ) # no effect here
116	  # definition of matcher output
117	, resolveAmbiguities    = cms.bool( True )
118	, resolveByMatchQuality = cms.bool( True )
119	)
120	switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus' ] )
121	
122	process.jetMatchHLTJets45 = cms.EDProducer(
123	  # matching in DeltaR, sorting by best DeltaR
124	  "PATTriggerMatcherDRLessByR"
125	  # matcher input collections
126	, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
127	#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
128	, matched = cms.InputTag( 'patTrigger' )
129	  # selections of trigger objects
130	#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
131	, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet45_IsoPFTau45_v*", 0, 0) && filter( "hltQuadJet45IsoPFTau45" )') 
132	  # selection of matches
133	, maxDPtRel   = cms.double( 0.5 ) # no effect here
134	, maxDeltaR   = cms.double( 0.5 )
135	, maxDeltaEta = cms.double( 0.2 ) # no effect here
136	  # definition of matcher output
137	, resolveAmbiguities    = cms.bool( True )
138	, resolveByMatchQuality = cms.bool( True )
139	)
140	switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets45' ] )
141	
142	process.tauMatchHLTTaus45 = cms.EDProducer(
143	  # matching in DeltaR, sorting by best DeltaR
144	  "PATTriggerMatcherDRLessByR"
145	  # matcher input collections
146	#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
147	, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
148	, matched = cms.InputTag( 'patTrigger' )
149	  # selections of trigger objects
150	, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet45_IsoPFTau45_v*",1 , 0 )')#&& filter( "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40" )')
151	#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
152	  # selection of matches
153	, maxDPtRel   = cms.double( 0.5 ) # no effect here
154	, maxDeltaR   = cms.double( 0.5 )
155	, maxDeltaEta = cms.double( 0.2 ) # no effect here
156	  # definition of matcher output
157	, resolveAmbiguities    = cms.bool( True )
158	, resolveByMatchQuality = cms.bool( True )
159	)
160	switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus45' ] )
161	################################################################################################################################################
162	
163	
164	####################################
165	#  PV vertex selector
166	####################################
167	from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
168	
169	process.goodOfflinePrimaryVertices = cms.EDFilter(
170	    "PrimaryVertexObjectFilter",
171	    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
172	    src=cms.InputTag('offlinePrimaryVertices')
173	    )
174	    
175	    
176	process.noscraping = cms.EDFilter("FilterOutScraping",
177	                                  applyfilter = cms.untracked.bool(True),
178	                                  debugOn = cms.untracked.bool(True),
179	                                  numtrack = cms.untracked.uint32(10),
180	                                  thresh = cms.untracked.double(0.20)
181	)    
182	################################################################################################################################################
183	
184	####################################
185	#  HBHE noise filter
186	####################################
187	#Noise filter
188	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
189	
190	
191	####################################
192	#  For Summer11 MC, MG, Powheg
193	#  Gen filter for pythia bug
194	####################################
195	
196	process.load('GeneratorInterface/GenFilters/TotalKinematicsFilter_cfi')
197	
198	
199	
200	
201	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
202	
203	
204	####################################
205	#  True to run on MC
206	####################################
207	
208	runOnMC = True
209	
210	####################################
211	#  Number of events read
212	####################################
213	## Maximal Number of Events
214	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
215	####################################
216	#  Output Filename (PAT)
217	####################################
218	process.out.fileName = cms.untracked.string('patTuple.root')
219	
220	
221	
222	
223	####################################
224	#  Load the PAT config
225	####################################
226	process.load("PhysicsTools.PatAlgos.patSequences_cff")
227	
228	
229	####################################
230	#  Load the PAT config
231	####################################
232	# Configure PAT to use PF2PAT instead of AOD sources
233	# this function will modify the PAT sequences. It is currently 
234	# not possible to run PF2PAT+PAT and standart PAT at the same time
235	from PhysicsTools.PatAlgos.tools.pfTools import *
236	
237	
238	####################################
239	# An empty postfix means that only PF2PAT is run,
240	# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
241	# collections have standard names + postfix (e.g. patElectronPFlow)  
242	####################################
243	
244	
245	postfix = "PF2PAT"  # to have only PF2PAT
246	jetAlgo="AK5"
247	
248	usePFnoPU = True # before any top projection
249	
250	
251	####################################
252	# levels to be accessible from the jets
253	# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
254	# and remain uncorrected, if none of these levels is enabled here
255	useL1FastJet    = True  # needs useL1Offset being off, error otherwise
256	useL1Offset     = False # needs useL1FastJet being off, error otherwise
257	useL2Relative   = True
258	useL3Absolute   = True
259	# useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
260	useL5Flavor     = False
261	useL7Parton     = False
262	####################################
263	################################################################################################################################################
264	
265	# JEC set
266	jecSetBase = jetAlgo
267	jecSetPF = jecSetBase + 'PF'
268	if usePFnoPU:
269	  jecSetPF += 'chs'
270	
271	# JEC levels
272	if useL1FastJet and useL1Offset:
273	  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )
274	jecLevels = []
275	if useL1FastJet:
276	  jecLevels.append( 'L1FastJet' )
277	if useL1Offset:
278	  jecLevels.append( 'L1Offset' )
279	if useL2Relative:
280	  jecLevels.append( 'L2Relative' )
281	if useL3Absolute:
282	  jecLevels.append( 'L3Absolute' )
283	# if useL2L3Residual and not runOnMC:
284	#   jecLevelsPF.append( 'L2L3Residual' )
285	if useL5Flavor:
286	  jecLevels.append( 'L5Flavor' )
287	if useL7Parton:
288	  jecLevels.append( 'L7Parton' )
289	
290	################################################################################################################################################
291	
292	
293	
294	####################################
295	#  Enable PF2PAT
296	####################################
297	usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) ) 
298	####################################
299	# Please keep following line for taus !
300	adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
301	
302	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=True, postfix=postfix) 
303	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
304	
305	####################################
306	#  Setup JEC (useless ? already done by usePF2PAT ?
307	####################################
308	
309	applyPostfix(process,"patJetCorrFactors",postfix).levels     = jecLevels
310	applyPostfix(process,"patJetCorrFactors",postfix).rho        = cms.InputTag("kt6PFJets","rho")
311	applyPostfix(process,"patJetCorrFactors",postfix).payload    = jecSetPF
312	applyPostfix(process,"pfJets",postfix).doAreaFastjet    = True
313	applyPostfix(process,"pfJets",postfix).doRhoFastjet     = False
314	applyPostfix(process,"pfPileUp",postfix).checkClosestZVertex = cms.bool(False) 
315	applyPostfix(process,"pfPileUp",postfix).Vertices = 'goodOfflinePrimaryVertices'
316	
317	
318	
319	####################################
320	# to use tau-cleaned jet collection uncomment the following:
321	#  Desable for now
322	####################################
323	#getattr(process,"pfNoTau"+postfix).enable = True
324	####################################
325	
326	
327	
328	
329	
330	################################################################################################################################################
331	####################################
332	#  changes for iso and deltaR
333	####################################
334	#muons # spectial recipe for 428_p7
335	applyPostfix(process,"pfIsolatedMuons",postfix).isolationValueMapsCharged = cms.VInputTag( cms.InputTag( 'muPFIsoValueCharged03PF2PAT' ) )
336	applyPostfix(process,"pfIsolatedMuons",postfix).isolationValueMapsNeutral = cms.VInputTag( cms.InputTag( 'muPFIsoValueNeutral03PF2PAT' ), cms.InputTag( 'muPFIsoValueGamma03PF2PAT' ) )
337	applyPostfix(process,"pfIsolatedMuons",postfix).deltaBetaIsolationValueMap = cms.InputTag( 'muPFIsoValuePU03PF2PAT' )
338	applyPostfix(process,"patMuons",postfix).isolationValues.pfNeutralHadrons = cms.InputTag( 'muPFIsoValueNeutral03PF2PAT' )
339	applyPostfix(process,"patMuons",postfix).isolationValues.pfPhotons = cms.InputTag( 'muPFIsoValueGamma03PF2PAT' )
340	applyPostfix(process,"patMuons",postfix).isolationValues.pfChargedHadrons = cms.InputTag( 'muPFIsoValueCharged03PF2PAT' )
341	applyPostfix(process,"patMuons",postfix).isolationValues.pfPUChargedHadrons = cms.InputTag( 'muPFIsoValuePU03PF2PAT' ) 
342	applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
343	#electrons
344	applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
345	applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
346	applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
347	applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
348	
349	applyPostfix(process,"pfIsolatedMuons",postfix).isolationCut = cms.double(9999.)
350	applyPostfix(process,"pfIsolatedElectrons",postfix).isolationCut = cms.double(9999.)
351	
352	################################################################################################################################################
353	
354	
355	
356	
357	
358	####################################
359	#  Add tcMET. Not used anymore...
360	from PhysicsTools.PatAlgos.tools.metTools import *
361	
362	addTcMET(process, 'TC')
363	process.patMETsTC.addGenMET = False
364	####################################
365	
366	
367	####################################
368	#  Remove some MC dependencies
369	####################################
370	removeMCMatching(process, ['All'])
371	removeIfInSequence(process, 'patJetGenJetMatchAK5JPT', "patDefaultSequence")
372	removeIfInSequence(process, 'patJetPartonMatchAK5JPT', "patDefaultSequence")
373	removeIfInSequence(process, 'patJetPartons',           "patDefaultSequence")
374	removeIfInSequence(process, 'patJetPartonAssociation', "patDefaultSequence")
375	removeIfInSequence(process, 'patJetPartonAssociationAK5JPT',  "patDefaultSequence")
376	removeIfInSequence(process, 'patJetFlavourAssociation',       "patDefaultSequence")
377	removeIfInSequence(process, 'patJetFlavourAssociationAK5JPT', "patDefaultSequence")
378	################################################################################################################################################
379	
380	
381	
382	####################################
383	#  Produce kt6PFJets
384	#  For jet area and Rho subtraction
385	####################################
386	process.load('RecoJets.Configuration.RecoJets_cff')
387	from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
388	
389	process.kt6PFJets               = kt4PFJets.clone()
390	process.kt6PFJets.rParam        = 0.6     
391	process.kt6PFJets.src           = cms.InputTag('pfNoElectron'+postfix)
392	process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
393	process.kt6PFJets.doRhoFastjet  = True
394	process.kt6PFJets.doAreaFastjet = True
395	#process.kt6PFJets.voronoiRfact  = 0.9
396	
397	#process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJets", "rho")
398	getattr(process,"patJetCorrFactors"+postfix).rho = cms.InputTag("kt6PFJets", "rho")
399	
400	process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")
401	
402	process.kt6NeutralPFJets = kt4PFJets.clone()
403	process.kt6NeutralPFJets.rParam        = 0.6     
404	process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
405	process.kt6NeutralPFJets.doRhoFastjet  = True
406	process.kt6NeutralPFJets.doAreaFastjet = True
407	process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
408	#process.kt6PFJets.voronoiRfact  = 0.9
409	################################################################################################################################################
410	
411	
412	####################################
413	#  Selecion jet according to 
414	#  The sandard jetID PURE09
415	####################################
416	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
417	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
418	process.jetIDSelector.version = cms.string('PURE09')
419	
420	
421	
422	####################################
423	#  Produce various eID
424	####################################
425	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
426	#process.load("RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi")
427	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
428	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi")
429	
430	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
431	
432	process.patElectrons.addElectronID = cms.bool(True)
433	process.patElectrons.electronIDSources = cms.PSet(
434	    eidLoose= cms.InputTag("eidLoose"),
435	    eidTight= cms.InputTag("eidTight"),
436	    eidRobustTight= cms.InputTag("eidRobustTight"),
437	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
438	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
439	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
440	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
441	    cicLoose         =  cms.InputTag("cicLoose"),
442	    cicMedium        =  cms.InputTag("cicMedium"),
443	    cicTight         =  cms.InputTag("cicTight"),
444	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
445	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
446	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
447	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
448	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
449	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
450	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
451	    cicTightMC         =  cms.InputTag("cicTightMC"),
452	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
453	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
454	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
455	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
456	)
457	
458	process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)
459	
460	process.patElectronsPF2PAT.addElectronID = cms.bool(True)
461	process.patElectronsPF2PAT.electronIDSources = cms.PSet(
462	    eidLoose= cms.InputTag("eidLoose"),
463	    eidTight= cms.InputTag("eidTight"),
464	    eidRobustTight= cms.InputTag("eidRobustTight"),
465	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
466	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
467	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
468	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
469	    cicLoose         =  cms.InputTag("cicLoose"),
470	    cicMedium        =  cms.InputTag("cicMedium"),
471	    cicTight         =  cms.InputTag("cicTight"),
472	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
473	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
474	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
475	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
476	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
477	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
478	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
479	    cicTightMC         =  cms.InputTag("cicTightMC"),
480	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
481	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
482	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
483	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
484	)
485	
486	################################################################################################################################################
487	
488	
489	
490	####################################
491	#  Produce PDF weights
492	#  To speed up the processing
493	#  Interactivaly, before staring CMSSW
494	#  do "scramv1 setup lhapdffull; scramv1 b;"
495	#  with crab, ass these two lines after 
496	#  eval `scramv1 runtime -sh in crab_.../job/CMSSW.sh
497	####################################
498	process.pdfWeights = cms.EDProducer("PdfWeightProducer",
499	            # Fix POWHEG if buggy (this PDF set will also appear on output,
500	            # so only two more PDF sets can be added in PdfSetNames if not "")
501	            #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
502	            #GenTag = cms.untracked.InputTag("genParticles"),
503	            PdfInfoTag = cms.untracked.InputTag("generator"),
504	            PdfSetNames = cms.untracked.vstring(
505	                    "cteq66.LHgrid",
506	                    #"MRST2006nnlo.LHgrid"
507	                    "NNPDF10_100.LHgrid"
508	            )
509	)
510	 
511	
512	
513	################################################################################################################################################
514	####################################
515	#  Extra MET collections
516	####################################
517	from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
518	process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
519	
520	
521	#process.ak5PFL1Fastjet.algorithm  =  cms.string('AK5PFchs')
522	#process.ak5PFL1FastL2L3.algorithm =  cms.string('AK5PFchs')
523	
524	process.ak5PFL1FastjetPFNoPU = ak5PFL1Fastjet.clone()
525	process.ak5PFL1FastjetPFNoPU.algorithm  =  cms.string('AK5PFchs')
526	
527	process.ak5PFL1FastL2L3PFNoPU = ak5PFL1FastL2L3.clone()
528	process.ak5PFL1FastL2L3PFNoPU.algorithm  =  cms.string('AK5PFchs')
529	
530	process.pfCandsNotInJet.topCollection              = cms.InputTag('pfJetsPF2PAT')
531	process.pfCandsNotInJet.bottomCollection           = cms.InputTag('pfNoElectronPF2PAT')
532	
533	
534	process.pfJetMETcorr.src            = cms.InputTag('pfJetsPF2PAT')
535	process.pfJetMETcorr.offsetCorrLabel = cms.string("ak5PFL1FastjetPFNoPU")
536	process.pfJetMETcorr.jetCorrLabel    = cms.string("ak5PFL1FastL2L3PFNoPU")
537	#process.pfCandMETcorr.src = cms.InputTag('pfCandsNotInJetPF2PAT') 
538	
539	process.pfType1CorrectedMet.src   = cms.InputTag('pfMETPF2PAT')
540	process.pfType1p2CorrectedMet.src = cms.InputTag('pfMETPF2PAT')
541	
542	process.patMETsTypeIPF = process.patMETsPF2PAT.clone(
543	        metSource = cms.InputTag("pfType1CorrectedMet"),
544	        addMuonCorrections = cms.bool(False)
545	    )
546	
547	
548	process.patMETsTypeIIPF = process.patMETsPF2PAT.clone(
549	        metSource = cms.InputTag("pfType1p2CorrectedMet"),
550	        addMuonCorrections = cms.bool(False)
551	    )
552	
553	
554	
555	################################################################################################################################################
556	#pfMET TypeI
557	
558	
559	####################################
560	#  Our Tree Producers
561	####################################
562	
563	
564	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
565	        isAOD               = cms.bool(True), # true if processing AOD data
566	        isData              = cms.bool(False), # true if processing AOD data
567	        readPDFWeights      = cms.bool(True),
568	        verbose         = cms.int32(0), #0: nothing - >1 gradually more information
569	        TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::REDIGI311X 
570	        TriggerInfo         = cms.bool(True), 
571	        saveAllTriggers     = cms.bool(True),#should be True by default !!
572	        triggerList     = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
573	        prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5","HLT_QuadJet40_IsoPFTau40_v1","HLT_QuadJet40_IsoPFTau40_v2","HLT_QuadJet40_IsoPFTau40_v3",
574	                "HLT_QuadJet40_IsoPFTau40_v4","HLT_QuadJet40_IsoPFTau40_v5","HLT_QuadJet40_IsoPFTau40_v6",
575	                "HLT_QuadJet40_IsoPFTau40_v7","HLT_QuadJet40_IsoPFTau40_v8","HLT_QuadJet40_IsoPFTau40_v9",
576	                "HLT_QuadJet40_IsoPFTau40_v10","HLT_QuadJet40_IsoPFTau40_v11","HLT_QuadJet40_IsoPFTau40_v12",
577	                "HLT_QuadJet40_IsoPFTau40_v13","HLT_QuadJet40_IsoPFTau40_v14","HLT_QuadJet40_IsoPFTau40_v15"),
578	        hltJetsMatcher      = cms.string("jetMatchHLTJets"),
579	        hltTausMatcher      = cms.string("tauMatchHLTTaus"),
580	        hltJetsMatcher45    = cms.string("jetMatchHLTJets45"),
581	        hltTausMatcher45    = cms.string("tauMatchHLTTaus45"),  
582	        doGenParticleCollection = cms.bool(True),
583	        doElectrons     = cms.bool(True),
584	        electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
585	        electron_cut_pt     = cms.double(10),
586	        electron_cut_eta    = cms.double(2.5), 
587	        doPhotons       = cms.bool(False),
588	        photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
589	        photon_cut_pt       = cms.double(10),
590	        photon_cut_eta      = cms.double(2.5), 
591	        doMuons         = cms.bool(True),
592	        muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
593	        muon_cut_pt         = cms.double(10),
594	        muon_cut_eta        = cms.double(2.5), 
595	        doTaus          = cms.bool(True),
596	        tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
597	        tau_cut_pt      = cms.double(10),
598	        tau_cut_eta     = cms.double(2.4),      
599	        trackProducer       = cms.InputTag("generalTracks"),
600	        doTracks            = cms.bool(False),
601	        track_cut_pt        = cms.double(0.5),
602	        track_cut_eta       = cms.double(2.4),
603	        beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
604	        jet_cut_pt      = cms.double(10),
605	        jet_cut_eta     = cms.double(2.5), 
606	        met_cut_pt          = cms.double(-99),      
607	        saveAllVertex       = cms.bool(True),
608	        vertexProducer      = cms.InputTag("goodOfflinePrimaryVertices"),
609	        rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
610	        #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
611	            neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
612	            jetmetProducer      = cms.VPSet(
613	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"),   algo =cms.untracked.string("pf")),
614	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsTypeIPF"),  algo =cms.untracked.string("pfType1")),
615	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsTypeIIPF"), algo =cms.untracked.string("pfType12"))
616	
617	                                       )
618	    )
619	
620	
621	process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )
622	
623	
624	# loads your analyzer
625	process.MyModule = cms.EDAnalyzer('NTupleProducer',
626	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
627	                saveAllTriggers     = cms.bool(True),# False: see list above
628	                saveTriggersWith    = cms.vstring(""),
629	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
630	                doElectrons         = cms.bool(True),
631	                electron_cut_pt     = cms.double(10),
632	                electron_cut_eta    = cms.double(2.5),
633	                doPhotons           = cms.bool(True),
634	                photon_cut_pt       = cms.double(10),
635	                photon_cut_eta      = cms.double(2.5),
636	                doMuons             = cms.bool(True),
637	                muon_cut_pt         = cms.double(10),
638	                muon_cut_eta        = cms.double(2.5),
639	                doTaus              = cms.bool(True),
640	                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
641	                tau_cut_pt          = cms.double(5),
642	                tau_cut_eta         = cms.double(3.),
643	                doTracks            = cms.bool(False),
644	                track_cut_pt        = cms.double(0.5),
645	                track_cut_eta       = cms.double(2.4),
646	                doJets              = cms.bool(True),
647	                jetAlgos            = cms.vstring("ak5","pf", "tc"),
648	                jet_cut_pt          = cms.double(10),
649	                jet_cut_eta         = cms.double(2.5),
650	                saveAllVertex       = cms.bool(True),
651	                saveMCInfo          = cms.bool(True),
652	                
653	                # Configuration for skimming !
654	                #
655	                # Events will be recorded if one of the following parameters is activated :
656	                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True 
657	                # 
658	                numberOfLept4Skim   = cms.int32(-1),# for skims ! Total number of sel lepton,  -1 for no skiming
659	                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
660	                numberOfElec4Skim   = cms.int32(0),# number of sel electron
661	                useMuonIdSkim         = cms.bool(False),
662	                muon_cut_pt_skim      = cms.double(10),
663	                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
664	                useElectronIdSkim     = cms.bool(False),
665	                electron_cut_pt_skim  = cms.double(10),
666	                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
667	                
668	                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
669	                #TMEMESkimList        = cms.vint32(10000),
670	                #TMEMESkimList        = cms.vint32(2,10101,20200,20,11010,22000,11,11001,10110,21100),
671	                #TMEMESkimList        = cms.vint32(10010,21000,10001,20100),
672	                #TMEMESkimList        = cms.vint32(0),
673	                #TMEMESkimList        = cms.vint32(20000),
674	                #TMEMESkimList        = cms.vint32(1,10,10100,11000),
675	                TMEMESkimList         = cms.vint32(),
676	                
677	                doMCDiLepSkimming     = cms.bool(False),
678	                MCDiLepList           = cms.vstring(""),
679	                
680	                doTauSkimming         = cms.bool(True), # skim on the number of reco taus (no id so far)
681	                numberOfTau4Skim      = cms.int32(1),
682	                tau_cut_pt_skim       = cms.double(40),
683	                tau_algo_skim         = cms.string("selectedPatTausPF2PAT"),
684	                
685	                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
686	                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),
687	                
688	                doJetSkimming         = cms.bool(True), # skim on the number of jets
689	                numberOfJet4Skim      = cms.int32(3),
690	                jet_cut_pt_skim       = cms.double(40), 
691	                jet_cut_eta_skim      = cms.double(2.5),
692	                jet_algo_skim         = cms.string("pf"),
693	                
694	                # ttbar-> tau+jets specific
695	                doTauJetAna_TauJetTrigSkimming  = cms.bool(False), # need to set doTrigger doTauSkimming doJetSkimming to true
696	                doTauJetAna_TauJetSkimming      = cms.bool(True),  # need to set doTauSkimming doJetSkimming to true
697	                doTauJetAna_TmemeTauJetSkimming = cms.bool(False) # need to set doTMEMESkimming doTauSkimming doJetSkimming to true, only for MC ttbar
698	        
699	                
700	        )
701	
702	
703	################################################################################################################################################
704	
705	####################################
706	#  top projections in PF2PAT:
707	####################################
708	getattr(process,"pfNoPileUp"  +postfix).enable = True
709	getattr(process,"pfNoMuon"    +postfix).enable = False 
710	getattr(process,"pfNoElectron"+postfix).enable = False
711	getattr(process,"pfNoTau"     +postfix).enable = False
712	getattr(process,"pfNoJet"     +postfix).enable = False
713	
714	####################################
715	#  verbose flags for the PF2PAT modules
716	getattr(process,"pfNoMuon"+postfix).verbose = False
717	#  top projections in PF2PAT:
718	####################################
719	
720	
721	
722	if runOnMC == False:
723	    removeMCMatchingPF2PAT( process, '')    
724	
725	
726	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
727	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
728	
729	####################################
730	#  Configure PF2PAT
731	####################################
732	getattr(process,"patPF2PATSequence"+postfix).replace(
733	    getattr(process,"pfNoElectron"+postfix),
734	    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJets )
735	
736	####################################
737	#  Define the Path
738	####################################
739	process.p = cms.Path( process.totalKinematicsFilter*
740	                      process.pdfWeights*  ## to be uncomment for ttbar
741	                      process.goodOfflinePrimaryVertices*
742	                      process.HBHENoiseFilter*
743	                      process.noscraping + 
744	                      process.patElectronIDs *                      
745	                      getattr(process,"patPF2PATSequence"+postfix)*
746	                      process.producePFMETCorrections*
747	                      process.patMETsTypeIPF*
748	                      process.patMETsTypeIIPF*
749	                      process.neutralPFCandidatesProducer*
750	                      process.kt6NeutralPFJets
751	              
752	#    second PF2PAT
753	#    + getattr(process,"patPF2PATSequence"+postfix2)
754	)
755	if not postfix=="":
756	    process.p += process.patDefaultSequence
757	    
758	  
759	#################################### 
760	# Add NTuple producers to the sequence 
761	####################################
762	process.p += process.MiniTreeProduction
763	   
764	process.p += process.MyModule   
765	 
766	####################################   
767	#  Output content
768	#################################### 
769	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
770	process.out.outputCommands = cms.untracked.vstring('drop *',
771	                              # 'keep *_*_*_PAT',
772	                           'keep edmTriggerResults_*_*_*',
773	                           #'keep double_kt6PFJets_rho_*',
774	                           #'keep *_hltTriggerSummaryAOD_*_*',
775	                           #'keep *_offlineBeamSpot_*_*',
776	                           #'keep *_offlinePrimaryVertices_*_PAT',
777	                           #'keep *_muons_*_*',
778	                           #'keep *_muonTCMETValueMapProducer_*_*',
779	                           #'keep recoTracks_*_*_*',
780	                           #'keep GenEventInfoProduct_*_*_*',
781	                           #'keep recoGenParticles_*_*_*',
782	                                                   #*patEventContentNoCleaning 
783	                           'keep TopTreeMTEvent_*_*_*'
784	                           ) 
785	
786	
787	
788	
789	process.maxEvents.input = -1            ##  (e.g. -1 to run on all events)
790	
791	
792	
793	process.MessageLogger.cerr.FwkReport.reportEvery = 10
794	
795	
796	process.source = cms.Source(
797	    "PoolSource",
798	    noEventSort = cms.untracked.bool(True),
799	    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
800	    fileNames = cms.untracked.vstring(
801	      '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FEEE3638-F297-E011-AAF8-00304867BEC0.root',
802	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FCFD87D7-9E98-E011-BDA2-0018F3D09642.root',
803	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FCFB9DE1-8598-E011-BE64-003048679076.root',
804	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FAD1CEE7-7A98-E011-89A0-001A92971B7E.root',
805	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FAA9FD72-E497-E011-A542-001A92971BC8.root',
806	       
807	    )
808	)
