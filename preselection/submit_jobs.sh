#!/bin/bash

# Define path for job directories
BASE_PATH=$PWD/out
#BASE_PATH=/path/to/job/directory
mkdir -p $BASE_PATH

# Set processes
PROCESSES=( \
    #test \
    #Run2011A_MultiJet \
    #Run2011B_MultiJet \
    #Missing_Run2011A_MultiJet \
    #Run2011A_SingleMu \
    #Run2011B_SingleMu \
    TTJets \
    WJetsToLNu \
    DYJetsToLL \
    T_TuneZ2_s \
    T_TuneZ2_tW \
    T_TuneZ2_t-channel \
    Tbar_TuneZ2_s \
    Tbar_TuneZ2_tW \
    Tbar_TuneZ2_t-channel \
    )

# Create JDL files and job directories
for PROCESS in ${PROCESSES[@]}
do
    ls -d /eos/user/l/llayer/opendata_files/legacy_id/$PROCESS/* > data/$PROCESS.txt
    python create_job.py $PROCESS $BASE_PATH
done

# Submit jobs
THIS_PWD=$PWD
for PROCESS in ${PROCESSES[@]}
do
    cd $BASE_PATH/$PROCESS
    condor_submit job.jdl
    cd $THIS_PWD
done
