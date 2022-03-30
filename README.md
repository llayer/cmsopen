[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![DOI](https://zenodo.org/badge/274393227.svg)](https://zenodo.org/badge/latestdoi/274393227)

# INFERNO for CMS Open Data
Repository for the study of the INFERNO algorithm (P. de Castro and T. Dorigo, Comp.\ Phys.\ Comm.\ 244 (2019) 170) with CMS Open Data. In order to test and benchmark the algorithm in a real-world application, a full, systematics-dominated analysis of the CMS experiment, “Measurement of the $\mathrm{t}\bar{\mathrm{t}}$ production cross section in the $\tau$+jets channel in pp collisions at $\sqrt{s}=7\ \mbox{TeV}$” (CMS Collaboration, The European Physical Journal C, 2013) is reproduced with CMS Open Data. The application of the INFERNO-powered neural network architecture to this analysis demonstrates the potential to reduce the impact of systematic uncertainties in real LHC analysis.


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
