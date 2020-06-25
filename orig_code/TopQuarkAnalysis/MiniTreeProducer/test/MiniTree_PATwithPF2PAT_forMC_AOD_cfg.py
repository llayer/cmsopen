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
25	process.GlobalTag.globaltag = cms.string('START42_V12::All')
26
27
28
29	process.load("Configuration.StandardSequences.MagneticField_cff")
30
31	## Standard PAT Configuration File
32	#process.load("PhysicsTools.PatAlgos.patSequences_cff")
33
34
35	## Output Module Configuration (expects a path 'p')
36	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
37	process.out = cms.OutputModule("PoolOutputModule",
38	                               fileName = cms.untracked.string('patTuple.root'),
39	                               # save only events passing the full path
40	                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
41	                               # save PAT Layer 1 output; you need a '*' to
42	                               # unpack the list of commands 'patEventContent'
43	                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
44	                               )
45
46	process.outpath = cms.EndPath(process.out)
47
48
49
50
51
52
53
54	# PAT Layer 0+1
55	#process.load("PhysicsTools.PatAlgos.patSequences_cff")
56
57	process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
58
59	#from PhysicsTools.PatAlgos.tools.trigTools import *
60	#switchOnTrigger( process )
61
62	process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
63	                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
64	                                           minimumNDOF = cms.uint32(4) ,
65	                                           maxAbsZ = cms.double(24),
66	                                           maxd0 = cms.double(2)
67	)
68
69	process.noscraping = cms.EDFilter("FilterOutScraping",
70	                                  applyfilter = cms.untracked.bool(True),
71	                                  debugOn = cms.untracked.bool(True),
72	                                  numtrack = cms.untracked.uint32(10),
73	                                  thresh = cms.untracked.double(0.20)
74	)
75
76	#Noise filter
77	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
78
79
80
81
82
83
84
85
86
87
88	# the source is already defined in patTemplate_cfg.
89	# overriding source and various other things
90	#process.load("CommonTools.ParticleFlow.Sources.source_ZtoEles_DBS_312_cfi")
91	#process.source = cms.Source("PoolSource",
92	#     fileNames = cms.untracked.vstring('file:myAOD.root')
93	#)
94
95
96	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
97	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
98
99	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
100	runOnMC = True
101
102	## Maximal Number of Events
103	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
104	process.out.fileName = cms.untracked.string('patTuple.root')
105
106	# load the PAT config
107	process.load("PhysicsTools.PatAlgos.patSequences_cff")
108
109
110	# Configure PAT to use PF2PAT instead of AOD sources
111	# this function will modify the PAT sequences. It is currently
112	# not possible to run PF2PAT+PAT and standart PAT at the same time
113	from PhysicsTools.PatAlgos.tools.pfTools import *
114
115	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
116	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
117	#process.isoValMuonWithCharged.deltaR = 0.3
118	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
119	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
120
121
122	# An empty postfix means that only PF2PAT is run,
123	# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
124	# collections have standard names + postfix (e.g. patElectronPFlow)
125
126
127	postfix = "PF2PAT"  # to have only PF2PAT
128	jetAlgo="AK5"
129
130	usePFnoPU = True # before any top projection
131
132
133	# levels to be accessible from the jets
134	# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
135	# and remain uncorrected, if none of these levels is enabled here
136	useL1FastJet    = True  # needs useL1Offset being off, error otherwise
137	useL1Offset     = False # needs useL1FastJet being off, error otherwise
138	useL2Relative   = True
139	useL3Absolute   = True
140	# useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
141	useL5Flavor     = False
142	useL7Parton     = False
143
144	# JEC set
145	jecSetBase = jetAlgo
146	jecSetPF = jecSetBase + 'PF'
147	if usePFnoPU:
148	  jecSetPF += 'chs'
149
150	# JEC levels
151	if useL1FastJet and useL1Offset:
152	  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )
153	jecLevels = []
154	if useL1FastJet:
155	  jecLevels.append( 'L1FastJet' )
156	if useL1Offset:
157	  jecLevels.append( 'L1Offset' )
158	if useL2Relative:
159	  jecLevels.append( 'L2Relative' )
160	if useL3Absolute:
161	  jecLevels.append( 'L3Absolute' )
162	# if useL2L3Residual and not runOnMC:
163	#   jecLevelsPF.append( 'L2L3Residual' )
164	if useL5Flavor:
165	  jecLevels.append( 'L5Flavor' )
166	if useL7Parton:
167	  jecLevels.append( 'L7Parton' )
168
169
170
171
172	usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) )
173	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
174
175	# to use tau-cleaned jet collection uncomment the following:
176	#getattr(process,"pfNoTau"+postfix).enable = True
177
178	#changes for iso and deltaR
179	#muons
180	applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
181	applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
182	applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
183	applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
184	#electrons
185	applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
186	applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
187	applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
188	applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
189	# jec should be ok
190	# by default: 'L1Offset','L2Relative','L3Absolute'
191	#  and primaryVertices = cms.InputTag("offlinePrimaryVertices")
192
193	addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
194	                 'AK5', 'JPT',
195	                 doJTA        = True,
196	                 doBTagging   = True,
197	#modifdg (L1Offset to substract PU effect)
198	#                 jetCorrLabel = ('AK5JPT', cms.vstring([ 'L2Relative', 'L3Absolute'])),
199	                 jetCorrLabel = ('AK5JPT', cms.vstring(['L1Offset', 'L2Relative', 'L3Absolute'])),
200	                 doType1MET   = False,
201	                 doL1Cleaning = False,
202	                 doL1Counters = True,
203	                 genJetCollection = cms.InputTag("ak5GenJets"),
204	                 doJetID      = True
205	                 )
206
207	from PhysicsTools.PatAlgos.tools.metTools import *
208
209	addTcMET(process, 'TC')
210	process.load('RecoJets.Configuration.RecoJets_cff')
211	from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
212
213	process.kt6PFJets               = kt4PFJets.clone()
214	process.kt6PFJets.rParam        = 0.6
215	process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
216	process.kt6PFJets.doRhoFastjet  = True
217	process.kt6PFJets.doAreaFastjet = True
218	#process.kt6PFJets.voronoiRfact  = 0.9
219
220	process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")
221
222	process.kt6NeutralPFJets = kt4PFJets.clone()
223	process.kt6NeutralPFJets.rParam        = 0.6
224	process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
225	process.kt6NeutralPFJets.doRhoFastjet  = True
226	process.kt6NeutralPFJets.doAreaFastjet = True
227	process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
228	#process.kt6PFJets.voronoiRfact  = 0.9
229
230
231	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
232	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
233	process.jetIDSelector.version = cms.string('PURE09')
234
235
236
237	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
238	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
239
240	process.patElectrons.addElectronID = cms.bool(True)
241	process.patElectrons.electronIDSources = cms.PSet(
242	    eidLoose= cms.InputTag("eidLoose"),
243	    eidTight= cms.InputTag("eidTight"),
244	    eidRobustTight= cms.InputTag("eidRobustTight"),
245	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
246	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
247	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
248	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
249	    cicLoose         =  cms.InputTag("cicLoose"),
250	    cicMedium        =  cms.InputTag("cicMedium"),
251	    cicTight         =  cms.InputTag("cicTight"),
252	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
253	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
254	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
255	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
256	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
257	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
258	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
259	    cicTightMC         =  cms.InputTag("cicTightMC"),
260	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
261	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
262	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
263	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC")
264	)
265
266
267	process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)
268
269	process.patElectronsPF2PAT.addElectronID = cms.bool(True)
270	process.patElectronsPF2PAT.electronIDSources = cms.PSet(
271	    eidLoose= cms.InputTag("eidLoose"),
272	    eidTight= cms.InputTag("eidTight"),
273	    eidRobustTight= cms.InputTag("eidRobustTight"),
274	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
275	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
276	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
277	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
278	    cicLoose         =  cms.InputTag("cicLoose"),
279	    cicMedium        =  cms.InputTag("cicMedium"),
280	    cicTight         =  cms.InputTag("cicTight"),
281	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
282	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
283	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
284	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
285	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
286	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
287	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
288	    cicTightMC         =  cms.InputTag("cicTightMC"),
289	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
290	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
291	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
292	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC")
293	)
294
295
296
297
298
299	# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
300	# and add it to path
301	#postfix2 = "PFlow2"
302	#jetAlgo2="AK7"
303	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)
304
305	# to use tau-cleaned jet collection uncomment the following:
306	#getattr(process,"pfNoTau"+postfix).enable = True
307
308	# to switch default tau to HPS tau uncomment the following:
309	#adaptPFTaus(process,"hpsPFTau",postfix=postfix)
310
311
312
313
314
315	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
316	                isAOD               = cms.bool(True), # true if processing AOD data
317	                isData              = cms.bool(False), # true if processing AOD data
318	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
319	                TriggerMenu         = cms.InputTag("TriggerResults"),    # or TriggerResults::REDIGI311X
320	                TriggerInfo         = cms.bool(True),
321	                saveAllTriggers     = cms.bool(True),#should be True by default !!
322	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
323	                prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2"),
324	                hltJetsMatcher      = cms.string(""),
325	                doGenParticleCollection = cms.bool(True),
326	                doElectrons         = cms.bool(True),
327	                electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
328	                electron_cut_pt     = cms.double(10),
329	                electron_cut_eta    = cms.double(2.5),
330	                doPhotons           = cms.bool(False),
331	                photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
332	                photon_cut_pt       = cms.double(10),
333	                photon_cut_eta      = cms.double(2.5),
334	                doMuons             = cms.bool(True),
335	                muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
336	                muon_cut_pt         = cms.double(10),
337	                muon_cut_eta        = cms.double(2.5),
338	                doTaus              = cms.bool(False),
339	                tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
340	                #tauProducer         = cms.vstring("selectedPatTaushpsPFTauPF2PAT"),
341	                tau_cut_pt          = cms.double(5),
342	                tau_cut_eta         = cms.double(2.5),
343	                trackProducer       = cms.InputTag("generalTracks"),
344	                doTracks            = cms.bool(False),
345	                track_cut_pt        = cms.double(0.5),
346	                track_cut_eta       = cms.double(2.4),
347	                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
348	                jet_cut_pt          = cms.double(10),
349	                jet_cut_eta         = cms.double(2.5),
350	                met_cut_pt          = cms.double(-99),
351	                saveAllVertex       = cms.bool(True),
352	                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
353	                rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
354	                #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
355	                neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
356	                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("pf")),
357	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
358	                                                cms.PSet(jet = cms.untracked.string("selectedPatJets"),       met = cms.untracked.string("patMETs"),       algo =cms.untracked.string("ak5"))
359	                                             )
360	        )
361
362
363
364	process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )
365
366	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
367
368	# loads your analyzer
369	process.MyModule = cms.EDAnalyzer('NTupleProducer',
370	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
371	                saveAllTriggers     = cms.bool(True),# False: see list above
372	                saveTriggersWith    = cms.vstring(""),
373	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
374	                doElectrons         = cms.bool(True),
375	                electron_cut_pt     = cms.double(10),
376	                electron_cut_eta    = cms.double(2.5),
377	                doPhotons           = cms.bool(True),
378	                photon_cut_pt       = cms.double(10),
379	                photon_cut_eta      = cms.double(2.5),
380	                doMuons             = cms.bool(True),
381	                muon_cut_pt         = cms.double(7),
382	                muon_cut_eta        = cms.double(2.5),
383	                doTaus              = cms.bool(True),
384	                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
385	                tau_cut_pt          = cms.double(5),
386	                tau_cut_eta         = cms.double(3.),
387	                doTracks            = cms.bool(False),
388	                track_cut_pt        = cms.double(0.5),
389	                track_cut_eta       = cms.double(2.4),
390	                doJets              = cms.bool(True),
391	                jetAlgos            = cms.vstring("ak5","pf", "tc"),
392	                jet_cut_pt          = cms.double(10),
393	                jet_cut_eta         = cms.double(2.5),
394	                saveAllVertex       = cms.bool(True),
395	                saveMCInfo          = cms.bool(True),
396
397	                # Configuration for skimming !
398	                #
399	                # Events will be recorded if one of the following parameters is activated :
400	                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True
401	                #
402	                numberOfLept4Skim   = cms.int32(2),# for skims ! Total number of sel lepton,  -1 for no skiming
403	                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
404	                numberOfElec4Skim   = cms.int32(0),# number of sel electron
405	                useMuonIdSkim         = cms.bool(False),
406	                muon_cut_pt_skim      = cms.double(10),
407	                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
408	                useElectronIdSkim     = cms.bool(False),
409	                electron_cut_pt_skim  = cms.double(7),
410	                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
411
412	                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
413	                TMEMESkimList         = cms.vint32(),
414
415	                doMCDiLepSkimming     = cms.bool(False),
416	                MCDiLepList           = cms.vstring(""),
417
418	                doTauSkimming         = cms.bool(False), # skim on the number of reco taus (no id so far)
419	                numberOfTau4Skim      = cms.int32(1),
420	                tau_cut_pt_skim       = cms.double(5),
421	                tau_algo_skim         = cms.string("selectedPatTaus"),
422
423	                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
424	                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),
425
426	                doJetSkimming         = cms.bool(False), # skim on the number of jets
427	                numberOfJet4Skim      = cms.int32(1),
428	                jet_cut_pt_skim       = cms.double(10),
429	                jet_cut_eta_skim      = cms.double(2.5),
430	                jet_algo_skim         = cms.string("ak5"),
431
432
433	        )
434
435
436
437	# top projections in PF2PAT:
438	getattr(process,"pfNoPileUp"  +postfix).enable = True
439	getattr(process,"pfNoMuon"    +postfix).enable = False
440	getattr(process,"pfNoElectron"+postfix).enable = False
441	getattr(process,"pfNoTau"     +postfix).enable = False
442	getattr(process,"pfNoJet"     +postfix).enable = False
443
444	# verbose flags for the PF2PAT modules
445	getattr(process,"pfNoMuon"+postfix).verbose = False
446
447
448
449
450	if runOnMC == False:
451	    # removing MC matching for standard PAT sequence
452	    # for the PF2PAT+PAT sequence, it is done in the usePF2PAT function
453	    removeMCMatchingPF2PAT( process, '')
454
455
456	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
457	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
458
459
460	# Let it run
461	process.p = cms.Path( process.HBHENoiseFilter*
462	                      process.noscraping +
463	                      process.patElectronIDs *
464	                      process.kt6PFJets*
465	                      process.neutralPFCandidatesProducer*
466	                      process.kt6NeutralPFJets*
467	                      getattr(process,"patPF2PATSequence"+postfix)
468
469	#    second PF2PAT
470	#    + getattr(process,"patPF2PATSequence"+postfix2)
471	)
472	if not postfix=="":
473	    process.p += process.patDefaultSequence
474
475
476	process.p += process.MiniTreeProduction
477
478	process.p += process.MyModule
479
480	# Add PF2PAT output to the created file
481	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
482	process.out.outputCommands = cms.untracked.vstring('drop *',
483	                                                   #'keep *_*_*_PAT',
484	                                                   #'keep edmTriggerResults_*_*_*',
485	                                                   #'keep double_kt6PFJets_rho_*',
486	                                                   #'keep *_hltTriggerSummaryAOD_*_*',
487	                                                   #'keep *_offlineBeamSpot_*_*',
488	                                                   #'keep *_offlinePrimaryVertices_*_*',
489	                                                   #'keep *_muons_*_*',
490	                                                   #'keep *_muonTCMETValueMapProducer_*_*',
491	                                                   #'keep recoTracks_*_*_*',
492	                                                   #'keep GenEventInfoProduct_*_*_*',
493	                                                   #'keep recoGenParticles_*_*_*',
494	                                                   #*patEventContentNoCleaning
495	                                                   'keep TopTreeMTEvent_*_*_*'
496	                                                   )
497
498
499	## ------------------------------------------------------
500	#  In addition you usually want to change the following
501	#  parameters:
502	## ------------------------------------------------------
503	#
504	#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
505	#                                         ##
506	#   process.source.fileNames = [          ##
507	#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
508	#   ]                                     ##  (e.g. 'file:AOD.root')
509	#                                         ##
510
511
512
513	process.maxEvents.input = 1000            ##  (e.g. -1 to run on all events)
514
515
516
517	#                                         ##
518	#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
519	#                                         ##
520	#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
521	#                                         ##
522	#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)
523
524	process.MessageLogger.cerr.FwkReport.reportEvery = 10
525
526	# to relax the muon isolation, uncomment the following:
527	#process.pfIsolatedMuonsPFlow.combinedIsolationCut = cms.double(99999.)
528	#process.PF2PAT.pfMuonSequence.pfIsolatedMuons.combinedIsolationCut = cms.double(99999.)
529
530
531	process.source = cms.Source(
532	        "PoolSource",
533	        noEventSort = cms.untracked.bool(True),
534	        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
535	        fileNames = cms.untracked.vstring(
536
537
538	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/04E39182-EA7D-E011-820E-00151796D58C.root',
539	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/08FF289A-E97D-E011-9734-0015178C6558.root',
540	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/0A97B89F-EC7D-E011-B7E1-0024E876803E.root',
541	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/0E41151C-ED7D-E011-A00D-00151796C040.root',
542	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/0E87D78E-E57D-E011-902D-00266CF20468.root',
543	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/1AF9D2EF-E77D-E011-B319-00151796D7C0.root',
544	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/2493ABBD-E87D-E011-BFC3-00151796D774.root',
545	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/284140DC-E77D-E011-9323-0026B94E287F.root',
546	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/288498E3-EF7D-E011-8C55-00A0D1EEC298.root',
547	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/2CA15EB7-E47D-E011-BDA4-0015178C4BF4.root',
548	        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/2E2514EC-E77D-E011-AC66-0015178C4C7C.root'
549
550
551
552
553
554	        )
555	)
556
557
