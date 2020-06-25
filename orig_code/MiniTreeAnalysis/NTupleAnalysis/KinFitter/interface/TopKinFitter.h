1	#ifndef TopKinFitter_h
2	#define TopKinFitter_h
3	
4	#include "TMath.h"
5	
6	#include "../interface/TKinFitter.h"
7	
8	/*
9	  \class   TopKinFitter TopKinFitter.h "TopQuarkAnalysis/TopKinFitter/interface/TopKinFitter.h"
10	  
11	  \brief   one line description to be added here...
12	
13	  text to be added here...
14	  
15	**/
16	
17	class TopKinFitter {
18	  
19	 public:
20	  
21	  /// supported parameterizations
22	  enum Param{ kEMom, kEtEtaPhi, kEtThetaPhi };
23	
24	 public:
25	  /// default constructor
26	  explicit TopKinFitter(const int maxNrIter=200, const double maxDeltaS=5e-5, const double maxF=1e-4,
27	                        const double mW=80.4, const double mTop=173.);
28	  /// default destructor
29	  ~TopKinFitter();
30	
31	  /// return chi2 of fit (not normalized to degrees of freedom)
32	  double fitS()  const { return fitter_->getS(); };
33	  /// return number of used iterations
34	  int fitNrIter() const { return fitter_->getNbIter(); };
35	  /// return fit probability
36	  double fitProb() const { return TMath::Prob(fitter_->getS(), fitter_->getNDF()); };
37	  
38	 protected:
39	  /// convert Param to human readable form
40	  std::string param(const Param& param) const;
41	  
42	 protected:
43	  // kinematic fitter
44	  TKinFitter* fitter_;
45	  /// maximal allowed number of iterations to be used for the fit
46	  int maxNrIter_;
47	  /// maximal allowed chi2 (not normalized to degrees of freedom)
48	  double maxDeltaS_;
49	  /// maximal allowed distance from constraints
50	  double maxF_;
51	  /// W mass value used for constraints
52	  double mW_;
53	  /// top mass value used for constraints
54	  double mTop_;
55	};
56	
57	#endif
