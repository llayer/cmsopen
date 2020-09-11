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
    input = cms.untracked.int32(-1)
)

## Define the TFileService
process.TFileService = cms.Service("TFileService",
fileName = cms.string('nano.root')
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
process.analyzeTau = cms.EDAnalyzer('TopTauAnalyze',
    isData = cms.bool(False),
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

#process.p = cms.Path(process.Top)
process.p1 = cms.Path( process.analyzeTau)
