#!/bin/bash

# Define path for job directories
BASE_PATH=//afs/cern.ch/work/l/llayer/legacy/CMSSW_5_3_32/src/cmsopen/aod2nano/pdfweights/jobs
#BASE_PATH=/path/to/job/directory
mkdir -p $BASE_PATH

# Set processes
PROCESSES=( \
    #testTT \
    #test
    TTJets \
    #missing_TTJets \
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
