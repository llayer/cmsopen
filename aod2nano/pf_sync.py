import sys
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import os

cmssw_base = os.environ['CMSSW_BASE']
print(cmssw_base)

########################################################################
#################### Passed arguments ##################################
########################################################################
#
# can be invoked with no parameters passed, in this case use default values
#
if len(sys.argv) > 2:
    runOnMC = int(sys.argv[2])
else:
  print("Usage: cmsRun analyzer_cfg.py <mc flag> <skim tau flag>")
  runOnMC = 1
  skim = 1
  # do not stop execution at this point, run with default arguments
  #sys.exit("Wrong usage!")


print "Run on MC = ", runOnMC
maxEvents = -1
pfMuonIsoConeR03 = True
pfElectronIsoConeR03 = True
#skim = False
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
    files = ["file:data.root"]#  "root://eospublic.cern.ch//eos/opendata/cms/Run2011A/MultiJet/AOD/12Oct2013-v1/20001/BE90B0AD-EF4B-E311-8429-003048F010A2.root"]
else:
    #files = ["root://eospublic.cern.ch//eos/opendata/cms/MonteCarlo2011/Summer11LegDR/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S13_START53_LV6-v1/00000/0005D1FB-4BCF-E311-9FE4-002590A8312A.root"]
    files = ["file:TTJets.root"] # wjets.root"] #

process.source = cms.Source(
    "PoolSource", fileNames=cms.untracked.vstring(*files))

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

# JSON (good luminosity sections), only if processing data
if runOnMC == 0:
    print "Load lumi"
    goodJSON = cmssw_base+'/src/cmsopen/aod2nano/data/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
    #goodJSON = '/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pattuples2011/data/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON_truncated.txt'
    print goodJSON
    myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')
    #print myLumis
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
# Trigger matching
#

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )

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



####################################
#  Event cleaning
####################################
#Noise filter
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')

process.scrapingFilter = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25)
)

"""
process.eventCleaningData = cms.Sequence(
  scrapingFilter
)

process.eventCleaningMC = cms.Sequence(
)

process.eventCleaning = cms.Sequence()

  HBHENoiseFilter
+ CSCTightHaloFilter
+ hcalLaserEventFilter
+ EcalDeadCellTriggerPrimitiveFilter
+ eeBadScFilter
+ trackingFailureFilter
)
"""

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

# Additional variables
if pfMuonIsoConeR03:
  getattr( process, 'pfIsolatedMuons' + postfix ).isolationValueMapsCharged  = cms.VInputTag( cms.InputTag( 'muPFIsoValueCharged03' + postfix ))
  getattr( process, 'pfIsolatedMuons' + postfix ).deltaBetaIsolationValueMap = cms.InputTag( 'muPFIsoValuePU03' + postfix )
  getattr( process, 'pfIsolatedMuons' + postfix ).isolationValueMapsNeutral  = cms.VInputTag( cms.InputTag( 'muPFIsoValueNeutral03' + postfix )
                                                                                                , cms.InputTag( 'muPFIsoValueGamma03' + postfix ))
  getattr( process, 'pfMuons' + postfix ).isolationValueMapsCharged  = cms.VInputTag( cms.InputTag( 'muPFIsoValueCharged03' + postfix ))
  getattr( process, 'pfMuons' + postfix ).deltaBetaIsolationValueMap = cms.InputTag( 'muPFIsoValuePU03' + postfix )
  getattr( process, 'pfMuons' + postfix ).isolationValueMapsNeutral  = cms.VInputTag( cms.InputTag( 'muPFIsoValueNeutral03' + postfix )
                                                                                        , cms.InputTag( 'muPFIsoValueGamma03' + postfix ))
  getattr( process, 'patMuons' + postfix ).isolationValues.pfNeutralHadrons   = cms.InputTag( 'muPFIsoValueNeutral03' + postfix )
  getattr( process, 'patMuons' + postfix ).isolationValues.pfChargedAll       = cms.InputTag( 'muPFIsoValueChargedAll03' + postfix )
  getattr( process, 'patMuons' + postfix ).isolationValues.pfPUChargedHadrons = cms.InputTag( 'muPFIsoValuePU03' + postfix )
  getattr( process, 'patMuons' + postfix ).isolationValues.pfPhotons          = cms.InputTag( 'muPFIsoValueGamma03' + postfix )
  getattr( process, 'patMuons' + postfix ).isolationValues.pfChargedHadrons   = cms.InputTag( 'muPFIsoValueCharged03' + postfix )

if pfElectronIsoConeR03:
  getattr( process, 'pfIsolatedElectrons' + postfix ).isolationValueMapsCharged  = cms.VInputTag( cms.InputTag( 'elPFIsoValueCharged03PFId' + postfix ))
  getattr( process, 'pfIsolatedElectrons' + postfix ).deltaBetaIsolationValueMap = cms.InputTag( 'elPFIsoValuePU03PFId' + postfix )
  getattr( process, 'pfIsolatedElectrons' + postfix ).isolationValueMapsNeutral  = cms.VInputTag( cms.InputTag( 'elPFIsoValueNeutral03PFId' + postfix )
                                                                                                    , cms.InputTag( 'elPFIsoValueGamma03PFId'   + postfix ))
  getattr( process, 'pfElectrons' + postfix ).isolationValueMapsCharged  = cms.VInputTag( cms.InputTag( 'elPFIsoValueCharged03PFId' + postfix ))
  getattr( process, 'pfElectrons' + postfix ).deltaBetaIsolationValueMap = cms.InputTag( 'elPFIsoValuePU03PFId' + postfix )
  getattr( process, 'pfElectrons' + postfix ).isolationValueMapsNeutral  = cms.VInputTag( cms.InputTag( 'elPFIsoValueNeutral03PFId' + postfix )
                                                                                            , cms.InputTag( 'elPFIsoValueGamma03PFId'   + postfix ))
  getattr( process, 'patElectrons' + postfix ).isolationValues.pfNeutralHadrons   = cms.InputTag( 'elPFIsoValueNeutral03PFId' + postfix )
  getattr( process, 'patElectrons' + postfix ).isolationValues.pfChargedAll       = cms.InputTag( 'elPFIsoValueChargedAll03PFId' + postfix )
  getattr( process, 'patElectrons' + postfix ).isolationValues.pfPUChargedHadrons = cms.InputTag( 'elPFIsoValuePU03PFId' + postfix )
  getattr( process, 'patElectrons' + postfix ).isolationValues.pfPhotons          = cms.InputTag( 'elPFIsoValueGamma03PFId' + postfix )
  getattr( process, 'patElectrons' + postfix ).isolationValues.pfChargedHadrons   = cms.InputTag( 'elPFIsoValueCharged03PFId' + postfix )

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
if runOnMC:

    print "Produce pdf weights"
    process.pdfWeights = cms.EDProducer("PdfWeightProducer",
                # Fix POWHEG if buggy (this PDF set will also appear on output,
                # so only two more PDF sets can be added in PdfSetNames if not "")
                # FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
                #GenTag = cms.untracked.InputTag("genParticles"),
                PdfInfoTag = cms.untracked.InputTag("generator"),
                PdfSetNames = cms.untracked.vstring(
                    #"cteq66.LHgrid",
                    #"NNPDF10_100.LHgrid"
                    "cteq66.LHgrid",
                    #"NNPDF10_100.LHgrid"
                )
    )
"""

####################################
##########  NanoAOD ################
####################################

if runOnMC:
    isData = False
    prefilter = True
    prefilter_tau_trigger = False
    prefilter_jet_trigger = False
    jet_cut_pt = 10.
    tau_cut_pt = 10.
else:
    isData = True
    prefilter = True
    prefilter_tau_trigger = False
    prefilter_jet_trigger = False
    jet_cut_pt = 10.
    tau_cut_pt = 10.

process.MyModule = cms.EDAnalyzer('TopTauAnalyze',
    isData = cms.bool(isData),
    inFile = cms.string(files[0]),
    jecUncName = cms.string(cmssw_base + '/src/cmsopen/aod2nano/JEC/START53_LV6A1_Uncertainty_AK5PF.txt'),
    prefilter = cms.bool(prefilter),
    prefilter_jet_trigger = cms.bool(prefilter_jet_trigger),
    prefilter_tau_trigger = cms.bool(prefilter_tau_trigger),
    electron_cut_pt     = cms.double(10),
    electron_cut_eta    = cms.double(2.5),
    muon_cut_pt         = cms.double(10),
    muon_cut_eta        = cms.double(2.5),
    tau_cut_pt      = cms.double(tau_cut_pt),
    tau_cut_eta     = cms.double(2.5),    #!! original is 2.4
    jet_cut_pt      = cms.double(jet_cut_pt),
    jet_cut_eta     = cms.double(2.5),
    verbose = cms.bool(True)
    )
#jecUncName = cms.FileInPath('PhysObjectExtractorTool/PhysObjectExtractor/JEC/'+JecString+'Uncertainty_AK5PF.txt',
#jerResName = cms.FileInPath('PhysObjectExtractorTool/PhysObjectExtractor/JEC/JetResolutionInputAK5PF.txt',

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

process.taujet = cms.Path()
process.taujet += process.nEventsTotal
process.taujet += process.goodOfflinePrimaryVertices

event_cleaning = True
if event_cleaning:
    process.taujet += process.HBHENoiseFilter
    if runOnMC == False:
        print "add scraping filter for data"
        process.taujet += process.scrapingFilter
process.taujet += process.patPF2PATSequencePF
process.taujet += process.nEventsFiltered

if "TTJets" in files[0]:
    process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")
    process.taujet += process.makeGenEvt
    print "Create gen event"

process.taujet += process.MyModule

#cms.Path(process.taujet)
process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('taujet'))
process.out.outputCommands = cms.untracked.vstring(
    'drop *'
    )

process.TFileService = cms.Service("TFileService", fileName = cms.string("nano.root") )
process.outp = cms.EndPath(process.out)
