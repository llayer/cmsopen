import FWCore.ParameterSet.Config as cms

process  = cms.Process("Demo")

# Fake input for the input files
process.source = cms.Source ("PoolSource",
                             fileNames=cms.untracked.vstring
                             (#'file:/storage1/cms/lebihan/official2/CMSSW_4_1_4/src/TopQuarkAnalysis/MiniTreeProducer/test/patTuple.root'
                              #'rfio:/dpm/in2p3.fr/home/cms/phedex/store/user/jandrea/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/MiniTreeStrasbg_38X_pass6_WJets_V3/a6686bdad833c5cab2158cc4079f45a7/MiniTree_93_1_pJe.root'
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_10_1_mOo.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_11_1_nhS.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_12_1_Dax.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_13_1_4vH.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_14_1_5oB.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_15_1_id5.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_16_1_Sr0.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_17_1_BGZ.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_18_1_Kyr.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_19_1_1N6.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_1_1_TkC.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_20_1_6W5.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_21_1_wIn.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_22_1_j0J.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_23_1_sRp.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_24_1_xN4.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_25_1_Twv.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_26_1_0DM.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_27_1_0pC.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_28_1_Qqu.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_29_1_kO7.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_2_1_xvY.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_30_1_Zoh.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_31_1_pUo.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_32_1_MTT.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_33_1_AWa.root',
'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_34_1_C5i.root'

#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_35_1_NP3.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_36_1_5Gl.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_37_1_0EX.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_38_1_rc4.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_39_1_jnp.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_3_1_IxH.root', 
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_40_1_yFd.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_41_1_WOi.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_42_1_cGO.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_43_1_rDI.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_44_1_b0t.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_45_1_OP4.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_46_1_dg6.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_47_1_8xk.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_48_1_3PQ.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_49_1_oMl.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_4_1_bPS.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_50_1_Oyq.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_51_1_fa3.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_52_1_pFS.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_53_1_kGG.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_54_1_0rb.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_55_1_BvP.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_56_1_vNg.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_57_1_lJR.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_5_1_8pv.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_6_1_7oj.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_7_1_qHI.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_8_1_GtE.root',
#'file:/opt/sbg/data/data1/cms/lebihan/42b216abb44334e8e637e6eae018f497/patTuple_9_1_ORS.root'
)
                             )
                             
                             
process.TFileService = cms.Service("TFileService", fileName = cms.string("NTuple.root") )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

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
                
                doTMEMESkimming       = cms.bool(True), # skim on the TMEME
                #TMEMESkimList         = cms.vint32(),
                TMEMESkimList         = cms.vint32(10000),                                             # tau+jets
                #TMEMESkimList         = cms.vint32(2,10101,20200,20,11010,22000,11,11001,10110,21100),  # dilepton
                #TMEMESkimList         = cms.vint32(10010,21000,10001,20100),                           # lepton + hadronic tau
                #TMEMESkimList         = cms.vint32(0),                                                 # full hadronic
                #TMEMESkimList         = cms.vint32(20000),                                             # di-tau hadronic
                #TMEMESkimList         = cms.vint32(1,10,10100,11000),                                  # lepton+jets
                
                doMCDiLepSkimming     = cms.bool(False),
                MCDiLepList           = cms.vstring(""),
                
                doTauSkimming         = cms.bool(True), # skim on the number of reco taus (no id so far)
                numberOfTau4Skim      = cms.int32(1),
                tau_cut_pt_skim       = cms.double(40),
                tau_algo_skim         = cms.string("selectedPatTausPF2PAT"),
                
                doTriggerSkimming     = cms.bool(False), # skim on trigger decisions
                triggerSkimList       = cms.vstring("HLT_QuadJet40_IsoPFTau40","HLT_QuadJet45_IsoPFTau45"),
                
                doJetSkimming         = cms.bool(True), # skim on the number of jets
                numberOfJet4Skim      = cms.int32(3),
                jet_cut_pt_skim       = cms.double(40), 
                jet_cut_eta_skim      = cms.double(2.5),
                jet_algo_skim         = cms.string("pf"),
                
                # ttbar-> tau+jets specific
                doTauJetAna_TauJetTrigSkimming  = cms.bool(False), # need to set doTrigger doTauSkimming doJetSkimming to true
                doTauJetAna_TauJetSkimming      = cms.bool(False),  # need to set doTauSkimming doJetSkimming to true
                doTauJetAna_TmemeTauJetSkimming = cms.bool(True) # need to set doTMEMESkimming doTauSkimming doJetSkimming to true, only for MC ttbar
             
                
        )


process.p = cms.Path(process.MyModule)


