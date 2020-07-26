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
#process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )
process.GlobalTag.globaltag = cms.string('GR_R_42_V23::All')

process.GlobalTag.toGet = cms.VPSet(
  cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
        tag = cms.string("TrackProbabilityCalibration_2D_2011Data_v1_offline"),
       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU")),
  cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
        tag = cms.string("TrackProbabilityCalibration_3D_2011Data_v1_offline"),
       connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BTAU"))
)

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
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")

#trigger matching & dynamic prescales stuff
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )



################################################################################################################################################


####################################
#  For jet matching at HLC level
#  Used by tau+jets analysis
####################################
process.jetMatchHLTJets = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
  # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
  # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)
switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets' ] )
################################################################################################################################################

####################################
#  For tau matching at HLC level
#  Used by tau+jets analysis
####################################
process.tauMatchHLTTaus = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*",1 , 0 )')#&& filter( "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40" )')
#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
  # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
  # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)
switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus' ] )

process.jetMatchHLTJets45 = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
#, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
#, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*" )' )
, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet45_IsoPFTau45_v*", 0, 0) && filter( "hltQuadJet45IsoPFTau45" )') 
  # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
  # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)
switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets45' ] )

process.tauMatchHLTTaus45 = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
, src     = cms.InputTag( 'selectedPatTausPF2PAT' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
, matchedCuts = cms.string( 'type( "TriggerTau" ) && path( "HLT_QuadJet45_IsoPFTau45_v*",1 , 0 )')#&& filter( "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40" )')
#, matchedCuts = cms.string( 'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )') 
  # selection of matches
, maxDPtRel   = cms.double( 0.5 ) # no effect here
, maxDeltaR   = cms.double( 0.5 )
, maxDeltaEta = cms.double( 0.2 ) # no effect here
  # definition of matcher output
, resolveAmbiguities    = cms.bool( True )
, resolveByMatchQuality = cms.bool( True )
)
switchOnTriggerMatching( process, triggerMatchers = [ 'tauMatchHLTTaus45' ] )
################################################################################################################################################


####################################
#  PV vertex selector
####################################
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector

process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )
    
    
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(True),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.20)
)    
################################################################################################################################################

####################################
#  HBHE noise filter
####################################
#Noise filter
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')


####################################
#  For Summer11 MC, MG, Powheg
#  Gen filter for pythia bug
####################################

process.load('GeneratorInterface/GenFilters/TotalKinematicsFilter_cfi')




process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))


####################################
#  True to run on MC
####################################

runOnMC = False

####################################
#  Number of events read
####################################
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
####################################
#  Output Filename (PAT)
####################################
process.out.fileName = cms.untracked.string('patTuple.root')




####################################
#  Load the PAT config
####################################
process.load("PhysicsTools.PatAlgos.patSequences_cff")


####################################
#  Load the PAT config
####################################
# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *


####################################
# An empty postfix means that only PF2PAT is run,
# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
# collections have standard names + postfix (e.g. patElectronPFlow)  
####################################


postfix = "PF2PAT"  # to have only PF2PAT
jetAlgo="AK5"

usePFnoPU = True # before any top projection


####################################
# levels to be accessible from the jets
# jets are corrected to L3Absolute (MC), L2L3Residual (data) automatically, if enabled here
# and remain uncorrected, if none of these levels is enabled here
useL1FastJet    = True  # needs useL1Offset being off, error otherwise
useL1Offset     = False # needs useL1FastJet being off, error otherwise
useL2Relative   = True
useL3Absolute   = True
useL2L3Residual = True  # takes effect only on data; currently disabled for CMSSW_4_2_X GlobalTags!
useL5Flavor     = False
useL7Parton     = False
####################################
################################################################################################################################################

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
if useL2L3Residual and not runOnMC:
   jecLevels.append( 'L2L3Residual' )
if useL5Flavor:
  jecLevels.append( 'L5Flavor' )
if useL7Parton:
  jecLevels.append( 'L7Parton' )

################################################################################################################################################



####################################
#  Enable PF2PAT
####################################
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runOnMC, postfix=postfix, jetCorrections = ( jecSetPF , jecLevels) ) 
####################################
# Please keep following line for taus !
adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )

#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=True, postfix=postfix) 
#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix )

####################################
#  Setup JEC (useless ? already done by usePF2PAT ?
####################################

applyPostfix(process,"patJetCorrFactors",postfix).levels     = jecLevels
applyPostfix(process,"patJetCorrFactors",postfix).rho        = cms.InputTag("kt6PFJets","rho")
applyPostfix(process,"patJetCorrFactors",postfix).payload    = jecSetPF
applyPostfix(process,"pfJets",postfix).doAreaFastjet    = True
applyPostfix(process,"pfJets",postfix).doRhoFastjet     = False
applyPostfix(process,"pfPileUp",postfix).checkClosestZVertex = cms.bool(False) 
applyPostfix(process,"pfPileUp",postfix).Vertices = 'goodOfflinePrimaryVertices'



####################################
# to use tau-cleaned jet collection uncomment the following:
#  Desable for now
####################################
#getattr(process,"pfNoTau"+postfix).enable = True
####################################





################################################################################################################################################
####################################
#  changes for iso and deltaR
####################################
#muons # spectial recipe for 428_p7
applyPostfix(process,"pfIsolatedMuons",postfix).isolationValueMapsCharged = cms.VInputTag( cms.InputTag( 'muPFIsoValueCharged03PF2PAT' ) )
applyPostfix(process,"pfIsolatedMuons",postfix).isolationValueMapsNeutral = cms.VInputTag( cms.InputTag( 'muPFIsoValueNeutral03PF2PAT' ), cms.InputTag( 'muPFIsoValueGamma03PF2PAT' ) )
applyPostfix(process,"pfIsolatedMuons",postfix).deltaBetaIsolationValueMap = cms.InputTag( 'muPFIsoValuePU03PF2PAT' )
applyPostfix(process,"patMuons",postfix).isolationValues.pfNeutralHadrons = cms.InputTag( 'muPFIsoValueNeutral03PF2PAT' )
applyPostfix(process,"patMuons",postfix).isolationValues.pfPhotons = cms.InputTag( 'muPFIsoValueGamma03PF2PAT' )
applyPostfix(process,"patMuons",postfix).isolationValues.pfChargedHadrons = cms.InputTag( 'muPFIsoValueCharged03PF2PAT' )
applyPostfix(process,"patMuons",postfix).isolationValues.pfPUChargedHadrons = cms.InputTag( 'muPFIsoValuePU03PF2PAT' ) 
applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
#electrons
applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)


applyPostfix(process,"pfIsolatedMuons",postfix).isolationCut = cms.double(9999.)
applyPostfix(process,"pfIsolatedElectrons",postfix).isolationCut = cms.double(9999.)
################################################################################################################################################





####################################
#  Add tcMET. Not used anymore...
from PhysicsTools.PatAlgos.tools.metTools import *

addTcMET(process, 'TC')
process.patMETsTC.addGenMET = False
####################################


####################################
#  Remove some MC dependencies
####################################
removeMCMatching(process, ['All'])
removeIfInSequence(process, 'patJetGenJetMatchAK5JPT', "patDefaultSequence")
removeIfInSequence(process, 'patJetPartonMatchAK5JPT', "patDefaultSequence")
removeIfInSequence(process, 'patJetPartons',           "patDefaultSequence")
removeIfInSequence(process, 'patJetPartonAssociation', "patDefaultSequence")
removeIfInSequence(process, 'patJetPartonAssociationAK5JPT',  "patDefaultSequence")
removeIfInSequence(process, 'patJetFlavourAssociation',       "patDefaultSequence")
removeIfInSequence(process, 'patJetFlavourAssociationAK5JPT', "patDefaultSequence")
################################################################################################################################################



####################################
#  Produce kt6PFJets
#  For jet area and Rho subtraction
####################################
process.load('RecoJets.Configuration.RecoJets_cff')
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets

process.kt6PFJets               = kt4PFJets.clone()
process.kt6PFJets.rParam        = 0.6     
process.kt6PFJets.src           = cms.InputTag('pfNoElectron'+postfix)
process.kt6PFJets.Rho_EtaMax    = cms.double( 4.4)
process.kt6PFJets.doRhoFastjet  = True
process.kt6PFJets.doAreaFastjet = True
#process.kt6PFJets.voronoiRfact  = 0.9

#process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJets", "rho")
getattr(process,"patJetCorrFactors"+postfix).rho = cms.InputTag("kt6PFJets", "rho")

process.load("TopQuarkAnalysis.MiniTreeProducer.neutralpfcandidates_cfi")

process.kt6NeutralPFJets = kt4PFJets.clone()
process.kt6NeutralPFJets.rParam        = 0.6     
process.kt6NeutralPFJets.Rho_EtaMax    = cms.double( 4.4)
process.kt6NeutralPFJets.doRhoFastjet  = True
process.kt6NeutralPFJets.doAreaFastjet = True
process.kt6NeutralPFJets.src           = "neutralPFCandidatesProducer"
#process.kt6PFJets.voronoiRfact  = 0.9
################################################################################################################################################


####################################
#  Selecion jet according to 
#  The sandard jetID PURE09
####################################
process.load('PhysicsTools.SelectorUtils.pfJetIDSelector_cfi')
process.load('PhysicsTools.SelectorUtils.jetIDSelector_cfi')
process.jetIDSelector.version = cms.string('PURE09')



####################################
#  Produce various eID
####################################
process.load("TopQuarkAnalysis.MiniTreeProducer.simpleEleIdSequence_cff")
#process.load("RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi")
#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi")

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

################################################################################################################################################



####################################
#  Produce PDF weights
#  To speed up the processing
#  Interactivaly, before staring CMSSW
#  do "scramv1 setup lhapdffull; scramv1 b;"
#  with crab, ass these two lines after 
#  eval `scramv1 runtime -sh in crab_.../job/CMSSW.sh
####################################
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
            # Fix POWHEG if buggy (this PDF set will also appear on output,
            # so only two more PDF sets can be added in PdfSetNames if not "")
            #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
            #GenTag = cms.untracked.InputTag("genParticles"),
            PdfInfoTag = cms.untracked.InputTag("generator"),
            PdfSetNames = cms.untracked.vstring(
                    "cteq66.LHgrid"
            )
)
 


################################################################################################################################################
####################################
#  Extra MET collections
####################################
from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")


#process.ak5PFL1Fastjet.algorithm  =  cms.string('AK5PFchs')
#process.ak5PFL1FastL2L3.algorithm =  cms.string('AK5PFchs')

process.ak5PFL1FastjetPFNoPU = ak5PFL1Fastjet.clone()
process.ak5PFL1FastjetPFNoPU.algorithm  =  cms.string('AK5PFchs')

process.ak5PFL1FastL2L3ResidualPFNoPU = ak5PFL1FastL2L3Residual.clone()
process.ak5PFL1FastL2L3ResidualPFNoPU.algorithm  =  cms.string('AK5PFchs')

process.pfCandsNotInJet.topCollection              = cms.InputTag('pfJetsPF2PAT')
process.pfCandsNotInJet.bottomCollection           = cms.InputTag('pfNoElectronPF2PAT')


process.pfJetMETcorr.src            = cms.InputTag('pfJetsPF2PAT')
process.pfJetMETcorr.offsetCorrLabel = cms.string("ak5PFL1FastjetPFNoPU")
process.pfJetMETcorr.jetCorrLabel    = cms.string("ak5PFL1FastL2L3ResidualPFNoPU")
#process.pfCandMETcorr.src = cms.InputTag('pfCandsNotInJetPF2PAT') 

process.pfType1CorrectedMet.src   = cms.InputTag('pfMETPF2PAT')
process.pfType1p2CorrectedMet.src = cms.InputTag('pfMETPF2PAT')

process.patMETsTypeIPF = process.patMETsPF2PAT.clone(
        metSource = cms.InputTag("pfType1CorrectedMet"),
        addMuonCorrections = cms.bool(False)
    )


process.patMETsTypeIIPF = process.patMETsPF2PAT.clone(
        metSource = cms.InputTag("pfType1p2CorrectedMet"),
        addMuonCorrections = cms.bool(False)
    )



################################################################################################################################################
#pfMET TypeI


####################################
#  Our Tree Producers
####################################


process.MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
        isAOD               = cms.bool(True), # true if processing AOD data
        isData              = cms.bool(True), # true if processing AOD data
        readPDFWeights      = cms.bool(False),
        verbose         = cms.int32(0), #0: nothing - >1 gradually more information
        TriggerMenu         = cms.InputTag("TriggerResults::HLT"),    # or TriggerResults::REDIGI311X 
        TriggerInfo         = cms.bool(True), 
        saveAllTriggers     = cms.bool(True),#should be True by default !!
        triggerList     = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
        prescaleList        = cms.vstring("HLT_QuadJet40_v1","HLT_QuadJet40_v2","HLT_QuadJet40_v3","HLT_QuadJet40_v4","HLT_QuadJet40_v5","HLT_QuadJet40_IsoPFTau40_v1","HLT_QuadJet40_IsoPFTau40_v2","HLT_QuadJet40_IsoPFTau40_v3",
                "HLT_QuadJet40_IsoPFTau40_v4","HLT_QuadJet40_IsoPFTau40_v5","HLT_QuadJet40_IsoPFTau40_v6",
                "HLT_QuadJet40_IsoPFTau40_v7","HLT_QuadJet40_IsoPFTau40_v8","HLT_QuadJet40_IsoPFTau40_v9",
                "HLT_QuadJet40_IsoPFTau40_v10","HLT_QuadJet40_IsoPFTau40_v11","HLT_QuadJet40_IsoPFTau40_v12",
                "HLT_QuadJet40_IsoPFTau40_v13","HLT_QuadJet40_IsoPFTau40_v14","HLT_QuadJet40_IsoPFTau40_v15"),
        hltJetsMatcher      = cms.string("jetMatchHLTJets"),
        hltTausMatcher      = cms.string("tauMatchHLTTaus"),
        hltJetsMatcher45    = cms.string("jetMatchHLTJets45"),
        hltTausMatcher45    = cms.string("tauMatchHLTTaus45"),  
        doGenParticleCollection = cms.bool(True),
        doElectrons     = cms.bool(True),
        electronProducer    = cms.InputTag("selectedPatElectronsPF2PAT"),
        electron_cut_pt     = cms.double(10),
        electron_cut_eta    = cms.double(2.5), 
        doPhotons       = cms.bool(False),
        photonProducer      = cms.InputTag("selectedPatPhotonsPF2PAT"),
        photon_cut_pt       = cms.double(10),
        photon_cut_eta      = cms.double(2.5), 
        doMuons         = cms.bool(True),
        muonProducer        = cms.InputTag("selectedPatMuonsPF2PAT"),
        muon_cut_pt         = cms.double(10),
        muon_cut_eta        = cms.double(2.5), 
        doTaus          = cms.bool(True),
        tauProducer         = cms.vstring("selectedPatTausPF2PAT"),
        tau_cut_pt      = cms.double(10),
        tau_cut_eta     = cms.double(2.4),      
        trackProducer       = cms.InputTag("generalTracks"),
        doTracks            = cms.bool(False),
        track_cut_pt        = cms.double(0.5),
        track_cut_eta       = cms.double(2.4),
        beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
        jet_cut_pt      = cms.double(10),
        jet_cut_eta     = cms.double(2.5), 
        met_cut_pt          = cms.double(-99),      
        saveAllVertex       = cms.bool(True),
        vertexProducer      = cms.InputTag("goodOfflinePrimaryVertices"),
        rho_PUUE_dens       = cms.InputTag("kt6PFJets", "rho"),
        #rho_PUUE_dens      = cms.InputTag("kt6CaloJets", "rho"),
            neutralRho_PUUE_dens= cms.InputTag("kt6NeutralPFJets", "rho"),
            jetmetProducer      = cms.VPSet(
                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsPF2PAT"),   algo =cms.untracked.string("pf")),
                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsTypeIPF"),  algo =cms.untracked.string("pfType1")),
                                        cms.PSet(jet = cms.untracked.string("selectedPatJetsPF2PAT"), met = cms.untracked.string("patMETsTypeIIPF"), algo =cms.untracked.string("pfType12"))

                                       )
    )


process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )


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
                muon_cut_pt         = cms.double(10),
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
                numberOfLept4Skim   = cms.int32(-1),# for skims ! Total number of sel lepton,  -1 for no skiming
                numberOfMuon4Skim   = cms.int32(0),# number of sel muon
                numberOfElec4Skim   = cms.int32(0),# number of sel electron
                useMuonIdSkim         = cms.bool(False),
                muon_cut_pt_skim      = cms.double(10),
                muon_cut_iso_skim     = cms.double(-1),  # PLEASE NO ISO FOR SKIMMING!!!
                useElectronIdSkim     = cms.bool(False),
                electron_cut_pt_skim  = cms.double(10),
                electron_cut_iso_skim = cms.double(-1), # PLEASE NO ISO FOR SKIMMING!!!
                
                doTMEMESkimming       = cms.bool(False), # skim on the TMEME
                #TMEMESkimList        = cms.vint32(10000),
                #TMEMESkimList        = cms.vint32(2,10101,20200,20,11010,22000,11,11001,10110,21100),
                #TMEMESkimList        = cms.vint32(10010,21000,10001,20100),
                #TMEMESkimList        = cms.vint32(0),
                #TMEMESkimList        = cms.vint32(20000),
                #TMEMESkimList        = cms.vint32(1,10,10100,11000),
                TMEMESkimList         = cms.vint32(),
                
                doMCDiLepSkimming     = cms.bool(False),
                MCDiLepList           = cms.vstring(""),
                
                doTauSkimming         = cms.bool(True), # skim on the number of reco taus (no id so far)
                numberOfTau4Skim      = cms.int32(1),
                tau_cut_pt_skim       = cms.double(40),
                tau_algo_skim         = cms.string("selectedPatTausPF2PAT"),
                
                doTriggerSkimming     = cms.bool(True), # skim on trigger decisions
                triggerSkimList       = cms.vstring("HLT_QuadJet40_IsoPFTau40","HLT_QuadJet45_IsoPFTau45"),
                
                doJetSkimming         = cms.bool(True), # skim on the number of jets
                numberOfJet4Skim      = cms.int32(3),
                jet_cut_pt_skim       = cms.double(40), 
                jet_cut_eta_skim      = cms.double(2.5),
                jet_algo_skim         = cms.string("pf"),
                
                # ttbar-> tau+jets specific
                doTauJetAna_TauJetTrigSkimming  = cms.bool(True), # need to set doTrigger doTauSkimming doJetSkimming to true
                doTauJetAna_TauJetSkimming      = cms.bool(False),  # need to set doTauSkimming doJetSkimming to true
                doTauJetAna_TmemeTauJetSkimming = cms.bool(False) # need to set doTMEMESkimming doTauSkimming doJetSkimming to true, only for MC ttbar
        
                
        )


################################################################################################################################################

####################################
#  top projections in PF2PAT:
####################################
getattr(process,"pfNoPileUp"  +postfix).enable = True
getattr(process,"pfNoMuon"    +postfix).enable = False 
getattr(process,"pfNoElectron"+postfix).enable = False
getattr(process,"pfNoTau"     +postfix).enable = False
getattr(process,"pfNoJet"     +postfix).enable = False

####################################
#  verbose flags for the PF2PAT modules
getattr(process,"pfNoMuon"+postfix).verbose = False
#  top projections in PF2PAT:
####################################



if runOnMC == False:
    removeMCMatchingPF2PAT( process, '')    


#process.load('PhysicsTools.PFCandProducer.Isolation.isoValMuonWithCharged_cfi')
#process.isoValMuonWithCharged.deltaR = cms.double(0.3)

####################################
#  Configure PF2PAT
####################################
getattr(process,"patPF2PATSequence"+postfix).replace(
    getattr(process,"pfNoElectron"+postfix),
    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJets )

####################################
#  Define the Path
####################################
process.p = cms.Path( 
                      process.goodOfflinePrimaryVertices*
                      process.HBHENoiseFilter*
                      process.noscraping + 
                      process.patElectronIDs *                      
                      getattr(process,"patPF2PATSequence"+postfix)*
                      process.producePFMETCorrections*
                      process.patMETsTypeIPF*
                      process.patMETsTypeIIPF*
                      process.neutralPFCandidatesProducer*
                      process.kt6NeutralPFJets
              
#    second PF2PAT
#    + getattr(process,"patPF2PATSequence"+postfix2)
)
if not postfix=="":
    process.p += process.patDefaultSequence
    
  
#################################### 
# Add NTuple producers to the sequence 
####################################
process.p += process.MiniTreeProduction
   
process.p += process.MyModule   
 
####################################   
#  Output content
#################################### 
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
process.out.outputCommands = cms.untracked.vstring('drop *',
                              # 'keep *_*_*_PAT',
                           'keep edmTriggerResults_*_*_*',
                           #'keep double_kt6PFJets_rho_*',
                           #'keep *_hltTriggerSummaryAOD_*_*',
                           #'keep *_offlineBeamSpot_*_*',
                           #'keep *_offlinePrimaryVertices_*_PAT',
                           #'keep *_muons_*_*',
                           #'keep *_muonTCMETValueMapProducer_*_*',
                           #'keep recoTracks_*_*_*',
                           #'keep GenEventInfoProduct_*_*_*',
                           #'keep recoGenParticles_*_*_*',
                                                   #*patEventContentNoCleaning 
                           'keep TopTreeMTEvent_*_*_*'
                           ) 




process.maxEvents.input = -1            ##  (e.g. -1 to run on all events)



process.MessageLogger.cerr.FwkReport.reportEvery = 10


process.source = cms.Source(
    "PoolSource",
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(
       '/store/data/Run2011A/MuEG/AOD/May10ReReco-v1/0005/FE2BB376-C07C-E011-B3A6-001EC9D81A4A.root'  
    )
)
