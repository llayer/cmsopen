import sys
import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

########################################################################
#################### Passed arguments ##################################
########################################################################
#
# can be invoked with no parameters passed, in this case use default values
#

maxEvents = -1 #100

########################################################################
#################### Setup process #####################################
########################################################################

process = cms.Process('PAT')
# load needed tools
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Global tag for 2011 dat
# Before should be done:
# ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db START53_LV6A1.db
# ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1 START53_LV6A1
print "Set global tag for MC"
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db')
process.GlobalTag.globaltag = 'START53_LV6A1::All'


process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvents))

#files = ["file:/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/analyzer/tt.root"]
files = ["root://eospublic.cern.ch//eos/opendata/cms/MonteCarlo2011/Summer11LegDR/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S13_START53_LV6-v1/00000/7CE6694B-30C8-E311-A567-002590A371AC.root"]

process.source = cms.Source(
    "PoolSource", fileNames=cms.untracked.vstring(*files))

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(limit = cms.untracked.int32(-1))

# Output
process.out = cms.OutputModule('PoolOutputModule',
                               fileName = cms.untracked.string('pat.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('taujet')),
                               outputCommands = cms.untracked.vstring('keep *'),
                               )

####################################
#  Produce PDF weights
#  To speed up the processing
#  Interactivaly, before staring CMSSW
#  do "scramv1 setup lhapdffull; scramv1 b;"
#  with crab, ass these two lines after
#  eval `scramv1 runtime -sh in crab_.../job/CMSSW.sh
####################################


#text_file = open("evlist.txt", "r")
text_file = open("/afs/cern.ch/work/l/llayer/CMSSW_5_3_32/src/workspace/pdfweights/TTJets_ids.txt", "r")
lines = text_file.read().split('\n')
print lines[:5]
text_file.close()

process.pdffilter = cms.EDFilter("PDFFilter",
    eventlist = cms.vstring(lines) #276938, 276940)
    )


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



process.pdfana = cms.EDAnalyzer('EventBasicsAnalyzer')


####################################
#  Output content
####################################

process.taujet = cms.Path(process.pdffilter + process.pdfWeights + process.pdfana)

#cms.Path(process.taujet)
process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('taujet'))
process.out.outputCommands = cms.untracked.vstring(
    'drop *'
    )
    #'drop *',
    #'keep *_nEventsTotal_*_*',
    #'keep *_nEventsFiltered_*_*')

process.TFileService = cms.Service("TFileService", fileName = cms.string("pdf.root") )

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
