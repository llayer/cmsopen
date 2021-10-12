# Columnar analysis

## Setup
The columnar analysis was developed with the following package versions and on Softwarestack 97a with ROOT versionn '6.20/06' on the SWAN platform:

```
awkward==0.12.17
coffea==0.6.42
h5py==2.9.0
matplotlib==3.1.0
pandas==0.24.2
numpy==1.16.4
root-numpy==4.8.0
root-pandas==0.7.0
scikit-learn==0.21.2
scipy==1.3.0
seaborn==0.9.0
shap==0.39.0
tables==3.6.1
uproot==3.10.12
xgboost==0.90
```

## Event selection
The event selection can be done with the script 'proc.py'.

## Trigger efficiencies
The trigger efficiencies can be produced with 'trigger_eff.py'.

## B-tagging
The b-tagging corrections and procedures are accesible here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/BtagRecommendation2011OpenData#Data_MC_Scale_Factors.
The efficiencies can be found in the 'data' directory.

## JES/JER
The JES uncertainties can be found here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECUncertaintySources#2011_JEC_uncertainty_correlation and the JER here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#JER_Scaling_factors_and_Unce_AN2. 

## Cross-sections
The relevant cross-sections can be found here: https://github.com/cernopendata/opendata.cern.ch/issues/2476
