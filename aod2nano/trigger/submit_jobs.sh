#!/bin/bash
CMSSW_BASE=/afs/cern.ch/work/l/llayer/legacy/CMSSW_5_3_32
# Define path for job directories
BASE_PATH=$CMSSW_BASE/src/cmsopen/aod2nano/trigger/jobs
#BASE_PATH=/path/to/job/directory
mkdir -p $BASE_PATH

# Set processes
PROCESSES=( \
    Run2011A_MultiJet \
    Run2011B_MultiJet \
    Run2011A_SingleMu \
    Run2011B_SingleMu \
    )

# Create JDL files and job directories
for PROCESS in ${PROCESSES[@]}
do
    python create_job.py $PROCESS $BASE_PATH
done

# Submit jobs
#THIS_PWD=$PWD
#for PROCESS in ${PROCESSES[@]}
#do
#    cd $BASE_PATH/$PROCESS
#    condor_submit job.jdl
#    cd $THIS_PWD
#done
