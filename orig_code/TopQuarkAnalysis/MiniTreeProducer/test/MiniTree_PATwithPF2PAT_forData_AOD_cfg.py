## import skeleton process
#from PhysicsTools.PatAlgos.patTemplate_cfg import *


import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")


## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")



## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.PyReleaseValidation.autoCond import autoCond
#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
#process.GlobalTag.globaltag = cms.string('GR_P_V20::All')
process.GlobalTag.globaltag = cms.string('GR_R_42_V14::All')
process.load("Configuration.StandardSequences.MagneticField_cff")


## Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")


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
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")


#trigger matching & dynamic prescales stuff
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )

process.jetMatchHLTJets = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
, matchedCuts = cms.string(  'type( "TriggerJet" ) && path( "HLT_QuadJet40_v*" )' )
  # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
  # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)
switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets' ] )



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
runOnMC = False

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.out.fileName = cms.untracked.string('patTuple.root')

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")


# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

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

#to use tau-cleaned jet collection uncomment the following:
#getattr(process,"pfNoTau"+postfix).enable = True


addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                'AK5', 'JPT',
                doJTA        = True,
                doBTagging   = True,
                jetCorrLabel = ('AK5JPT', cms.vstring(['L2Relative', 'L3Absolute'])),
                doType1MET   = False,
                doL1Cleaning = False,
                doL1Counters = True,
                genJetCollection = cms.InputTag("ak5GenJets"),
                doJetID      = True
                )




from PhysicsTools.PatAlgos.tools.metTools import *

addTcMET(process, 'TC')
process.patMETsTC.addGenMET = False

removeMCMatching(process, ['All'])
removeIfInSequence(process, 'patJetGenJetMatchAK5JPT', "patDefaultSequence")
removeIfInSequence(process, 'patJetPartonMatchAK5JPT', "patDefaultSequence")
removeIfInSequence(process, 'patJetPartons',           "patDefaultSequence")
removeIfInSequence(process, 'patJetPartonAssociation', "patDefaultSequence")
removeIfInSequence(process, 'patJetPartonAssociationAK5JPT',  "patDefaultSequence")
removeIfInSequence(process, 'patJetFlavourAssociation',       "patDefaultSequence")
removeIfInSequence(process, 'patJetFlavourAssociationAK5JPT', "patDefaultSequence")


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
                isData              = cms.bool(True), # true if processing AOD data
                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
                TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files)
                TriggerInfo         = cms.bool(True),
                saveAllTriggers     = cms.bool(True),#should be True by default !!
                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
                prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5"),
                hltJetsMatcher      = cms.string("jetMatchHLTJets"),
                doGenParticleCollection = cms.bool(False),
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
                #tauProducer         = cms.vstring("selectedPatTaus"),
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
                neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
                #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"), algo =cms.untracked.string("pf")),
                                                #cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"),     algo =cms.untracked.string("tc" )),
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
    removeMCMatchingPF2PAT( process, '' )


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
                                                    'keep TopTreeMTEvent_*_*_*'
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



process.maxEvents.input = -1             ##  (e.g. -1 to run on all events)



#                                         ##
#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#                                         ##
#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
#                                         ##
#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)

process.MessageLogger.cerr.FwkReport.reportEvery = 10

# to relax the muon isolation, uncomment the following:
#process.pfIsolatedMuonsPFlow.combinedIsolationCut = 99999


process.source = cms.Source(
        "PoolSource",
        noEventSort = cms.untracked.bool(True),
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring(

       '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/FE2BB376-C07C-E011-B3A6-001EC9D81A4A.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/EC91B253-AE7C-E011-9F96-485B39800BFB.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/E89314D1-A37C-E011-8FA7-E0CB4E5536BB.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/E87997CC-AD7C-E011-B307-E0CB4E19F999.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/DEBE1714-E27C-E011-9E34-E0CB4E55366A.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D870C654-C47C-E011-BD47-E0CB4E1A1150.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D689ABEB-B87C-E011-962B-E0CB4E4408DF.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/D065B7F8-C47C-E011-AD82-E0CB4E29C4F5.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/BC59235C-C07C-E011-A581-E0CB4EA0A8ED.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/AC3A2F69-BD7C-E011-9287-E0CB4E4408EE.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/A6EB7E32-CC7C-E011-8572-E0CB4E29C50A.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/9CFB03E7-D77C-E011-8DCC-E0CB4E553657.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/989F5D58-AE7C-E011-8419-E0CB4E29C4D5.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/986255F1-B47C-E011-AEDD-E0CB4E553644.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/82BCEC06-B57C-E011-BEE6-0022198F5B1E.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/7EB6BBFA-C47C-E011-BCE5-485B39800B86.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/78E9C834-C97C-E011-B1CE-002618FDA1C2.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/7669575E-C07C-E011-A369-90E6BA442F0F.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/706CCFD8-C17C-E011-B98A-E0CB4EA0A8EC.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/625C0091-B07C-E011-B248-90E6BA0D09D4.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/607DC8A8-F77C-E011-A456-E0CB4E5536EF.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/5C67BCF0-B77C-E011-9057-485B39800BC7.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/52EAB736-BC7C-E011-8637-90E6BA0D09EA.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/4A3D0DE9-D77C-E011-B137-485B39800C16.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/409C81F7-D77C-E011-A758-E0CB4E29C4C0.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/3ED00CFE-B47C-E011-AF39-90E6BA442F38.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/388AAC35-AC7C-E011-9F41-E0CB4E29C4D2.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/2E77E8C6-BA7C-E011-B292-E0CB4E29C4EC.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/283C0C31-B47C-E011-AB86-E0CB4E19F9A9.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/1669D46C-D17C-E011-B59E-E0CB4E19F9BB.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/1479C68D-BF7C-E011-A978-00261834B53C.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/0013FC4E-C07C-E011-B239-E0CB4E29C4B7.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0004/FC4D42FB-D77C-E011-A9AF-E0CB4E29C4D1.root',
        '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0004/FAEB9F99-B37C-E011-A753-E0CB4E29C4B7.root'



        )
)


