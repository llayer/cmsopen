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
25	process.GlobalTag.globaltag = cms.string('START41_V0::All')
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
62	process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesDA_cfi")
63	process.offlinePrimaryVertices = process.offlinePrimaryVerticesDA.clone()
64	process.offlinePrimaryVertices.useBeamConstraint = cms.bool(True)
65	process.offlinePrimaryVertices.TkClusParameters.TkDAClusParameters.Tmin = cms.double(4.)
66	process.offlinePrimaryVertices.TkClusParameters.TkDAClusParameters.vertexSize = cms.double(0.01)
67	
68	from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
69	
70	process.goodOfflinePrimaryVertices = cms.EDFilter(
71	    "PrimaryVertexObjectFilter",
72	    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
73	    src=cms.InputTag('offlinePrimaryVertices::PAT')
74	    )
75	    
76	    
77	process.noscraping = cms.EDFilter("FilterOutScraping",
78	                                  applyfilter = cms.untracked.bool(True),
79	                                  debugOn = cms.untracked.bool(True),
80	                                  numtrack = cms.untracked.uint32(10),
81	                                  thresh = cms.untracked.double(0.20)
82	)    
83	
84	#Noise filter
85	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
86	
87	
88	
89	
90	
91	
92	
93	
94	
95	
96	# the source is already defined in patTemplate_cfg.
97	# overriding source and various other things
98	#process.load("CommonTools.ParticleFlow.Sources.source_ZtoEles_DBS_312_cfi")
99	#process.source = cms.Source("PoolSource", 
100	#     fileNames = cms.untracked.vstring('file:myAOD.root')
101	#)
102	
103	
104	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
105	process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
106	
107	# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
108	runOnMC = True
109	
110	## Maximal Number of Events
111	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
112	process.out.fileName = cms.untracked.string('patTuple.root')
113	
114	# load the PAT config
115	process.load("PhysicsTools.PatAlgos.patSequences_cff")
116	
117	
118	# Configure PAT to use PF2PAT instead of AOD sources
119	# this function will modify the PAT sequences. It is currently 
120	# not possible to run PF2PAT+PAT and standart PAT at the same time
121	from PhysicsTools.PatAlgos.tools.pfTools import *
122	
123	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
124	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
125	#process.isoValMuonWithCharged.deltaR = 0.3
126	#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
127	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
128	 
129	 
130	# An empty postfix means that only PF2PAT is run,
131	# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
132	# collections have standard names + postfix (e.g. patElectronPFlow)  
133	
134	
135	postfix = "PF2PAT"  # to have only PF2PAT
136	jetAlgo="AK5"
137	
138	usePFnoPU = True # before any top projection
139	
140	
141	# levels to be accessible from the jets
142	# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
143	# and remain uncorrected, if none of these levels is enabled here
144	useL1FastJet    = True  # needs useL1Offset being off, error otherwise
145	useL1Offset     = False # needs useL1FastJet being off, error otherwise
146	useL2Relative   = True
147	useL3Absolute   = True
148	# useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
149	useL5Flavor     = False
150	useL7Parton     = False
151	
152	# JEC set
153	jecSetBase = jetAlgo
154	jecSetPF = jecSetBase + 'PF'
155	if usePFnoPU:
156	  jecSetPF += 'chs'
157	
158	# JEC levels
159	if useL1FastJet and useL1Offset:
160	  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )
161	jecLevels = []
162	if useL1FastJet:
163	  jecLevels.append( 'L1FastJet' )
164	if useL1Offset:
165	  jecLevels.append( 'L1Offset' )
166	if useL2Relative:
167	  jecLevels.append( 'L2Relative' )
168	if useL3Absolute:
169	  jecLevels.append( 'L3Absolute' )
170	# if useL2L3Residual and not runOnMC:
171	#   jecLevelsPF.append( 'L2L3Residual' )
172	if useL5Flavor:
173	  jecLevels.append( 'L5Flavor' )
174	if useL7Parton:
175	  jecLevels.append( 'L7Parton' )
176	
177	
178	
179	
180	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) ) 
181	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
182	
183	usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix) 
184	adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
185	applyPostfix(process,"patJetCorrFactors",postfix).levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
186	applyPostfix(process,"patJetCorrFactors",postfix).rho = cms.InputTag("kt6PFJets","rho")
187	applyPostfix(process,"patJetCorrFactors",postfix).payload = cms.string('AK5PFchs')
188	
189	
190	# to use tau-cleaned jet collection uncomment the following:
191	#getattr(process,"pfNoTau"+postfix).enable = True
192	
193	#changes for iso and deltaR
194	#muons
195	applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
196	applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
197	applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
198	applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
199	#electrons
200	applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
201	applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
202	applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
203	applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
204	# jec should be ok
205	# by default: 'L1Offset','L2Relative','L3Absolute'
206	#  and primaryVertices = cms.InputTag("offlinePrimaryVertices")
207	
208	addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
209	                 'AK5', 'JPT',
210	                 doJTA        = True,
211	                 doBTagging   = True,
212	#modifdg (L1Offset to substract PU effect)       
213	#                 jetCorrLabel = ('AK5JPT', cms.vstring([ 'L2Relative', 'L3Absolute'])),
214	                 jetCorrLabel = ('AK5JPT', cms.vstring(['L1Offset', 'L2Relative', 'L3Absolute'])),
215	                 doType1MET   = False,
216	                 doL1Cleaning = False,
217	                 doL1Counters = True,                 
218	                 genJetCollection = cms.InputTag("ak5GenJets"),
219	                 doJetID      = True
220	                 )
221	     
222	from PhysicsTools.PatAlgos.tools.metTools import *
223	
224	addTcMET(process, 'TC')
225	
226	process.pfPileUpPF2PAT.Enable = True
227	#process.pfPileUpPF2PAT.checkClosestZVertex = cms.bool(False)
228	process.pfPileUpPF2PAT.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
229	process.pfJetsPF2PAT.doAreaFastjet = True
230	process.pfJetsPF2PAT.doRhoFastjet = False
231	
232	process.load('RecoJets.Configuration.RecoJets_cff')
233	from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
234	
235	process.kt6PFJets               = kt4PFJets.clone()
236	process.kt6PFJets.rParam        = 0.6     
237	process.kt6PFJets.src           = cms.InputTag('pfNoElectron'+postfix)
238	process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
239	process.kt6PFJets.doRhoFastjet  = True
240	process.kt6PFJets.doAreaFastjet = True
241	#process.kt6PFJets.voronoiRfact  = 0.9
242	
243	#process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJets", "rho")
244	getattr(process,"patJetCorrFactors"+postfix).rho = cms.InputTag("kt6PFJets", "rho")
245	
246	process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")
247	
248	process.kt6NeutralPFJets = kt4PFJets.clone()
249	process.kt6NeutralPFJets.rParam        = 0.6     
250	process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
251	process.kt6NeutralPFJets.doRhoFastjet  = True
252	process.kt6NeutralPFJets.doAreaFastjet = True
253	process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
254	#process.kt6PFJets.voronoiRfact  = 0.9
255	
256	
257	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
258	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
259	process.jetIDSelector.version = cms.string('PURE09')
260	
261	
262	
263	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
264	#process.load("RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi")
265	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
266	#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi")
267	
268	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
269	
270	process.patElectrons.addElectronID = cms.bool(True)
271	process.patElectrons.electronIDSources = cms.PSet(
272	    eidLoose= cms.InputTag("eidLoose"),
273	    eidTight= cms.InputTag("eidTight"),
274	    eidRobustTight= cms.InputTag("eidRobustTight"),
275	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
276	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
277	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
278	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
279	    cicLoose         =  cms.InputTag("cicLoose"),
280	    cicMedium        =  cms.InputTag("cicMedium"),
281	    cicTight         =  cms.InputTag("cicTight"),
282	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
283	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
284	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
285	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
286	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
287	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
288	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
289	    cicTightMC         =  cms.InputTag("cicTightMC"),
290	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
291	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
292	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
293	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
294	)
295	
296	process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)
297	
298	process.patElectronsPF2PAT.addElectronID = cms.bool(True)
299	process.patElectronsPF2PAT.electronIDSources = cms.PSet(
300	    eidLoose= cms.InputTag("eidLoose"),
301	    eidTight= cms.InputTag("eidTight"),
302	    eidRobustTight= cms.InputTag("eidRobustTight"),
303	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
304	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
305	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
306	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
307	    cicLoose         =  cms.InputTag("cicLoose"),
308	    cicMedium        =  cms.InputTag("cicMedium"),
309	    cicTight         =  cms.InputTag("cicTight"),
310	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
311	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
312	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
313	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
314	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
315	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
316	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
317	    cicTightMC         =  cms.InputTag("cicTightMC"),
318	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
319	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
320	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
321	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
322	)
323	
324	
325	
326	
327	
328	# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
329	# and add it to path
330	#postfix2 = "PFlow2"
331	#jetAlgo2="AK7"
332	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)
333	
334	# to use tau-cleaned jet collection uncomment the following:
335	#getattr(process,"pfNoTau"+postfix).enable = True 
336	
337	# to switch default tau to HPS tau uncomment the following:
338	#adaptPFTaus(process,"hpsPFTau",postfix=postfix)
339	
340	
341	
342	
343	
344	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
345	        isAOD               = cms.bool(True), # true if processing AOD data
346	        isData              = cms.bool(False), # true if processing AOD data
347	        verbose         = cms.int32(0), #0: nothing - >1 gradually more information
348	            TriggerMenu         = cms.InputTag("TriggerResults"),    # or TriggerResults::REDIGI311X 
349	        TriggerInfo         = cms.bool(True), 
350	                saveAllTriggers     = cms.bool(True),#should be True by default !!
351	        triggerList     = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
352	        prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2"),
353	        hltJetsMatcher      = cms.string(""),
354	        doGenParticleCollection = cms.bool(True),
355	        doElectrons     = cms.bool(True),
356	        electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
357	        electron_cut_pt     = cms.double(10),
358	        electron_cut_eta    = cms.double(2.5), 
359	        doPhotons       = cms.bool(False),
360	        photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
361	        photon_cut_pt       = cms.double(10),
362	        photon_cut_eta      = cms.double(2.5), 
363	        doMuons         = cms.bool(True),
364	        muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
365	        muon_cut_pt         = cms.double(10),
366	        muon_cut_eta        = cms.double(2.5), 
367	        doTaus          = cms.bool(True),
368	        tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
369	        #tauProducer         = cms.vstring("selectedPatTaushpsPFTauPF2PAT"),
370	        tau_cut_pt      = cms.double(10),
371	        tau_cut_eta     = cms.double(2.4),      
372	        trackProducer       = cms.InputTag("generalTracks"),
373	        doTracks            = cms.bool(False),
374	                track_cut_pt        = cms.double(0.5),
375	                track_cut_eta       = cms.double(2.4),
376	        beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
377	        jet_cut_pt      = cms.double(10),
378	        jet_cut_eta     = cms.double(2.5), 
379	        met_cut_pt          = cms.double(-99),      
380	        saveAllVertex       = cms.bool(True),
381	        vertexProducer      = cms.InputTag("offlinePrimaryVertices::PAT"),
382	                rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
383	        #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
384	            neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
385	            jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("pf")),
386	                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
387	                                        cms.PSet(jet = cms.untracked.string("selectedPatJets"),       met = cms.untracked.string("patMETs"),       algo =cms.untracked.string("ak5"))
388	                                     )
389	    )
390	
391	
392	process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )
393	
394	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
395	
396	# loads your analyzer
397	process.MyModule = cms.EDAnalyzer('NTupleProducer',
398	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
399	                saveAllTriggers     = cms.bool(True),# False: see list above
400	                saveTriggersWith    = cms.vstring(""),
401	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
402	                doElectrons         = cms.bool(True),
403	                electron_cut_pt     = cms.double(7),
404	                electron_cut_eta    = cms.double(2.5),
405	                doPhotons           = cms.bool(True),
406	                photon_cut_pt       = cms.double(10),
407	                photon_cut_eta      = cms.double(2.5),
408	                doMuons             = cms.bool(True),
409	                muon_cut_pt         = cms.double(10),
410	                muon_cut_eta        = cms.double(2.5),
411	                doTaus              = cms.bool(True),
412	                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
413	                tau_cut_pt          = cms.double(5),
414	                tau_cut_eta         = cms.double(3.),
415	                doTracks            = cms.bool(False),
416	                track_cut_pt        = cms.double(0.5),
417	                track_cut_eta       = cms.double(2.4),
418	                doJets              = cms.bool(True),
419	                jetAlgos            = cms.vstring("ak5","pf", "tc"),
420	                jet_cut_pt          = cms.double(10),
421	                jet_cut_eta         = cms.double(2.5),
422	                saveAllVertex       = cms.bool(True),
423	                saveMCInfo          = cms.bool(True),
424	                
425	                # Configuration for skimming !
426	                #
427	                # Events will be recorded if one of the following parameters is activated :
428	                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True 
429	                # 
430	                numberOfLept4Skim   = cms.int32(2),# for skims ! Total number of sel lepton,  -1 for no skiming
431	                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
432	                numberOfElec4Skim   = cms.int32(0),# number of sel electron
433	                useMuonIdSkim         = cms.bool(False),
434	                muon_cut_pt_skim      = cms.double(10),
435	                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
436	                useElectronIdSkim     = cms.bool(False),
437	                electron_cut_pt_skim  = cms.double(7),
438	                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
439	                
440	                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
441	                TMEMESkimList         = cms.vint32(),
442	                
443	                doMCDiLepSkimming     = cms.bool(False),
444	                MCDiLepList           = cms.vstring(""),
445	                
446	                doTauSkimming         = cms.bool(False), # skim on the number of reco taus (no id so far)
447	                numberOfTau4Skim      = cms.int32(1),
448	                tau_cut_pt_skim       = cms.double(5),
449	                tau_algo_skim         = cms.string("selectedPatTaus"),
450	                
451	                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
452	                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),
453	                
454	                doJetSkimming         = cms.bool(False), # skim on the number of jets
455	                numberOfJet4Skim      = cms.int32(1),
456	                jet_cut_pt_skim       = cms.double(10), 
457	                jet_cut_eta_skim      = cms.double(2.5),
458	                jet_algo_skim         = cms.string("ak5"),
459	                
460	                
461	        )
462	
463	
464	
465	
466	# top projections in PF2PAT:
467	getattr(process,"pfNoPileUp"  +postfix).enable = True
468	getattr(process,"pfNoMuon"    +postfix).enable = False 
469	getattr(process,"pfNoElectron"+postfix).enable = False
470	getattr(process,"pfNoTau"     +postfix).enable = False
471	getattr(process,"pfNoJet"     +postfix).enable = False
472	
473	# verbose flags for the PF2PAT modules
474	getattr(process,"pfNoMuon"+postfix).verbose = False
475	
476	
477	
478	
479	if runOnMC == False:
480	    # removing MC matching for standard PAT sequence
481	    # for the PF2PAT+PAT sequence, it is done in the usePF2PAT function
482	    removeMCMatchingPF2PAT( process, '')    
483	
484	
485	#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
486	#process.isoValMuonWithCharged.deltaR = cms.double(0.3)
487	
488	getattr(process,"patPF2PATSequence"+postfix).replace(
489	    getattr(process,"pfNoElectron"+postfix),
490	    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJets )
491	
492	# Let it run
493	process.p = cms.Path( process.offlinePrimaryVertices*
494	                      process.goodOfflinePrimaryVertices*
495	                      process.HBHENoiseFilter*
496	                      process.noscraping + 
497	                      process.patElectronIDs *
498	                      getattr(process,"patPF2PATSequence"+postfix)*
499	#                      process.kt6PFJets*
500	#                      getattr(process,"patPF2PATSequence"+postfix)*
501	              process.neutralPFCandidatesProducer*
502	              process.kt6NeutralPFJets
503	              
504	#    second PF2PAT
505	#    + getattr(process,"patPF2PATSequence"+postfix2)
506	)
507	if not postfix=="":
508	    process.p += process.patDefaultSequence
509	    
510	    
511	process.p += process.MiniTreeProduction
512	   
513	process.p += process.MyModule   
514	   
515	# Add PF2PAT output to the created file
516	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
517	process.out.outputCommands = cms.untracked.vstring('drop *',
518	                               #'keep *_*_*_PAT',
519	                           #'keep edmTriggerResults_*_*_*',
520	                           #'keep double_kt6PFJets_rho_*',
521	                           #'keep *_hltTriggerSummaryAOD_*_*',
522	                           #'keep *_offlineBeamSpot_*_*',
523	                           #'keep *_offlinePrimaryVertices_*_PAT',
524	                           #'keep *_muons_*_*',
525	                           #'keep *_muonTCMETValueMapProducer_*_*',
526	                           #'keep recoTracks_*_*_*',
527	                           #'keep GenEventInfoProduct_*_*_*',
528	                           #'keep recoGenParticles_*_*_*',
529	                                                   #*patEventContentNoCleaning 
530	                           'keep TopTreeMTEvent_*_*_*'
531	                           ) 
532	
533	
534	## ------------------------------------------------------
535	#  In addition you usually want to change the following
536	#  parameters:
537	## ------------------------------------------------------
538	#
539	#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
540	#                                         ##
541	#   process.source.fileNames = [          ##
542	#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
543	#   ]                                     ##  (e.g. 'file:AOD.root')
544	#                                         ##
545	
546	
547	
548	process.maxEvents.input = 100            ##  (e.g. -1 to run on all events)
549	
550	
551	
552	#                                         ##
553	#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
554	#                                         ##
555	#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
556	#                                         ##
557	#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)    
558	
559	process.MessageLogger.cerr.FwkReport.reportEvery = 10
560	
561	# to relax the muon isolation, uncomment the following:
562	#process.pfIsolatedMuonsPFlow.combinedIsolationCut = cms.double(99999.)
563	#process.PF2PAT.pfMuonSequence.pfIsolatedMuons.combinedIsolationCut = cms.double(99999.)
564	
565	
566	process.source = cms.Source(
567	    "PoolSource",
568	    noEventSort = cms.untracked.bool(True),
569	    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
570	    fileNames = cms.untracked.vstring(
571	#         'file:/opt/sbg/data/data1/cms/gele/PAT/DATA/TTbarTest/0E00730B-164E-E011-BA88-E0CB4E1A117F.root'
572	         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/TTbarAODSIM/TTbarRef_AODSIM_1.root',
573	         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/TTbarAODSIM/TTbarRef_AODSIM_2.root'
574	#         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/ZjetsAODSIM/ZjetsRef_AODSIM_1.root',
575	#         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/ZjetsAODSIM/ZjetsRef_AODSIM_2.root',
576	#         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/ZjetsAODSIM/ZjetsRef_AODSIM_3.root',
577	#         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/ZjetsAODSIM/ZjetsRef_AODSIM_4.root',
578	#         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/ZjetsAODSIM/ZjetsRef_AODSIM_5.root',
579	#         'file:/opt/sbg/cms/ui3_data3/gele/PAT/MCData/Ref/ZjetsAODSIM/ZjetsRef_AODSIM_6.root'
580	    )
581	)
