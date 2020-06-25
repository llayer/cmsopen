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
25	process.GlobalTag.globaltag = cms.string('GR_R_41_V0::All')
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
184	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) ) 
185	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
186	
187	usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix) 
188	#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )
189	applyPostfix(process,"patJetCorrFactors",postfix).levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
190	applyPostfix(process,"patJetCorrFactors",postfix).rho = cms.InputTag("kt6PFJets","rho")
191	applyPostfix(process,"patJetCorrFactors",postfix).payload = cms.string('AK5PFchs')
192	
193	
194	#to use tau-cleaned jet collection uncomment the following:
195	#getattr(process,"pfNoTau"+postfix).enable = True
196	
197	
198	addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
199	                'AK5', 'JPT',
200	                doJTA        = True,
201	                doBTagging   = True,
202	                jetCorrLabel = ('AK5JPT', cms.vstring(['L2Relative', 'L3Absolute'])),
203	                doType1MET   = False,
204	                doL1Cleaning = False,
205	                doL1Counters = True,                 
206	                genJetCollection = cms.InputTag("ak5GenJets"),
207	                doJetID      = True
208	                )
209	     
210	
211	
212	
213	from PhysicsTools.PatAlgos.tools.metTools import *
214	
215	addTcMET(process, 'TC')
216	process.patMETsTC.addGenMET = False
217	
218	removeMCMatching(process, ['All'])
219	removeIfInSequence(process, 'patJetGenJetMatchAK5JPT', "patDefaultSequence")
220	removeIfInSequence(process, 'patJetPartonMatchAK5JPT', "patDefaultSequence")
221	removeIfInSequence(process, 'patJetPartons',           "patDefaultSequence")
222	removeIfInSequence(process, 'patJetPartonAssociation', "patDefaultSequence")
223	removeIfInSequence(process, 'patJetPartonAssociationAK5JPT',  "patDefaultSequence")
224	removeIfInSequence(process, 'patJetFlavourAssociation',       "patDefaultSequence")
225	removeIfInSequence(process, 'patJetFlavourAssociationAK5JPT', "patDefaultSequence")
226	
227	
228	process.load('RecoJets.Configuration.RecoJets_cff')
229	from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
230	
231	
232	process.kt6PFJets               = kt4PFJets.clone()
233	process.kt6PFJets.rParam        = 0.6     
234	process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
235	process.kt6PFJets.doRhoFastjet  = True
236	process.kt6PFJets.doAreaFastjet = True
237	#process.kt6PFJets.voronoiRfact  = 0.9
238	
239	process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")
240	
241	process.kt6NeutralPFJets = kt4PFJets.clone()
242	process.kt6NeutralPFJets.rParam        = 0.6     
243	process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
244	process.kt6NeutralPFJets.doRhoFastjet  = True
245	process.kt6NeutralPFJets.doAreaFastjet = True
246	process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
247	#process.kt6PFJets.voronoiRfact  = 0.9
248	
249	
250	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
251	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
252	process.jetIDSelector.version = cms.string('PURE09')
253	
254	
255	
256	
257	
258	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
259	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
260	
261	process.patElectrons.addElectronID = cms.bool(True)
262	process.patElectrons.electronIDSources = cms.PSet(
263	    eidLoose= cms.InputTag("eidLoose"),
264	    eidTight= cms.InputTag("eidTight"),
265	    eidRobustTight= cms.InputTag("eidRobustTight"),
266	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
267	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
268	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
269	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
270	    cicLoose         =  cms.InputTag("cicLoose"),
271	    cicMedium        =  cms.InputTag("cicMedium"),
272	    cicTight         =  cms.InputTag("cicTight"),
273	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
274	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
275	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
276	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
277	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
278	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
279	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
280	    cicTightMC         =  cms.InputTag("cicTightMC"),
281	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
282	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
283	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
284	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
285	)
286	
287	
288	process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)
289	
290	process.patElectronsPF2PAT.addElectronID = cms.bool(True)
291	process.patElectronsPF2PAT.electronIDSources = cms.PSet(
292	    eidLoose= cms.InputTag("eidLoose"),
293	    eidTight= cms.InputTag("eidTight"),
294	    eidRobustTight= cms.InputTag("eidRobustTight"),
295	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
296	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
297	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
298	    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
299	    cicLoose         =  cms.InputTag("cicLoose"),
300	    cicMedium        =  cms.InputTag("cicMedium"),
301	    cicTight         =  cms.InputTag("cicTight"),
302	    cicSuperTight    =  cms.InputTag("cicSuperTight"),
303	    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
304	    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
305	    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
306	    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
307	    cicLooseMC         =  cms.InputTag("cicLooseMC"),
308	    cicMediumMC        =  cms.InputTag("cicMediumMC"),
309	    cicTightMC         =  cms.InputTag("cicTightMC"),
310	    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
311	    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
312	    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
313	    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC") 
314	)
315	
316	
317	
318	
319	
320	# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
321	# and add it to path
322	#postfix2 = "PFlow2"
323	#jetAlgo2="AK7"
324	#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)
325	
326	# to use tau-cleaned jet collection uncomment the following:
327	#getattr(process,"pfNoTau"+postfix).enable = True 
328	
329	# to switch default tau to HPS tau uncomment the following:
330	#adaptPFTaus(process,"hpsPFTau",postfix=postfix)
331	
332	
333	
334	
335	
336	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
337	                isAOD               = cms.bool(True), # true if processing AOD data
338	                isData              = cms.bool(True), # true if processing AOD data
339	                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
340	                TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
341	                TriggerInfo         = cms.bool(True), 
342	                saveAllTriggers     = cms.bool(True),#should be True by default !!
343	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
344	                prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5"),
345	                hltJetsMatcher      = cms.string("jetMatchHLTJets"),
346	                doGenParticleCollection = cms.bool(False),
347	                doElectrons         = cms.bool(True),
348	                electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
349	                electron_cut_pt     = cms.double(10),
350	                electron_cut_eta    = cms.double(2.5), 
351	                doPhotons           = cms.bool(False),
352	                photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
353	                photon_cut_pt       = cms.double(10),
354	                photon_cut_eta      = cms.double(2.5), 
355	                doMuons             = cms.bool(True),
356	                muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
357	                muon_cut_pt         = cms.double(10),
358	                muon_cut_eta        = cms.double(2.5), 
359	                doTaus              = cms.bool(True),
360	                tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
361	                #tauProducer         = cms.vstring("selectedPatTaus"),
362	                tau_cut_pt          = cms.double(10),
363	                tau_cut_eta         = cms.double(2.4),      
364	                trackProducer       = cms.InputTag("generalTracks"),
365	                doTracks            = cms.bool(False),
366	                track_cut_pt        = cms.double(0.5),
367	                track_cut_eta       = cms.double(2.4),
368	                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
369	                jet_cut_pt          = cms.double(10),
370	                jet_cut_eta         = cms.double(2.5), 
371	                met_cut_pt          = cms.double(-99),          
372	                saveAllVertex       = cms.bool(True),
373	                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
374	                rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
375	                neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
376	                #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
377	                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("pf")),
378	                                                #cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
379	                                                cms.PSet(jet = cms.untracked.string("selectedPatJets"),       met = cms.untracked.string("patMETs"),       algo =cms.untracked.string("ak5"))
380	                                             )
381	        )
382	
383	process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )
384	
385	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
386	
387	# loads your analyzer
388	process.MyModule = cms.EDAnalyzer('NTupleProducer',
389	                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
390	                saveAllTriggers     = cms.bool(True),# False: see list above
391	                saveTriggersWith    = cms.vstring(""),
392	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
393	                doElectrons         = cms.bool(True),
394	                electron_cut_pt     = cms.double(10),
395	                electron_cut_eta    = cms.double(2.5),
396	                doPhotons           = cms.bool(True),
397	                photon_cut_pt       = cms.double(10),
398	                photon_cut_eta      = cms.double(2.5),
399	                doMuons             = cms.bool(True),
400	                muon_cut_pt         = cms.double(7),
401	                muon_cut_eta        = cms.double(2.5),
402	                doTaus              = cms.bool(True),
403	                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
404	                tau_cut_pt          = cms.double(5),
405	                tau_cut_eta         = cms.double(3.),
406	                doTracks            = cms.bool(False),
407	                track_cut_pt        = cms.double(0.5),
408	                track_cut_eta       = cms.double(2.4),
409	                doJets              = cms.bool(True),
410	                jetAlgos            = cms.vstring("ak5","pf", "tc"),
411	                jet_cut_pt          = cms.double(10),
412	                jet_cut_eta         = cms.double(2.5),
413	                saveAllVertex       = cms.bool(True),
414	                saveMCInfo          = cms.bool(True),
415	                
416	                # Configuration for skimming !
417	                #
418	                # Events will be recorded if one of the following parameters is activated :
419	                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True 
420	                # 
421	                numberOfLept4Skim   = cms.int32(2),# for skims ! Total number of sel lepton,  -1 for no skiming
422	                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
423	                numberOfElec4Skim   = cms.int32(0),# number of sel electron
424	                useMuonIdSkim         = cms.bool(False),
425	                muon_cut_pt_skim      = cms.double(10),
426	                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
427	                useElectronIdSkim     = cms.bool(False),
428	                electron_cut_pt_skim  = cms.double(7),
429	                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
430	                
431	                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
432	                TMEMESkimList         = cms.vint32(),
433	                
434	                doMCDiLepSkimming     = cms.bool(False),
435	                MCDiLepList           = cms.vstring(""),
436	                
437	                doTauSkimming         = cms.bool(False), # skim on the number of reco taus (no id so far)
438	                numberOfTau4Skim      = cms.int32(1),
439	                tau_cut_pt_skim       = cms.double(5),
440	                tau_algo_skim         = cms.string("selectedPatTaus"),
441	                
442	                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
443	                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),
444	                
445	                doJetSkimming         = cms.bool(False), # skim on the number of jets
446	                numberOfJet4Skim      = cms.int32(1),
447	                jet_cut_pt_skim       = cms.double(10), 
448	                jet_cut_eta_skim      = cms.double(2.5),
449	                jet_algo_skim         = cms.string("ak5"),
450	                
451	                
452	        )
453	
454	
455	
456	
457	
458	# top projections in PF2PAT:
459	getattr(process,"pfNoPileUp"  +postfix).enable = True 
460	getattr(process,"pfNoMuon"    +postfix).enable = False
461	getattr(process,"pfNoElectron"+postfix).enable = False
462	getattr(process,"pfNoTau"     +postfix).enable = False
463	getattr(process,"pfNoJet"     +postfix).enable = False 
464	
465	# verbose flags for the PF2PAT modules
466	getattr(process,"pfNoMuon"+postfix).verbose = False
467	
468	
469	
470	
471	if runOnMC == False:
472	    # removing MC matching for standard PAT sequence
473	    # for the PF2PAT+PAT sequence, it is done in the usePF2PAT function
474	    removeMCMatchingPF2PAT( process, '' ) 
475	
476	
477	# Let it run
478	process.p = cms.Path( process.HBHENoiseFilter*
479	                      process.noscraping + 
480	                      process.patElectronIDs *
481	                      process.kt6PFJets*
482	                      getattr(process,"patPF2PATSequence"+postfix) *
483	                      process.neutralPFCandidatesProducer*
484	                      process.kt6NeutralPFJets
485	
486	                      
487	#    second PF2PAT
488	#    + getattr(process,"patPF2PATSequence"+postfix2)
489	)
490	if not postfix=="":
491	    process.p += process.patDefaultSequence
492	    
493	    
494	process.p += process.MiniTreeProduction
495	   
496	process.p += process.MyModule   
497	   
498	# Add PF2PAT output to the created file
499	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
500	process.out.outputCommands = cms.untracked.vstring('drop *',
501	                                                    'keep TopTreeMTEvent_*_*_*'
502	                                                   #'keep *_*_*_PAT',
503	                                                   #'keep edmTriggerResults_*_*_*',  
504	                                                   #'keep double_kt6PFJets_rho_*',
505	                                                   #'keep *_hltTriggerSummaryAOD_*_*',
506	                                                   #'keep *_offlineBeamSpot_*_*',
507	                                                   #'keep *_offlinePrimaryVertices_*_*',
508	                                                   #'keep *_muons_*_*',
509	                                                   #'keep *_muonTCMETValueMapProducer_*_*',
510	                                                   #'keep recoTracks_*_*_*',
511	                                                   #'keep GenEventInfoProduct_*_*_*',
512	                                                   #'keep recoGenParticles_*_*_*',
513	                                                   #*patEventContentNoCleaning 
514	                                                   ) 
515	
516	
517	## ------------------------------------------------------
518	#  In addition you usually want to change the following
519	#  parameters:
520	## ------------------------------------------------------
521	#
522	#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
523	#                                         ##
524	#   process.source.fileNames = [          ##
525	#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
526	#   ]                                     ##  (e.g. 'file:AOD.root')
527	#                                         ##
528	
529	
530	
531	process.maxEvents.input = -1             ##  (e.g. -1 to run on all events)
532	
533	
534	
535	#                                         ##
536	#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
537	#                                         ##
538	#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
539	#                                         ##
540	#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)    
541	
542	process.MessageLogger.cerr.FwkReport.reportEvery = 10
543	
544	# to relax the muon isolation, uncomment the following:
545	#process.pfIsolatedMuonsPFlow.combinedIsolationCut = 99999
546	
547	
548	process.source = cms.Source(
549	        "PoolSource",
550	        noEventSort = cms.untracked.bool(True),
551	        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
552	        fileNames = cms.untracked.vstring(
553	
554	       '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/FE2BB376-C07C-E011-B3A6-001EC9D81A4A.root',
555	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/EC91B253-AE7C-E011-9F96-485B39800BFB.root',
556	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/E89314D1-A37C-E011-8FA7-E0CB4E5536BB.root',
557	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/E87997CC-AD7C-E011-B307-E0CB4E19F999.root',
558	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/DEBE1714-E27C-E011-9E34-E0CB4E55366A.root',
559	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D870C654-C47C-E011-BD47-E0CB4E1A1150.root',
560	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D689ABEB-B87C-E011-962B-E0CB4E4408DF.root',
561	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D065B7F8-C47C-E011-AD82-E0CB4E29C4F5.root',
562	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/BC59235C-C07C-E011-A581-E0CB4EA0A8ED.root',
563	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/AC3A2F69-BD7C-E011-9287-E0CB4E4408EE.root',
564	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/A6EB7E32-CC7C-E011-8572-E0CB4E29C50A.root',
565	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/9CFB03E7-D77C-E011-8DCC-E0CB4E553657.root',
566	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/989F5D58-AE7C-E011-8419-E0CB4E29C4D5.root',
567	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/986255F1-B47C-E011-AEDD-E0CB4E553644.root',
568	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/82BCEC06-B57C-E011-BEE6-0022198F5B1E.root',
569	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/7EB6BBFA-C47C-E011-BCE5-485B39800B86.root',
570	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/78E9C834-C97C-E011-B1CE-002618FDA1C2.root',
571	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/7669575E-C07C-E011-A369-90E6BA442F0F.root',
572	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/706CCFD8-C17C-E011-B98A-E0CB4EA0A8EC.root',
573	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/625C0091-B07C-E011-B248-90E6BA0D09D4.root',
574	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/607DC8A8-F77C-E011-A456-E0CB4E5536EF.root',
575	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/5C67BCF0-B77C-E011-9057-485B39800BC7.root',
576	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/52EAB736-BC7C-E011-8637-90E6BA0D09EA.root',
577	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/4A3D0DE9-D77C-E011-B137-485B39800C16.root',
578	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/409C81F7-D77C-E011-A758-E0CB4E29C4C0.root',
579	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/3ED00CFE-B47C-E011-AF39-90E6BA442F38.root',
580	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/388AAC35-AC7C-E011-9F41-E0CB4E29C4D2.root',
581	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/2E77E8C6-BA7C-E011-B292-E0CB4E29C4EC.root',
582	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/283C0C31-B47C-E011-AB86-E0CB4E19F9A9.root',
583	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/1669D46C-D17C-E011-B59E-E0CB4E19F9BB.root',
584	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/1479C68D-BF7C-E011-A978-00261834B53C.root',
585	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/0013FC4E-C07C-E011-B239-E0CB4E29C4B7.root',
586	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0004/FC4D42FB-D77C-E011-A9AF-E0CB4E29C4D1.root',
587	        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0004/FAEB9F99-B37C-E011-A753-E0CB4E29C4B7.root'
588	
589	
590	
591	        )
592	)
593	
594	
