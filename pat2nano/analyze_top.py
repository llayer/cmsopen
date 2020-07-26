import FWCore.ParameterSet.Config as cms

## Define the process
process = cms.Process("Top")

## Define the input sample
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:pat2.root'
  )
)
## restrict the number of events for testing
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
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
"""
process.analyzeTau = cms.EDAnalyzer("TopTauAnalyze",
    vertices = cms.InputTag("goodOfflinePrimaryVertices"),
    muons = cms.InputTag("selectedPatMuonsPF"),
    electrons = cms.InputTag("selectedPatElectronsPF"),
    taus = cms.InputTag("selectedPatTausPF"),
    met = cms.InputTag("patMETsPF"),
    jets = cms.InputTag("selectedPatJetsPF"),
    verbose = cms.bool(True)
)"""
process.analyzeTau = cms.EDAnalyzer("TopTauAnalyze",
    trigger = cms.InputTag("TriggerResults::HLT"),
    patTriggerEvent = cms.InputTag("patTriggerEventPF"),
    taus = cms.InputTag("selectedPatTausPF"),
    jets = cms.InputTag("selectedPatJetsPF"),
    muons = cms.InputTag("selectedPatMuonsPF"),
    electrons = cms.InputTag("selectedPatElectronsPF"),
    vertices = cms.InputTag("goodOfflinePrimaryVertices"),
    met   = cms.InputTag("patMETsPF"),
    genEvent = cms.InputTag("genEvt"),
    verbose = cms.bool(True)
)

#process.p = cms.Path(process.Top)
process.p1 = cms.Path( process.analyzeTau)

