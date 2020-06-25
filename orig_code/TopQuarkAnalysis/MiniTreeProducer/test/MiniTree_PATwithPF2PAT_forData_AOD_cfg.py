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
15	## Maximal Number of Events
16	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
17
18	## Geometry and Detector Conditions (needed for a few patTuple production steps)
19	process.load("Configuration.StandardSequences.Geometry_cff")
20	process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
21
22	from Configuration.PyReleaseValidation.autoCond import autoCond
23	#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
24	#process.GlobalTag.globaltag = cms.string('GR_P_V20::All')
25	process.GlobalTag.globaltag = cms.string('GR_R_42_V14::All')
26	process.load("Configuration.StandardSequences.MagneticField_cff")
27
28
29	## Standard PAT Configuration File
30	process.load("PhysicsTools.PatAlgos.patSequences_cff")
31
32
33	## Output Module Configuration (expects a path 'p')
34	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
35	process.out = cms.OutputModule("PoolOutputModule",
36	                               fileName = cms.untracked.string('patTuple.root'),
37	                               # save only events passing the full path
38	                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
39	                               # save PAT Layer 1 output; you need a '*' to
40	                               # unpack the list of commands 'patEventContent'
41	                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
42	                               )
43
44	process.outpath = cms.EndPath(process.out)
45
46
47
48
49
50
51
52	# PAT Layer 0+1
53	process.load("PhysicsTools.PatAlgos.patSequences_cff")
54
55	process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
56
57
58	#trigger matching & dynamic prescales stuff
59	from PhysicsTools.PatAlgos.tools.trigTools import *
60	switchOnTrigger( process )
61
62	process.jetMatchHLTJets = cms.EDProducer(
63	  # matching in DeltaR, sorting by best DeltaR
64	  "PATTriggerMatcherDRLessByR"
65	  # matcher input collections
66	, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
67	, matched = cms.InputTag( 'patTrigger' )
68	  # selections of trigger objects
69	#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
70	, matchedCuts = cms.string(  'type( "TriggerJet" ) && path( "HLT_QuadJet40_v*" )' )
71	  # selection of matches
72	, maxDPtRel   = cms.double( 0.5 ) # no effect here
73	, maxDeltaR   = cms.double( 0.5 )
74	, maxDeltaEta = cms.double( 0.2 ) # no effect here
75	  # definition of matcher output
76	, resolveAmbiguities    = cms.bool( True )
77	, resolveByMatchQuality = cms.bool( True )
78	)
79	switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets' ] )
80
81
82
83	process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
84	                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
85	                                           minimumNDOF = cms.uint32(4) ,
86	                                           maxAbsZ = cms.double(24),
87	                                           maxd0 = cms.double(2)
88	)
89
90	process.noscraping = cms.EDFilter("FilterOutScraping",
91	                                  applyfilter = cms.untracked.bool(True),
92	                                  debugOn = cms.untracked.bool(True),
93	                                  numtrack = cms.untracked.uint32(10),
94	                                  thresh = cms.untracked.double(0.20)
95	)
96
97	#Noise filter
98	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
99
100
101
102
103
104
105
106
107
108
109	# the source is already defined in patTemplate_cfg.
110	# overriding source and various other things
111	#process.load("CommonTools.ParticleFlow.Sources.source_ZtoEles_DBS_312_cfi")
112	#process.source = cms.Source("PoolSource",
113	#     fileNames = cms.untracked.vstring('file:myAOD.root')
114	#)
115
116
117	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
118	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
119
120	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
121	runOnMC = False
122
123	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
124	process.out.fileName = cms.untracked.string('patTuple.root')
125
126	# load the PAT config
127	process.load("PhysicsTools.PatAlgos.patSequences_cff")
128
129
130	# Configure PAT to use PF2PAT instead of AOD sources
131	# this function will modify the PAT sequences. It is currently
132	# not possible to run PF2PAT+PAT and standart PAT at the same time
133	from PhysicsTools.PatAlgos.tools.pfTools import *
134
135	# An empty postfix means that only PF2PAT is run,
136	# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
137	# collections have standard names + postfix (e.g. patElectronPFlow)
138
139
140	postfix = "PF2PAT"  # to have only PF2PAT
141	jetAlgo="AK5"
142	usePFnoPU = True # before any top projection
143
144
145	# levels to be accessible from the jets
146	# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
147	# and remain uncorrected, if none of these levels is enabled here
148	useL1FastJet    = True  # needs useL1Offset being off, error otherwise
149	useL1Offset     = False # needs useL1FastJet being off, error otherwise
150	useL2Relative   = True
151	useL3Absolute   = True
152	# useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
153	useL5Flavor     = False
154	useL7Parton     = False
155
156	# JEC set
157	jecSetBase = jetAlgo
158	jecSetPF = jecSetBase + 'PF'
159	if usePFnoPU:
160	  jecSetPF += 'chs'
161
162	# JEC levels
163	if useL1FastJet and useL1Offset:
164	  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )
165	jecLevels = []
166	if useL1FastJet:
167	  jecLevels.append( 'L1FastJet' )
168	if useL1Offset:
169	  jecLevels.append( 'L1Offset' )
170	if useL2Relative:
171	  jecLevels.append( 'L2Relative' )
172	if useL3Absolute:
173	  jecLevels.append( 'L3Absolute' )
174	# if useL2L3Residual and not runOnMC:
175	#   jecLevelsPF.append( 'L2L3Residual' )
176	if useL5Flavor:
177	  jecLevels.append( 'L5Flavor' )
178	if useL7Parton:
179	  jecLevels.append( 'L7Parton' )
180
181
182
183
184	usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) )
185
186	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
187
188	#to use tau-cleaned jet collection uncomment the following:
189	#getattr(process,"pfNoTau"+postfix).enable = True
190
191
192	addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
193	                'AK5', 'JPT',
194	                doJTA        = True,
195	                doBTagging   = True,
196	                jetCorrLabel = ('AK5JPT', cms.vstring(['L2Relative', 'L3Absolute'])),
197	                doType1MET   = False,
198	                doL1Cleaning = False,
199	                doL1Counters = True,
200	                genJetCollection = cms.InputTag("ak5GenJets"),
201	                doJetID      = True
202	                )
203
204
205
206
207	from PhysicsTools.PatAlgos.tools.metTools import *
208
209	addTcMET(process, 'TC')
210	process.patMETsTC.addGenMET = False
211
212	removeMCMatching(process, ['All'])
213	removeIfInSequence(process, 'patJetGenJetMatchAK5JPT', "patDefaultSequence")
214	removeIfInSequence(process, 'patJetPartonMatchAK5JPT', "patDefaultSequence")
215	removeIfInSequence(process, 'patJetPartons',           "patDefaultSequence")
216	removeIfInSequence(process, 'patJetPartonAssociation', "patDefaultSequence")
217	removeIfInSequence(process, 'patJetPartonAssociationAK5JPT',  "patDefaultSequence")
218	removeIfInSequence(process, 'patJetFlavourAssociation',       "patDefaultSequence")
219	removeIfInSequence(process, 'patJetFlavourAssociationAK5JPT', "patDefaultSequence")
220
221
222	process.load('RecoJets.Configuration.RecoJets_cff')
223	from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
224
225
226	process.kt6PFJets               = kt4PFJets.clone()
227	process.kt6PFJets.rParam        = 0.6
228	process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
229	process.kt6PFJets.doRhoFastjet  = True
230	process.kt6PFJets.doAreaFastjet = True
231	#process.kt6PFJets.voronoiRfact  = 0.9
232
233	process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")
234
235	process.kt6NeutralPFJets = kt4PFJets.clone()
236	process.kt6NeutralPFJets.rParam        = 0.6
237	process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
238	process.kt6NeutralPFJets.doRhoFastjet  = True
239	process.kt6NeutralPFJets.doAreaFastjet = True
240	process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
241	#process.kt6PFJets.voronoiRfact  = 0.9
242
243
244	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
245	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
246	process.jetIDSelector.version = cms.string('PURE09')
247
248
249
250
251
252	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
253	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
254
255	process.patElectrons.addElectronID = cms.bool(True)
256	process.patElectrons.electronIDSources = cms.PSet(
257	    eidLoose= cms.InputTag("eidLoose"),
258	    eidTight= cms.InputTag("eidTight"),
259	    eidRobustTight= cms.InputTag("eidRobustTight"),
260	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
261	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
262	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
263	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
264	    cicLoose         =  cms.InputTag("cicLoose"),
265	    cicMedium        =  cms.InputTag("cicMedium"),
266	    cicTight         =  cms.InputTag("cicTight"),
267	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
268	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
269	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
270	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
271	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
272	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
273	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
274	    cicTightMC         =  cms.InputTag("cicTightMC"),
275	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
276	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
277	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
278	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC")
279	)
280
281
282	process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)
283
284	process.patElectronsPF2PAT.addElectronID = cms.bool(True)
285	process.patElectronsPF2PAT.electronIDSources = cms.PSet(
286	    eidLoose= cms.InputTag("eidLoose"),
287	    eidTight= cms.InputTag("eidTight"),
288	    eidRobustTight= cms.InputTag("eidRobustTight"),
289	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
290	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
291	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
292	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
293	    cicLoose         =  cms.InputTag("cicLoose"),
294	    cicMedium        =  cms.InputTag("cicMedium"),
295	    cicTight         =  cms.InputTag("cicTight"),
296	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
297	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
298	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
299	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
300	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
301	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
302	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
303	    cicTightMC         =  cms.InputTag("cicTightMC"),
304	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
305	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
306	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
307	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC")
308	)
309
310
311
312
313
314	# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
315	# and add it to path
316	#postfix2 = "PFlow2"
317	#jetAlgo2="AK7"
318	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)
319
320	# to use tau-cleaned jet collection uncomment the following:
321	#getattr(process,"pfNoTau"+postfix).enable = True
322
323	# to switch default tau to HPS tau uncomment the following:
324	#adaptPFTaus(process,"hpsPFTau",postfix=postfix)
325
326
327
328
329
330	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
331	                isAOD               = cms.bool(True), # true if processing AOD data
332	                isData              = cms.bool(True), # true if processing AOD data
333	                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
334	                TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files)
335	                TriggerInfo         = cms.bool(True),
336	                saveAllTriggers     = cms.bool(True),#should be True by default !!
337	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
338	                prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5"),
339	                hltJetsMatcher      = cms.string("jetMatchHLTJets"),
340	                doGenParticleCollection = cms.bool(False),
341	                doElectrons         = cms.bool(True),
342	                electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
343	                electron_cut_pt     = cms.double(10),
344	                electron_cut_eta    = cms.double(2.5),
345	                doPhotons           = cms.bool(False),
346	                photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
347	                photon_cut_pt       = cms.double(10),
348	                photon_cut_eta      = cms.double(2.5),
349	                doMuons             = cms.bool(True),
350	                muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
351	                muon_cut_pt         = cms.double(10),
352	                muon_cut_eta        = cms.double(2.5),
353	                doTaus              = cms.bool(False),
354	                tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
355	                #tauProducer         = cms.vstring("selectedPatTaus"),
356	                tau_cut_pt          = cms.double(5),
357	                tau_cut_eta         = cms.double(2.5),
358	                trackProducer       = cms.InputTag("generalTracks"),
359	                doTracks            = cms.bool(False),
360	                track_cut_pt        = cms.double(0.5),
361	                track_cut_eta       = cms.double(2.4),
362	                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
363	                jet_cut_pt          = cms.double(10),
364	                jet_cut_eta         = cms.double(2.5),
365	                met_cut_pt          = cms.double(-99),
366	                saveAllVertex       = cms.bool(True),
367	                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
368	                rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
369	                neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
370	                #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
371	                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("pf")),
372	                                                #cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
373	                                                cms.PSet(jet = cms.untracked.string("selectedPatJets"),       met = cms.untracked.string("patMETs"),       algo =cms.untracked.string("ak5"))
374	                                             )
375	        )
376
377
378	process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )
379
380	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
381
382	# loads your analyzer
383	process.MyModule = cms.EDAnalyzer('NTupleProducer',
384	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
385	                saveAllTriggers     = cms.bool(True),# False: see list above
386	                saveTriggersWith    = cms.vstring(""),
387	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
388	                doElectrons         = cms.bool(True),
389	                electron_cut_pt     = cms.double(10),
390	                electron_cut_eta    = cms.double(2.5),
391	                doPhotons           = cms.bool(True),
392	                photon_cut_pt       = cms.double(10),
393	                photon_cut_eta      = cms.double(2.5),
394	                doMuons             = cms.bool(True),
395	                muon_cut_pt         = cms.double(7),
396	                muon_cut_eta        = cms.double(2.5),
397	                doTaus              = cms.bool(True),
398	                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
399	                tau_cut_pt          = cms.double(5),
400	                tau_cut_eta         = cms.double(3.),
401	                doTracks            = cms.bool(False),
402	                track_cut_pt        = cms.double(0.5),
403	                track_cut_eta       = cms.double(2.4),
404	                doJets              = cms.bool(True),
405	                jetAlgos            = cms.vstring("ak5","pf", "tc"),
406	                jet_cut_pt          = cms.double(10),
407	                jet_cut_eta         = cms.double(2.5),
408	                saveAllVertex       = cms.bool(True),
409	                saveMCInfo          = cms.bool(True),
410
411	                # Configuration for skimming !
412	                #
413	                # Events will be recorded if one of the following parameters is activated :
414	                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True
415	                #
416	                numberOfLept4Skim   = cms.int32(2),# for skims ! Total number of sel lepton,  -1 for no skiming
417	                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
418	                numberOfElec4Skim   = cms.int32(0),# number of sel electron
419	                useMuonIdSkim         = cms.bool(False),
420	                muon_cut_pt_skim      = cms.double(10),
421	                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
422	                useElectronIdSkim     = cms.bool(False),
423	                electron_cut_pt_skim  = cms.double(7),
424	                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
425
426	                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
427	                TMEMESkimList         = cms.vint32(),
428
429	                doMCDiLepSkimming     = cms.bool(False),
430	                MCDiLepList           = cms.vstring(""),
431
432	                doTauSkimming         = cms.bool(False), # skim on the number of reco taus (no id so far)
433	                numberOfTau4Skim      = cms.int32(1),
434	                tau_cut_pt_skim       = cms.double(5),
435	                tau_algo_skim         = cms.string("selectedPatTaus"),
436
437	                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
438	                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),
439
440	                doJetSkimming         = cms.bool(False), # skim on the number of jets
441	                numberOfJet4Skim      = cms.int32(1),
442	                jet_cut_pt_skim       = cms.double(10),
443	                jet_cut_eta_skim      = cms.double(2.5),
444	                jet_algo_skim         = cms.string("ak5"),
445
446
447	        )
448
449
450
451
452	# top projections in PF2PAT:
453	getattr(process,"pfNoPileUp"  +postfix).enable = True
454	getattr(process,"pfNoMuon"    +postfix).enable = False
455	getattr(process,"pfNoElectron"+postfix).enable = False
456	getattr(process,"pfNoTau"     +postfix).enable = False
457	getattr(process,"pfNoJet"     +postfix).enable = False
458
459	# verbose flags for the PF2PAT modules
460	getattr(process,"pfNoMuon"+postfix).verbose = False
461
462
463
464
465	if runOnMC == False:
466	    # removing MC matching for standard PAT sequence
467	    # for the PF2PAT+PAT sequence, it is done in the usePF2PAT function
468	    removeMCMatchingPF2PAT( process, '' )
469
470
471	# Let it run
472	process.p = cms.Path( process.HBHENoiseFilter*
473	                      process.noscraping +
474	                      process.patElectronIDs *
475	                      process.kt6PFJets*
476	                      process.neutralPFCandidatesProducer*
477	                      process.kt6NeutralPFJets*
478	                      getattr(process,"patPF2PATSequence"+postfix)
479
480
481	#    second PF2PAT
482	#    + getattr(process,"patPF2PATSequence"+postfix2)
483	)
484	if not postfix=="":
485	    process.p += process.patDefaultSequence
486
487
488	process.p += process.MiniTreeProduction
489
490	process.p += process.MyModule
491
492	# Add PF2PAT output to the created file
493	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
494	process.out.outputCommands = cms.untracked.vstring('drop *',
495	                                                    'keep TopTreeMTEvent_*_*_*'
496	                                                   #'keep *_*_*_PAT',
497	                                                   #'keep edmTriggerResults_*_*_*',
498	                                                   #'keep double_kt6PFJets_rho_*',
499	                                                   #'keep *_hltTriggerSummaryAOD_*_*',
500	                                                   #'keep *_offlineBeamSpot_*_*',
501	                                                   #'keep *_offlinePrimaryVertices_*_*',
502	                                                   #'keep *_muons_*_*',
503	                                                   #'keep *_muonTCMETValueMapProducer_*_*',
504	                                                   #'keep recoTracks_*_*_*',
505	                                                   #'keep GenEventInfoProduct_*_*_*',
506	                                                   #'keep recoGenParticles_*_*_*',
507	                                                   #*patEventContentNoCleaning
508	                                                   )
509
510
511	## ------------------------------------------------------
512	#  In addition you usually want to change the following
513	#  parameters:
514	## ------------------------------------------------------
515	#
516	#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
517	#                                         ##
518	#   process.source.fileNames = [          ##
519	#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
520	#   ]                                     ##  (e.g. 'file:AOD.root')
521	#                                         ##
522
523
524
525	process.maxEvents.input = -1             ##  (e.g. -1 to run on all events)
526
527
528
529	#                                         ##
530	#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
531	#                                         ##
532	#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
533	#                                         ##
534	#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)
535
536	process.MessageLogger.cerr.FwkReport.reportEvery = 10
537
538	# to relax the muon isolation, uncomment the following:
539	#process.pfIsolatedMuonsPFlow.combinedIsolationCut = 99999
540
541
542	process.source = cms.Source(
543	        "PoolSource",
544	        noEventSort = cms.untracked.bool(True),
545	        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
546	        fileNames = cms.untracked.vstring(
547
548	       '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/FE2BB376-C07C-E011-B3A6-001EC9D81A4A.root',
549	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/EC91B253-AE7C-E011-9F96-485B39800BFB.root',
550	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/E89314D1-A37C-E011-8FA7-E0CB4E5536BB.root',
551	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/E87997CC-AD7C-E011-B307-E0CB4E19F999.root',
552	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/DEBE1714-E27C-E011-9E34-E0CB4E55366A.root',
553	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D870C654-C47C-E011-BD47-E0CB4E1A1150.root',
554	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D689ABEB-B87C-E011-962B-E0CB4E4408DF.root',
555	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D065B7F8-C47C-E011-AD82-E0CB4E29C4F5.root',
556	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/BC59235C-C07C-E011-A581-E0CB4EA0A8ED.root',
557	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/AC3A2F69-BD7C-E011-9287-E0CB4E4408EE.root',
558	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/A6EB7E32-CC7C-E011-8572-E0CB4E29C50A.root',
559	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/9CFB03E7-D77C-E011-8DCC-E0CB4E553657.root',
560	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/989F5D58-AE7C-E011-8419-E0CB4E29C4D5.root',
561	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/986255F1-B47C-E011-AEDD-E0CB4E553644.root',
562	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/82BCEC06-B57C-E011-BEE6-0022198F5B1E.root',
563	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/7EB6BBFA-C47C-E011-BCE5-485B39800B86.root',
564	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/78E9C834-C97C-E011-B1CE-002618FDA1C2.root',
565	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/7669575E-C07C-E011-A369-90E6BA442F0F.root',
566	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/706CCFD8-C17C-E011-B98A-E0CB4EA0A8EC.root',
567	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/625C0091-B07C-E011-B248-90E6BA0D09D4.root',
568	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/607DC8A8-F77C-E011-A456-E0CB4E5536EF.root',
569	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/5C67BCF0-B77C-E011-9057-485B39800BC7.root',
570	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/52EAB736-BC7C-E011-8637-90E6BA0D09EA.root',
571	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/4A3D0DE9-D77C-E011-B137-485B39800C16.root',
572	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/409C81F7-D77C-E011-A758-E0CB4E29C4C0.root',
573	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/3ED00CFE-B47C-E011-AF39-90E6BA442F38.root',
574	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/388AAC35-AC7C-E011-9F41-E0CB4E29C4D2.root',
575	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/2E77E8C6-BA7C-E011-B292-E0CB4E29C4EC.root',
576	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/283C0C31-B47C-E011-AB86-E0CB4E19F9A9.root',
577	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/1669D46C-D17C-E011-B59E-E0CB4E19F9BB.root',
578	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/1479C68D-BF7C-E011-A978-00261834B53C.root',
579	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/0013FC4E-C07C-E011-B239-E0CB4E29C4B7.root',
580	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0004/FC4D42FB-D77C-E011-A9AF-E0CB4E29C4D1.root',
581	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0004/FAEB9F99-B37C-E011-A753-E0CB4E29C4B7.root'
582
583
584
585	        )
586	)
587
588	
