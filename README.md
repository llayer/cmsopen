
[![DOI](https://zenodo.org/badge/274393227.svg)](https://zenodo.org/badge/latestdoi/274393227)

# INFERNO for CMS Open Data
Repository for the analysis of CMS open data

## 1. NanoAOD production
The first step is to write out only the relevant information from the Open Data CMS AOD format. 
On top a preselection can be applied that keeps only events that fulfill certain kinematic criteria.

## 2. Columnar analysis of NanoAOD
The second step includes the calculation of efficiencies, final eventselection and various corrections to improve
the data / MC agreement. 

## 3. Inference
The inference is done with the CMS combine tool.

## 4. INFERNO training
For the INFERNO training the pytorch_inferno package is used.

## 5. Comparison
The final step of this study is the comparison of inference based on a NN trained with a BCE loss and inference based on the INFERNO output.
