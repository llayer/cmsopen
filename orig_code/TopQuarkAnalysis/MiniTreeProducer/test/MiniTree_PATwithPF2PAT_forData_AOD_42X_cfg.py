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
23	#startup == START311_V2::All for CMSSW_4_1_4 (seems obsolete, to be confirmed!)
24	#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
25	#process.GlobalTag.globaltag = cms.string('START41_V0::All')
26	process.GlobalTag.globaltag = cms.string('GR_R_42_V20::All')
27	
28	
29	process.GlobalTag.toGet = cms.VPSet(
30	  cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
31	#        tag = cms.string("TrackProbabilityCalibration_2D_2010Data_v1_offline"),
32	        tag = cms.string("TrackProbabilityCalibration_2D_2011Data_v1_offline"),
33	#       tag = cms.string("TrackProbabilityCalibration_2D_2011_v1_mc"),
34	       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU")),
35	  cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
36	#        tag = cms.string("TrackProbabilityCalibration_3D_2010Data_v1_offline"),
37	        tag = cms.string("TrackProbabilityCalibration_3D_2011Data_v1_offline"),
38	#       tag = cms.string("TrackProbabilityCalibration_3D_2011_v1_mc"),
39	       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU"))
40	)
41	
42	
43	
44	process.load("Configuration.StandardSequences.MagneticField_cff")
45	
46	## Standard PAT Configuration File
47	#process.load("PhysicsTools.PatAlgos.patSequences_cff")
48	
49	
50	## Output Module Configuration (expects a path 'p')
51	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
52	process.out = cms.OutputModule("PoolOutputModule",
53	                               fileName = cms.untracked.string('patTuple.root'),
54	                               # save only events passing the full path
55	                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
56	                               # save PAT Layer 1 output; you need a '*' to
57	                               # unpack the list of commands 'patEventContent'
58	                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
59	                               )
60	
61	process.outpath = cms.EndPath(process.out)
62	
63	
64	
65	
66	
67	
68	
69	# PAT Layer 0+1
70	process.load("PhysicsTools.PatAlgos.patSequences_cff")
71	
72	process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
73	
74	#trigger matching & dynamic prescales stuff
75	from PhysicsTools.PatAlgos.tools.trigTools import *
76	switchOnTrigger( process )
77	
78	process.jetMatchHLTJets = cms.EDProducer(
79	  # matching in DeltaR, sorting by best DeltaR
80	  "PATTriggerMatcherDRLessByR"
81	  # matcher input collections
82	, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
83	#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
84	, matched = cms.InputTag( 'patTrigger' )
85	  # selections of trigger objects
86	#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
87	, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
88	  # selection of matches
89	, maxDPtRel   = cms.double( 0.5 ) # no effect here
90	, maxDeltaR   = cms.double( 0.5 )
91	, maxDeltaEta = cms.double( 0.2 ) # no effect here
92	  # definition of matcher output
93	, resolveAmbiguities    = cms.bool( True )
94	, resolveByMatchQuality = cms.bool( True )
95	)
96	switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets' ] )
97	
98	process.tauMatchHLTTaus = cms.EDProducer(
99	  # matching in DeltaR, sorting by best DeltaR
100	  "PATTriggerMatcherDRLessByR"
101	  # matcher input collections
102	#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
103	, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
104	, matched = cms.InputTag( 'patTrigger' )
105	  # selections of trigger objects
106	, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" , 1, 0)' )
107	#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
108	  # selection of matches
109	, maxDPtRel   = cms.double( 0.5 ) # no effect here
110	, maxDeltaR   = cms.double( 0.5 )
111	, maxDeltaEta = cms.double( 0.2 ) # no effect here
112	  # definition of matcher output
113	, resolveAmbiguities    = cms.bool( True )
114	, resolveByMatchQuality = cms.bool( True )
115	)
116	switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus' ] )
117	
118	process.jetMatchHLTJets45 = cms.EDProducer(
119	  # matching in DeltaR, sorting by best DeltaR
120	  "PATTriggerMatcherDRLessByR"
121	  # matcher input collections
122	, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
123	#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
124	, matched = cms.InputTag( 'patTrigger' )
125	  # selections of trigger objects
126	#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
127	, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet45_IsoPFTau45_v*", 0, 0) && filter( "hltQuadJet45IsoPFTau45" )') 
128	  # selection of matches
129	, maxDPtRel   = cms.double( 0.5 ) # no effect here
130	, maxDeltaR   = cms.double( 0.5 )
131	, maxDeltaEta = cms.double( 0.2 ) # no effect here
132	  # definition of matcher output
133	, resolveAmbiguities    = cms.bool( True )
134	, resolveByMatchQuality = cms.bool( True )
135	)
136	switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets45' ] )
137	
138	process.tauMatchHLTTaus45 = cms.EDProducer(
139	  # matching in DeltaR, sorting by best DeltaR
140	  "PATTriggerMatcherDRLessByR"
141	  # matcher input collections
142	#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
143	, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
144	, matched = cms.InputTag( 'patTrigger' )
145	  # selections of trigger objects
146	, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet45_IsoPFTau45_v*" , 1, 0)' )
147	#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
148	  # selection of matches
149	, maxDPtRel   = cms.double( 0.5 ) # no effect here
150	, maxDeltaR   = cms.double( 0.5 )
151	, maxDeltaEta = cms.double( 0.2 ) # no effect here
152	  # definition of matcher output
153	, resolveAmbiguities    = cms.bool( True )
154	, resolveByMatchQuality = cms.bool( True )
155	)
156	switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus45' ] )
157	
158	
159	from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
160	
161	process.goodOfflinePrimaryVertices = cms.EDFilter(
162	    "PrimaryVertexObjectFilter",
163	    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
164	    src=cms.InputTag('offlinePrimaryVertices')
165	    )
166	    
167	    
168	process.noscraping = cms.EDFilter("FilterOutScraping",
169	                                  applyfilter = cms.untracked.bool(True),
170	                                  debugOn = cms.untracked.bool(True),
171	                                  numtrack = cms.untracked.uint32(10),
172	                                  thresh = cms.untracked.double(0.20)
173	)    
174	
175	#Noise filter
176	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
177	
178	
179	
180	
181	
182	
183	
184	
185	
186	
187	# the source is already defined in patTemplate_cfg.
188	# overriding source and various other things
189	#process.load("CommonTools.ParticleFlow.Sources.source_ZtoEles_DBS_312_cfi")
190	#process.source = cms.Source("PoolSource", 
191	#     fileNames = cms.untracked.vstring('file:myAOD.root')
192	#)
193	
194	
195	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
196	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
197	
198	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
199	runOnMC = False
200	
201	## Maximal Number of Events
202	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
203	process.out.fileName = cms.untracked.string('patTuple.root')
204	
205	# load the PAT config
206	process.load("PhysicsTools.PatAlgos.patSequences_cff")
207	
208	
209	# Configure PAT to use PF2PAT instead of AOD sources
210	# this function will modify the PAT sequences. It is currently 
211	# not possible to run PF2PAT+PAT and standart PAT at the same time
212	from PhysicsTools.PatAlgos.tools.pfTools import *
213	
214	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
215	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
216	#process.isoValMuonWithCharged.deltaR = 0.3
217	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
218	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
219	 
220	 
221	# An empty postfix means that only PF2PAT is run,
222	# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
223	# collections have standard names + postfix (e.g. patElectronPFlow)  
224	
225	
226	postfix = "PF2PAT"  # to have only PF2PAT
227	jetAlgo="AK5"
228	
229	usePFnoPU = True # before any top projection
230	
231	
232	# levels to be accessible from the jets
233	# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
234	# and remain uncorrected, if none of these levels is enabled here
235	useL1FastJet    = True  # needs useL1Offset being off, error otherwise
236	useL1Offset     = False # needs useL1FastJet being off, error otherwise
237	useL2Relative   = True
238	useL3Absolute   = True
239	useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
240	useL5Flavor     = False
241	useL7Parton     = False
242	
243	# JEC set
244	jecSetBase = jetAlgo
245	jecSetPF = jecSetBase + 'PF'
246	if usePFnoPU:
247	  jecSetPF += 'chs'
248	
249	# JEC levels
250	if useL1FastJet and useL1Offset:
251	  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )
252	jecLevels = []
253	if useL1FastJet:
254	  jecLevels.append( 'L1FastJet' )
255	if useL1Offset:
256	  jecLevels.append( 'L1Offset' )
257	if useL2Relative:
258	  jecLevels.append( 'L2Relative' )
259	if useL3Absolute:
260	  jecLevels.append( 'L3Absolute' )
261	if useL2L3Residual and not runOnMC:
262	  jecLevels.append( 'L2L3Residual' )
263	if useL5Flavor:
264	  jecLevels.append( 'L5Flavor' )
265	if useL7Parton:
266	  jecLevels.append( 'L7Parton' )
267	
268	
269	
270	
271	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) ) 
272	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
273	
274	usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix) 
275	adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
276	
277	applyPostfix(process,"patJetCorrFactors",postfix).levels = cms.vstring('L1FastJet','L2Relative','L3Absolute', 'L2L3Residual' )
278	applyPostfix(process,"patJetCorrFactors",postfix).rho = cms.InputTag("kt6PFJets","rho")
279	applyPostfix(process,"patJetCorrFactors",postfix).payload = cms.string('AK5PFchs')
280	applyPostfix(process,"pfPileUp",postfix).checkClosestZVertex = cms.bool(False) 
281	
282	
283	# to use tau-cleaned jet collection uncomment the following:
284	#getattr(process,"pfNoTau"+postfix).enable = True
285	
286	#changes for iso and deltaR
287	#muons
288	applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
289	applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
290	applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
291	applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
292	#electrons
293	applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
294	applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
295	applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
296	applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
297	# jec should be ok
298	# by default: 'L1Offset','L2Relative','L3Absolute'
299	#  and primaryVertices = cms.InputTag("offlinePrimaryVertices")
300	
301	#addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
302	#                 'AK5', 'JPT',
303	#                 doJTA        = True,
304	#                 doBTagging   = True,
305	#modifdg (L1Offset to substract PU effect)       
306	#                 jetCorrLabel = ('AK5JPT', cms.vstring([ 'L2Relative', 'L3Absolute'])),
307	#                 jetCorrLabel = ('AK5JPT', cms.vstring(['L1Offset', 'L2Relative', 'L3Absolute'])),
308	#                 doType1MET   = False,
309	#                 doL1Cleaning = False,
310	#                 doL1Counters = True,                 
311	#                 genJetCollection = cms.InputTag("ak5GenJets"),
312	#                 doJetID      = True
313	#                 )
314	     
315	from PhysicsTools.PatAlgos.tools.metTools import *
316	
317	addTcMET(process, 'TC')
318	process.patMETsTC.addGenMET = False
319	
320	process.pfPileUpPF2PAT.Enable = True
321	#process.pfPileUpPF2PAT.checkClosestZVertex = cms.bool(False)
322	process.pfPileUpPF2PAT.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
323	process.pfJetsPF2PAT.doAreaFastjet = True
324	process.pfJetsPF2PAT.doRhoFastjet = False
325	
326	removeMCMatching(process, ['All'])
327	removeIfInSequence(process, 'patJetGenJetMatchAK5JPT', "patDefaultSequence")
328	removeIfInSequence(process, 'patJetPartonMatchAK5JPT', "patDefaultSequence")
329	removeIfInSequence(process, 'patJetPartons',           "patDefaultSequence")
330	removeIfInSequence(process, 'patJetPartonAssociation', "patDefaultSequence")
331	removeIfInSequence(process, 'patJetPartonAssociationAK5JPT',  "patDefaultSequence")
332	removeIfInSequence(process, 'patJetFlavourAssociation',       "patDefaultSequence")
333	removeIfInSequence(process, 'patJetFlavourAssociationAK5JPT', "patDefaultSequence")
334	
335	process.load('RecoJets.Configuration.RecoJets_cff')
336	from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
337	
338	process.kt6PFJets               = kt4PFJets.clone()
339	process.kt6PFJets.rParam        = 0.6     
340	process.kt6PFJets.src           = cms.InputTag('pfNoElectron'+postfix)
341	process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
342	process.kt6PFJets.doRhoFastjet  = True
343	process.kt6PFJets.doAreaFastjet = True
344	#process.kt6PFJets.voronoiRfact  = 0.9
345	
346	#process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJets", "rho")
347	getattr(process,"patJetCorrFactors"+postfix).rho = cms.InputTag("kt6PFJets", "rho")
348	
349	process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")
350	
351	process.kt6NeutralPFJets = kt4PFJets.clone()
352	process.kt6NeutralPFJets.rParam        = 0.6     
353	process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
354	process.kt6NeutralPFJets.doRhoFastjet  = True
355	process.kt6NeutralPFJets.doAreaFastjet = True
356	process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
357	#process.kt6PFJets.voronoiRfact  = 0.9
358	
359	
360	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
361	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
362	process.jetIDSelector.version = cms.string('PURE09')
363	
364	
365	
366	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
367	#process.load("RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi")
368	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
369	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi")
370	
371	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
372	
373	process.patElectrons.addElectronID = cms.bool(True)
374	process.patElectrons.electronIDSources = cms.PSet(
375	    eidLoose= cms.InputTag("eidLoose"),
376	    eidTight= cms.InputTag("eidTight"),
377	    eidRobustTight= cms.InputTag("eidRobustTight"),
378	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
379	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
380	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
381	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
382	    cicLoose         =  cms.InputTag("cicLoose"),
383	    cicMedium        =  cms.InputTag("cicMedium"),
384	    cicTight         =  cms.InputTag("cicTight"),
385	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
386	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
387	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
388	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
389	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
390	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
391	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
392	    cicTightMC         =  cms.InputTag("cicTightMC"),
393	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
394	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
395	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
396	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
397	)
398	
399	process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)
400	
401	process.patElectronsPF2PAT.addElectronID = cms.bool(True)
402	process.patElectronsPF2PAT.electronIDSources = cms.PSet(
403	    eidLoose= cms.InputTag("eidLoose"),
404	    eidTight= cms.InputTag("eidTight"),
405	    eidRobustTight= cms.InputTag("eidRobustTight"),
406	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
407	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
408	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
409	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
410	    cicLoose         =  cms.InputTag("cicLoose"),
411	    cicMedium        =  cms.InputTag("cicMedium"),
412	    cicTight         =  cms.InputTag("cicTight"),
413	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
414	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
415	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
416	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
417	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
418	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
419	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
420	    cicTightMC         =  cms.InputTag("cicTightMC"),
421	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
422	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
423	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
424	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
425	)
426	
427	
428	
429	
430	
431	# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
432	# and add it to path
433	#postfix2 = "PFlow2"
434	#jetAlgo2="AK7"
435	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)
436	
437	# to use tau-cleaned jet collection uncomment the following:
438	#getattr(process,"pfNoTau"+postfix).enable = True 
439	
440	
441	
442	
443	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
444	        isAOD               = cms.bool(True), # true if processing AOD data
445	        isData              = cms.bool(True), # true if processing AOD data
446	        readPDFWeights      = cms.bool(False),
447	        verbose         = cms.int32(0), #0: nothing - >1 gradually more information
448	            TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::REDIGI311X 
449	        TriggerInfo         = cms.bool(True), 
450	                saveAllTriggers     = cms.bool(True),#should be True by default !!
451	        triggerList     = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
452	        prescaleList        = cms.vstring(
453	                "HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5",
454	                "HLT_QuadJet40_IsoPFTau40_v1","HLT_QuadJet40_IsoPFTau40_v2","HLT_QuadJet40_IsoPFTau40_v3",
455	                "HLT_QuadJet40_IsoPFTau40_v4","HLT_QuadJet40_IsoPFTau40_v5","HLT_QuadJet40_IsoPFTau40_v6",
456	                "HLT_QuadJet40_IsoPFTau40_v7","HLT_QuadJet40_IsoPFTau40_v8","HLT_QuadJet40_IsoPFTau40_v9",
457	                "HLT_QuadJet40_IsoPFTau40_v10","HLT_QuadJet40_IsoPFTau40_v11","HLT_QuadJet40_IsoPFTau40_v12",
458	                
459	                "HLT_QuadJet45_IsoPFTau45_v1","HLT_QuadJet45_IsoPFTau45_v2","HLT_QuadJet45_IsoPFTau45_v3",
460	                "HLT_QuadJet45_IsoPFTau45_v4","HLT_QuadJet45_IsoPFTau45_v5","HLT_QuadJet45_IsoPFTau45_v6",
461	                "HLT_QuadJet45_IsoPFTau45_v7","HLT_QuadJet45_IsoPFTau45_v8","HLT_QuadJet45_IsoPFTau45_v9",
462	                "HLT_QuadJet45_IsoPFTau45_v10","HLT_QuadJet45_IsoPFTau45_v11","HLT_QuadJet45_IsoPFTau45_v12",
463	                "HLT_QuadJet45_IsoPFTau45_v13","HLT_QuadJet45_IsoPFTau45_v14"),         
464	                
465	        hltJetsMatcher      = cms.string("jetMatchHLTJets"),
466	        hltTausMatcher      = cms.string("tauMatchHLTTaus"),
467	        hltJetsMatcher45    = cms.string("jetMatchHLTJets45"),
468	        hltTausMatcher45    = cms.string("tauMatchHLTTaus45"),        
469	        doGenParticleCollection = cms.bool(False),
470	        doElectrons     = cms.bool(True),
471	        electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
472	        electron_cut_pt     = cms.double(10),
473	        electron_cut_eta    = cms.double(2.5), 
474	        doPhotons       = cms.bool(False),
475	        photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
476	        photon_cut_pt       = cms.double(10),
477	        photon_cut_eta      = cms.double(2.5), 
478	        doMuons         = cms.bool(True),
479	        muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
480	        muon_cut_pt         = cms.double(10),
481	        muon_cut_eta        = cms.double(2.5), 
482	        doTaus          = cms.bool(True),
483	        tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
484	        tau_cut_pt      = cms.double(10),
485	        tau_cut_eta     = cms.double(2.4),      
486	        trackProducer       = cms.InputTag("generalTracks"),
487	        doTracks            = cms.bool(False),
488	        track_cut_pt        = cms.double(0.5),
489	        track_cut_eta       = cms.double(2.4),
490	        beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
491	        jet_cut_pt      = cms.double(10),
492	        jet_cut_eta     = cms.double(2.5), 
493	        met_cut_pt          = cms.double(-99),      
494	        saveAllVertex       = cms.bool(True),
495	        vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
496	                rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
497	        #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
498	            neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
499	            jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("pf")),
500	                                        #cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
501	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patType1CorrectedPFMet"),   algo =cms.untracked.string("pf")),
502	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patType1p2CorrectedPFMet"), algo =cms.untracked.string("pf")),
503	                                       cms.PSet(jet = cms.untracked.string("selectedPatJets"),       met = cms.untracked.string("patMETs"),       algo =cms.untracked.string("ak5"))
504	                                     )
505	    )
506	
507	
508	process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )
509	
510	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
511	
512	# loads your analyzer
513	process.MyModule = cms.EDAnalyzer('NTupleProducer',
514	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
515	                saveAllTriggers     = cms.bool(True),# False: see list above
516	                saveTriggersWith    = cms.vstring(""),
517	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
518	                doElectrons         = cms.bool(True),
519	                electron_cut_pt     = cms.double(7),
520	                electron_cut_eta    = cms.double(2.5),
521	                doPhotons           = cms.bool(True),
522	                photon_cut_pt       = cms.double(10),
523	                photon_cut_eta      = cms.double(2.5),
524	                doMuons             = cms.bool(True),
525	                muon_cut_pt         = cms.double(10),
526	                muon_cut_eta        = cms.double(2.5),
527	                doTaus              = cms.bool(True),
528	                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
529	                tau_cut_pt          = cms.double(5),
530	                tau_cut_eta         = cms.double(3.),
531	                doTracks            = cms.bool(False),
532	                track_cut_pt        = cms.double(0.5),
533	                track_cut_eta       = cms.double(2.4),
534	                doJets              = cms.bool(True),
535	                #jetAlgos            = cms.vstring("ak5","pf", "tc"),
536	                jetAlgos            = cms.vstring("ak5","pf"),
537	                jet_cut_pt          = cms.double(10),
538	                jet_cut_eta         = cms.double(2.5),
539	                saveAllVertex       = cms.bool(True),
540	                saveMCInfo          = cms.bool(True),
541	                
542	                # Configuration for skimming !
543	                #
544	                # Events will be recorded if one of the following parameters is activated :
545	                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True 
546	                # 
547	                numberOfLept4Skim   = cms.int32(-1),# for skims ! Total number of sel lepton,  -1 for no skiming
548	                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
549	                numberOfElec4Skim   = cms.int32(0),# number of sel electron
550	                useMuonIdSkim         = cms.bool(False),
551	                muon_cut_pt_skim      = cms.double(10),
552	                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
553	                useElectronIdSkim     = cms.bool(False),
554	                electron_cut_pt_skim  = cms.double(7),
555	                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
556	                
557	                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
558	                TMEMESkimList         = cms.vint32(),
559	                
560	                doMCDiLepSkimming     = cms.bool(False),
561	                MCDiLepList           = cms.vstring(""),
562	                
563	                doTauSkimming         = cms.bool(False), # skim on the number of reco taus (no id so far)
564	                numberOfTau4Skim      = cms.int32(1),
565	                tau_cut_pt_skim       = cms.double(40),
566	                tau_algo_skim         = cms.string("selectedPatTausPF2PAT"),
567	                
568	                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
569	                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),
570	                
571	                doJetSkimming         = cms.bool(False), # skim on the number of jets
572	                numberOfJet4Skim      = cms.int32(3),
573	                jet_cut_pt_skim       = cms.double(40), 
574	                jet_cut_eta_skim      = cms.double(2.5),
575	                jet_algo_skim         = cms.string("pf"),
576	                
577	                # ttbar-> tau+jets specific
578	                doTauJetAna_TauJetTrigSkimming  = cms.bool(False), # need to set doTrigger doTauSkimming doJetSkimming to true
579	                doTauJetAna_TauJetSkimming      = cms.bool(False), # need to set doTauSkimming doJetSkimming to true
580	                doTauJetAna_TmemeTauJetSkimming = cms.bool(False)  # need to set doTMEMESkimming doTauSkimming doJetSkimming to true, only for MC ttbar
581	           
582	                
583	        )
584	
585	
586	
587	
588	
589	
590	
591	
592	
593	
594	
595	
596	
597	
598	# top projections in PF2PAT:
599	getattr(process,"pfNoPileUp"  +postfix).enable = True
600	getattr(process,"pfNoMuon"    +postfix).enable = False 
601	getattr(process,"pfNoElectron"+postfix).enable = False
602	getattr(process,"pfNoTau"     +postfix).enable = False
603	getattr(process,"pfNoJet"     +postfix).enable = False
604	
605	# verbose flags for the PF2PAT modules
606	getattr(process,"pfNoMuon"+postfix).verbose = False
607	
608	
609	
610	
611	if runOnMC == False:
612	    # removing MC matching for standard PAT sequence
613	    # for the PF2PAT+PAT sequence, it is done in the usePF2PAT function
614	    removeMCMatchingPF2PAT( process, '')    
615	
616	
617	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
618	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
619	
620	
621	
622	
623	process.load("PhysicsTools.PatUtils.patPFMETCorrections_cff")
624	
625	process.selectedPatJetsForMETtype1p2Corr.src =cms.InputTag('selectedPatJetsPF2PAT')
626	process.selectedPatJetsForMETtype2Corr.src =cms.InputTag('selectedPatJetsPF2PAT')
627	process.patPFJetMETtype1p2Corr.type1JetPtThreshold = cms.double(10.0)
628	process.patPFJetMETtype1p2Corr.skipEM = cms.bool(False)
629	process.patPFJetMETtype1p2Corr.skipMuons = cms.bool(False) 
630	
631	
632	getattr(process,"patPF2PATSequence"+postfix).replace(
633	    getattr(process,"pfNoElectron"+postfix),
634	    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJets )
635	
636	# Let it run
637	process.p = cms.Path( process.goodOfflinePrimaryVertices*
638	                      process.HBHENoiseFilter*
639	                      process.noscraping + 
640	                      process.patElectronIDs *
641	                      getattr(process,"patPF2PATSequence"+postfix)*
642	                      process.producePatPFMETCorrections*
643	#                      process.kt6PFJets*
644	#                      getattr(process,"patPF2PATSequence"+postfix)*
645	              process.neutralPFCandidatesProducer*
646	              process.kt6NeutralPFJets
647	              
648	#    second PF2PAT
649	#    + getattr(process,"patPF2PATSequence"+postfix2)
650	)
651	if not postfix=="":
652	    process.p += process.patDefaultSequence
653	    
654	    
655	process.p += process.MiniTreeProduction
656	   
657	process.p += process.MyModule   
658	   
659	# Add PF2PAT output to the created file
660	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
661	process.out.outputCommands = cms.untracked.vstring('drop *',
662	                               #'keep *_*_*_PAT',
663	                           'keep edmTriggerResults_*_*_*',
664	                           #'keep double_kt6PFJets_rho_*',
665	                           #'keep *_hltTriggerSummaryAOD_*_*',
666	                           #'keep *_offlineBeamSpot_*_*',
667	                           #'keep *_offlinePrimaryVertices_*_PAT',
668	                           #'keep *_muons_*_*',
669	                           #'keep *_muonTCMETValueMapProducer_*_*',
670	                           #'keep recoTracks_*_*_*',
671	                           #'keep GenEventInfoProduct_*_*_*',
672	                           #'keep recoGenParticles_*_*_*',
673	                                                   #*patEventContentNoCleaning 
674	                           'keep TopTreeMTEvent_*_*_*'
675	                           ) 
676	
677	
678	## ------------------------------------------------------
679	#  In addition you usually want to change the following
680	#  parameters:
681	## ------------------------------------------------------
682	#
683	#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
684	#                                         ##
685	#   process.source.fileNames = [          ##
686	#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
687	#   ]                                     ##  (e.g. 'file:AOD.root')
688	#                                         ##
689	
690	
691	
692	process.maxEvents.input = -1            ##  (e.g. -1 to run on all events)
693	
694	
695	
696	#                                         ##
697	#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
698	#                                         ##
699	#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
700	#                                         ##
701	#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)    
702	
703	process.MessageLogger.cerr.FwkReport.reportEvery = 10
704	
705	# to relax the muon isolation, uncomment the following:
706	#process.pfIsolatedMuonsPFlow.combinedIsolationCut = cms.double(99999.)
707	#process.PF2PAT.pfMuonSequence.pfIsolatedMuons.combinedIsolationCut = cms.double(99999.)
708	
709	
710	process.source = cms.Source(
711	    "PoolSource",
712	    noEventSort = cms.untracked.bool(True),
713	    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
714	    fileNames = cms.untracked.vstring(
715	       '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/FE2BB376-C07C-E011-B3A6-001EC9D81A4A.root'  
716	    )
717	)
