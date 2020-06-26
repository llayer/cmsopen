# This is an example PAT configuration showing the usage of PAT on minbias data

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## global tag for data
process.GlobalTag.globaltag = cms.string('GR_R_38X_V15::All')



# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")

process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4) ,
                                           maxAbsZ = cms.double(24), 
                                           maxd0 = cms.double(2) 
)

process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(True),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.20)
)    

#Noise filter
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')



process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)

process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
    eidLoose= cms.InputTag("eidLoose"),
    eidTight= cms.InputTag("eidTight"),
    eidRobustTight= cms.InputTag("eidRobustTight"),
    eidRobustLoose= cms.InputTag("eidRobustLoose"),
    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
    simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
    simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
    simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
    simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
    simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),
)





from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *

#process.load("RecoJets.Configuration.RecoJPTJets_cff")
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')


from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])    

from PhysicsTools.PatAlgos.tools.tauTools import *
#switchToPFTauHPS(process)
addTauCollection(process, tauCollection = cms.InputTag('hpsPFTauProducer'), algoLabel = "hps", typeLabel = "PFTau")

process.patJets.addTagInfos     = False #False for AOD

#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#switchJECSet( process, "Spring10")





addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'AK5', 'JPT',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5JPT', cms.vstring(['L2Relative', 'L3Absolute', "L2L3Residual"])),
                 doType1MET   = False,
                 doL1Cleaning = False,
                 doL1Counters = True,                 
                 genJetCollection = cms.InputTag("ak5GenJets"),
                 doJetID      = True
                 )
     
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5', 'PF',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute', "L2L3Residual"])),
                 doType1MET   = True,
                 doL1Cleaning = True,                 
                 doL1Counters = False,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True
                 )




#process.ak5CaloL2Relative.useCondDB = False
#process.ak5CaloL3Absolute.useCondDB = False
#process.ak5CaloResidual.useCondDB = False

#process.ak5PFL2Relative.useCondDB = False
#process.ak5PFL3Absolute.useCondDB = False
#process.ak5PFResidual.useCondDB = False

#process.ak5JPTL2Relative.useCondDB = False
#process.ak5JPTL3Absolute.useCondDB = False
#process.ak5JPTResidual.useCondDB = False




addTcMET(process, 'TC')
addPfMET(process, 'PF')

process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
process.jetIDSelector.version = cms.string('PURE09')

process.load('RecoJets.Configuration.RecoGenJets_cff')
process.load('RecoJets.Configuration.GenJetParticles_cff')


process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
                isAOD               = cms.bool(True), # true if processing AOD data
                isData              = cms.bool(True), # true if processing AOD data
                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
                TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
                TriggerInfo         = cms.bool(True), 
                saveAllTriggers     = cms.bool(True),#should be True by default !!
                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
                EventType           = cms.InputTag("Zjets"), 
                SampleName          = cms.string('top'),
                SampleCrossSection  = cms.double(9999.),   
                doGenParticleCollection = cms.bool(True),
                doElectrons         = cms.bool(True),
                electronProducer    = cms.InputTag("selectedPatElectrons"),
                electron_cut_pt     = cms.double(10),
                electron_cut_eta    = cms.double(2.5), 
                doPhotons           = cms.bool(True),
                photonProducer      = cms.InputTag("selectedPatPhotons"),
                photon_cut_pt       = cms.double(10),
                photon_cut_eta      = cms.double(2.5), 
                doMuons             = cms.bool(True),
                muonProducer        = cms.InputTag("selectedPatMuons"),
                muon_cut_pt         = cms.double(10),
                muon_cut_eta        = cms.double(2.5), 
                doTaus              = cms.bool(True),
                tauProducer         = cms.vstring("selectedPatTaus","selectedPatTausHpsPFTau"),
                #tauProducer         = cms.vstring("selectedPatTaus"),
                tau_cut_pt          = cms.double(10),
                tau_cut_eta         = cms.double(2.4),      
                trackProducer       = cms.InputTag("generalTracks"),
                doTracks            = cms.bool(False),
                track_cut_pt        = cms.double(0.5),
                track_cut_eta       = cms.double(2.4),
                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
                jet_cut_pt          = cms.double(10),
                jet_cut_eta         = cms.double(2.5), 
                met_cut_pt          = cms.double(-99),          
                saveAllVertex       = cms.bool(True),
                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJets"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("ak5")),
#                                               cms.PSet(jet = cms.untracked.string("selectedPatJetsSC5Calo"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("sc5")),
                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"), algo =cms.untracked.string("tc")),
                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5PF"), met = cms.untracked.string("patMETsPF"), algo =cms.untracked.string("pf"))
                                                )
        )



















# Add the files 
#readFiles = cms.untracked.vstring()
#secFiles = cms.untracked.vstring()
#
#readFiles.extend( [
#       'file:/store/data/Commissioning10/MinimumBias/RECO/v7/000/132/462/7C814F10-CB3C-DF11-B1F1-0030487C778E.root'
#        ] );
#process.source.fileNames = readFiles

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))

process.source = cms.Source(
        "PoolSource",
        noEventSort = cms.untracked.bool(True),
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring(
               #"rfio:/dpm/in2p3.fr/home/cms/phedex/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v1/0015/1A85A092-63D9-DF11-97CF-0026B9278603.root"
                 "rfio:/dpm/in2p3.fr/home/cms/phedex/store/data/Run2010B/Electron/RECO/Nov4ReReco_v1/0114/40E3C3CE-B5E9-DF11-A6B0-0018F3D09626.root"
                #'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_1_1.root',
                #'file:/opt/sbg/cms/ui4_data1/gele/MCData/CMSSW_3_6_1_patch2/TTReference_20K/reco_7TeV_2_1.root',
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Wtaunu.root'
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Zjets-madgraph.root'
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_files/Ztautau.root'
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_1_1.root',
                #'file:/opt/sbg/cms/ui4_data1/lebihan/test_minitrees/Zjets_reference_20k/reco_7TeV_2_1.root'
        )
)






# let it run

#print
#print "============== Warning =============="
#print "technical trigger filter:    DISABLED"
#print "physics declare bit filter:  DISABLED"
#print "primary vertex filter:       DISABLED"

#process.dump=cms.EDAnalyzer('EventContentAnalyzer')

process.p = cms.Path(#process.hltHighLevel +
                      #process.primaryVertexFilter *
                      process.HBHENoiseFilter*
                      process.noscraping +  
                      #process.genParticlesForJets*
                      #process.ak5GenJets*
                      #process.recoJPTJets*
                      process.eIdSequence*
                      process.simpleEleIdSequence*
                      process.produceAndDiscriminateHPSPFTaus *
                      process.patElectronIDs *
                      process.patDefaultSequence*
                      #process.patTriggerSequence *
                      process.MiniTreeProduction
    )

# rename output file
process.out.fileName = cms.untracked.string('MiniTree.root')

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(500)

# process 1 event
process.maxEvents.input = 20000
process.options.wantSummary = True

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
process.out.outputCommands = patEventContentNoCleaning
process.out.outputCommands += patExtraAodEventContent
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += [
         'drop *',
         'keep TopTreeMTEvent_*_*_*'
        ]
