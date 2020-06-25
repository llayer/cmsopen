1	// Classname: TAbsFitParticle
2	// Author: Jan E. Sundermann, Verena Klose (TU Dresden)      
3	
4	
5	//________________________________________________________________
6	// 
7	// TAbsFitParticle::
8	// --------------------
9	//
10	// Abstract base class for particles to be used with kinematic fitter
11	//
12	
13	#include "../interface/TAbsFitParticle.h"
14	#include <iostream>
15	#include <sstream>
16	#include <iomanip>
17	#include "TClass.h"
18	
19	
20	TAbsFitParticle::TAbsFitParticle():
21	  TNamed("NoName","NoTitle")
22	  ,_nPar(0)     
23	  ,_u1()   
24	  ,_u2()
25	  ,_u3()
26	  ,_covMatrix()
27	  ,_covMatrixFit()
28	  ,_covMatrixDeltaY()
29	  ,_pull()
30	  ,_iniparameters(1,1)
31	  ,_parameters(1,1)
32	  ,_pini()
33	  ,_pcurr()
34	{
35	 
36	}
37	
38	TAbsFitParticle::TAbsFitParticle(const TString &name, const TString &title ):
39	  TNamed(name,title)
40	  ,_nPar(0)     
41	  ,_u1()   
42	  ,_u2()
43	  ,_u3()
44	  ,_covMatrix()
45	  ,_covMatrixFit()
46	  ,_covMatrixDeltaY()
47	  ,_pull()
48	  ,_iniparameters(1,1)
49	  ,_parameters(1,1)
50	  ,_pini()
51	  ,_pcurr()
52	{
53	 
54	}
55	
56	
57	TAbsFitParticle::~TAbsFitParticle() {
58	
59	}
60	
61	TString
62	TAbsFitParticle::getInfoString() {
63	  // Collect information to be used for printout
64	
65	  std::stringstream info;
66	  info << std::scientific << std::setprecision(6);
67	
68	  info << "__________________________" << std::endl
69	       << std::endl;
70	
71	  info << "OBJ: " << IsA()->GetName() << "\t" << GetName() << "\t" << GetTitle() << std::endl;
72	
73	  info << std::setw(22) << "initial parameters:"  << std::setw(5) << " " << std::setw(20) << "current parameters:" << std::endl;
74	  for (int i = 0; i< _nPar ;i++){
75	    info << "par[" << i << "] = "
76	         << std::setw(18) << (*getParIni())(i,0) 
77	         << std::setw(20) << (*getParCurr())(i,0) << std::endl;
78	  }
79	
80	  info << std::setw(22) << "initial 4vector:" << std::setw(5) << " " << std::setw(20) << "current 4vector:" << std::endl;
81	  for (int i = 0; i< 4 ;i++){
82	    info << "p[" << i << "] = "
83	         << std::setw(20) << (*getIni4Vec())[i] 
84	         << std::setw(20) << (*getCurr4Vec())[i] << std::endl;
85	   }
86	  info << "mass = " 
87	       << std::setw(20) << (*getIni4Vec()).M() 
88	       << std::setw(20) << (*getCurr4Vec()).M() << std::endl;
89	
90	   info << "u1  = " << _u1.X() << ", " << _u1.Y() << ", " << _u1.Z() << std::endl;
91	   info << "u2  = " << _u2.X() << ", " << _u2.Y() << ", " << _u2.Z() << std::endl;
92	   info << "u3  = " << _u3.X() << ", " << _u3.Y() << ", " << _u3.Z() << std::endl;
93	
94	   return info.str();
95	
96	}
97	
98	void 
99	TAbsFitParticle::print() {
100	  // Print particle contents
101	
102	  std::cout << this->getInfoString() <<std::endl;
103	
104	}
105	
106	void TAbsFitParticle::reset() {
107	  // Reset particle to initial values
108	
109	  _parameters = _iniparameters;  
110	  _pcurr = _pini;
111	  setCovMatrixFit( 0 );
112	  _pull.ResizeTo(_nPar, 1);
113	  _pull.Zero();
114	
115	}
116	
117	void TAbsFitParticle::setCovMatrix(const TMatrixD* theCovMatrix) {
118	  // Set the measured covariance matrix
119	
120	  _covMatrix.ResizeTo(_nPar, _nPar);
121	  if(theCovMatrix==0) {
122	    _covMatrix.Zero();
123	  } else if (theCovMatrix->GetNcols() ==_nPar && theCovMatrix->GetNrows() ==_nPar) {
124	    _covMatrix = (*theCovMatrix);
125	  } else {
126	    std::cout << "WrongMatrixSize"
127	      << GetName() << "::setCovMatrix - Covariance matrix needs to be a "
128	      << _nPar << "x" << _nPar << " matrix." <<std::endl;
129	  }
130	
131	}
132	
133	
134	void TAbsFitParticle::setCovMatrixFit(const TMatrixD* theCovMatrixFit) {
135	  // Set the fitted covariance matrix
136	
137	  _covMatrixFit.ResizeTo(_nPar, _nPar);
138	  if(theCovMatrixFit==0) {
139	    _covMatrixFit.Zero();
140	  } else if (theCovMatrixFit->GetNcols() ==_nPar && theCovMatrixFit->GetNrows() ==_nPar) {
141	    _covMatrixFit = (*theCovMatrixFit);
142	  } else {
143	    std::cout << "WrongMatrixSize"
144	      << GetName() << "::setCovMatrixFit - Fitted covariance matrix needs to be a "
145	      << _nPar << "x" << _nPar << " matrix." <<std::endl;
146	  }
147	
148	}
149	
150	void TAbsFitParticle::calcCovMatrixDeltaY() {
151	  // Calculates V(deltaY) ==  V(y_meas) - V(y_fit)
152	
153	  _covMatrixDeltaY.ResizeTo( _nPar, _nPar );
154	  _covMatrixDeltaY = _covMatrix;
155	  if(_covMatrixFit.GetNrows() == _nPar && _covMatrixFit.GetNcols() == _nPar)
156	    _covMatrixDeltaY -= _covMatrixFit;
157	  else 
158	    std::cout << "WrongMatrixSize"
159	      << GetName() << "::calcCovMatrixDeltaY - _covMatrixFit probably not set." << std::endl;
160	}
161	
162	const TMatrixD* TAbsFitParticle::getPull() {
163	  // Calculates the pull (y_fit - y_meas) / sigma
164	  // with sigma = Sqrt( sigma[y_meas]^2 - V[y_fit]^2 )
165	  // for all parameters
166	
167	  _pull.ResizeTo( _nPar, 1 );
168	  _pull = _parameters;
169	  _pull -= _iniparameters;
170	  calcCovMatrixDeltaY(); 
171	  for (int i = 0; i<_nPar; i++) {
172	    Double_t sigmaDeltaY = _covMatrixDeltaY(i, i);
173	    if (sigmaDeltaY < 0) {
174	      std::cout << "NegativeDiagonalElem" << "V[deltaY] has a negative diagonal element." <<std::endl;
175	      _pull.Zero();
176	      return &_pull;
177	    } else {
178	      _pull(i,0) /= TMath::Sqrt( sigmaDeltaY );
179	    }
180	  }
181	
182	  return &_pull;
183	
184	}
185	
186	void TAbsFitParticle::applycorr(TMatrixD* corrMatrix) {
187	  // Apply corrections to the parameters wrt. to the
188	  // initial parameters y* = y + delta(y)
189	  // This method will also calculate the fitted 
190	  // 4vector of the particle
191	
192	  // update _parameters-Matrix
193	  _parameters = _iniparameters;
194	  _parameters += (*corrMatrix);
195	
196	  // calculates new 4vec
197	  TLorentzVector* vec = calc4Vec( &_parameters );
198	  _pcurr = (*vec);
199	  delete vec;
200	
201	}
202	
203	void TAbsFitParticle::setParIni(const TMatrixD* parini) {
204	  if (parini == 0) return;
205	  else if( parini->GetNrows() == _iniparameters.GetNrows()
206	           && parini->GetNcols() == _iniparameters.GetNcols() )
207	    _iniparameters = (*parini) ;
208	  else {
209	    std::cout << "WrongMatrixSize"
210	      << GetName() << "::setParIni - Matrices don't fit." << std::endl;
211	    return;
212	      }
213	}
214	
215	const TMatrixD* TAbsFitParticle::getCovMatrixDeltaY() {
216	  //
217	  calcCovMatrixDeltaY(); 
218	  return &_covMatrixDeltaY; 
219	}
220	
221	
