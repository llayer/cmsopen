1	
2	import FWCore.ParameterSet.Config as cms
3	
4	source = cms.Source(
5	        "PoolSource",
6	
7	        noEventSort = cms.untracked.bool(True),
8	        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
9	        fileNames = cms.untracked.vstring(
10	                'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_1_1.root',
11	                'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_2_1.root',
12	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Wtaunu.root'
13	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Zjets-madgraph.root'
14	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Ztautau.root'
15	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_1_1.root',
16	                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_2_1.root'
17	        )
18	)
