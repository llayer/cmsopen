# cmsopen
Repository for the analysis of CMS open data 

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
