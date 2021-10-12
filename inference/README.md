To setup the inference one has to install combine (https://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/):

```
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_2_18
cd CMSSW_10_2_13/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
cd $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v8.2.0
scramv1 b clean; scramv1 b # always make a clean build
``` 
and combine harvester package:
```
cd CMSSW_10_2_13/src
cmsenv
git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
scram b
```


