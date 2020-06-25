1	// Classname: TAbsFitConstraint
2	// Author: Jan E. Sundermann, Verena Klose (TU Dresden)      
3	
4	
5	//________________________________________________________________
6	// 
7	// TAbsFitConstraint::
8	// --------------------
9	//
10	// Abstract base class for fit constraints
11	//
12	
13	#include "../interface/TAbsFitConstraint.h"
14	#include <iostream>
15	#include <sstream>
16	#include <iomanip>
17	#include "TClass.h"
18	
19	
20	TAbsFitConstraint::TAbsFitConstraint() 
21	  : TNamed("NoName","NoTitle")
22	  ,_covMatrix()
23	  ,_covMatrixFit()
24	  ,_covMatrixDeltaAlpha()
25	  ,_iniparameters()
26	  ,_parameters()
27	
28	{
29	  _nPar = 0;
30	}
31	
32	TAbsFitConstraint::TAbsFitConstraint(const TString &name, const TString &title) 
33	  : TNamed(name, title)
34	  ,_covMatrix()
35	  ,_covMatrixFit()
36	  ,_covMatrixDeltaAlpha()
37	  ,_iniparameters()
38	  ,_parameters()
39	
40	{
41	  _nPar = 0;
42	}
43	
44	TAbsFitConstraint::~TAbsFitConstraint() {
45	
46	}
47	
48	void TAbsFitConstraint::reset() {
49	  // Reset parameters to initial values
50	
51	  _parameters = _iniparameters;  
52	  setCovMatrixFit( 0 );
53	
54	}
55	
56	void TAbsFitConstraint::setCovMatrix(const TMatrixD* theCovMatrix) {
57	  // Set measured alpha covariance matrix
58	
59	  _covMatrix.ResizeTo(_nPar, _nPar);
60	  if(theCovMatrix==0) {
61	    _covMatrix.Zero();
62	  } else if (theCovMatrix->GetNcols() ==_nPar && theCovMatrix->GetNrows() ==_nPar) {
63	    _covMatrix = (*theCovMatrix);
64	  } else {
65	    std::cout << "WrongMatrixSize " << 
66	      //<< GetName() << 
67	      "::setCovMatrix - Measured alpha covariance matrix needs to be a "
68	      << _nPar << "x" << _nPar << " matrix." << std::endl;
69	  }
70	
71	}
72	
73	void TAbsFitConstraint::setCovMatrixFit(const TMatrixD* theCovMatrixFit) {
74	  // Set the fitted covariance matrix
75	
76	  _covMatrixFit.ResizeTo(_nPar, _nPar);
77	  if(theCovMatrixFit==0) {
78	    _covMatrixFit.Zero();
79	  } else if (theCovMatrixFit->GetNcols() ==_nPar && theCovMatrixFit->GetNrows() ==_nPar) {
80	    _covMatrixFit = (*theCovMatrixFit);
81	  } else {
82	    std::cout << "WrongMatrixSize "
83	      << GetName() << "::setCovMatrixFit - Fitted covariance matrix needs to be a "
84	      << _nPar << "x" << _nPar << " matrix."<<std::endl;
85	  }
86	
87	}
88	
89	void TAbsFitConstraint::calcCovMatrixDeltaAlpha() {
90	  // Calculates V(deltaAlpha) ==  V(alpha_meas) - V(alpha_fit)
91	
92	  _covMatrixDeltaAlpha.ResizeTo( _nPar, _nPar );
93	  _covMatrixDeltaAlpha = _covMatrix;
94	  if(_covMatrixFit.GetNrows() == _nPar && _covMatrixFit.GetNcols() == _nPar)
95	    _covMatrixDeltaAlpha -= _covMatrixFit;
96	  else
97	    std::cout <<"WrongMatrixSize "
98	      << GetName() << "::calcCovMatrixDeltaAlpha - _covMatrixFit probably not set."<<std::endl;
99	}
100	
101	
102	void TAbsFitConstraint::applyDeltaAlpha(TMatrixD* corrMatrix) {
103	  // Apply corrections to the parameters wrt. to the
104	  // initial parameters alpha* = alpha + delta(alpha)
105	
106	  _parameters = _iniparameters;
107	  _parameters += (*corrMatrix);
108	
109	}
110	
111	void TAbsFitConstraint::setParIni(const TMatrixD* parini) {
112	  // Set initial parameter values (before the fit)
113	
114	  if (parini == 0) return;
115	  else if( parini->GetNrows() == _iniparameters.GetNrows() &&
116	           parini->GetNcols() == _iniparameters.GetNcols() )
117	    _iniparameters = (*parini) ;
118	  else {
119	    std::cout <<"WrongMatrixSize "<<
120	      //<< GetName() <<
121	       "::setParIni - Matrices don't fit."<<std::endl;
122	    return;
123	  }
124	
125	}
126	
127	const TMatrixD* TAbsFitConstraint::getCovMatrixDeltaAlpha() {
128	  // Returns covariance matrix delta(alpha)
129	
130	  calcCovMatrixDeltaAlpha(); 
131	  return &_covMatrixDeltaAlpha; 
132	
133	}
134	
135	TString TAbsFitConstraint::getInfoString() {
136	  // Collect information to be used for printout
137	
138	  std::stringstream info;
139	  info << std::scientific << std::setprecision(6);
140	
141	  info << "__________________________" << std::endl
142	       << std::endl;
143	  info <<"OBJ: " << IsA()->GetName() << "\t" << GetName() << "\t" << GetTitle() << std::endl;
144	  
145	  info << "initial value: " << getInitValue() << std::endl;
146	  info << "current value: " << getCurrentValue() << std::endl;
147	
148	  return info.str();
149	
150	}
151	
152	void TAbsFitConstraint::print() {
153	  // Print constraint contents
154	
155	  std::cout <<"KinFitter " << this->getInfoString()<<std::endl;
156	
157	}
158	
