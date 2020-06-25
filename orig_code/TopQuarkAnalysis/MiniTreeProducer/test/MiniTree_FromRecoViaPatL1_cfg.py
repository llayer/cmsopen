1	# This is an example PAT configuration showing the usage of PAT on minbias data
2	
3	# Starting with a skeleton process which gets imported with the following line
4	from PhysicsTools.PatAlgos.patTemplate_cfg import *
5	
6	from PhysicsTools.PatAlgos.tools.coreTools import *
7	
8	## global tag for data
9	# for DATA 
10	#process.GlobalTag.globaltag = cms.string('GR_R_36X_V12A::All')
11	# for prompt reco
12	#process.GlobalTag.globaltag = cms.string('GR10_P_V7::All')
13	# for MC
14	#process.GlobalTag.globaltag = cms.string('START3X_V26A::All')
15	process.GlobalTag.globaltag = cms.string('START38_V6::All')
16	
17	
18	
19	# PAT Layer 0+1
20	process.load("PhysicsTools.PatAlgos.patSequences_cff")
21	
22	process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
23	
24	process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
25	                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
26	                                           minimumNDOF = cms.uint32(4) ,
27	                                           maxAbsZ = cms.double(24), 
28	                                           maxd0 = cms.double(2) 
29	)
30	
31	process.noscraping = cms.EDFilter("FilterOutScraping",
32	                                  applyfilter = cms.untracked.bool(True),
33	                                  debugOn = cms.untracked.bool(True),
34	                                  numtrack = cms.untracked.uint32(10),
35	                                  thresh = cms.untracked.double(0.20)
36	)    
37	
38	#Noise filter
39	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
40	
41	
42	
43	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
44	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
45	
46	process.patElectrons.addElectronID = cms.bool(True)
47	process.patElectrons.electronIDSources = cms.PSet(
48	    eidLoose= cms.InputTag("eidLoose"),
49	    eidTight= cms.InputTag("eidTight"),
50	    eidRobustTight= cms.InputTag("eidRobustTight"),
51	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
52	    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
53	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
54	    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
55	    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
56	    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
57	    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
58	    simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
59	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
60	    simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
61	    simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
62	    simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
63	    simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),
64	)
65	
66	
67	
68	
69	
70	from PhysicsTools.PatAlgos.tools.jetTools import *
71	from PhysicsTools.PatAlgos.tools.metTools import *
72	from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
73	
74	process.load("RecoJets.Configuration.RecoJPTJets_cff")
75	process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
76	
77	switchJECSet( process, "Spring10")
78	
79	from PhysicsTools.PatAlgos.tools.coreTools import *
80	removeMCMatching(process, ['All'])    
81	
82	from PhysicsTools.PatAlgos.tools.tauTools import *
83	#switchToPFTauHPS(process)
84	addTauCollection(process, tauCollection = cms.InputTag('hpsPFTauProducer'), algoLabel = "hps", typeLabel = "PFTau")
85	
86	
87	addJetCollection35X(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
88	                 'AK5', 'JPT',
89	                 doJTA        = True,
90	                 doBTagging   = True,
91	                 jetCorrLabel = ('AK5','JPT'),
92	                 doType1MET   = False,
93	                 doL1Cleaning = False,
94	                 doL1Counters = True,                 
95	                 genJetCollection = cms.InputTag("ak5GenJets"),
96	                 doJetID      = False
97	                 )
98	     
99	addJetCollection35X(process,cms.InputTag('ak5PFJets'),
100	                 'AK5', 'PF',
101	                 doJTA        = True,
102	                 doBTagging   = True,
103	                 jetCorrLabel = ('AK5','PF'),
104	                 doType1MET   = True,
105	                 doL1Cleaning = True,                 
106	                 doL1Counters = False,
107	                 genJetCollection=cms.InputTag("ak5GenJets"),
108	                 doJetID      = False
109	                 )
110	
111	process.patJetCorrFactorsAK5JPT.corrSample  = "Spring10"
112	
113	# run b-tagging sequences
114	from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
115	run36xOn35xInput( process )
116	
117	
118	
119	
120	addTcMET(process, 'TC')
121	addPfMET(process, 'PF')
122	
123	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
124	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
125	process.jetIDSelector.version = cms.string('PURE09')
126	
127	process.load('RecoJets.Configuration.RecoGenJets_cff')
128	process.load('RecoJets.Configuration.GenJetParticles_cff')
129	
130	
131	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
132	                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
133	                TriggerMenu         = cms.InputTag("TriggerResults::REDIGI"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
134	                TriggerInfo         = cms.bool(True), 
135	                saveAllTriggers     = cms.bool(False),#should be False by default !!
136	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
137	                EventType           = cms.InputTag("Zjets"), 
138	                SampleName          = cms.string('top'),
139	                SampleCrossSection  = cms.double(9999.),   
140	                doGenParticleCollection = cms.bool(True),
141	                doElectrons         = cms.bool(True),
142	                electronProducer    = cms.InputTag("selectedPatElectrons"),
143	                electron_cut_pt     = cms.double(10),
144	                electron_cut_eta    = cms.double(2.5), 
145	                doMuons             = cms.bool(True),
146	                muonProducer        = cms.InputTag("selectedPatMuons"),
147	                muon_cut_pt         = cms.double(10),
148	                muon_cut_eta        = cms.double(2.5), 
149	                doTaus              = cms.bool(True),
150	                tauProducer         = cms.vstring("selectedPatTaus","selectedPatTausHpsPFTau"),
151	                tau_cut_pt          = cms.double(10),
152	                tau_cut_eta         = cms.double(2.4),      
153	                trackProducer       = cms.InputTag("generalTracks"),
154	                doTracks            = cms.bool(False),
155	                track_cut_pt        = cms.double(0.5),
156	                track_cut_eta       = cms.double(2.4),
157	                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
158	                jet_cut_pt          = cms.double(10),
159	                jet_cut_eta         = cms.double(2.5), 
160	                met_cut_pt          = cms.double(-99),          
161	                saveAllVertex       = cms.bool(True),
162	                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
163	                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJets"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("ak5")),
164	#                                               cms.PSet(jet = cms.untracked.string("selectedPatJetsSC5Calo"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("sc5")),
165	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"), algo =cms.untracked.string("tc")),
166	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5PF"), met = cms.untracked.string("patMETsPF"), algo =cms.untracked.string("pf"))
167	                                                )
168	        )
169	
170	
171	
172	# Add the files 
173	#readFiles = cms.untracked.vstring()
174	#secFiles = cms.untracked.vstring()
175	#
176	#readFiles.extend( [
177	#       'file:/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/462/7C814F10-CB3C-DF11-B1F1-0030487C778E.root'
178	#        ] );
179	#process.source.fileNames = readFiles
180	
181	process.load("InputFiles_cfi")
182	
183	# let it run
184	
185	#print
186	#print "============== Warning =============="
187	#print "technical trigger filter:    DISABLED"
188	#print "physics declare bit filter:  DISABLED"
189	#print "primary vertex filter:       DISABLED"
190	
191	#process.dump=cms.EDAnalyzer('EventContentAnalyzer')
192	
193	process.p = cms.Path(#process.hltHighLevel +
194	                     #process.primaryVertexFilter *
195	                     #process.HBHENoiseFilter*
196	                     #process.noscraping +  
197	                      #process.genParticlesForJets*
198	                      #process.ak5GenJets*
199	                      process.recoJPTJets*
200	                      process.eIdSequence*
201	                      process.simpleEleIdSequence*
202	                      process.produceAndDiscriminateHPSPFTaus *
203	                      process.patElectronIDs *
204	                      process.patDefaultSequence*
205	                      process.patTriggerSequence *
206	                      process.MiniTreeProduction
207	    )
208	
209	# rename output file
210	process.out.fileName = cms.untracked.string('MiniTree.root')
211	
212	# reduce verbosity
213	process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(500)
214	
215	# process 1 event
216	process.maxEvents.input = -1
217	process.options.wantSummary = True
218	
219	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
220	from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
221	from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
222	process.out.outputCommands = patEventContentNoCleaning
223	process.out.outputCommands += patExtraAodEventContent
224	process.out.outputCommands += patTriggerEventContent
225	process.out.outputCommands += [
226	         'drop *',
227	         'keep TopTreeMTEvent_*_*_*'
228	        ]
229	

