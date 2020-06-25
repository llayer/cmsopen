1	import FWCore.ParameterSet.Config as cms
2	
3	from TopQuarkAnalysis.MiniTreeProducer.simpleCutBasedElectronIDSpring10_cfi import *
4	
5	
6	simpleEleId95relIso = simpleCutBasedElectronID.clone()
7	simpleEleId95relIso.electronQuality = '95relIso'
8	
9	simpleEleId90relIso = simpleCutBasedElectronID.clone()
10	simpleEleId90relIso.electronQuality = '90relIso'
11	
12	simpleEleId85relIso = simpleCutBasedElectronID.clone()
13	simpleEleId85relIso.electronQuality = '85relIso'
14	
15	simpleEleId80relIso = simpleCutBasedElectronID.clone()
16	simpleEleId80relIso.electronQuality = '80relIso'
17	
18	simpleEleId70relIso = simpleCutBasedElectronID.clone()
19	simpleEleId70relIso.electronQuality = '70relIso'
20	
21	simpleEleId60relIso = simpleCutBasedElectronID.clone()
22	simpleEleId60relIso.electronQuality = '60relIso'
23	
24	
25	simpleEleId95cIso = simpleCutBasedElectronID.clone()
26	simpleEleId95cIso.electronQuality = '95cIso'
27	
28	simpleEleId90cIso = simpleCutBasedElectronID.clone()
29	simpleEleId90cIso.electronQuality = '90cIso'
30	
31	simpleEleId85cIso = simpleCutBasedElectronID.clone()
32	simpleEleId85cIso.electronQuality = '85cIso'
33	
34	simpleEleId80cIso = simpleCutBasedElectronID.clone()
35	simpleEleId80cIso.electronQuality = '80cIso'
36	
37	simpleEleId70cIso = simpleCutBasedElectronID.clone()
38	simpleEleId70cIso.electronQuality = '70cIso'
39	
40	simpleEleId60cIso = simpleCutBasedElectronID.clone()
41	simpleEleId60cIso.electronQuality = '60cIso'
42	
43	
44	from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi import *
45	cicVeryLoose   = eidVeryLoose.clone()
46	cicLoose       = eidLoose.clone()
47	cicMedium      = eidMedium.clone()
48	cicTight       = eidTight.clone()
49	cicSuperTight  = eidSuperTight.clone()
50	cicHyperTight1 = eidHyperTight1.clone()
51	cicHyperTight2 = eidHyperTight2.clone()
52	cicHyperTight3 = eidHyperTight3.clone()
53	
54	from RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi import *
55	cicVeryLooseMC   = eidVeryLooseMC.clone()
56	cicLooseMC       = eidLooseMC.clone()
57	cicMediumMC      = eidMediumMC.clone()
58	cicTightMC       = eidTightMC.clone()
59	cicSuperTightMC  = eidSuperTightMC.clone()
60	cicHyperTight1MC = eidHyperTight1MC.clone()
61	cicHyperTight2MC = eidHyperTight2MC.clone()
62	cicHyperTight3MC = eidHyperTight3MC.clone()
63	
64	simpleEleIdSequence = cms.Sequence(simpleEleId95relIso+
65	                                   simpleEleId90relIso+
66	                                   simpleEleId85relIso+
67	                                   simpleEleId80relIso+
68	                                   simpleEleId70relIso+
69	                                   simpleEleId60relIso+
70	                                   simpleEleId95cIso+
71	                                   simpleEleId90cIso+
72	                                   simpleEleId85cIso+
73	                                   simpleEleId80cIso+
74	                                   simpleEleId70cIso+
75	                                   simpleEleId60cIso+
76	                                   cicVeryLoose+
77	                                   cicLoose+
78	                                   cicMedium+
79	                                   cicTight+
80	                                   cicSuperTight+
81	                                   cicHyperTight1+
82	                                   cicHyperTight2+
83	                                   cicHyperTight3+
84	                                   cicVeryLooseMC+
85	                                   cicLooseMC+
86	                                   cicMediumMC+
87	                                   cicTightMC+
88	                                   cicSuperTightMC+
89	                                   cicHyperTight1MC+
90	                                   cicHyperTight2MC+
91	                                   cicHyperTight3MC
92	                                   )
