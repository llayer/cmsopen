
## Setup
On lxplus a singularity image has to be loaded (https://cms-sw.github.io/singularity.html)
```
cmssw-slc6
export SCRAM_ARCH=slc6_amd64_gcc472
source /cvmfs/cms.cern.ch/cmsset_default.sh
```
The relevant CMSSW version can be checked out with the command:
```
cmsrel CMSSW_5_3_32
```
The environment can then be loaded via:
```
cd CMSSW_5_3_32/src/
cmsenv
```
and the code can be compiled via:
```
scram b -j8
```
Additionally the following symlinks have to be set:
```
# Data
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA FT_53_LV5_AN1
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db FT_53_LV5_AN1_RUNA.db
#MC
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1.db START53_LV6A1.db
ln -sf /cvmfs/cms-opendata-conddb.cern.ch/START53_LV6A1 START53_LV6A1
```
Locally this can be tested with:
```
cmsRun pf_sync.py
```
