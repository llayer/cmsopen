

#!/bin/bash
#
# Launch the full minitree production
# A.-C. 20/11/2009
#
#
#lumiCalc.py -c frontier://LumiCalc/CMS_LUMI_PROD -i crab_0_110701_101306/res/lumiSummary.json --nowarning overview -hltpath HLT_QuadJet40_IsoPFTau40

#lumiCalc2.py -i crab_0_110713_181302/res/lumiSummary.json --nowarning overview -hltpath HLT_QuadJet40_IsoPFTau40
#lumiCalc2.py -i crab_0_110714_142936/res/lumiSummary.json --nowarning overview -hltpath HLT_QuadJet40_IsoPFTau40
#pixelLumiCalc.py -i lumiSummary_428p7_3-6fb_crab.json --nowarning overview 

eval `scramv1 runtime -sh`
#source /opt/sbg/data/data1/sbg/ypatois/tmp/anne-problem/CMSSW_4_2_5/CRAB/CRAB_2_7_8_patch1/crab.sh 
source /opt/exp_soft/cms/CRAB/CRAB_2_8_2/crab.sh

#
#compareJSON.py --and lumiSummary.json lumiSummary_truncated.json 
#filterJSON.py --min 140380 --max 141220 old.json --output new.json
#filterJSON.py --min 140380 --max 141220 --runs 140381,140385 --runs 140388 old.json --output new.json
#https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGoodLumiSectionsJSONFile
#printJSON.py

#-------------------------------------------------------------------------------------------------------------
#------- Fall 11 
#./submit_MiniTreeProducer.sh TTbar /TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM


#------- Summer 11

./submit_MiniTreeProducer.sh TTbar      /TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh WJetsToLNu /WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh DYJetsToLL /DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM

#./submit_MiniTreeProducer.sh singleToptW    /T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM          
#./submit_MiniTreeProducer.sh singleTopbartW /Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM          
#./submit_MiniTreeProducer.sh singleTopt     /T_TuneZ2_t-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM        
#./submit_MiniTreeProducer.sh singleTopbart  /Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM         
#./submit_MiniTreeProducer.sh singleTops     /T_TuneZ2_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM         
#./submit_MiniTreeProducer.sh singleTopbars  /Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM         
#
#
##------- Summer 11 systematics

##./submit_MiniTreeProducer.sh TTbar_mass161_5 /TTJets_TuneZ2_mass161_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM
##./submit_MiniTreeProducer.sh TTbar_mass163_5 /TTJets_TuneZ2_mass163_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM
#./submit_MiniTreeProducer.sh TTbar_mass166_5 /TTJets_TuneZ2_mass166_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM
##./submit_MiniTreeProducer.sh TTbar_mass169_5 /TTJets_TuneZ2_mass169_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM
##./submit_MiniTreeProducer.sh TTbar_mass175_5 /TTJets_TuneZ2_mass175_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM
#./submit_MiniTreeProducer.sh TTbar_mass178_5 /TTJets_TuneZ2_mass178_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM
##./submit_MiniTreeProducer.sh TTbar_mass181_5 /TTJets_TuneZ2_mass181_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM
##./submit_MiniTreeProducer.sh TTbar_mass184_5 /TTJets_TuneZ2_mass184_5_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v3/AODSIM 
    
#./submit_MiniTreeProducer.sh TTbar_matchingdown /TTjets_TuneZ2_matchingdown_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh TTbar_scaleup     /TTjets_TuneZ2_scaleup_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh TTbar_matchingup   /TTjets_TuneZ2_matchingup_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh TTbar_scaledown    /TTjets_TuneZ2_scaledown_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM 

#./submit_MiniTreeProducer.sh T_TuneZ2_scaleup_tW    /T_TuneZ2_scaleup_tW-channel-DS_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh Tbar_TuneZ2_scaleup_tW /Tbar_TuneZ2_scaleup_tW-channel-DR_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh T_TuneZ2_scaledown_tW    /T_TuneZ2_scaledown_tW-channel-DS_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh Tbar_TuneZ2_scaledown_tW /Tbar_TuneZ2_scaledown_tW-channel-DR_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM

#./submit_MiniTreeProducer.sh T_TuneZ2_scaleup_s     /T_TuneZ2_scaleup_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh Tbar_TuneZ2_scaleup_s  /Tbar_TuneZ2_scaleup_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh T_TuneZ2_scaleup_t     /T_TuneZ2_scaleup_t-channel-DR_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh Tbar_TuneZ2_scaleup_t  /Tbar_TuneZ2_scaleup_t-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM

#./submit_MiniTreeProducer.sh T_TuneZ2_scaledown_s     /T_TuneZ2_scaledown_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh Tbar_TuneZ2_scaledown_s  /Tbar_TuneZ2_scaledown_s-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh T_TuneZ2_scaledown_t     /T_TuneZ2_scaledown_t-channel-DR_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh Tbar_TuneZ2_scaledown_t  /Tbar_TuneZ2_scaledown_t-channel_7TeV-powheg-tauola/Summer11-PU_S4_START42_V11-v1/AODSI
#
#
#./submit_MiniTreeProducer.sh  ZJetsToLL_TuneZ2_scaleup      /ZJetsToLL_TuneZ2_scaleup_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh  ZJetsToLL_TuneZ2_scaledown    /ZJetsToLL_TuneZ2_scaledown_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh  ZJetsToLL_TuneZ2_matchingup   /ZJetsToLL_TuneZ2_matchingup_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh  ZJetsToLL_TuneZ2_matchingdown /ZJetsToLL_TuneZ2_matchingdown_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM 

#./submit_MiniTreeProducer.sh  WJetsToLNu_TuneZ2_scaleup       /WJetsToLNu_TuneZ2_scaleup_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh  WJetsToLNu_TuneZ2_scaledown     /WJetsToLNu_TuneZ2_scaledown_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh  WJetsToLNu_TuneZ2_matchingup    /WJetsToLNu_TuneZ2_matchingup_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM
#./submit_MiniTreeProducer.sh  WJetsToLNu_TuneZ2_matchingdown  /WJetsToLNu_TuneZ2_matchingdown_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM 
#
#
#
#------ not needed yet

#./submit_MiniTreeProducer.sh DYToMuMu_M-20   /DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh DYToEE_M-20     /DYToEE_M-20_TuneZ2_7TeV-pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh DYToTauTau_M-20 /DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh DYToTauTau_M-20 /DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM 


#./submit_MiniTreeProducer.sh  QCD0to5      /QCD_Pt-0to5_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD5to15     /QCD_Pt-5to15_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM

#./submit_MiniTreeProducer.sh  QCD15to30    /QCD_Pt-15to30_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD30to50    /QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD50to80    /QCD_Pt-50to80_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD80to120   /QCD_Pt-80to120_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD120to170  /QCD_Pt-120to170_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD170to300  /QCD_Pt-170to300_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD300to470  /QCD_Pt-300to470_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD470to600  /QCD_Pt-470to600_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM

#./submit_MiniTreeProducer.sh  QCD600to800    /QCD_Pt-600to800_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD800to1000   /QCD_Pt-800to1000_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD1000to1400  /QCD_Pt-1000to1400_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD1400 to1800 /QCD_Pt-1400to1800_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#./submit_MiniTreeProducer.sh  QCD1800        /QCD_Pt-1800_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM



