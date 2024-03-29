## NanoAOD production
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
With the scripts submit_jobs.sh this can be submitted to the lxplus condor batch system and with validate_jobs.py it can be checked whether all jobs finished succesfully

## Preselection
After the NanoAOD production the NanoAOD can be further skimmed by using the scripts in the folder "preselection".
The script 'pre_sel.py' can be used for a preselection of the analyzed events, while the script pre_trigger.py can be used to preselect events for the trigger measurement. The selected events can be merged with the script merge.py.

## Luminosity
The luminosity can be calculated with the brilcalc tool as described in http://opendata.cern.ch/docs/cms-guide-luminosity-calculation

## PDF weights
Calculating the PDF weights takes quite some time per event. To speed this up this is only calculated for the events that pass the final event selection.
Before running the PDF jobs one should execute:
```
#  Produce PDF weights
#  To speed up the processing
#  Interactivaly, before staring CMSSW
scramv1 setup lhapdffull; 
scramv1 b;
#  with crab, add these two lines after
# eval `scramv1 runtime -sh in crab_.../job/CMSSW.sh
```
