
1	import FWCore.ParameterSet.Config as cms
2	
3	
4	neutralPFCandidatesProducer = cms.EDProducer('NeutralPFCandidates',
5	   inputPFcandidates         = cms.InputTag("particleFlow")
6	)
