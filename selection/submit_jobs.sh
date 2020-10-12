#!/bin/bash

# Define path for job directories
BASE_PATH=/afs/cern.ch/work/l/llayer/CMSSW_10_2_18/src/out
#BASE_PATH=/path/to/job/director
mkdir -p $BASE_PATH

# Type of job
#OB_TYPE="selection"
JOB_TYPE="trigger"

# Set processes
PROCESSES=( \
    #Run2011A_MultiJet \
    Missing_Run2011A_MultiJet \
    #Run2011B_MultiJet \
    #Run2011A_SingleMu \
    #Run2011B_SingleMu \
    #Run2011A_MultiJet_Trigger \
    #Run2011B_MultiJet_Trigger \
    #Run2011A_SingleMu_Trigger \
    #Run2011B_SingleMu_Trigger \
    #TTJets \
    #WJetsToLNu \
    #DYJetsToLL \
    #T_TuneZ2_s \
    #T_TuneZ2_tW \
    #T_TuneZ2_t-channel \
    #Tbar_TuneZ2_s \
    #Tbar_TuneZ2_tW \
    #Tbar_TuneZ2_t-channel \
    )

# Create JDL files and job directories
for PROCESS in ${PROCESSES[@]}
do
    #ls -d /eos/user/l/llayer/opendata_files/$PROCESS/* > data/$PROCESS.txt
    python create_job.py $PROCESS $BASE_PATH $JOB_TYPE
done

# Submit jobs
#THIS_PWD=$PWD
#for PROCESS in ${PROCESSES[@]}
#do
#    cd $BASE_PATH/$PROCESS
#    condor_submit job.jdl
#    cd $THIS_PWD
#done
