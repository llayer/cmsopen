1	# This is an example PAT configuration showing the usage of PAT on minbias data
2	
3	# Starting with a skeleton process which gets imported with the following line
4	from PhysicsTools.PatAlgos.patTemplate_cfg import *
5	
6	from PhysicsTools.PatAlgos.tools.coreTools import *
7	
8	## global tag for data
9	process.GlobalTag.globaltag = cms.string('GR_R_38X_V15::All')
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
73	removeMCMatching(process, ['All'])    
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
85	
86	
87	
88	addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
89	                 'AK5', 'JPT',
90	                 doJTA        = True,
91	                 doBTagging   = True,
92	                 jetCorrLabel = ('AK5JPT', cms.vstring(['L2Relative', 'L3Absolute', "L2L3Residual"])),
93	                 doType1MET   = False,
94	                 doL1Cleaning = False,
95	                 doL1Counters = True,                 
96	                 genJetCollection = cms.InputTag("ak5GenJets"),
97	                 doJetID      = True
98	                 )
99	     
100	addJetCollection(process,cms.InputTag('ak5PFJets'),
101	                 'AK5', 'PF',
102	                 doJTA        = True,
103	                 doBTagging   = True,
104	                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute', "L2L3Residual"])),
105	                 doType1MET   = True,
106	                 doL1Cleaning = True,                 
107	                 doL1Counters = False,
108	                 genJetCollection=cms.InputTag("ak5GenJets"),
109	                 doJetID      = True
110	                 )
111	
112	
113	
114	
115	#process.ak5CaloL2Relative.useCondDB = False
116	#process.ak5CaloL3Absolute.useCondDB = False
117	#process.ak5CaloResidual.useCondDB = False
118	
119	#process.ak5PFL2Relative.useCondDB = False
120	#process.ak5PFL3Absolute.useCondDB = False
121	#process.ak5PFResidual.useCondDB = False
122	
123	#process.ak5JPTL2Relative.useCondDB = False
124	#process.ak5JPTL3Absolute.useCondDB = False
125	#process.ak5JPTResidual.useCondDB = False
126	
127	
128	
129	
130	addTcMET(process, 'TC')
131	addPfMET(process, 'PF')
132	
133	process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
134	process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
135	process.jetIDSelector.version = cms.string('PURE09')
136	
137	process.load('RecoJets.Configuration.RecoGenJets_cff')
138	process.load('RecoJets.Configuration.GenJetParticles_cff')
139	
140	
141	process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
142	                isAOD               = cms.bool(True), # true if processing AOD data
143	                isData              = cms.bool(True), # true if processing AOD data
144	                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
145	                TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
146	                TriggerInfo         = cms.bool(True), 
147	                saveAllTriggers     = cms.bool(True),#should be True by default !!
148	                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
149	                EventType           = cms.InputTag("Zjets"), 
150	                SampleName          = cms.string('top'),
151	                SampleCrossSection  = cms.double(9999.),   
152	                doGenParticleCollection = cms.bool(True),
153	                doElectrons         = cms.bool(True),
154	                electronProducer    = cms.InputTag("selectedPatElectrons"),
155	                electron_cut_pt     = cms.double(10),
156	                electron_cut_eta    = cms.double(2.5), 
157	                doPhotons           = cms.bool(True),
158	                photonProducer      = cms.InputTag("selectedPatPhotons"),
159	                photon_cut_pt       = cms.double(10),
160	                photon_cut_eta      = cms.double(2.5), 
161	                doMuons             = cms.bool(True),
162	                muonProducer        = cms.InputTag("selectedPatMuons"),
163	                muon_cut_pt         = cms.double(10),
164	                muon_cut_eta        = cms.double(2.5), 
165	                doTaus              = cms.bool(True),
166	                tauProducer         = cms.vstring("selectedPatTaus","selectedPatTausHpsPFTau"),
167	                #tauProducer         = cms.vstring("selectedPatTaus"),
168	                tau_cut_pt          = cms.double(10),
169	                tau_cut_eta         = cms.double(2.4),      
170	                trackProducer       = cms.InputTag("generalTracks"),
171	                doTracks            = cms.bool(False),
172	                track_cut_pt        = cms.double(0.5),
173	                track_cut_eta       = cms.double(2.4),
174	                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
175	                jet_cut_pt          = cms.double(10),
176	                jet_cut_eta         = cms.double(2.5), 
177	                met_cut_pt          = cms.double(-99),          
178	                saveAllVertex       = cms.bool(True),
179	                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
180	                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJets"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("ak5")),
181	#                                               cms.PSet(jet = cms.untracked.string("selectedPatJetsSC5Calo"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("sc5")),
182	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"), algo =cms.untracked.string("tc")),
183	                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5PF"), met = cms.untracked.string("patMETsPF"), algo =cms.untracked.string("pf"))
184	                                                )
185	        )
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
203	
204	
205	# Add the files 
206	#readFiles = cms.untracked.vstring()
207	#secFiles = cms.untracked.vstring()
208	#
209	#readFiles.extend( [
210	#       'file:/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/462/7C814F10-CB3C-DF11-B1F1-0030487C778E.root'
211	#        ] );
212	#process.source.fileNames = readFiles
213	
214	process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))
215	
216	process.source = cms.Source(
217	        "PoolSource",
218	        noEventSort = cms.untracked.bool(True),
219	        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
220	        fileNames = cms.untracked.vstring(
221	               #"rfio:/dpm/in2p3.fr/home/cms/phedex/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v1/0015/1A85A092-63D9-DF11-97CF-0026B9278603.root"
222	                 "rfio:/dpm/in2p3.fr/home/cms/phedex/store/data/Run2010B/Electron/RECO/Nov4ReReco_v1/0114/40E3C3CE-B5E9-DF11-A6B0-0018F3D09626.root"
223	                #'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_1_1.root',
224	                #'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_2_1.root',
225	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Wtaunu.root'
226	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Zjets-madgraph.root'
227	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Ztautau.root'
228	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_1_1.root',
229	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_2_1.root'
230	        )
231	)
232	
233	
234	
235	
236	
237	
238	# let it run
239	
240	#print
241	#print "============== Warning =============="
242	#print "technical trigger filter:    DISABLED"
243	#print "physics declare bit filter:  DISABLED"
244	#print "primary vertex filter:       DISABLED"
245	
246	#process.dump=cms.EDAnalyzer('EventContentAnalyzer')
247	
248	process.p = cms.Path(#process.hltHighLevel +
249	                      #process.primaryVertexFilter *
250	                      process.HBHENoiseFilter*
251	                      process.noscraping +  
252	                      #process.genParticlesForJets*
253	                      #process.ak5GenJets*
254	                      #process.recoJPTJets*
255	                      process.eIdSequence*
256	                      process.simpleEleIdSequence*
257	                      process.produceAndDiscriminateHPSPFTaus *
258	                      process.patElectronIDs *
259	                      process.patDefaultSequence*
260	                      #process.patTriggerSequence *
261	                      process.MiniTreeProduction
262	    )
263	
264	# rename output file
265	process.out.fileName = cms.untracked.string('MiniTree.root')
266	
267	# reduce verbosity
268	process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(500)
269	
270	# process 1 event
271	process.maxEvents.input = 20000
272	process.options.wantSummary = True
273	
274	from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
275	from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
276	from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
277	process.out.outputCommands = patEventContentNoCleaning
278	process.out.outputCommands += patExtraAodEventContent
279	process.out.outputCommands += patTriggerEventContent
280	process.out.outputCommands += [
281	         'drop *',
282	         'keep TopTreeMTEvent_*_*_*'
283	        ]
