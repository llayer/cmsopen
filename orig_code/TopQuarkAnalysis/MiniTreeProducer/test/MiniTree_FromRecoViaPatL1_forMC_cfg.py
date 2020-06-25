1	# This is an example PAT configuration showing the usage of PAT on minbias data
2	
3	# Starting with a skeleton process which gets imported with the following line
4	from PhysicsTools.PatAlgos.patTemplate_cfg import *
5	
6	from PhysicsTools.PatAlgos.tools.coreTools import *
7	
8	## global tag for data
9	process.GlobalTag.globaltag = cms.string('START38_V13::All')
10	
11	
12	
13	# PAT Layer 0+1
14	process.load("PhysicsTools.PatAlgos.patSequences_cff")
15	
16	process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
17	
18	process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
19	                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
20	                                           minimumNDOF = cms.uint32(4) ,
21	                                           maxAbsZ = cms.double(24), 
22	                                           maxd0 = cms.double(2) 
23	)
24	
25	process.noscraping = cms.EDFilter("FilterOutScraping",
26	                                  applyfilter = cms.untracked.bool(True),
27	                                  debugOn = cms.untracked.bool(True),
28	                                  numtrack = cms.untracked.uint32(10),
29	                                  thresh = cms.untracked.double(0.20)
30	)    
31	
32	#Noise filter
33	process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
34	
35	
36	
37	process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
38	process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
39	
40	process.patElectrons.addElectronID = cms.bool(True)
41	process.patElectrons.electronIDSources = cms.PSet(
42	    eidLoose= cms.InputTag("eidLoose"),
43	    eidTight= cms.InputTag("eidTight"),
44	    eidRobustTight= cms.InputTag("eidRobustTight"),
45	    eidRobustLoose= cms.InputTag("eidRobustLoose"),
46	    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
47	    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
48	    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
49	    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
50	    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
51	    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
52	    simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
53	    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
54	    simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
55	    simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
56	    simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
57	    simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),
58	)
59	
60	
61	
62	
63	
64	from PhysicsTools.PatAlgos.tools.jetTools import *
65	from PhysicsTools.PatAlgos.tools.metTools import *
66	from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
67	
68	#process.load("RecoJets.Configuration.RecoJPTJets_cff")
69	#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
70	
71	switchJECSet( process, "Spring10")
72	
73	from PhysicsTools.PatAlgos.tools.coreTools import *
74	#removeMCMatching(process, ['All'])    
75	
76	from PhysicsTools.PatAlgos.tools.tauTools import *
77	#switchToPFTauHPS(process)
78	addTauCollection(process, tauCollection = cms.InputTag('hpsPFTauProducer'), algoLabel = "hps", typeLabel = "PFTau")
79	
80	process.patJets.addTagInfos     = False #False for AOD
81	
82	addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
83	                 'AK5', 'JPT',
84	                 doJTA        = True,
85	                 doBTagging   = True,
86	                 jetCorrLabel = ('AK5','JPT'),
87	                 doType1MET   = False,
88	                 doL1Cleaning = False,
89	                 doL1Counters = True,                 
90	                 genJetCollection = cms.InputTag("ak5GenJets"),
91	                 doJetID      = True
92	                 )
93	     
94	addJetCollection(process,cms.InputTag('ak5PFJets'),
95	                 'AK5', 'PF',
96	                 doJTA        = True,
97	                 doBTagging   = True,
98	                 jetCorrLabel = ('AK5','PF'),
99	                 doType1MET   = True,
100	                 doL1Cleaning = True,                 
101	                 doL1Counters = False,
102	                 genJetCollection=cms.InputTag("ak5GenJets"),
103	                 doJetID      = True
104	                 )
105	
106	process.patJetCorrFactorsAK5JPT.corrSample  = "Spring10"
107	process.ak5CaloResidual.useCondDB = True
108	process.ak5PFResidual.useCondDB   = True
109	process.ak5JPTResidual.useCondDB  = True
110	
111	
112	
113	
114	
115	
116	
117	addTcMET(process, 'TC')
118	addPfMET(process, 'PF')
119	
120	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
121	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
122	process.jetIDSelector.version = cms.string('PURE09')
123	
124	process.load('RecoJets.Configuration.RecoGenJets_cff')
125	process.load('RecoJets.Configuration.GenJetParticles_cff')
126	
127	
128	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
129	                isAOD               = cms.bool(True), # true if processing AOD data
130	                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
131	                TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
132	                TriggerInfo         = cms.bool(True), 
133	                saveAllTriggers     = cms.bool(True),#should be True by default !!
134	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
135	                EventType           = cms.InputTag("Zjets"), 
136	                SampleName          = cms.string('top'),
137	                SampleCrossSection  = cms.double(9999.),   
138	                doGenParticleCollection = cms.bool(True),
139	                doElectrons         = cms.bool(True),
140	                electronProducer    = cms.InputTag("selectedPatElectrons"),
141	                electron_cut_pt     = cms.double(10),
142	                electron_cut_eta    = cms.double(2.5), 
143	                doMuons             = cms.bool(True),
144	                muonProducer        = cms.InputTag("selectedPatMuons"),
145	                muon_cut_pt         = cms.double(10),
146	                muon_cut_eta        = cms.double(2.5), 
147	                doPhotons           = cms.bool(True),
148	                photonProducer      = cms.InputTag("selectedPatPhotons"),
149	                photon_cut_pt       = cms.double(10),
150	                photon_cut_eta      = cms.double(2.5), 
151	                doTaus              = cms.bool(True),
152	                tauProducer         = cms.vstring("selectedPatTaus","selectedPatTausHpsPFTau"),
153	                tau_cut_pt          = cms.double(10),
154	                tau_cut_eta         = cms.double(2.4),      
155	                trackProducer       = cms.InputTag("generalTracks"),
156	                doTracks            = cms.bool(False),
157	                track_cut_pt        = cms.double(0.5),
158	                track_cut_eta       = cms.double(2.4),
159	                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
160	                jet_cut_pt          = cms.double(10),
161	                jet_cut_eta         = cms.double(2.5), 
162	                met_cut_pt          = cms.double(-99),          
163	                saveAllVertex       = cms.bool(True),
164	                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
165	                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJets"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("ak5")),
166	#                                               cms.PSet(jet = cms.untracked.string("selectedPatJetsSC5Calo"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("sc5")),
167	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"), algo =cms.untracked.string("tc")),
168	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5PF"), met = cms.untracked.string("patMETsPF"), algo =cms.untracked.string("pf"))
169	                                                )
170	        )
171	
172	
173	
174	# Add the files 
175	#readFiles = cms.untracked.vstring()
176	#secFiles = cms.untracked.vstring()
177	#
178	#readFiles.extend( [
179	#       'file:/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/462/7C814F10-CB3C-DF11-B1F1-0030487C778E.root'
180	#        ] );
181	#process.source.fileNames = readFiles
182	
183	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))
184	
185	process.source = cms.Source(
186	        "PoolSource",
187	        noEventSort = cms.untracked.bool(True),
188	        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
189	        fileNames = cms.untracked.vstring(
190	               #"file:1A85A092-63D9-DF11-97CF-0026B9278603.root"
191	                'file:/opt/sbg/cms/ui4_data1/lebihan/TTJets_TuneD6T_7TeV-madgraph-tauola_START38_V12-v1_gen-sim-reco.root'
192	        )
193	)
194	
195	
196	
197	
198	
199	
200	# let it run
201	
202	#print
203	#print "============== Warning =============="
204	#print "technical trigger filter:    DISABLED"
205	#print "physics declare bit filter:  DISABLED"
206	#print "primary vertex filter:       DISABLED"
207	
208	#process.dump=cms.EDAnalyzer('EventContentAnalyzer')
209	
210	process.p = cms.Path(#process.hltHighLevel +
211	                      #process.primaryVertexFilter *
212	                      #process.HBHENoiseFilter*
213	                      #process.noscraping +  
214	                      #process.genParticlesForJets*
215	                      #process.ak5GenJets*
216	                      #process.recoJPTJets*
217	                      process.eIdSequence*
218	                      process.simpleEleIdSequence*
219	                      process.produceAndDiscriminateHPSPFTaus *
220	                      process.patElectronIDs *
221	                      process.patDefaultSequence*
222	                      #process.patTriggerSequence *
223	                      process.MiniTreeProduction
224	    )
225	
226	# rename output file
227	process.out.fileName = cms.untracked.string('MiniTree.root')
228	
229	# reduce verbosity
230	process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(500)
231	
232	# process 1 event
233	process.maxEvents.input = 2000
234	process.options.wantSummary = True
235	
236	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
237	from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
238	from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
239	process.out.outputCommands = patEventContentNoCleaning
240	process.out.outputCommands += patExtraAodEventContent
241	process.out.outputCommands += patTriggerEventContent
242	process.out.outputCommands += [
243	         'drop *',
244	         'keep TopTreeMTEvent_*_*_*'
245	        ]
246	
