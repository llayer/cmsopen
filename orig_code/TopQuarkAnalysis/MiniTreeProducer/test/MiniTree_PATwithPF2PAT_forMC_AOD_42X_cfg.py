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
26	process.GlobalTag.globaltag = cms.string('START42_V13::All')
27	
28	
29	process.GlobalTag.toGet = cms.VPSet(
30	  cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
31	#        tag = cms.string("TrackProbabilityCalibration_2D_2010Data_v1_offline"),
32	#        tag = cms.string("TrackProbabilityCalibration_2D_2011Data_v1_offline"),
33	       tag = cms.string("TrackProbabilityCalibration_2D_2011_v1_mc"),
34	       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU")),
35	  cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
36	#        tag = cms.string("TrackProbabilityCalibration_3D_2010Data_v1_offline"),
37	#        tag = cms.string("TrackProbabilityCalibration_3D_2011Data_v1_offline"),
38	       tag = cms.string("TrackProbabilityCalibration_3D_2011_v1_mc"),
39	       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU"))
40	)
41	
42	
43	
44	
45	process.load("Configuration.StandardSequences.MagneticField_cff")
46	
47	## Standard PAT Configuration File
48	#process.load("PhysicsTools.PatAlgos.patSequences_cff")
49	
50	
51	## Output Module Configuration (expects a path 'p')
52	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
53	process.out = cms.OutputModule("PoolOutputModule",
54	                               fileName = cms.untracked.string('patTuple.root'),
55	                               # save only events passing the full path
56	                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
57	                               # save PAT Layer 1 output; you need a '*' to
58	                               # unpack the list of commands 'patEventContent'
59	                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
60	                               )
61	
62	process.outpath = cms.EndPath(process.out)
63	
64	
65	
66	
67	
68	
69	
70	# PAT Layer 0+1
71	process.load("PhysicsTools.PatAlgos.patSequences_cff")
72	
73	process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
74	
75	#trigger matching & dynamic prescales stuff
76	from PhysicsTools.PatAlgos.tools.trigTools import *
77	switchOnTrigger( process )
78	
79	process.jetMatchHLTJets = cms.EDProducer(
80	  # matching in DeltaR, sorting by best DeltaR
81	  "PATTriggerMatcherDRLessByR"
82	  # matcher input collections
83	, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
84	#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
85	, matched = cms.InputTag( 'patTrigger' )
86	  # selections of trigger objects
87	#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
88	, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
89	  # selection of matches
90	, maxDPtRel   = cms.double( 0.5 ) # no effect here
91	, maxDeltaR   = cms.double( 0.5 )
92	, maxDeltaEta = cms.double( 0.2 ) # no effect here
93	  # definition of matcher output
94	, resolveAmbiguities    = cms.bool( True )
95	, resolveByMatchQuality = cms.bool( True )
96	)
97	switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets' ] )
98	
99	process.tauMatchHLTTaus = cms.EDProducer(
100	  # matching in DeltaR, sorting by best DeltaR
101	  "PATTriggerMatcherDRLessByR"
102	  # matcher input collections
103	#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
104	, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
105	, matched = cms.InputTag( 'patTrigger' )
106	  # selections of trigger objects
107	, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*",1 , 0 )')#&& filter( "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40" )')
108	#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
109	  # selection of matches
110	, maxDPtRel   = cms.double( 0.5 ) # no effect here
111	, maxDeltaR   = cms.double( 0.5 )
112	, maxDeltaEta = cms.double( 0.2 ) # no effect here
113	  # definition of matcher output
114	, resolveAmbiguities    = cms.bool( True )
115	, resolveByMatchQuality = cms.bool( True )
116	)
117	switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus' ] )
118	
119	process.jetMatchHLTJets45 = cms.EDProducer(
120	  # matching in DeltaR, sorting by best DeltaR
121	  "PATTriggerMatcherDRLessByR"
122	  # matcher input collections
123	, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
124	#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
125	, matched = cms.InputTag( 'patTrigger' )
126	  # selections of trigger objects
127	#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
128	, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet45_IsoPFTau45_v*", 0, 0) && filter( "hltQuadJet45IsoPFTau45" )') 
129	  # selection of matches
130	, maxDPtRel   = cms.double( 0.5 ) # no effect here
131	, maxDeltaR   = cms.double( 0.5 )
132	, maxDeltaEta = cms.double( 0.2 ) # no effect here
133	  # definition of matcher output
134	, resolveAmbiguities    = cms.bool( True )
135	, resolveByMatchQuality = cms.bool( True )
136	)
137	switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets45' ] )
138	
139	process.tauMatchHLTTaus45 = cms.EDProducer(
140	  # matching in DeltaR, sorting by best DeltaR
141	  "PATTriggerMatcherDRLessByR"
142	  # matcher input collections
143	#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
144	, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
145	, matched = cms.InputTag( 'patTrigger' )
146	  # selections of trigger objects
147	, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet45_IsoPFTau45_v*",1 , 0 )')#&& filter( "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40" )')
148	#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
149	  # selection of matches
150	, maxDPtRel   = cms.double( 0.5 ) # no effect here
151	, maxDeltaR   = cms.double( 0.5 )
152	, maxDeltaEta = cms.double( 0.2 ) # no effect here
153	  # definition of matcher output
154	, resolveAmbiguities    = cms.bool( True )
155	, resolveByMatchQuality = cms.bool( True )
156	)
157	switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus45' ] )
158	
159	
160	from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
161	
162	process.goodOfflinePrimaryVertices = cms.EDFilter(
163	    "PrimaryVertexObjectFilter",
164	    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
165	    src=cms.InputTag('offlinePrimaryVertices')
166	    )
167	    
168	    
169	process.noscraping = cms.EDFilter("FilterOutScraping",
170	                                  applyfilter = cms.untracked.bool(True),
171	                                  debugOn = cms.untracked.bool(True),
172	                                  numtrack = cms.untracked.uint32(10),
173	                                  thresh = cms.untracked.double(0.20)
174	)    
175	
176	#Noise filter
177	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
178	
179	
180	
181	
182	
183	
184	
185	
186	
187	
188	# the source is already defined in patTemplate_cfg.
189	# overriding source and various other things
190	#process.load("CommonTools.ParticleFlow.Sources.source_ZtoEles_DBS_312_cfi")
191	#process.source = cms.Source("PoolSource", 
192	#     fileNames = cms.untracked.vstring('file:myAOD.root')
193	#)
194	
195	
196	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
197	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
198	
199	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
200	runOnMC = True
201	
202	## Maximal Number of Events
203	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
204	process.out.fileName = cms.untracked.string('patTuple.root')
205	
206	# load the PAT config
207	process.load("PhysicsTools.PatAlgos.patSequences_cff")
208	
209	
210	# Configure PAT to use PF2PAT instead of AOD sources
211	# this function will modify the PAT sequences. It is currently 
212	# not possible to run PF2PAT+PAT and standart PAT at the same time
213	from PhysicsTools.PatAlgos.tools.pfTools import *
214	
215	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
216	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
217	#process.isoValMuonWithCharged.deltaR = 0.3
218	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
219	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
220	 
221	 
222	# An empty postfix means that only PF2PAT is run,
223	# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
224	# collections have standard names + postfix (e.g. patElectronPFlow)  
225	
226	
227	postfix = "PF2PAT"  # to have only PF2PAT
228	jetAlgo="AK5"
229	
230	usePFnoPU = True # before any top projection
231	
232	
233	# levels to be accessible from the jets
234	# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
235	# and remain uncorrected, if none of these levels is enabled here
236	useL1FastJet    = True  # needs useL1Offset being off, error otherwise
237	useL1Offset     = False # needs useL1FastJet being off, error otherwise
238	useL2Relative   = True
239	useL3Absolute   = True
240	# useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
241	useL5Flavor     = False
242	useL7Parton     = False
243	
244	# JEC set
245	jecSetBase = jetAlgo
246	jecSetPF = jecSetBase + 'PF'
247	if usePFnoPU:
248	  jecSetPF += 'chs'
249	
250	# JEC levels
251	if useL1FastJet and useL1Offset:
252	  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )
253	jecLevels = []
254	if useL1FastJet:
255	  jecLevels.append( 'L1FastJet' )
256	if useL1Offset:
257	  jecLevels.append( 'L1Offset' )
258	if useL2Relative:
259	  jecLevels.append( 'L2Relative' )
260	if useL3Absolute:
261	  jecLevels.append( 'L3Absolute' )
262	# if useL2L3Residual and not runOnMC:
263	#   jecLevelsPF.append( 'L2L3Residual' )
264	if useL5Flavor:
265	  jecLevels.append( 'L5Flavor' )
266	if useL7Parton:
267	  jecLevels.append( 'L7Parton' )
268	
269	
270	
271	
272	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) ) 
273	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
274	
275	usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix) 
276	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
277	
278	applyPostfix(process,"patJetCorrFactors",postfix).levels     = cms.vstring('L1FastJet','L2Relative','L3Absolute')
279	applyPostfix(process,"patJetCorrFactors",postfix).rho        = cms.InputTag("kt6PFJets","rho")
280	applyPostfix(process,"patJetCorrFactors",postfix).payload    = cms.string('AK5PFchs')
281	applyPostfix(process,"pfPileUp",postfix).checkClosestZVertex = cms.bool(False) 
282	
283	
284	# to use tau-cleaned jet collection uncomment the following:
285	#getattr(process,"pfNoTau"+postfix).enable = True
286	
287	#changes for iso and deltaR
288	#muons
289	applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
290	applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
291	applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
292	applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
293	#electrons
294	applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
295	applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
296	applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
297	applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
298	# jec should be ok
299	# by default: 'L1Offset','L2Relative','L3Absolute'
300	#  and primaryVertices = cms.InputTag("offlinePrimaryVertices")
301	
302	#addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
303	#                 'AK5', 'JPT',
304	#                 doJTA        = True,
305	#                 doBTagging   = True,
306	#modifdg (L1Offset to substract PU effect)       
307	#                 jetCorrLabel = ('AK5JPT', cms.vstring([ 'L2Relative', 'L3Absolute'])),
308	#                 jetCorrLabel = ('AK5JPT', cms.vstring(['L1Offset', 'L2Relative', 'L3Absolute'])),
309	#                 doType1MET   = False,
310	#                 doL1Cleaning = False,
311	#                 doL1Counters = True,                 
312	#                 genJetCollection = cms.InputTag("ak5GenJets"),
313	#                 doJetID      = True
314	#                 )
315	     
316	from PhysicsTools.PatAlgos.tools.metTools import *
317	
318	addTcMET(process, 'TC')
319	process.patMETsTC.addGenMET = False
320	
321	process.pfPileUpPF2PAT.Enable = True
322	#process.pfPileUpPF2PAT.checkClosestZVertex = cms.bool(False)
323	process.pfPileUpPF2PAT.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
324	process.pfJetsPF2PAT.doAreaFastjet = True
325	process.pfJetsPF2PAT.doRhoFastjet = False
326	
327	removeMCMatching(process, ['All'])
328	removeIfInSequence(process, 'patJetGenJetMatchAK5JPT', "patDefaultSequence")
329	removeIfInSequence(process, 'patJetPartonMatchAK5JPT', "patDefaultSequence")
330	removeIfInSequence(process, 'patJetPartons',           "patDefaultSequence")
331	removeIfInSequence(process, 'patJetPartonAssociation', "patDefaultSequence")
332	removeIfInSequence(process, 'patJetPartonAssociationAK5JPT',  "patDefaultSequence")
333	removeIfInSequence(process, 'patJetFlavourAssociation',       "patDefaultSequence")
334	removeIfInSequence(process, 'patJetFlavourAssociationAK5JPT', "patDefaultSequence")
335	
336	process.load('RecoJets.Configuration.RecoJets_cff')
337	from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
338	
339	process.kt6PFJets               = kt4PFJets.clone()
340	process.kt6PFJets.rParam        = 0.6     
341	process.kt6PFJets.src           = cms.InputTag('pfNoElectron'+postfix)
342	process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
343	process.kt6PFJets.doRhoFastjet  = True
344	process.kt6PFJets.doAreaFastjet = True
345	#process.kt6PFJets.voronoiRfact  = 0.9
346	
347	#process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJets", "rho")
348	getattr(process,"patJetCorrFactors"+postfix).rho = cms.InputTag("kt6PFJets", "rho")
349	
350	process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")
351	
352	process.kt6NeutralPFJets = kt4PFJets.clone()
353	process.kt6NeutralPFJets.rParam        = 0.6     
354	process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
355	process.kt6NeutralPFJets.doRhoFastjet  = True
356	process.kt6NeutralPFJets.doAreaFastjet = True
357	process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
358	#process.kt6PFJets.voronoiRfact  = 0.9
359	
360	
361	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
362	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
363	process.jetIDSelector.version = cms.string('PURE09')
364	
365	
366	
367	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
368	#process.load("RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi")
369	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
370	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi")
371	
372	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
373	
374	process.patElectrons.addElectronID = cms.bool(True)
375	process.patElectrons.electronIDSources = cms.PSet(
376	    eidLoose= cms.InputTag("eidLoose"),
377	    eidTight= cms.InputTag("eidTight"),
378	    eidRobustTight= cms.InputTag("eidRobustTight"),
379	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
380	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
381	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
382	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
383	    cicLoose         =  cms.InputTag("cicLoose"),
384	    cicMedium        =  cms.InputTag("cicMedium"),
385	    cicTight         =  cms.InputTag("cicTight"),
386	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
387	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
388	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
389	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
390	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
391	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
392	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
393	    cicTightMC         =  cms.InputTag("cicTightMC"),
394	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
395	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
396	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
397	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
398	)
399	
400	process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)
401	
402	process.patElectronsPF2PAT.addElectronID = cms.bool(True)
403	process.patElectronsPF2PAT.electronIDSources = cms.PSet(
404	    eidLoose= cms.InputTag("eidLoose"),
405	    eidTight= cms.InputTag("eidTight"),
406	    eidRobustTight= cms.InputTag("eidRobustTight"),
407	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
408	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
409	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
410	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
411	    cicLoose         =  cms.InputTag("cicLoose"),
412	    cicMedium        =  cms.InputTag("cicMedium"),
413	    cicTight         =  cms.InputTag("cicTight"),
414	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
415	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
416	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
417	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
418	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
419	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
420	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
421	    cicTightMC         =  cms.InputTag("cicTightMC"),
422	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
423	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
424	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
425	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
426	)
427	
428	
429	
430	
431	
432	# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
433	# and add it to path
434	#postfix2 = "PFlow2"
435	#jetAlgo2="AK7"
436	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)
437	
438	# to use tau-cleaned jet collection uncomment the following:
439	#getattr(process,"pfNoTau"+postfix).enable = True 
440	
441	# to switch default tau to HPS tau uncomment the following:
442	#adaptPFTaus(process,"hpsPFTau",postfix=postfix)
443	
444	
445	
446	
447	
448	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
449	        isAOD               = cms.bool(True), # true if processing AOD data
450	        isData              = cms.bool(False), # true if processing AOD data
451	        readPDFWeights      = cms.bool(False),
452	        verbose         = cms.int32(0), #0: nothing - >1 gradually more information
453	        TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::REDIGI311X 
454	        TriggerInfo         = cms.bool(True), 
455	        saveAllTriggers     = cms.bool(True),#should be True by default !!
456	        triggerList     = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
457	        prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5","HLT_QuadJet40_IsoPFTau40_v1","HLT_QuadJet40_IsoPFTau40_v2","HLT_QuadJet40_IsoPFTau40_v3",
458	                "HLT_QuadJet40_IsoPFTau40_v4","HLT_QuadJet40_IsoPFTau40_v5","HLT_QuadJet40_IsoPFTau40_v6",
459	                "HLT_QuadJet40_IsoPFTau40_v7","HLT_QuadJet40_IsoPFTau40_v8","HLT_QuadJet40_IsoPFTau40_v9",
460	                "HLT_QuadJet40_IsoPFTau40_v10","HLT_QuadJet40_IsoPFTau40_v11","HLT_QuadJet40_IsoPFTau40_v12",
461	                "HLT_QuadJet40_IsoPFTau40_v13","HLT_QuadJet40_IsoPFTau40_v14","HLT_QuadJet40_IsoPFTau40_v15"),
462	        hltJetsMatcher      = cms.string("jetMatchHLTJets"),
463	        hltTausMatcher      = cms.string("tauMatchHLTTaus"),
464	        hltJetsMatcher45    = cms.string("jetMatchHLTJets45"),
465	        hltTausMatcher45    = cms.string("tauMatchHLTTaus45"),  
466	        doGenParticleCollection = cms.bool(True),
467	        doElectrons     = cms.bool(True),
468	        electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
469	        electron_cut_pt     = cms.double(10),
470	        electron_cut_eta    = cms.double(2.5), 
471	        doPhotons       = cms.bool(False),
472	        photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
473	        photon_cut_pt       = cms.double(10),
474	        photon_cut_eta      = cms.double(2.5), 
475	        doMuons         = cms.bool(True),
476	        muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
477	        muon_cut_pt         = cms.double(10),
478	        muon_cut_eta        = cms.double(2.5), 
479	        doTaus          = cms.bool(True),
480	        tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
481	        tau_cut_pt      = cms.double(10),
482	        tau_cut_eta     = cms.double(2.4),      
483	        trackProducer       = cms.InputTag("generalTracks"),
484	        doTracks            = cms.bool(False),
485	        track_cut_pt        = cms.double(0.5),
486	        track_cut_eta       = cms.double(2.4),
487	        beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
488	        jet_cut_pt      = cms.double(10),
489	        jet_cut_eta     = cms.double(2.5), 
490	        met_cut_pt          = cms.double(-99),      
491	        saveAllVertex       = cms.bool(True),
492	        vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
493	        rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
494	        #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
495	            neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
496	            jetmetProducer      = cms.VPSet(
497	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"),   algo =cms.untracked.string("pf")),
498	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsTypeIPF"),  algo =cms.untracked.string("pfType1")),
499	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsTypeIIPF"), algo =cms.untracked.string("pfType2"))
500	                                )
501	    )
502	
503	
504	process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )
505	
506	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
507	
508	# loads your analyzer
509	process.MyModule = cms.EDAnalyzer('NTupleProducer',
510	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
511	                saveAllTriggers     = cms.bool(True),# False: see list above
512	                saveTriggersWith    = cms.vstring(""),
513	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
514	                doElectrons         = cms.bool(True),
515	                electron_cut_pt     = cms.double(7),
516	                electron_cut_eta    = cms.double(2.5),
517	                doPhotons           = cms.bool(True),
518	                photon_cut_pt       = cms.double(10),
519	                photon_cut_eta      = cms.double(2.5),
520	                doMuons             = cms.bool(True),
521	                muon_cut_pt         = cms.double(10),
522	                muon_cut_eta        = cms.double(2.5),
523	                doTaus              = cms.bool(True),
524	                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
525	                tau_cut_pt          = cms.double(5),
526	                tau_cut_eta         = cms.double(3.),
527	                doTracks            = cms.bool(False),
528	                track_cut_pt        = cms.double(0.5),
529	                track_cut_eta       = cms.double(2.4),
530	                doJets              = cms.bool(True),
531	                jetAlgos            = cms.vstring("ak5","pf", "tc"),
532	                jet_cut_pt          = cms.double(10),
533	                jet_cut_eta         = cms.double(2.5),
534	                saveAllVertex       = cms.bool(True),
535	                saveMCInfo          = cms.bool(True),
536	                
537	                # Configuration for skimming !
538	                #
539	                # Events will be recorded if one of the following parameters is activated :
540	                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True 
541	                # 
542	                numberOfLept4Skim   = cms.int32(2),# for skims ! Total number of sel lepton,  -1 for no skiming
543	                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
544	                numberOfElec4Skim   = cms.int32(0),# number of sel electron
545	                useMuonIdSkim         = cms.bool(False),
546	                muon_cut_pt_skim      = cms.double(10),
547	                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
548	                useElectronIdSkim     = cms.bool(False),
549	                electron_cut_pt_skim  = cms.double(10),
550	                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
551	                
552	                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
553	                #TMEMESkimList        = cms.vint32(10000),
554	                #TMEMESkimList        = cms.vint32(2,10101,20200,20,11010,22000,11,11001,10110,21100),
555	                #TMEMESkimList        = cms.vint32(10010,21000,10001,20100),
556	                #TMEMESkimList        = cms.vint32(0),
557	                #TMEMESkimList        = cms.vint32(20000),
558	                #TMEMESkimList        = cms.vint32(1,10,10100,11000),
559	                TMEMESkimList         = cms.vint32(),
560	                
561	                doMCDiLepSkimming     = cms.bool(False),
562	                MCDiLepList           = cms.vstring(""),
563	                
564	                doTauSkimming         = cms.bool(False), # skim on the number of reco taus (no id so far)
565	                numberOfTau4Skim      = cms.int32(1),
566	                tau_cut_pt_skim       = cms.double(5),
567	                tau_algo_skim         = cms.string("selectedPatTausPF2PAT"),
568	                
569	                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
570	                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),
571	                
572	                doJetSkimming         = cms.bool(False), # skim on the number of jets
573	                numberOfJet4Skim      = cms.int32(3),
574	                jet_cut_pt_skim       = cms.double(20), 
575	                jet_cut_eta_skim      = cms.double(2.5),
576	                jet_algo_skim         = cms.string("pf"),
577	                
578	                # ttbar-> tau+jets specific
579	                doTauJetAna_TauJetTrigSkimming  = cms.bool(False), # need to set doTrigger doTauSkimming doJetSkimming to true
580	                doTauJetAna_TauJetSkimming      = cms.bool(False),  # need to set doTauSkimming doJetSkimming to true
581	                doTauJetAna_TmemeTauJetSkimming = cms.bool(False) # need to set doTMEMESkimming doTauSkimming doJetSkimming to true, only for MC ttbar
582	        
583	                
584	        )
585	
586	
587	
588	
589	# top projections in PF2PAT:
590	getattr(process,"pfNoPileUp"  +postfix).enable = True
591	getattr(process,"pfNoMuon"    +postfix).enable = False 
592	getattr(process,"pfNoElectron"+postfix).enable = False
593	getattr(process,"pfNoTau"     +postfix).enable = False
594	getattr(process,"pfNoJet"     +postfix).enable = False
595	
596	# verbose flags for the PF2PAT modules
597	getattr(process,"pfNoMuon"+postfix).verbose = False
598	
599	
600	
601	
602	if runOnMC == False:
603	    removeMCMatchingPF2PAT( process, '')    
604	
605	
606	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
607	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
608	process.load('GeneratorInterface/GenFilters/TotalKinematicsFilter_cfi')
609	
610	
611	
612	
613	# Produce PDF weights (maximum is 3)
614	process.pdfWeights = cms.EDProducer("PdfWeightProducer",
615	           # Fix POWHEG if buggy (this PDF set will also appear on output,
616	           # so only two more PDF sets can be added in PdfSetNames if not "")
617	           #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
618	           #GenTag = cms.untracked.InputTag("genParticles"),
619	           PdfInfoTag = cms.untracked.InputTag("generator"),
620	           PdfSetNames = cms.untracked.vstring(
621	                   "cteq66.LHgrid"
622	           )
623	)
624	
625	
626	
627	
628	
629	
630	
631	
632	
633	
634	
635	
636	
637	
638	
639	
640	
641	#process.load("PhysicsTools.PatUtils.patPFMETCorrections_cff")
642	
643	#process.selectedPatJetsForMETtype1p2Corr.src =cms.InputTag('selectedPatJetsPF2PAT')
644	#process.selectedPatJetsForMETtype2Corr.src =cms.InputTag('selectedPatJetsPF2PAT')
645	#process.patPFJetMETtype1p2Corr.type1JetPtThreshold = cms.double(10.0)
646	#process.patPFJetMETtype1p2Corr.skipEM = cms.bool(False)
647	#process.patPFJetMETtype1p2Corr.skipMuons = cms.bool(False) 
648	
649	
650	
651	
652	##### Extra MET collections########
653	from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5PFJet
654	
655	#pfMET TypeI
656	
657	process.metJESCorAK5PFTypeI = metJESCorAK5PFJet.clone(
658	        inputUncorJetsLabel = "pfJetsPF2PAT", #is uncorrected since correction is performed at pat level!!!!!!!
659	        metType = "PFMET",
660	        inputUncorMetLabel = "pfMETPF2PAT",
661	        useTypeII = False,
662	        jetPTthreshold = cms.double(10.0),
663	        #corrector = cms.string('ak5PFL1FastL2L3')
664	
665	    )
666	    
667	    
668	process.patMETsTypeIPF = process.patMETsPF2PAT.clone(
669	        metSource = cms.InputTag("metJESCorAK5PFTypeI")
670	    )
671	
672	
673	
674	process.metJESCorAK5PFTypeII = metJESCorAK5PFJet.clone(
675	        inputUncorJetsLabel = "pfJetsPF2PAT", #is uncorrected since correction is performed at pat level!!!!!!!
676	        metType = "PFMET",
677	        inputUncorMetLabel = "pfMETPF2PAT",
678	        useTypeII = True,
679	        jetPTthreshold = cms.double(10.0),
680	        #corrector = cms.string('ak5PFL1FastL2L3')
681	
682	    )
683	    
684	    
685	process.patMETsTypeIIPF = process.patMETsPF2PAT.clone(
686	        metSource = cms.InputTag("metJESCorAK5PFTypeII")
687	    )
688	
689	
690	getattr(process,"patPF2PATSequence"+postfix).replace(
691	    getattr(process,"pfNoElectron"+postfix),
692	    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJets )
693	
694	# Let it run
695	process.p = cms.Path( process.totalKinematicsFilter*
696	                      #process.pdfWeights*  ## to be uncomment for ttbar
697	                      process.goodOfflinePrimaryVertices*
698	                      process.HBHENoiseFilter*
699	                      process.noscraping + 
700	                      process.patElectronIDs *
701	                      getattr(process,"patPF2PATSequence"+postfix)*
702	                      #process.producePatPFMETCorrections*
703	                      #process.kt6PFJets*
704	#                      process.kt6PFJets*
705	#                      getattr(process,"patPF2PATSequence"+postfix)*
706	                      process.metJESCorAK5PFTypeI*
707	                      process.patMETsTypeIPF*
708	                      process.metJESCorAK5PFTypeII*
709	                      process.patMETsTypeIIPF*
710	                      process.neutralPFCandidatesProducer*
711	                      process.kt6NeutralPFJets
712	              
713	#    second PF2PAT
714	#    + getattr(process,"patPF2PATSequence"+postfix2)
715	)
716	if not postfix=="":
717	    process.p += process.patDefaultSequence
718	    
719	    
720	process.p += process.MiniTreeProduction
721	   
722	process.p += process.MyModule   
723	   
724	# Add PF2PAT output to the created file
725	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
726	process.out.outputCommands = cms.untracked.vstring('drop *',
727	                           'keep *_*_*_PAT',
728	                           'keep edmTriggerResults_*_*_*',
729	                           #'keep double_kt6PFJets_rho_*',
730	                           #'keep *_hltTriggerSummaryAOD_*_*',
731	                           #'keep *_offlineBeamSpot_*_*',
732	                           #'keep *_offlinePrimaryVertices_*_PAT',
733	                           #'keep *_muons_*_*',
734	                           #'keep *_muonTCMETValueMapProducer_*_*',
735	                           #'keep recoTracks_*_*_*',
736	                           #'keep GenEventInfoProduct_*_*_*',
737	                           #'keep recoGenParticles_*_*_*',
738	                                                   #*patEventContentNoCleaning 
739	                           'keep TopTreeMTEvent_*_*_*'
740	                           ) 
741	
742	
743	## ------------------------------------------------------
744	#  In addition you usually want to change the following
745	#  parameters:
746	## ------------------------------------------------------
747	#
748	#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
749	#                                         ##
750	#   process.source.fileNames = [          ##
751	#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
752	#   ]                                     ##  (e.g. 'file:AOD.root')
753	#                                         ##
754	
755	
756	
757	process.maxEvents.input = -1            ##  (e.g. -1 to run on all events)
758	
759	
760	
761	#                                         ##
762	#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
763	#                                         ##
764	#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
765	#                                         ##
766	#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)    
767	
768	process.MessageLogger.cerr.FwkReport.reportEvery = 10
769	
770	# to relax the muon isolation, uncomment the following:
771	#process.pfIsolatedMuonsPFlow.combinedIsolationCut = cms.double(99999.)
772	#process.PF2PAT.pfMuonSequence.pfIsolatedMuons.combinedIsolationCut = cms.double(99999.)
773	
774	
775	process.source = cms.Source(
776	    "PoolSource",
777	    noEventSort = cms.untracked.bool(True),
778	    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
779	    fileNames = cms.untracked.vstring(
780	      '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FEEE3638-F297-E011-AAF8-00304867BEC0.root',
781	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FCFD87D7-9E98-E011-BDA2-0018F3D09642.root',
782	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FCFB9DE1-8598-E011-BE64-003048679076.root',
783	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FAD1CEE7-7A98-E011-89A0-001A92971B7E.root',
784	        '/store/mc/Summer11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S4_START42_V11-v1/0000/FAA9FD72-E497-E011-A542-001A92971BC8.root',
785	       
786	    )
787	)
