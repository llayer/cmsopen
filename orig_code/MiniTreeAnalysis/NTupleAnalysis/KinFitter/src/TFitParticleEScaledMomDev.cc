1	// Classname: TFitParticleEScaledMomDev
2	// Author: Jan E. Sundermann, Verena Klose (TU Dresden)      
3	
4	
5	//________________________________________________________________
6	// 
7	// TFitParticleEScaledMomDev::
8	// --------------------
9	//
10	// Particle with special parametrization of the momentum 4vector and
11	// constant E/p (3 free parameters). The parametrization is chosen as
12	// follows:
13	//
14	// p = r*|p|*u_r + theta*u_theta + phi*u_phi
15	// E(fit) = E(ini)/P(ini)*p(fit)
16	//
17	// with u_r = p/|p|
18	//      u_phi = (u_z x u_r)/|u_z x u_r|
19	//      u_theta = (u_r x u_phi)/|u_r x u_phi|
20	//
21	// The initial parameters values are chosen like (r, theta, phi) = (1., 0., 0.)
22	// corresponding to the measured momentum.
23	//
24	
25	#include <iostream>
26	//#include "FWCore/MessageLogger/interface/MessageLogger.h"
27	#include "../interface/TFitParticleEScaledMomDev.h"
28	#include "TMath.h"
29	
30	
31	//----------------
32	// Constructor --
33	//----------------
34	TFitParticleEScaledMomDev::TFitParticleEScaledMomDev()
35	  :TAbsFitParticle()
36	{
37	  init(0, 0);
38	}
39	
40	TFitParticleEScaledMomDev::TFitParticleEScaledMomDev( const TFitParticleEScaledMomDev& fitParticle )
41	  :TAbsFitParticle( fitParticle.GetName(), fitParticle.GetTitle() )
42	{
43	
44	  _nPar = fitParticle._nPar;
45	  _u1 = fitParticle._u1;
46	  _u2 = fitParticle._u2;
47	  _u3 = fitParticle._u3;
48	  _covMatrix.ResizeTo(  fitParticle._covMatrix );
49	  _covMatrix = fitParticle._covMatrix;
50	  _iniparameters.ResizeTo( fitParticle._iniparameters );
51	  _iniparameters = fitParticle._iniparameters;
52	  _parameters.ResizeTo( fitParticle._parameters );
53	  _parameters = fitParticle._parameters;
54	  _pini = fitParticle._pini;
55	  _pcurr = fitParticle._pcurr;
56	
57	}
58	
59	TFitParticleEScaledMomDev::TFitParticleEScaledMomDev(TLorentzVector* pini, const TMatrixD* theCovMatrix)
60	  :TAbsFitParticle()
61	{
62	  init(pini, theCovMatrix);
63	}
64	
65	TFitParticleEScaledMomDev::TFitParticleEScaledMomDev(const TString &name, const TString &title,
66	                                         TLorentzVector* pini, const TMatrixD* theCovMatrix)
67	  :TAbsFitParticle(name, title)
68	{
69	  init(pini, theCovMatrix);
70	}  
71	
72	TAbsFitParticle* TFitParticleEScaledMomDev::clone( TString newname ) const {
73	  // Returns a copy of itself
74	  
75	  TAbsFitParticle* myclone = new TFitParticleEScaledMomDev( *this );
76	  if ( newname.Length() > 0 ) myclone->SetName(newname);
77	  return myclone;
78	
79	}
80	
81	//--------------
82	// Destructor --
83	//--------------
84	TFitParticleEScaledMomDev::~TFitParticleEScaledMomDev() {
85	
86	}
87	
88	//--------------
89	// Operations --
90	//--------------
91	void TFitParticleEScaledMomDev::init(TLorentzVector* pini, const TMatrixD* theCovMatrix) {
92	
93	  _nPar = 3;
94	  setIni4Vec(pini);
95	  _iniparameters.ResizeTo(_nPar,1);
96	  _iniparameters(0,0)=1.;
97	  _iniparameters(1,0)=0.;
98	  _iniparameters(2,0)=0.;
99	  _parameters.ResizeTo(_nPar,1);
100	  _parameters = _iniparameters;
101	  setCovMatrix(theCovMatrix);
102	
103	}
104	
105	TLorentzVector* TFitParticleEScaledMomDev::calc4Vec( const TMatrixD* params ) {
106	  // Calculates a 4vector corresponding to the given
107	  // parameter values
108	
109	  if (params == 0) {
110	    return 0;
111	  }
112	
113	  if ( params->GetNcols() != 1 || params->GetNrows() !=_nPar ) {
114	    std::cout << "WrongMatrixSize"
115	      << GetName() << "::calc4Vec - Parameter matrix has wrong size."<< std::endl;
116	    return 0;
117	  }
118	  
119	  Double_t X = _pini.P() * (*params)(0,0) *_u1.X() +
120	    (*params)(1,0) * _u2.X()+
121	    (*params)(2,0) * _u3.X();
122	  Double_t Y =  _pini.P() * (*params)(0,0) * _u1.Y() +
123	    (*params)(1,0) * _u2.Y()+
124	    (*params)(2,0) * _u3.Y();
125	  Double_t Z =  _pini.P()*(*params)(0,0)*_u1.Z() +
126	    (*params)(1,0) * _u2.Z()+
127	    (*params)(2,0) * _u3.Z();
128	  Double_t pcurr = TMath::Sqrt( X*X + Y*Y + Z*Z );
129	  Double_t E =  _pini.E()*pcurr/_pini.P();
130	
131	  TLorentzVector* vec = new TLorentzVector( X, Y, Z, E );
132	  return vec;
133	
134	}
135	
136	void TFitParticleEScaledMomDev::setIni4Vec(const TLorentzVector* pini) {
137	  // Set the initial 4vector. Will also set the 
138	  // inital parameter values
139	
140	  if (pini == 0) {
141	
142	    _u1.SetXYZ(0., 0., 0.);
143	    _u3.SetXYZ(0., 0., 0.);
144	    _u2.SetXYZ(0., 0., 0.);
145	    _pini.SetXYZT(0., 0., 0., 0.);
146	
147	  } else {
148	
149	    _pini = (*pini);
150	    _pcurr = _pini;
151	    
152	    _u1 = pini->Vect();
153	    _u1 *= 1./_u1.Mag();
154	    
155	    TVector3 uz(0., 0., 1.);
156	    _u3 = uz.Cross(_u1);
157	    _u3 *= 1./_u3.Mag();
158	  
159	    _u2 = _u3.Cross(_u1);
160	    _u2 *= 1./_u2.Mag();
161	  }  
162	
163	  _parameters = _iniparameters;
164	  
165	}
166	
167	TMatrixD*  TFitParticleEScaledMomDev::getDerivative() {
168	  // returns derivative dP/dy with P=(p,E) and y=(r, theta, phi) 
169	  // the free parameters of the fit. The columns of the matrix contain 
170	  // (dP/dr, dP/dtheta, dP/dphi).
171	
172	  TMatrixD* DerivativeMatrix = new TMatrixD(4,3);
173	  (*DerivativeMatrix) *= 0.;
174	
175	   //1st column: dP/dr
176	  (*DerivativeMatrix)(0,0)=_pini.P()*_u1.X();
177	  (*DerivativeMatrix)(1,0)=_pini.P()*_u1.Y();
178	  (*DerivativeMatrix)(2,0)=_pini.P()*_u1.Z();
179	  (*DerivativeMatrix)(3,0)=_pini.P()*_pini.E()*_parameters(0,0)/_pcurr.P();
180	  //2nd column: dP/dtheta
181	  (*DerivativeMatrix)(0,1)=_u2.X();
182	  (*DerivativeMatrix)(1,1)=_u2.Y();
183	  (*DerivativeMatrix)(2,1)=_u2.Z();
184	  (*DerivativeMatrix)(3,1)=_pini.E()/_pini.P()/_pcurr.P()*_parameters(1,0);
185	  //3rd column: dP/dphi
186	  (*DerivativeMatrix)(0,2)=_u3.X();
187	  (*DerivativeMatrix)(1,2)=_u3.Y();
188	  (*DerivativeMatrix)(2,2)=_u3.Z();;
189	  (*DerivativeMatrix)(3,2)=_pini.E()/_pini.P()/_pcurr.P()*_parameters(2,0);
190	
191	  return DerivativeMatrix;  
192	
193	}
194	
195	TMatrixD* TFitParticleEScaledMomDev::transform(const TLorentzVector& vec) {
196	  // Returns the parameters corresponding to the given 
197	  // 4vector wrt. to the current base vectors u_r, u_theta, and u_phi
198	
199	  // construct rotation matrix
200	  TRotation rot;
201	  rot.RotateAxes( _u1, _u2, _u3 );
202	  rot.Invert();
203	
204	  // rotate vector
205	  TVector3 vec3( vec.Vect() );
206	  vec3.Transform( rot );
207	
208	  // retrieve parameters
209	  TMatrixD* tparams = new TMatrixD( _nPar, 1 );
210	  (*tparams)(0,0) = vec3(0)/_pini.P();
211	  (*tparams)(1,0) = vec3(1);
212	  (*tparams)(2,0) = vec3(2);
213	
214	  return tparams;
215	
216	}
