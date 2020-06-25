1	# This is an example PAT configuration showing the usage of PAT on minbias data
2	
3	# Starting with a skeleton process which gets imported with the following line
4	from PhysicsTools.PatAlgos.patTemplate_cfg import *
5	
6	from PhysicsTools.PatAlgos.tools.coreTools import *
7	
8	## global tag for data
9	process.GlobalTag.globaltag = cms.string('START38_V14::All')
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
71	
72	from PhysicsTools.PatAlgos.tools.coreTools import *
73	#removeMCMatching(process, ['All'])    
74	
75	from PhysicsTools.PatAlgos.tools.tauTools import *
76	#switchToPFTauHPS(process)
77	addTauCollection(process, tauCollection = cms.InputTag('hpsPFTauProducer'), algoLabel = "hps", typeLabel = "PFTau")
78	
79	process.patJets.addTagInfos     = False #False for AOD
80	
81	#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
82	#switchJECSet( process, "Spring10")
83	
84	
85	#process.ak5CaloL2Relative.useCondDB = False
86	#process.ak5CaloL3Absolute.useCondDB = False
87	#process.ak5CaloResidual.useCondDB = False
88	
89	#process.ak5PFL2Relative.useCondDB = False
90	#process.ak5PFL3Absolute.useCondDB = False
91	#process.ak5PFResidual.useCondDB = False
92	
93	#process.ak5JPTL2Relative.useCondDB = False
94	#process.ak5JPTL3Absolute.useCondDB = False
95	#process.ak5JPTResidual.useCondDB = False
96	
97	
98	
99	addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
100	                 'AK5', 'JPT',
101	                 doJTA        = True,
102	                 doBTagging   = True,
103	                 jetCorrLabel = ('AK5JPT', cms.vstring(['L2Relative', 'L3Absolute'])),
104	                 doType1MET   = False,
105	                 doL1Cleaning = False,
106	                 doL1Counters = True,                 
107	                 genJetCollection = cms.InputTag("ak5GenJets"),
108	                 doJetID      = True
109	                 )
110	     
111	addJetCollection(process,cms.InputTag('ak5PFJets'),
112	                 'AK5', 'PF',
113	                 doJTA        = True,
114	                 doBTagging   = True,
115	                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute'])),
116	                 doType1MET   = True,
117	                 doL1Cleaning = True,                 
118	                 doL1Counters = False,
119	                 genJetCollection=cms.InputTag("ak5GenJets"),
120	                 doJetID      = True
121	                 )
122	
123	
124	
125	
126	
127	
128	addTcMET(process, 'TC')
129	addPfMET(process, 'PF')
130	
131	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
132	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
133	process.jetIDSelector.version = cms.string('PURE09')
134	
135	process.load('RecoJets.Configuration.RecoGenJets_cff')
136	process.load('RecoJets.Configuration.GenJetParticles_cff')
137	
138	
139	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
140	                isAOD               = cms.bool(True), # true if processing AOD data
141	                isData              = cms.bool(False), # true if processing AOD data
142	                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
143	                TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
144	                TriggerInfo         = cms.bool(True), 
145	                saveAllTriggers     = cms.bool(True),#should be True by default !!
146	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
147	                EventType           = cms.InputTag("Zjets"), 
148	                SampleName          = cms.string('top'),
149	                SampleCrossSection  = cms.double(9999.),   
150	                doGenParticleCollection = cms.bool(True),
151	                doElectrons         = cms.bool(True),
152	                electronProducer    = cms.InputTag("selectedPatElectrons"),
153	                electron_cut_pt     = cms.double(10),
154	                electron_cut_eta    = cms.double(2.5), 
155	                doPhotons           = cms.bool(True),
156	                photonProducer      = cms.InputTag("selectedPatPhotons"),
157	                photon_cut_pt       = cms.double(10),
158	                photon_cut_eta      = cms.double(2.5), 
159	                doMuons             = cms.bool(True),
160	                muonProducer        = cms.InputTag("selectedPatMuons"),
161	                muon_cut_pt         = cms.double(10),
162	                muon_cut_eta        = cms.double(2.5), 
163	                doTaus              = cms.bool(True),
164	                tauProducer         = cms.vstring("selectedPatTaus","selectedPatTausHpsPFTau"),
165	                #tauProducer         = cms.vstring("selectedPatTaus"),
166	                tau_cut_pt          = cms.double(10),
167	                tau_cut_eta         = cms.double(2.4),      
168	                trackProducer       = cms.InputTag("generalTracks"),
169	                doTracks            = cms.bool(True),
170	                track_cut_pt        = cms.double(0.5),
171	                track_cut_eta       = cms.double(2.4),
172	                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
173	                jet_cut_pt          = cms.double(10),
174	                jet_cut_eta         = cms.double(2.5), 
175	                met_cut_pt          = cms.double(-99),          
176	                saveAllVertex       = cms.bool(True),
177	                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
178	                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJets"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("ak5")),
179	#                                               cms.PSet(jet = cms.untracked.string("selectedPatJetsSC5Calo"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("sc5")),
180	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"), algo =cms.untracked.string("tc")),
181	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5PF"), met = cms.untracked.string("patMETsPF"), algo =cms.untracked.string("pf"))
182	                                                )
183	        )
184	
185	
186	
187	
188	
189	
190	
191	
192	
193	
194	
195	
196	
197	
198	
199	
200	
201	
202	
203	# Add the files 
204	#readFiles = cms.untracked.vstring()
205	#secFiles = cms.untracked.vstring()
206	#
207	#readFiles.extend( [
208	#       'file:/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/462/7C814F10-CB3C-DF11-B1F1-0030487C778E.root'
209	#        ] );
210	#process.source.fileNames = readFiles
211	
212	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))
213	
214	process.source = cms.Source(
215	        "PoolSource",
216	        noEventSort = cms.untracked.bool(True),
217	        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
218	        fileNames = cms.untracked.vstring(
219	               "rfio:/dpm/in2p3.fr/home/cms/phedex/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v1/0015/1A85A092-63D9-DF11-97CF-0026B9278603.root",
220	               "rfio:/dpm/in2p3.fr/home/cms/phedex/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v1/0016/265F8E28-7BDA-DF11-A65C-0026B94DBDF0.root"
221	                #'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_1_1.root',
222	                #'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_2_1.root',
223	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Wtaunu.root'
224	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Zjets-madgraph.root'
225	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Ztautau.root'
226	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_1_1.root',
227	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_2_1.root'
228	        )
229	)
230	
231	
232	
233	
234	
235	
236	# let it run
237	
238	#print
239	#print "============== Warning =============="
240	#print "technical trigger filter:    DISABLED"
241	#print "physics declare bit filter:  DISABLED"
242	#print "primary vertex filter:       DISABLED"
243	
244	#process.dump=cms.EDAnalyzer('EventContentAnalyzer')
245	
246	process.p = cms.Path(#process.hltHighLevel +
247	                      #process.primaryVertexFilter *
248	                      #process.HBHENoiseFilter*
249	                      #process.noscraping +  
250	                      #process.genParticlesForJets*
251	                      #process.ak5GenJets*
252	                      #process.recoJPTJets*
253	                      process.eIdSequence*
254	                      process.simpleEleIdSequence*
255	                      process.produceAndDiscriminateHPSPFTaus *
256	                      process.patElectronIDs *
257	                      process.patDefaultSequence*
258	                      #process.patTriggerSequence *
259	                      process.MiniTreeProduction
260	    )
261	
262	# rename output file
263	process.out.fileName = cms.untracked.string('MiniTree.root')
264	
265	# reduce verbosity
266	process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(500)
267	
268	# process 1 event
269	process.maxEvents.input = 20000
270	process.options.wantSummary = True
271	
272	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
273	from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
274	from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
275	process.out.outputCommands = patEventContentNoCleaning
276	process.out.outputCommands += patExtraAodEventContent
277	process.out.outputCommands += patTriggerEventContent
278	process.out.outputCommands += [
279	         'drop *',
280	         'keep TopTreeMTEvent_*_*_*'
281	        ]
