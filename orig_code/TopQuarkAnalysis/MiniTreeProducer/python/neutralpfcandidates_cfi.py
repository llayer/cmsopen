
import FWCore.ParameterSet.Config as cms


neutralPFCandidatesProducer = cms.EDProducer('NeutralPFCandidates',
   inputPFcandidates         = cms.InputTag("particleFlow")
)
