import FWCore.ParameterSet.Config as cms

MiniTreeProduction = cms.EDProducer('MiniTreeProducer',
                verbose             = cms.int32(1), #0: nothing - >1 gradually more information
                TriggerMenu         = cms.InputTag("TriggerResults::REDIGI"),    # or TriggerResults::HLT8E29  (for the old synchro set of 2 files) 
                TriggerInfo         = cms.bool(True),
                saveAllTriggers     = cms.bool(False),#should be False by default !!
                triggerList         = cms.vstring("HLT_Mu15_L1Mu7","HLT_DoubleMu3","HLT_IsoEle10_Mu10_L1R","HLT_IsoEle18_L1R","HLT_DoubleIsoEle12_L1R","HLT_Mu5","HLT_Mu9","HLT_Mu11","HLT_Mu15","HLT_IsoMu9","HLT_Ele10_SW_L1R","HLT_Ele15_SW_L1R","HLT_Ele15_LW_L1R","HLT_Ele10_LW_L1R","HLT_DoubleEle5_SW_L1R","HLT_LooseIsoEle15_LW_L1R","HLT_L2Mu3","HLT_L2Mu5","HLT_L2Mu9","HLT_Jet15U","HLT_Photon10_L1R","HLT_Photon15_L1R","HLT_Photon10_Cleaned_L1R","HLT_Photon15_Cleaned_L1R","HLT_Ele15_SW_CaloEleId_L1R","HLT_Ele20_SW_L1R","HLT_DoubleEle10_SW_L1R"),
                EventType           = cms.InputTag("Zjets"),
                SampleName          = cms.string('top'),
                SampleCrossSection  = cms.double(9999.),
                doElectrons         = cms.bool(True),
                electronProducer    = cms.InputTag("selectedPatElectrons"),
                electron_cut_pt     = cms.double(10),
                electron_cut_eta    = cms.double(2.5),
                electron_cut_trkIso = cms.double(-99),
                doMuons             = cms.bool(True),
                muonProducer        = cms.InputTag("selectedPatMuons"),
                muon_cut_pt         = cms.double(10),
                muon_cut_eta        = cms.double(2.5),
                muon_cut_trkIso     = cms.double(-99),
                doTaus              = cms.bool(True),
                tauProducer         = cms.InputTag("selectedPatTaus"),
                tau_cut_pt          = cms.double(10),
                tau_cut_eta         = cms.double(2.4),
                trackProducer       = cms.InputTag("generalTracks"),
                doTracks            = cms.bool(False),
                track_cut_pt        = cms.double(0.5),
                track_cut_eta       = cms.double(2.4),
                beamSpotProducer    = cms.InputTag("offlineBeamSpot"),
                jet_cut_pt_uncor    = cms.double(10),
                jet_cut_pt          = cms.double(10),
                jet_cut_eta         = cms.double(2.5),
                met_cut_pt          = cms.double(-99),
                saveAllVertex       = cms.bool(True),
                vertexProducer      = cms.InputTag("offlinePrimaryVertices"),
                jetmetProducer      = cms.VPSet(cms.PSet(jet = cms.untracked.string("selectedPatJets"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("ak5")),
#                                               cms.PSet(jet = cms.untracked.string("selectedPatJetsSC5Calo"), met = cms.untracked.string("patMETs"), algo =cms.untracked.string("sc5")),
                                               # cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5JPT"), met = cms.untracked.string("patMETsTC"), algo =cms.untracked.string("tc")),
                                                cms.PSet(jet = cms.untracked.string("selectedPatJetsAK5PF"), met = cms.untracked.string("patMETsPF"), algo =cms.untracked.string("pf"))
                                                )
        )
        
MinTreeProd = cms.Sequence(MiniTreeProduction)
        
        
