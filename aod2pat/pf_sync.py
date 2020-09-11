import sys
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

########################################################################
#################### Passed arguments ##################################
########################################################################
#
# can be invoked with no parameters passed, in this case use default values
#
if len(sys.argv) < 2:
  print("Usage: cmsRun analyzer_cfg.py <mc flag>")
  runOnMC = 1
  # do not stop execution at this point, run with default arguments
  #sys.exit("Wrong usage!")
else:
  runOnMC = int(sys.argv[2])

print "Run on MC = ", runOnMC
maxEvents = 100
skim = False
########################################################################
#################### Setup process #####################################
########################################################################

process = cms.Process('PAT')
# load needed tools
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Global tag for 2011 dat
if runOnMC == 0:
    # Before should be done:
    # ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA FT_53_LV5_AN1
    # ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db FT_53_LV5_AN1_RUNA.db
    print "Set global tag for data"
    process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db')
    process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'
else:
    # Before should be done:
    # ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db START53_LV6A1.db
    # ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1 START53_LV6A1
    print "Set global tag for MC"
    process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db')
    process.GlobalTag.globaltag = 'START53_LV6A1::All'

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvents))
if runOnMC == 0:
    #files = ["root://eospublic.cern.ch//eos/opendata/cms/Run2011A/MultiJet/AOD/12Oct2013-v1/00000/001D2AFA-8B43-E311-AC56-02163E009EC4.root"]
    files = ["root://eospublic.cern.ch//eos/opendata/cms/Run2011A/MultiJet/AOD/12Oct2013-v1/20001/BE90B0AD-EF4B-E311-8429-003048F010A2.root"]
else:
    files = ["root://eospublic.cern.ch//eos/opendata/cms/MonteCarlo2011/Summer11LegDR/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S13_START53_LV6-v1/00000/0005D1FB-4BCF-E311-9FE4-002590A8312A.root"]

process.source = cms.Source(
    "PoolSource", fileNames=cms.untracked.vstring(*files))

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

# JSON (good luminosity sections), only if processing data
if runOnMC == 0:
    print "Load lumi"
    #goodJSON = '/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pattuples2011/data/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    goodJSON = '/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pattuples2011/data/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON_truncated.txt'
    print goodJSON
    myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')
    process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
    process.source.lumisToProcess.extend(myLumis)

process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(limit = cms.untracked.int32(-1))

# Output
process.out = cms.OutputModule('PoolOutputModule',
                               fileName = cms.untracked.string('pat.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('taujet')),
                               outputCommands = cms.untracked.vstring('keep *'),
                               )

########################################################################
#################### Configure Analyzer ##################################
########################################################################

# Count events before and after filter operations
process.nEventsTotal = cms.EDProducer("EventCountProducer")
process.nEventsFiltered = cms.EDProducer("EventCountProducer")

################
### Trigger ####
################

#
# Filter
#

if skim:
    # accept if any path succeeds (explicit)
    from HLTrigger.HLTfilters.hltHighLevel_cfi import *
    process.hltSelector = hltHighLevel.clone(
        HLTPaths = ['HLT_QuadJet40_IsoPFTau40*', 'HLT_QuadJet45_IsoPFTau45*'],
        throw = False
        )
#
# Trigger matching
#

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )

"""
process.tauMatchHLTTaus45 = cms.EDProducer(
  # matching in DeltaR, sorting by best DeltaR
  "PATTriggerMatcherDRLessByR"
  # matcher input collections
#, src     = cms.InputTag( 'selectedPatJetsPF2PAT' )
, src     = cms.InputTag( 'selectedPatTausPF' )
, matched = cms.InputTag( 'patTrigger' )
  # selections of trigger objects
, matchedCuts = cms.string( 'type( "TriggerTau" )') #' && path( "HLT_QuadJet45_IsoPFTau45_v*",1 , 0 )')#&& filter( "hltFilterPFTauTrack5TightIsoL1QuadJet20CentralPFTau40" )')
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
"""

def set_matcher(inTag, cuts):
    return cms.EDProducer(
    "PATTriggerMatcherDRLessByR"
    , src     = cms.InputTag( inTag )
    , matched = cms.InputTag( 'patTrigger' )
    , matchedCuts = cms.string( cuts )
    , maxDPtRel   = cms.double( 0.5 ) # no effect here
    , maxDeltaR   = cms.double( 0.5 )
    , maxDeltaEta = cms.double( 0.2 ) # no effect here
    , resolveAmbiguities    = cms.bool( True )
    , resolveByMatchQuality = cms.bool( True )
    )

# Jet and Tau matching for trigger hltQuadJet40IsoPFTau40
process.jetMatchHLTJets = set_matcher(
    'selectedPatJetsPF',
    'type( "TriggerJet" ) && path( "HLT_QuadJet40_IsoPFTau40_v*", 0, 0) && filter( "hltQuadJet40IsoPFTau40" )'
    )

process.tauMatchHLTTaus = set_matcher(
    'selectedPatTausPF',
    'type( "TriggerTau" ) && path( "HLT_QuadJet40_IsoPFTau40_v*",1 , 0 )'
    )

# Jet and Tau matching for trigger hltQuadJet45IsoPFTau45
process.jetMatchHLTJets45 = set_matcher(
    'selectedPatJetsPF',
    'type( "TriggerJet" ) && path( "HLT_QuadJet45_IsoPFTau45_v*", 0, 0) && filter( "hltQuadJet45IsoPFTau45" )'
    )

process.tauMatchHLTTaus45 = set_matcher(
    'selectedPatTausPF',
    'type( "TriggerTau" ) && path( "HLT_QuadJet45_IsoPFTau45_v*",1 , 0 )'
    )
switchOnTriggerMatching( process, triggerMatchers = [ 'jetMatchHLTJets', 'tauMatchHLTTaus', 'jetMatchHLTJets45', 'tauMatchHLTTaus45' ] )


####################################
#####  PV vertex selector ##########
####################################

process.goodOfflinePrimaryVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(True),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("isValid && !isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

"""
process.load('CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi')
process.goodOfflinePrimaryVertices.filter = cms.bool(True)
"""
"""
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector

process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )

"""
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(True),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.20)
)

####################################
#  HBHE noise filter
####################################
#Noise filter
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')


####################################
#  Load the PAT config
####################################
process.load('PhysicsTools.PatAlgos.patSequences_cff')

postfix = 'PF'

jecLevels = ['L1FastJet', 'L2Relative', 'L3Absolute']
#jecLevels = ['L1Offset', 'L2Relative', 'L3Absolute']
if runOnMC == 0:
    jecLevels.append('L2L3Residual')

from PhysicsTools.PatAlgos.tools.pfTools import usePF2PAT
usePF2PAT(process,
          runPF2PAT = True,
          postfix = postfix,
          runOnMC = runOnMC,
          pvCollection = cms.InputTag('goodOfflinePrimaryVertices'),
          jetAlgo = 'AK5',
          jetCorrections = ('AK5PFchs', jecLevels),
          typeIMetCorrections = True,
          )
#from PhysicsTools.PatAlgos.tools.pfTools import *
#adaptPFTaus( process, tauType='hpsPFTau', postfix=postfix ) # necessary??


####################################
#  top projections in PF2PAT:
####################################
getattr(process,"pfNoPileUp"  +postfix).enable = True
getattr(process,"pfNoMuon"    +postfix).enable = False
getattr(process,"pfNoElectron"+postfix).enable = False
getattr(process,"pfNoTau"     +postfix).enable = False
getattr(process,"pfNoJet"     +postfix).enable = False


###########################################
######### Additional variables ############
###########################################

#
# For the time being disable all cuts to be consistent with original analysis
#

#if 'L1FastJet' in jecLevels:
#    process.pfPileUpIsoPF.checkClosestZVertex = True # new PF isolation with L1Fastjet CHS
"""
process.pfMuonsFromVertexPF.d0Cut = 0.2
process.pfMuonsFromVertexPF.dzCut = 0.5
process.pfSelectedMuonsPF.cut = 'pt > 5.'
#process.pfSelectedMuonsPF.cut += ' && ' + muonCut # can use minimal (veto) muon selection on top of base pf cut
process.pfIsolatedMuonsPF.isolationCut = 0.2

process.pfElectronsFromVertexPF.d0Cut = 0.2
process.pfElectronsFromVertexPF.dzCut = 0.5
process.pfSelectedElectronsPF.cut = 'pt > 5. && gsfTrackRef.isNonnull && gsfTrackRef.trackerExpectedHitsInner.numberOfLostHits < 2'
#process.pfSelectedElectronsPF.cut += ' && ' + electronCut # can use minimal (veto) electron selection cut on top of base pf cut
process.pfIsolatedElectronsPF.isolationCut = 0.2
"""

# Additional variables
process.pfIsolatedElectronsPF.isolationValueMapsCharged   = cms.VInputTag('elPFIsoValueCharged03PFIdPF')
process.pfIsolatedElectronsPF.deltaBetaIsolationValueMap  = 'elPFIsoValuePU03PFIdPF'
process.pfIsolatedElectronsPF.isolationValueMapsNeutral   = cms.VInputTag('elPFIsoValueNeutral03PFIdPF', 'elPFIsoValueGamma03PFIdPF')
process.pfElectronsPF.isolationValueMapsCharged           = cms.VInputTag('elPFIsoValueCharged03PFIdPF')
process.pfElectronsPF.deltaBetaIsolationValueMap          = 'elPFIsoValuePU03PFIdPF'
process.pfElectronsPF.isolationValueMapsNeutral           = cms.VInputTag('elPFIsoValueNeutral03PFIdPF', 'elPFIsoValueGamma03PFIdPF')
process.patElectronsPF.isolationValues.pfNeutralHadrons   = 'elPFIsoValueNeutral03PFIdPF'
process.patElectronsPF.isolationValues.pfChargedAll       = 'elPFIsoValueChargedAll03PFIdPF'
process.patElectronsPF.isolationValues.pfPUChargedHadrons = 'elPFIsoValuePU03PFIdPF'
process.patElectronsPF.isolationValues.pfPhotons          = 'elPFIsoValueGamma03PFIdPF'
process.patElectronsPF.isolationValues.pfChargedHadrons   = 'elPFIsoValueCharged03PFIdPF'

process.patMuonsPF.embedTrack = True

# B-tagging info
process.patJetsPF.addTagInfos = True

"""
process.load('EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi')
process.eidMVASequence = cms.Sequence(process.mvaTrigV0 + process.mvaNonTrigV0)
process.patElectronsPF.electronIDSources.mvaTrigV0    = cms.InputTag("mvaTrigV0")
process.patElectronsPF.electronIDSources.mvaNonTrigV0 = cms.InputTag("mvaNonTrigV0")
"""

####################################
#  Produce PDF weights
#  To speed up the processing
#  Interactivaly, before staring CMSSW
#  do "scramv1 setup lhapdffull; scramv1 b;"
#  with crab, ass these two lines after
#  eval `scramv1 runtime -sh in crab_.../job/CMSSW.sh
####################################
"""
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
"""

####################################
#  Basic object selections
####################################


if skim:

    # Basic cuts - apply in analyzer
    """
    process.selectedPatMuonsPF.cut = 'pt > 10. && abs(eta) < 2.5'
    process.selectedPatElectronsPF.cut = 'pt > 10. && abs(eta) < 2.5'
    process.selectedPatTausPF.cut = 'pt > 10. && abs(eta) < 2.5'
    process.selectedPatJetsPF.cut = 'pt > 10. && abs(eta) < 2.5'
    """
    process.countJets = cms.EDFilter("CandViewCountFilter",
         src = cms.InputTag('selectedPatJetsPF'),
         minNumber = cms.uint32(3)
    )

    process.countTaus = cms.EDFilter("CandViewCountFilter",
         src = cms.InputTag('selectedPatTausPF'),
         minNumber = cms.uint32(1)
    )

####################################
##########  NanoAOD ################
####################################

if runOnMC:
    isData = False
else:
    isData = True

process.MyModule = cms.EDAnalyzer('TopTauAnalyze',
    isData = cms.bool(isData),
    electron_cut_pt     = cms.double(10),
    electron_cut_eta    = cms.double(2.5),
    muon_cut_pt         = cms.double(10),
    muon_cut_eta        = cms.double(2.5),
    tau_cut_pt      = cms.double(10),
    tau_cut_eta     = cms.double(2.5),    #!! original is 2.4
    jet_cut_pt      = cms.double(10),
    jet_cut_eta     = cms.double(2.5),
    verbose = cms.bool(True)
    )

####################################
#  Output content
####################################

# Remove MC matching if running on data
from PhysicsTools.PatAlgos.tools.coreTools import runOnData
if not runOnMC:
    runOnData(process, names = ['PFAll'], postfix = 'PF')

# Remove photons from PAT
from PhysicsTools.PatAlgos.tools.coreTools import removeSpecificPATObjects
removeSpecificPATObjects(process, names = ['Photons'], postfix = 'PF')

#cms.ignore(process.mvaTrigV0) + \
#cms.ignore(process.mvaNonTrigV0) + \

if skim:
    base_path = process.nEventsTotal * \
                process.hltSelector * \
                process.goodOfflinePrimaryVertices * \
                process.patPF2PATSequencePF * \
                process.countJets * \
                process.countTaus * \
                process.nEventsFiltered * \
                process.MyModule
else:
    base_path = process.nEventsTotal * \
                process.goodOfflinePrimaryVertices * \
                process.patPF2PATSequencePF * \
                process.nEventsFiltered * \
                process.MyModule

if runOnMC == 1:
    process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
    process.taujet = cms.Path(base_path + process.makeGenEvt)
    print "Create gen event"
else:
    process.taujet = cms.Path(base_path) #+ process.makeGenEvt)


#cms.Path(process.taujet)
process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('taujet'))
process.out.outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_nEventsTotal_*_*',
    'keep *_nEventsFiltered_*_*')

process.TFileService = cms.Service("TFileService", fileName = cms.string("nano_mc.root") )

"""
process.out.outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_selectedPatElectrons*_*_*',
    'keep *_selectedPatMuons*_*_*',
    'keep *_semilepMuons*_*_*',
    'keep *_selectedPatTaus*_*_*',
    'keep *_semilepElectrons*_*_*',
    'keep *_selectedPatJets*_*_*',
    'keep *_selectedPatJets*_genJets_*',
    'drop *_selectedPatJets*_pfCandidates_*',
    'drop *_selectedPatJetsForMETtype1p2CorrPF_*_*',
    'drop *_selectedPatJetsForMETtype2CorrPF_*_*',
    'drop CaloTowers_*_*_*',
    'keep *_patMETs*_*_*',
    'keep recoGenParticles_genParticles_*_*',
    'keep GenEventInfoProduct_*_*_*',
    'keep GenRunInfoProduct_*_*_*',
    'keep *_offlineBeamSpot_*_*',
    'keep *_goodOfflinePrimaryVertices_*_*',
    'keep edmTriggerResults_TriggerResults__HLT*',
    'keep edmTriggerResults_TriggerResults__REDIGI*',
    'keep edmTriggerResults_TriggerResults__PAT', # for post-tuple filtering on the goodData paths
    'keep *_hltTriggerSummaryAOD__HLT*',
    'keep *_hltTriggerSummaryAOD__REDIGI*',
      ## genEvt
    'keep *_decaySubset_*_*',
    'keep *_initSubset_*_*',
    'keep *_genEvt_*_*',
    'keep *_patTrigger*_*_*',
    )"""
process.outp = cms.EndPath(process.out)
