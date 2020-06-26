
import FWCore.ParameterSet.Config as cms

source = cms.Source(
        "PoolSource",

        noEventSort = cms.untracked.bool(True),
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring(
                'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_1_1.root',
                'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_2_1.root',
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Wtaunu.root'
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Zjets-madgraph.root'
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Ztautau.root'
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_1_1.root',
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_2_1.root'
        )
)
