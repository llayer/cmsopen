import FWCore.ParameterSet.Config as cms

## Define the process
process = cms.Process("Top")

## Define the input sample
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:pat.root'
  )
)
## restrict the number of events for testing
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

## Define the TFileService
process.TFileService = cms.Service("TFileService",
fileName = cms.string('analyzePatTopSelection.root')
)

#from PatTopSelectionAnalyzer_cfi import *
#process.aod2nanoaod = cms.EDAnalyzer("PatTopSelectionAnalyzer")
#process.monStart  = analyzePatTopSelection.clone(jets='goodJets')
#process.Top = cms.EDAnalyzer("PatTopSelectionAnalyzer",
#    elecs = cms.untracked.InputTag("selectedPatElectrons"),
#    muons = cms.untracked.InputTag("selectedPatMuons"),
#    jets  = cms.untracked.InputTag("selectedPatJets"),
#    met   = cms.untracked.InputTag("patMETs"))

#process.load("TopTauAnalyzer_cfi")
process.analyzeTau = cms.EDAnalyzer("TopTauAnalyze",
    taus = cms.InputTag("selectedPatTausPF"),
    jets = cms.InputTag("selectedPatJetsPF"),
    verbose = cms.bool(True)
)

#process.p = cms.Path(process.Top)
process.p1 = cms.Path( process.analyzeTau)

