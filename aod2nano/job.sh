#!/bin/bash

# Exit on error
set -e

echo "### Begin of job"

ID=$1
echo "ID:" $ID

PROCESS=$2
echo "Process:" $PROCESS

FILE=$3
echo "File:" $FILE

EOS_HOME=/eos/user/l/llayer
echo "EOS home:" $EOS_HOME

OUTPUT_DIR=${EOS_HOME}/opendata_files/legacy_id/
echo "Output directory:" $OUTPUT_DIR

CMSSW_BASE=/afs/cern.ch/work/l/llayer/legacy/CMSSW_5_3_32
echo "CMSSW base:" $CMSSW_BASE

CONFIG=${CMSSW_BASE}/src//workspace/pattuples2011/pf_sync.py

echo "CMSSW config:" $CONFIG

echo "Hostname:" `hostname`

echo "How am I?" `id`

echo "Where am I?" `pwd`

echo "What is my system?" `uname -a`

echo "### Start working"

# Trigger auto mount of EOS
ls -la $EOS_HOME

# Make output directory
mkdir -p ${OUTPUT_DIR}/${PROCESS}

# Setup CMSSW
THIS_DIR=$PWD
cd $CMSSW_BASE
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scramv1 runtime -sh`
cd $THIS_DIR

if [[ ${FILE} == *"Run2011"* ]]; then
  eval `ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA FT_53_LV5_AN1`
  eval `ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db FT_53_LV5_AN1_RUNA.db`
else
  eval `ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db START53_LV6A1.db`
  eval `ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1 START53_LV6A1`
  eval `scramv1 setup lhapdffull`
fi


# Copy config file
mkdir -p configs/
CONFIG_COPY=configs/cfg_${ID}.py
cp $CONFIG $CONFIG_COPY

# Modify CMSSW config to run only a single file
#sed -i -e "s,^files =,files = ['"${FILE}"'] #,g" $CONFIG_COPY
sed -i -e "s,^    files =,    files = ['"${FILE}"'] #,g" $CONFIG_COPY
sed -i -e 's,^files.extend,#files.extend,g' $CONFIG_COPY

# Modify CMSSW config to read lumi mask from EOS
#sed -i -e 's,data/Cert,'${CMSSW_BASE}'/src/workspace/AOD2NanoAOD/data/Cert,g' $CONFIG_COPY

FILEID=${FILE%.root}
HASH=$(basename "$FILEID")

# Modify config to write output directly to EOS
sed -i -e 's,nano.root,'${PROCESS}_${HASH}_${ID}.root',g' $CONFIG_COPY

# Print config
cat $CONFIG_COPY

# Run CMSSW config
#cmsRun $CONFIG_COPY 0

if [[ ${FILE} == *"Run2011"* ]]; then
    cmsRun $CONFIG_COPY 0
else
    cmsRun $CONFIG_COPY 1
fi

# Copy output file
xrdcp -f ${PROCESS}_${HASH}_${ID}.root root://eosuser.cern.ch/${OUTPUT_DIR}/${PROCESS}/${PROCESS}_${HASH}_${ID}.root
rm ${PROCESS}_${HASH}_${ID}.root

echo "### End of job"
