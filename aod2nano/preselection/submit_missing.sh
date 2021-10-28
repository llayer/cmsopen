#!/bin/bash

# Define path for job directories
BASE_PATH=$PWD/out
#BASE_PATH=/path/to/job/directory
mkdir -p $BASE_PATH

# Set processes
PROCESSES=( \
    #test \
    missing_Run2011A_MultiJet \
    missing_Run2011B_MultiJet \
    missing_TTJets \
    missing_WJetsToLNu \
    missing_DYJetsToLL \
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
