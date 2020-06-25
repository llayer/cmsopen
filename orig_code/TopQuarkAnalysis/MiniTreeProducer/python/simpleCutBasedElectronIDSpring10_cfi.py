1	import FWCore.ParameterSet.Config as cms
2	
3	## Electron ID Based on Simple Cuts: Spring10 MC tuned selections
4	#
5	#  Instructions on how to use this file
6	#  ====================================
7	#
8	#  The selections that are implemented in this python cfg are
9	#  explained in this twiki page:
10	#  https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
11	#  In summary, they come in 6 different tightness levels. For
12	#  each tightness, the user can select whether they want
13	#  combined isolation or relative isolations.
14	#
15	#  In order to use this cfg file you have to include it from the
16	#  python directory that you have placed it, clone some selection
17	#  of your preference and run it in your sequence
18	#  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
19	#  from ElectroWeakAnalysis.WENu.simpleCutBasedElectronID_cfi import *  
20	#
21	#  simpleEleId_95relIso = simpleCutBasedElectronID.clone()
22	#  simpleEleId_95relIso.electronQuality = '_95relIso_'
23	#  mySequence = cms.Sequence(...+...+..+simpleEleId95relIso+...)
24	#  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
25	#  Warning: make sure that you use the correct tags for the
26	#  RecoEgamma/ElectronIdentification package
27	#  consult this twiki to obtain the latest information:
28	#
29	#  https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID
30	#
31	#  this version of the file needs 
32	#  V00-03-07-03   RecoEgamma/ElectronIdentification
33	
34	simpleCutBasedElectronID = cms.EDProducer("EleIdCutBasedExtProducer",
35	
36	#   import here your collections
37	    src = cms.InputTag("gsfElectrons"),
38	    #reducedBarrelRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
39	    #reducedEndcapRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
40	    # Spring10 uses these names:
41	    reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
42	    reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
43	    # if you want the vertices or the offline beam spot
44	    verticesCollection = cms.InputTag("offlineBeamSpot"),
45	    dataMagneticFieldSetUp = cms.bool(False),
46	    dcsTag = cms.InputTag("scalersRawToDigi"),                                          
47	    algorithm = cms.string('eIDCB'),
48	
49	    #electronIDType: robust  for the simple Cut-Based
50	    #electronQuality: see later
51	    #electronVersion: use V03 with the offline beam spot
52	    electronIDType  = cms.string('robust'),
53	    electronQuality = cms.string('test'),
54	    electronVersion = cms.string('V04'),
55	
56	####
57	#### Selections with Relative Isolation                                          
58	    robust95relIsoEleIDCutsV04 = cms.PSet(
59	           barrel =  cms.vdouble(5.0e-01, 1.0e-02, 8.0e-01, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 1.5e-01, 
60	                                 2.0e+00, 1.2e-01, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 1, -1, 0.0, 0.0, ),
61	           endcap =  cms.vdouble(7.0e-02, 3.0e-02, 7.0e-01, 1.0e-02, -1, -1, 9999., 9999., 9999., 9999., 9999., 8.0e-02, 
62	                                 6.0e-02, 5.0e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 1, -1, 0.0, 0.0, ),
63	    ),
64	    robust90relIsoEleIDCutsV04 = cms.PSet(
65	           barrel =  cms.vdouble(1.2e-01, 1.0e-02, 8.0e-01, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 1.2e-01, 
66	                                 9.0e-02, 1.0e-01, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
67	           endcap =  cms.vdouble(5.0e-02, 3.0e-02, 7.0e-01, 9.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 5.0e-02, 
68	                                 6.0e-02, 3.0e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
69	    ),
70	    robust85relIsoEleIDCutsV04 = cms.PSet(
71	           barrel =  cms.vdouble(4.0e-02, 1.0e-02, 6.0e-02, 6.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9.0e-02, 
72	                                 8.0e-02, 1.0e-01, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
73	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 4.0e-02, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 5.0e-02, 
74	                                 5.0e-02, 2.5e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
75	    ),
76	    robust80relIsoEleIDCutsV04 = cms.PSet(
77	           barrel =  cms.vdouble(4.0e-02, 1.0e-02, 6.0e-02, 4.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9.0e-02, 
78	                                 7.0e-02, 1.0e-01, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
79	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 3.0e-02, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 4.0e-02, 
80	                                 5.0e-02, 2.5e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
81	    ),
82	    # 70% point modified with restricting cuts to physical values                                                                                    
83	    robust70relIsoEleIDCutsV04 = cms.PSet(
84	           barrel =  cms.vdouble(2.5e-02, 1.0e-02, 3.0e-02, 4.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 5.0e-02, 
85	                                 6.0e-02, 3.0e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
86	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 2.0e-02, 5.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 2.5e-02, 
87	                                 2.5e-02, 2.0e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
88	    ),
89	    # 60% point modified with restricting cuts to physical values                                                                                    
90	    robust60relIsoEleIDCutsV04 = cms.PSet(
91	           barrel =  cms.vdouble(2.5e-02, 1.0e-02, 2.5e-02, 4.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 4.0e-02, 
92	                                 4.0e-02, 3.0e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
93	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 2.0e-02, 5.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 2.5e-02, 
94	                                 2.0e-02, 2.0e-02, 9999., 9999., 9999., 9999., 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
95	    ),
96	
97	####
98	#### Selections with Combined Isolation
99	
100	    robust95cIsoEleIDCutsV04 = cms.PSet(
101	           barrel =  cms.vdouble(5.0e-01, 1.0e-02, 8.0e-01, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
102	                                 9999., 9999., 9999., 9999., 9999., 1.5e-01, 0.0, -9999., 9999., 9999., 1, -1, 0.0, 0.0, ),
103	           endcap =  cms.vdouble(7.0e-02, 3.0e-02, 7.0e-01, 1.0e-02, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
104	                                 9999., 9999., 9999., 9999., 9999., 1.0e-01, 0.0, -9999., 9999., 9999., 1, -1, 0.0, 0.0, ),
105	    ),
106	    robust90cIsoEleIDCutsV04 = cms.PSet(
107	           barrel =  cms.vdouble(1.2e-01, 1.0e-02, 8.0e-01, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
108	                                 9999., 9999., 9999., 9999., 9999., 1.0e-01, 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
109	           endcap =  cms.vdouble(5.0e-02, 3.0e-02, 7.0e-01, 9.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
110	                                 9999., 9999., 9999., 9999., 9999., 7.0e-02, 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
111	    ),
112	    robust85cIsoEleIDCutsV04 = cms.PSet(
113	           barrel =  cms.vdouble(4.0e-02, 1.0e-02, 6.0e-02, 6.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
114	                                 9999., 9999., 9999., 9999., 9999., 9.0e-02, 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
115	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 4.0e-02, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
116	                                 9999., 9999., 9999., 9999., 9999., 6.0e-02, 0.0, -9999., 9999., 9999., 1, -1, 0.02, 0.02, ),
117	    ),
118	    robust80cIsoEleIDCutsV04 = cms.PSet(
119	           barrel =  cms.vdouble(4.0e-02, 1.0e-02, 6.0e-02, 4.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
120	                                 9999., 9999., 9999., 9999., 9999., 7.0e-02, 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
121	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 3.0e-02, 7.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
122	                                 9999., 9999., 9999., 9999., 9999., 6.0e-02, 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
123	    ),
124	    # 70% point modified with restricting cuts to physical values                                          
125	    robust70cIsoEleIDCutsV04 = cms.PSet(
126	           barrel =  cms.vdouble(2.5e-02, 1.0e-02, 3.0e-02, 4.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
127	                                 9999., 9999., 9999., 9999., 9999., 4.0e-02, 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
128	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 2.0e-02, 5.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
129	                                 9999., 9999., 9999., 9999., 9999., 3.0e-02, 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
130	    ),
131	    # 60% point modified with restricting cuts to physical values
132	    robust60cIsoEleIDCutsV04 = cms.PSet(
133	           barrel =  cms.vdouble(2.5e-02, 1.0e-02, 2.5e-02, 4.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
134	                                 9999., 9999., 9999., 9999., 9999., 3.0e-02, 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
135	           endcap =  cms.vdouble(2.5e-02, 3.0e-02, 2.0e-02, 5.0e-03, -1, -1, 9999., 9999., 9999., 9999., 9999., 9999., 
136	                                 9999., 9999., 9999., 9999., 9999., 2.0e-02, 0.0, -9999., 9999., 9999., 0, -1, 0.02, 0.02, ),
137	    ),
138	
139	)
140	
