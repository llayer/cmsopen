1	// Classname: TFitParticleEtThetaPhi
2	// Author: S.Paktinat(IPM, CMS)     
3	// 27 July 2005
4	
5	//________________________________________________________________
6	// 
7	// TFitParticleEtThetaPhi::
8	// --------------------
9	//
10	// Particle with a special parametrization useful in hadron 
11	// colliders (3 free parameters (Et, Eta, Phi). The parametrization is 
12	// chosen as follows:
13	//
14	// p = (EtCosPhi, EtSinPhi, Et/Tan(Theta)
15	// E =  Et/Sin(Theta)
16	//
17	
18	#include <iostream>
19	//#include "FWCore/MessageLogger/interface/MessageLogger.h"
20	#include "../interface/TFitParticleEtThetaPhi.h"
21	#include "TMath.h"
22	#include <cmath>
23	
24	//----------------
25	// Constructor --
26	//----------------
27	TFitParticleEtThetaPhi::TFitParticleEtThetaPhi()
28	  :TAbsFitParticle()  
29	{
30	  init(0, 0);
31	}
32	
33	TFitParticleEtThetaPhi::TFitParticleEtThetaPhi( const TFitParticleEtThetaPhi& fitParticle )
34	  :TAbsFitParticle( fitParticle.GetName(), fitParticle.GetTitle() )
35	{
36	
37	  _nPar = fitParticle._nPar;
38	  _u1 = fitParticle._u1;
39	  _u2 = fitParticle._u2;
40	  _u3 = fitParticle._u3;
41	  _covMatrix.ResizeTo(  fitParticle._covMatrix );
42	  _covMatrix = fitParticle._covMatrix;
43	  _iniparameters.ResizeTo( fitParticle._iniparameters );
44	  _iniparameters = fitParticle._iniparameters;
45	  _parameters.ResizeTo( fitParticle._parameters );
46	  _parameters = fitParticle._parameters;
47	  _pini = fitParticle._pini;
48	  _pcurr = fitParticle._pcurr;
49	
50	}
51	
52	TFitParticleEtThetaPhi::TFitParticleEtThetaPhi(TLorentzVector* pini, const TMatrixD* theCovMatrix)
53	  :TAbsFitParticle()  
54	{
55	  init(pini, theCovMatrix);
56	}
57	
58	TFitParticleEtThetaPhi::TFitParticleEtThetaPhi(const TString &name, const TString &title, 
59	                           TLorentzVector* pini, const TMatrixD* theCovMatrix)
60	  :TAbsFitParticle(name, title)  
61	{
62	  init(pini, theCovMatrix);
63	}
64	
65	TAbsFitParticle* TFitParticleEtThetaPhi::clone( TString newname ) const {
66	  // Returns a copy of itself
67	  
68	  TAbsFitParticle* myclone = new TFitParticleEtThetaPhi( *this );
69	  if ( newname.Length() > 0 ) myclone->SetName(newname);
70	  return myclone;
71	
72	}
73	
74	//--------------
75	// Destructor --
76	//--------------
77	TFitParticleEtThetaPhi::~TFitParticleEtThetaPhi() {
78	
79	}
80	
81	//--------------
82	// Operations --
83	//--------------
84	void TFitParticleEtThetaPhi::init(TLorentzVector* pini, const TMatrixD* theCovMatrix ) {
85	
86	  _nPar = 3;
87	  setIni4Vec(pini);
88	  setCovMatrix(theCovMatrix);
89	
90	}
91	
92	TLorentzVector* TFitParticleEtThetaPhi::calc4Vec( const TMatrixD* params ) {
93	  // Calculates a 4vector corresponding to the given
94	  // parameter values
95	
96	  if (params == 0) {
97	    return 0;
98	  }
99	
100	  if ( params->GetNcols() != 1 || params->GetNrows() !=_nPar ) {
101	    std::cout <<"WrongMatrixSize" 
102	      << GetName() << "::calc4Vec - Parameter matrix has wrong size."<<std::endl;
103	    return 0;
104	  }
105	
106	  Double_t et = (*params)(0,0);
107	  Double_t theta = (*params)(1,0);
108	  Double_t phi = (*params)(2,0);
109	
110	  Double_t X = et*TMath::Cos(phi);
111	  Double_t Y = et*TMath::Sin(phi);
112	  Double_t Z = et/TMath::Tan(theta);
113	  Double_t E = et/TMath::Sin(theta);
114	                
115	  TLorentzVector* vec = new TLorentzVector( X, Y, Z, E );
116	  return vec;
117	
118	}
119	
120	void TFitParticleEtThetaPhi::setIni4Vec(const TLorentzVector* pini) {
121	  // Set the initial 4vector. Will also set the 
122	  // inital parameter values 
123	
124	  if (pini == 0) {
125	
126	    _u1.SetXYZ(0., 0., 0.);
127	    _u3.SetXYZ(0., 0., 0.);
128	    _u2.SetXYZ(0., 0., 0.);
129	    _pini.SetXYZT(0., 0., 0., 0.);
130	    _pcurr = _pini;
131	
132	    _iniparameters.ResizeTo(_nPar,1);
133	    _iniparameters(0,0) = 0.;
134	    _iniparameters(1,0) = 0.;
135	    _iniparameters(2,0) = 0.;
136	    
137	    _parameters.ResizeTo(_nPar,1);
138	    _parameters(0,0) = 0.;
139	    _parameters(1,0) = 0.;
140	    _parameters(2,0) = 0.;   
141	    
142	  } else {
143	    
144	    Double_t et = pini->E()*std::fabs(sin(pini->Theta()));
145	    Double_t theta = pini->Theta();
146	    Double_t phi = pini->Phi();
147	    
148	    _pini = (*pini);
149	    _pcurr = _pini;
150	    
151	    _iniparameters.ResizeTo(_nPar,1);
152	    _iniparameters(0,0) = et;
153	    _iniparameters(1,0) = theta;
154	    _iniparameters(2,0) = phi;
155	    
156	    _parameters.ResizeTo(_nPar,1);
157	    _parameters = _iniparameters;
158	
159	    _u1.SetXYZ( TMath::Cos(phi), TMath::Sin(phi), 0.); // the base vector of Et
160	    _u2.SetXYZ( TMath::Cos(phi)*TMath::Cos(theta), TMath::Sin(phi)*TMath::Cos(theta), -1.*TMath::Sin(theta) );// the base vector of Eta ( same as the base vector for Theta)
161	    _u3.SetXYZ( -1.*TMath::Sin(phi), TMath::Cos(phi), 0. );// the base vector of Phi
162	
163	  }
164	
165	}
166	
167	TMatrixD* TFitParticleEtThetaPhi::getDerivative() {
168	  // returns derivative dP/dy with P=(p,E) and y=(et, theta, phi) 
169	  // the free parameters of the fit. The columns of the matrix contain 
170	  // (dP/d(et), dP/d(theta), dP/d(phi)).
171	
172	  TMatrixD* DerivativeMatrix = new TMatrixD(4,3);
173	  (*DerivativeMatrix) *= 0.;
174	
175	  Double_t et = _parameters(0,0);
176	  Double_t theta = _parameters(1,0);
177	  Double_t phi = _parameters(2,0);
178	
179	  Double_t CotanTheta = 1./TMath::Tan(theta);
180	
181	  //1st column: dP/d(et)
182	  (*DerivativeMatrix)(0,0) = TMath::Cos(phi);
183	  (*DerivativeMatrix)(1,0) = TMath::Sin(phi);
184	  (*DerivativeMatrix)(2,0) = CotanTheta;
185	  (*DerivativeMatrix)(3,0) = 1./TMath::Sin(theta);
186	
187	  //2nd column: dP/d(theta)
188	  (*DerivativeMatrix)(0,1) = 0.;
189	  (*DerivativeMatrix)(1,1) = 0.;
190	  (*DerivativeMatrix)(2,1) = -1.*et*(1.+CotanTheta*CotanTheta);
191	  (*DerivativeMatrix)(3,1) = -1.*et*CotanTheta/(TMath::Sin(theta));
192	
193	   //3rd column: dP/d(phi)
194	  (*DerivativeMatrix)(0,2) = -1.*et*TMath::Sin(phi);
195	  (*DerivativeMatrix)(1,2) = et*TMath::Cos(phi);
196	  (*DerivativeMatrix)(2,2) = 0.;
197	  (*DerivativeMatrix)(3,2) = 0.;
198	
199	  return DerivativeMatrix;
200	
201	}
202	
203	TMatrixD* TFitParticleEtThetaPhi::transform(const TLorentzVector& vec) {
204	  // Returns the parameters corresponding to the given 
205	  // 4vector
206	
207	  // retrieve parameters
208	  TMatrixD* tparams = new TMatrixD( _nPar, 1 );
209	  (*tparams)(0,0) = vec.E()*std::fabs(sin(vec.Theta()));
210	  (*tparams)(1,0) = vec.Theta();
211	  (*tparams)(2,0) = vec.Phi();
212	
213	  return tparams;
214	
215	}
