## import skeleton process
#from PhysicsTools.PatAlgos.patTemplate_cfg import *


import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")


## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")




## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.PyReleaseValidation.autoCond import autoCond

# modifdg
#startup == START311_V2::All for CMSSW_4_1_4 (seems obsolete, to be confirmed!)
#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.GlobalTag.globaltag = cms.string('START42_V12::All')



process.load("Configuration.StandardSequences.MagneticField_cff")

## Standard PAT Configuration File
#process.load("PhysicsTools.PatAlgos.patSequences_cff")


## Output Module Configuration (expects a path 'p')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent )
                               )

process.outpath = cms.EndPath(process.out)







# PAT Layer 0+1
#process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")

#from PhysicsTools.PatAlgos.tools.trigTools import *
#switchOnTrigger( process )

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










# the source is already defined in patTemplate_cfg.
# overriding source and various other things
#process.load("CommonTools.ParticleFlow.Sources.source_ZtoEles_DBS_312_cfi")
#process.source = cms.Source("PoolSource",
#     fileNames = cms.untracked.vstring('file:myAOD.root')
#)


# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
runOnMC = True

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.out.fileName = cms.untracked.string('patTuple.root')

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")


# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
#process.isoValMuonWithCharged.deltaR = 0.3
#from PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi import isoValMuonWithCharged
#process.isoValMuonWithCharged.deltaR = cms.double(0.3)


# An empty postfix means that only PF2PAT is run,
# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
# collections have standard names + postfix (e.g. patElectronPFlow)


postfix = "PF2PAT"  # to have only PF2PAT
jetAlgo="AK5"

usePFnoPU = True # before any top projection


# levels to be accessible from the jets
# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
# and remain uncorrected, if none of these levels is enabled here
useL1FastJet    = True  # needs useL1Offset being off, error otherwise
useL1Offset     = False # needs useL1FastJet being off, error otherwise
useL2Relative   = True
useL3Absolute   = True
# useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
useL5Flavor     = False
useL7Parton     = False

# JEC set
jecSetBase = jetAlgo
jecSetPF = jecSetBase + 'PF'
if usePFnoPU:
  jecSetPF += 'chs'

# JEC levels
if useL1FastJet and useL1Offset:
  sys.exit( 'ERROR: switch off either "L1FastJet" or "L1Offset"' )
jecLevels = []
if useL1FastJet:
  jecLevels.append( 'L1FastJet' )
if useL1Offset:
  jecLevels.append( 'L1Offset' )
if useL2Relative:
  jecLevels.append( 'L2Relative' )
if useL3Absolute:
  jecLevels.append( 'L3Absolute' )
# if useL2L3Residual and not runOnMC:
#   jecLevelsPF.append( 'L2L3Residual' )
if useL5Flavor:
  jecLevels.append( 'L5Flavor' )
if useL7Parton:
  jecLevels.append( 'L7Parton' )




usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) )
#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )

# to use tau-cleaned jet collection uncomment the following:
#getattr(process,"pfNoTau"+postfix).enable = True

#changes for iso and deltaR
#muons
applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
#electrons
applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
# jec should be ok
# by default: 'L1Offset','L2Relative','L3Absolute'
#  and primaryVertices = cms.InputTag("offlinePrimaryVertices")

addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                 'AK5', 'JPT',
                 doJTA        = True,
                 doBTagging   = True,
#modifdg (L1Offset to substract PU effect)
#                 jetCorrLabel = ('AK5JPT', cms.vstring([ 'L2Relative', 'L3Absolute'])),
                 jetCorrLabel = ('AK5JPT', cms.vstring(['L1Offset', 'L2Relative', 'L3Absolute'])),
                 doType1MET   = False,
                 doL1Cleaning = False,
                 doL1Counters = True,
                 genJetCollection = cms.InputTag("ak5GenJets"),
                 doJetID      = True
                 )

from PhysicsTools.PatAlgos.tools.metTools import *

addTcMET(process, 'TC')
process.load('RecoJets.Configuration.RecoJets_cff')
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets

process.kt6PFJets               = kt4PFJets.clone()
process.kt6PFJets.rParam        = 0.6
process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
process.kt6PFJets.doRhoFastjet  = True
process.kt6PFJets.doAreaFastjet = True
#process.kt6PFJets.voronoiRfact  = 0.9

process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")

process.kt6NeutralPFJets = kt4PFJets.clone()
process.kt6NeutralPFJets.rParam        = 0.6
process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
process.kt6NeutralPFJets.doRhoFastjet  = True
process.kt6NeutralPFJets.doAreaFastjet = True
process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
#process.kt6PFJets.voronoiRfact  = 0.9


process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
process.jetIDSelector.version = cms.string('PURE09')



process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)

process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
    eidLoose= cms.InputTag("eidLoose"),
    eidTight= cms.InputTag("eidTight"),
    eidRobustTight= cms.InputTag("eidRobustTight"),
    eidRobustLoose= cms.InputTag("eidRobustLoose"),
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
    cicLoose         =  cms.InputTag("cicLoose"),
    cicMedium        =  cms.InputTag("cicMedium"),
    cicTight         =  cms.InputTag("cicTight"),
    cicSuperTight    =  cms.InputTag("cicSuperTight"),
    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
    cicLooseMC         =  cms.InputTag("cicLooseMC"),
    cicMediumMC        =  cms.InputTag("cicMediumMC"),
    cicTightMC         =  cms.InputTag("cicTightMC"),
    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC")
)


process.patElectronIDsPF2PAT = cms.Sequence(process.simpleEleIdSequence)

process.patElectronsPF2PAT.addElectronID = cms.bool(True)
process.patElectronsPF2PAT.electronIDSources = cms.PSet(
    eidLoose= cms.InputTag("eidLoose"),
    eidTight= cms.InputTag("eidTight"),
    eidRobustTight= cms.InputTag("eidRobustTight"),
    eidRobustLoose= cms.InputTag("eidRobustLoose"),
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
    cicVeryLoose     =  cms.InputTag("cicVeryLoose"),
    cicLoose         =  cms.InputTag("cicLoose"),
    cicMedium        =  cms.InputTag("cicMedium"),
    cicTight         =  cms.InputTag("cicTight"),
    cicSuperTight    =  cms.InputTag("cicSuperTight"),
    cicHyperTight1    =  cms.InputTag("cicHyperTight1"),
    cicHyperTight2    =  cms.InputTag("cicHyperTight2"),
    cicHyperTight3    =  cms.InputTag("cicHyperTight3"),
    cicVeryLooseMC     =  cms.InputTag("cicVeryLooseMC"),
    cicLooseMC         =  cms.InputTag("cicLooseMC"),
    cicMediumMC        =  cms.InputTag("cicMediumMC"),
    cicTightMC         =  cms.InputTag("cicTightMC"),
    cicSuperTightMC    =  cms.InputTag("cicSuperTightMC"),
    cicHyperTight1MC    =  cms.InputTag("cicHyperTight1MC"),
    cicHyperTight2MC    =  cms.InputTag("cicHyperTight2MC"),
    cicHyperTight3MC    =  cms.InputTag("cicHyperTight3MC")
)





# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
# and add it to path
#postfix2 = "PFlow2"
#jetAlgo2="AK7"
#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)

# to use tau-cleaned jet collection uncomment the following:
#getattr(process,"pfNoTau"+postfix).enable = True

# to switch default tau to HPS tau uncomment the following:
#adaptPFTaus(process,"hpsPFTau",postfix=postfix)





process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
                isAOD               = cms.bool(True), # true if processing AOD data
                isData              = cms.bool(False), # true if processing AOD data
                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
                TriggerMenu         = cms.InputTag("TriggerResults"),    # or TriggerResults::REDIGI311X
                TriggerInfo         = cms.bool(True),
                saveAllTriggers     = cms.bool(True),#should be True by default !!
                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
                prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2"),
                hltJetsMatcher      = cms.string(""),
                doGenParticleCollection = cms.bool(True),
                doElectrons         = cms.bool(True),
                electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
                electron_cut_pt     = cms.double(10),
                electron_cut_eta    = cms.double(2.5),
                doPhotons           = cms.bool(False),
                photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
                photon_cut_pt       = cms.double(10),
                photon_cut_eta      = cms.double(2.5),
                doMuons             = cms.bool(True),
                muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
                muon_cut_pt         = cms.double(10),
                muon_cut_eta        = cms.double(2.5),
                doTaus              = cms.bool(False),
                tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
                #tauProducer         = cms.vstring("selectedPatTaushpsPFTauPF2PAT"),
                tau_cut_pt          = cms.double(5),
                tau_cut_eta         = cms.double(2.5),
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
                rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
                #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
                neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("pf")),
                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
                                                cms.PSet(jet = cms.untracked.string("selectedPatJets"),       met = cms.untracked.string("patMETs"),       algo =cms.untracked.string("ak5"))
                                             )
        )



process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# loads your analyzer
process.MyModule = cms.EDAnalyzer('NTupleProducer',
                verbose             = cms.int32(0), #0: nothing - >1 gradually more information
                saveAllTriggers     = cms.bool(True),# False: see list above
                saveTriggersWith    = cms.vstring(""),
                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
                doElectrons         = cms.bool(True),
                electron_cut_pt     = cms.double(10),
                electron_cut_eta    = cms.double(2.5),
                doPhotons           = cms.bool(True),
                photon_cut_pt       = cms.double(10),
                photon_cut_eta      = cms.double(2.5),
                doMuons             = cms.bool(True),
                muon_cut_pt         = cms.double(7),
                muon_cut_eta        = cms.double(2.5),
                doTaus              = cms.bool(True),
                tauAlgos            = cms.vstring("selectedPatTausPF2PAT"),
                tau_cut_pt          = cms.double(5),
                tau_cut_eta         = cms.double(3.),
                doTracks            = cms.bool(False),
                track_cut_pt        = cms.double(0.5),
                track_cut_eta       = cms.double(2.4),
                doJets              = cms.bool(True),
                jetAlgos            = cms.vstring("ak5","pf", "tc"),
                jet_cut_pt          = cms.double(10),
                jet_cut_eta         = cms.double(2.5),
                saveAllVertex       = cms.bool(True),
                saveMCInfo          = cms.bool(True),

                # Configuration for skimming !
                #
                # Events will be recorded if one of the following parameters is activated :
                # numberOfLept4Skim!=-1 || doTMEMESkimming==True || doMCDiLepSkimming==True || doTauSkimming == True || doTriggerSkimming == True
                #
                numberOfLept4Skim   = cms.int32(2),# for skims ! Total number of sel lepton,  -1 for no skiming
                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
                numberOfElec4Skim   = cms.int32(0),# number of sel electron
                useMuonIdSkim         = cms.bool(False),
                muon_cut_pt_skim      = cms.double(10),
                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
                useElectronIdSkim     = cms.bool(False),
                electron_cut_pt_skim  = cms.double(7),
                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!

                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
                TMEMESkimList         = cms.vint32(),

                doMCDiLepSkimming     = cms.bool(False),
                MCDiLepList           = cms.vstring(""),

                doTauSkimming         = cms.bool(False), # skim on the number of reco taus (no id so far)
                numberOfTau4Skim      = cms.int32(1),
                tau_cut_pt_skim       = cms.double(5),
                tau_algo_skim         = cms.string("selectedPatTaus"),

                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
                triggerSkimList       = cms.vstring("HLT_QuadJet15U"),

                doJetSkimming         = cms.bool(False), # skim on the number of jets
                numberOfJet4Skim      = cms.int32(1),
                jet_cut_pt_skim       = cms.double(10),
                jet_cut_eta_skim      = cms.double(2.5),
                jet_algo_skim         = cms.string("ak5"),


        )



# top projections in PF2PAT:
getattr(process,"pfNoPileUp"  +postfix).enable = True
getattr(process,"pfNoMuon"    +postfix).enable = False
getattr(process,"pfNoElectron"+postfix).enable = False
getattr(process,"pfNoTau"     +postfix).enable = False
getattr(process,"pfNoJet"     +postfix).enable = False

# verbose flags for the PF2PAT modules
getattr(process,"pfNoMuon"+postfix).verbose = False




if runOnMC == False:
    # removing MC matching for standard PAT sequence
    # for the PF2PAT+PAT sequence, it is done in the usePF2PAT function
    removeMCMatchingPF2PAT( process, '')


#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
#process.isoValMuonWithCharged.deltaR = cms.double(0.3)


# Let it run
process.p = cms.Path( process.HBHENoiseFilter*
                      process.noscraping +
                      process.patElectronIDs *
                      process.kt6PFJets*
                      process.neutralPFCandidatesProducer*
                      process.kt6NeutralPFJets*
                      getattr(process,"patPF2PATSequence"+postfix)

#    second PF2PAT
#    + getattr(process,"patPF2PATSequence"+postfix2)
)
if not postfix=="":
    process.p += process.patDefaultSequence


process.p += process.MiniTreeProduction

process.p += process.MyModule

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   #'keep *_*_*_PAT',
                                                   #'keep edmTriggerResults_*_*_*',
                                                   #'keep double_kt6PFJets_rho_*',
                                                   #'keep *_hltTriggerSummaryAOD_*_*',
                                                   #'keep *_offlineBeamSpot_*_*',
                                                   #'keep *_offlinePrimaryVertices_*_*',
                                                   #'keep *_muons_*_*',
                                                   #'keep *_muonTCMETValueMapProducer_*_*',
                                                   #'keep recoTracks_*_*_*',
                                                   #'keep GenEventInfoProduct_*_*_*',
                                                   #'keep recoGenParticles_*_*_*',
                                                   #*patEventContentNoCleaning
                                                   'keep TopTreeMTEvent_*_*_*'
                                                   )


## ------------------------------------------------------
#  In addition you usually want to change the following
#  parameters:
## ------------------------------------------------------
#
#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#                                         ##
#   process.source.fileNames = [          ##
#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
#   ]                                     ##  (e.g. 'file:AOD.root')
#                                         ##



process.maxEvents.input = 1000            ##  (e.g. -1 to run on all events)



#                                         ##
#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#                                         ##
#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
#                                         ##
#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)

process.MessageLogger.cerr.FwkReport.reportEvery = 10

# to relax the muon isolation, uncomment the following:
#process.pfIsolatedMuonsPFlow.combinedIsolationCut = cms.double(99999.)
#process.PF2PAT.pfMuonSequence.pfIsolatedMuons.combinedIsolationCut = cms.double(99999.)


process.source = cms.Source(
        "PoolSource",
        noEventSort = cms.untracked.bool(True),
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring(


        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/04E39182-EA7D-E011-820E-00151796D58C.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/08FF289A-E97D-E011-9734-0015178C6558.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/0A97B89F-EC7D-E011-B7E1-0024E876803E.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/0E41151C-ED7D-E011-A00D-00151796C040.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/0E87D78E-E57D-E011-902D-00266CF20468.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/1AF9D2EF-E77D-E011-B319-00151796D7C0.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/2493ABBD-E87D-E011-BFC3-00151796D774.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/284140DC-E77D-E011-9323-0026B94E287F.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/288498E3-EF7D-E011-8C55-00A0D1EEC298.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/2CA15EB7-E47D-E011-BDA4-0015178C4BF4.root',
        '/store/mc/Summer11/TT_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S3_START42_V11-v2/0000/2E2514EC-E77D-E011-AC66-0015178C4C7C.root'





        )
)


