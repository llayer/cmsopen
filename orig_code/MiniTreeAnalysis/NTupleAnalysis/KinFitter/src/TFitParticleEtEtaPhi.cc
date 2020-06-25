1	// Classname: TFitParticleEtEtaPhi
2	// Author: S.Paktinat(IPM, CMS)     
3	// 27 July 2005
4	
5	
6	//________________________________________________________________
7	// 
8	// TFitParticleEtEtaPhi::
9	// --------------------
10	//
11	// Particle with a special parametrization useful in hadron 
12	// colliders (3 free parameters (Et, Eta, Phi). The parametrization is 
13	// chosen as follows:
14	//
15	// p = (EtCosPhi, EtSinPhi, EtSinhEta)
16	// E =  EtCoshEta
17	//
18	
19	#include <iostream>
20	//#include "FWCore/MessageLogger/interface/MessageLogger.h"
21	#include "../interface/TFitParticleEtEtaPhi.h"
22	#include "TMath.h"
23	#include <cmath>
24	
25	//----------------
26	// Constructor --
27	//----------------
28	TFitParticleEtEtaPhi::TFitParticleEtEtaPhi()
29	  :TAbsFitParticle()  
30	{
31	  init(0, 0);
32	}
33	
34	TFitParticleEtEtaPhi::TFitParticleEtEtaPhi( const TFitParticleEtEtaPhi& fitParticle )
35	  :TAbsFitParticle( fitParticle.GetName(), fitParticle.GetTitle() )
36	{
37	
38	  _nPar = fitParticle._nPar;
39	  _u1 = fitParticle._u1;
40	  _u2 = fitParticle._u2;
41	  _u3 = fitParticle._u3;
42	  _covMatrix.ResizeTo(  fitParticle._covMatrix );
43	  _covMatrix = fitParticle._covMatrix;
44	  _iniparameters.ResizeTo( fitParticle._iniparameters );
45	  _iniparameters = fitParticle._iniparameters;
46	  _parameters.ResizeTo( fitParticle._parameters );
47	  _parameters = fitParticle._parameters;
48	  _pini = fitParticle._pini;
49	  _pcurr = fitParticle._pcurr;
50	
51	}
52	
53	TFitParticleEtEtaPhi::TFitParticleEtEtaPhi(TLorentzVector* pini, const TMatrixD* theCovMatrix)
54	  :TAbsFitParticle()  
55	{
56	  init(pini, theCovMatrix);
57	}
58	
59	TFitParticleEtEtaPhi::TFitParticleEtEtaPhi(const TString &name, const TString &title, 
60	                           TLorentzVector* pini, const TMatrixD* theCovMatrix)
61	  :TAbsFitParticle(name, title)  
62	{
63	  init(pini, theCovMatrix);
64	}
65	
66	TAbsFitParticle* TFitParticleEtEtaPhi::clone( TString newname ) const {
67	  // Returns a copy of itself
68	  
69	  TAbsFitParticle* myclone = new TFitParticleEtEtaPhi( *this );
70	  if ( newname.Length() > 0 ) myclone->SetName(newname);
71	  return myclone;
72	
73	}
74	
75	//--------------
76	// Destructor --
77	//--------------
78	TFitParticleEtEtaPhi::~TFitParticleEtEtaPhi() {
79	
80	}
81	
82	//--------------
83	// Operations --
84	//--------------
85	void TFitParticleEtEtaPhi::init(TLorentzVector* pini, const TMatrixD* theCovMatrix ) {
86	
87	  _nPar = 3;
88	  setIni4Vec(pini);
89	  setCovMatrix(theCovMatrix);
90	
91	}
92	
93	TLorentzVector* TFitParticleEtEtaPhi::calc4Vec( const TMatrixD* params ) {
94	  // Calculates a 4vector corresponding to the given
95	  // parameter values
96	
97	  if (params == 0) {
98	    return 0;
99	  }
100	
101	  if ( params->GetNcols() != 1 || params->GetNrows() !=_nPar ) {
102	    std::cout << "WrongMatrixSize" 
103	      << GetName() << "::calc4Vec - Parameter matrix has wrong size.";
104	    return 0;
105	  }
106	
107	  Double_t et = (*params)(0,0);
108	  Double_t eta = (*params)(1,0);
109	  Double_t phi = (*params)(2,0);
110	
111	  Double_t X = et*TMath::Cos(phi);
112	  Double_t Y = et*TMath::Sin(phi);
113	  Double_t Z = et*TMath::SinH(eta);
114	  Double_t E = et*TMath::CosH(eta);
115	                
116	  TLorentzVector* vec = new TLorentzVector( X, Y, Z, E );
117	  return vec;
118	
119	}
120	
121	void TFitParticleEtEtaPhi::setIni4Vec(const TLorentzVector* pini) {
122	  // Set the initial 4vector. Will also set the 
123	  // inital parameter values 
124	
125	  if (pini == 0) {
126	
127	    _u1.SetXYZ(0., 0., 0.);
128	    _u3.SetXYZ(0., 0., 0.);
129	    _u2.SetXYZ(0., 0., 0.);
130	    _pini.SetXYZT(0., 0., 0., 0.);
131	    _pcurr = _pini;
132	
133	    _iniparameters.ResizeTo(_nPar,1);
134	    _iniparameters(0,0) = 0.;
135	    _iniparameters(1,0) = 0.;
136	    _iniparameters(2,0) = 0.;
137	    
138	    _parameters.ResizeTo(_nPar,1);
139	    _parameters(0,0) = 0.;
140	    _parameters(1,0) = 0.;
141	    _parameters(2,0) = 0.;   
142	    
143	  } else {
144	    
145	    Double_t et = pini->E()*std::fabs(sin(pini->Theta()));
146	    Double_t eta = pini->Eta();
147	    Double_t phi = pini->Phi();
148	    
149	    _pini = (*pini);
150	    _pcurr = _pini;
151	    
152	    _iniparameters.ResizeTo(_nPar,1);
153	    _iniparameters(0,0) = et;
154	    _iniparameters(1,0) = eta;
155	    _iniparameters(2,0) = phi;
156	    
157	    _parameters.ResizeTo(_nPar,1);
158	    _parameters = _iniparameters;
159	
160	    _u1.SetXYZ( TMath::Cos(phi), TMath::Sin(phi), 0.); // the base vector of Et
161	    _u2.SetXYZ( -1.*TMath::Cos(phi)*TMath::TanH(eta), -1.*TMath::Sin(phi)*TMath::TanH(eta), 1./TMath::CosH(eta) );// the base vector of Eta ( same as the base vector for Theta)
162	    _u3.SetXYZ( -1.*TMath::Sin(phi), TMath::Cos(phi), 0. );// the base vector of Phi
163	
164	  }
165	
166	}
167	
168	TMatrixD* TFitParticleEtEtaPhi::getDerivative() {
169	  // returns derivative dP/dy with P=(p,E) and y=(et, eta, phi) 
170	  // the free parameters of the fit. The columns of the matrix contain 
171	  // (dP/d(et), dP/d(eta), dP/d(phi)).
172	
173	  TMatrixD* DerivativeMatrix = new TMatrixD(4,3);
174	  (*DerivativeMatrix) *= 0.;
175	
176	  Double_t et = _parameters(0,0);
177	  Double_t eta = _parameters(1,0);
178	  Double_t phi = _parameters(2,0);
179	
180	  //1st column: dP/d(et)
181	  (*DerivativeMatrix)(0,0) = TMath::Cos(phi);
182	  (*DerivativeMatrix)(1,0) = TMath::Sin(phi);
183	  (*DerivativeMatrix)(2,0) = TMath::SinH(eta);
184	  (*DerivativeMatrix)(3,0) = TMath::CosH(eta);
185	
186	  //2nd column: dP/d(eta)
187	  (*DerivativeMatrix)(0,1) = 0.;
188	  (*DerivativeMatrix)(1,1) = 0.;
189	  (*DerivativeMatrix)(2,1) = et*TMath::CosH(eta);
190	  (*DerivativeMatrix)(3,1) = et*TMath::SinH(eta);
191	
192	   //3rd column: dP/d(phi)
193	  (*DerivativeMatrix)(0,2) = -1.*et*TMath::Sin(phi);
194	  (*DerivativeMatrix)(1,2) = et*TMath::Cos(phi);
195	  (*DerivativeMatrix)(2,2) = 0.;
196	  (*DerivativeMatrix)(3,2) = 0.;
197	
198	  return DerivativeMatrix;
199	
200	}
201	
202	TMatrixD* TFitParticleEtEtaPhi::transform(const TLorentzVector& vec) {
203	  // Returns the parameters corresponding to the given 
204	  // 4vector
205	
206	  // retrieve parameters
207	  TMatrixD* tparams = new TMatrixD( _nPar, 1 );
208	  (*tparams)(0,0) = vec.E()*std::fabs(sin(vec.Theta()));
209	  (*tparams)(1,0) = vec.Eta();
210	  (*tparams)(2,0) = vec.Phi();
211	
212	  return tparams;
213	
214	}
