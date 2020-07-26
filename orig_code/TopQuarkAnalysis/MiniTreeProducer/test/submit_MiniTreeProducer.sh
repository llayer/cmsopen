OR#!/bin/bash
#
# Launch the minitree production
# A.-C. 20/11/2009
#
# 
#./submit_topquarkanalysis.sh  Wtaunu /Wtaunu/Summer09-MC_31X_V3-v1/GEN-SIM-RECO  
#
#i.e. ./submit_topquarkanalysis.sh "EventType" "datasetname"  
#


echo "============================================================================================================"
echo " Creating minifiles for EventType " $1 "and DataSet " $2
echo "  "


#------------------------------------------
# Updating "EventType" in MiniTree_PATwithPF2PAT_forMC_AOD_cfg.py
#

#EventTypeline=`sed -n /EventType/p MiniTree_PATwithPF2PAT_forMC_AOD_cfg.py`
#OldEventType=`echo $EventTypeline | awk -F\" '{print $2}'`
#sed s/$OldEventType/$1/g  MiniTree_PATwithPF2PAT_forMC_AOD_cfg.py > temp1
#mv temp1 MiniTree_PATwithPF2PAT_forMC_AOD_cfg.py


#------------------------------------------
# Updating crab_MiniTreeProducer.cfg  

DataSetline=`sed -n /datasetpath/p crab_MiniTreeProducer.cfg`
OldDataSet=`echo $DataSetline | awk -F\= '{print $2}'`
OldDataSet=`echo $OldDataSet | sed s/\ //g`

sed s@$OldDataSet@$2@g  crab_MiniTreeProducer.cfg > temp2
mv temp2 crab_MiniTreeProducer.cfg

uidirline=`sed -n /ui_working_dir/p crab_MiniTreeProducer.cfg`
Olduidir=`echo $uidirline | awk -F\= '{print $2}'`
OldDuidir=`echo $Olduidir | sed s/\ //g`

Newuidir=crab_pdfCWR_NNPDF10_$1
echo $Newuidir
echo $Olduidir
sed s@$Olduidir@$Newuidir@g  crab_MiniTreeProducer.cfg > temp3
mv temp3 crab_MiniTreeProducer.cfg


#RemoteDirline=`sed -n /user_remote_dir/p crab_MiniTreeProducer.cfg`
#OldRemoteDir=`echo $RemoteDirline | awk -F\= '{print $2}'`
#OldRemoteDir=`echo $OldRemoteDir | sed s/\ //g`

#sed s@$OldRemoteDir@$3@g  crab_MiniTreeProducer.cfg > temp3
#mv temp3 crab_MiniTreeProducer.cfg


#------------------------------------------
# Submitting with crab  

crab -create -cfg crab_MiniTreeProducer.cfg
#crab -submit -c $Newuidir


