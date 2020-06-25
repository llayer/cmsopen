1	// Classname: TFitParticleEMomDev
2	// Author: Jan E. Sundermann, Verena Klose (TU Dresden)      
3	
4	
5	//________________________________________________________________
6	// 
7	// TFitParticleEMomDev
8	// --------------------
9	//
10	// Particle with special parametrization of the momentum 4vector and
11	// free mass (4 free parameters). The parametrization is chosen as
12	// follows:
13	//
14	// p = r*|p|*u_r + theta*u_theta + phi*u_phi
15	// E(fit) = E_meas * d
16	//
17	// with u_r = p/|p|
18	//      u_phi = (u_z x u_r)/|u_z x u_r|
19	//      u_theta = (u_r x u_phi)/|u_r x u_phi|
20	//
21	// The initial parameters values are chosen like (r, theta, phi, d) = (1., 0., 0., 1.)
22	// corresponding to the measured momentum and mass.
23	//
24	
25	#include <iostream>
26	//#include "FWCore/MessageLogger/interface/MessageLogger.h"
27	#include "../interface/TFitParticleEMomDev.h"
28	
29	
30	//----------------
31	// Constructor --
32	//----------------
33	TFitParticleEMomDev::TFitParticleEMomDev()
34	  :TAbsFitParticle()  
35	{
36	  init(0, 0);
37	}
38	
39	TFitParticleEMomDev::TFitParticleEMomDev( const TFitParticleEMomDev& fitParticle )
40	  :TAbsFitParticle( fitParticle.GetName(), fitParticle.GetTitle() )
41	{
42	
43	  _nPar = fitParticle._nPar;
44	  _u1 = fitParticle._u1;
45	  _u2 = fitParticle._u2;
46	  _u3 = fitParticle._u3;
47	  _covMatrix.ResizeTo(  fitParticle._covMatrix );
48	  _covMatrix = fitParticle._covMatrix;
49	  _iniparameters.ResizeTo( fitParticle._iniparameters );
50	  _iniparameters = fitParticle._iniparameters;
51	  _parameters.ResizeTo( fitParticle._parameters );
52	  _parameters = fitParticle._parameters;
53	  _pini = fitParticle._pini;
54	  _pcurr = fitParticle._pcurr;
55	
56	}
57	
58	TFitParticleEMomDev::TFitParticleEMomDev(TLorentzVector* pini, const TMatrixD* theCovMatrix)
59	  :TAbsFitParticle()  
60	{
61	  init(pini, theCovMatrix);
62	}
63	
64	TFitParticleEMomDev::TFitParticleEMomDev(const TString &name, const TString &title, 
65	                           TLorentzVector* pini, const TMatrixD* theCovMatrix)
66	  :TAbsFitParticle(name, title)  
67	{
68	  init(pini, theCovMatrix);
69	}
70	
71	TAbsFitParticle* TFitParticleEMomDev::clone( TString newname ) const {
72	  // Returns a copy of itself
73	  
74	  TAbsFitParticle* myclone = new TFitParticleEMomDev( *this );
75	  if ( newname.Length() > 0 ) myclone->SetName(newname);
76	  return myclone;
77	
78	}
79	
80	//--------------
81	// Destructor --
82	//--------------
83	TFitParticleEMomDev::~TFitParticleEMomDev() {
84	
85	}
86	
87	//--------------
88	// Operations --
89	//--------------
90	void TFitParticleEMomDev::init(TLorentzVector* pini, const TMatrixD* theCovMatrix ) {
91	
92	  _nPar = 4;
93	  setIni4Vec(pini);
94	  setCovMatrix(theCovMatrix);
95	
96	}
97	
98	void TFitParticleEMomDev::setIni4Vec(const TLorentzVector* pini) {
99	  // Set the initial 4vector. Will also set the 
100	  // inital parameter values
101	  _iniparameters.ResizeTo(_nPar,1);
102	  _parameters.ResizeTo(_nPar,1);
103	
104	  if (pini == 0) {
105	    _iniparameters(0,0)=0.;
106	    _iniparameters(1,0)=0.;
107	    _iniparameters(2,0)=0.;
108	    _iniparameters(3,0)=0.;
109	
110	    _u1.SetXYZ(0., 0., 0.);
111	    _u3.SetXYZ(0., 0., 0.);
112	    _u2.SetXYZ(0., 0., 0.);
113	    _pini.SetXYZT(0., 0., 0., 0.);
114	    _pcurr = _pini;
115	    
116	  } else {
117	    _iniparameters(0,0)=1.;
118	    _iniparameters(1,0)=0.;
119	    _iniparameters(2,0)=0.;
120	    _iniparameters(3,0)=1.;
121	
122	    _pini = (*pini);
123	    _pcurr = _pini;
124	
125	    _u1 = pini->Vect();
126	    _u1 *= 1./_u1.Mag();
127	    
128	    TVector3 uz(0., 0., 1.);
129	    _u3 = uz.Cross(_u1);
130	    _u3 *= 1./_u3.Mag();
131	    
132	    _u2 = _u3.Cross(_u1);
133	    _u2 *= 1./_u2.Mag();
134	
135	  }
136	
137	  // reset parameters
138	  _parameters = _iniparameters;
139	
140	}
141	
142	TLorentzVector* TFitParticleEMomDev::calc4Vec( const TMatrixD* params ) {
143	  // Calculates a 4vector corresponding to the given
144	  // parameter values
145	
146	  if (params == 0) {
147	    return 0;
148	  }
149	
150	  if ( params->GetNcols() != 1 || params->GetNrows() !=_nPar ) {
151	    std::cout << "WrongMatrixSize"
152	      << GetName() << "::calc4Vec - Parameter matrix has wrong size.";
153	    return 0;
154	  }
155	
156	  Double_t X = _pini.P() * (*params)(0,0) *_u1.X() +
157	    (*params)(1,0) * _u2.X()+
158	    (*params)(2,0) * _u3.X();
159	  Double_t Y = _pini.P() * (*params)(0,0) *_u1.Y() +
160	    (*params)(1,0) * _u2.Y()+
161	    (*params)(2,0) * _u3.Y();
162	  Double_t Z = _pini.P() * (*params)(0,0) *_u1.Z() +
163	    (*params)(1,0) * _u2.Z()+
164	    (*params)(2,0) * _u3.Z();
165	  Double_t E = _pini.E()*(*params)(3,0);
166	
167	  TLorentzVector* vec = new TLorentzVector( X, Y, Z, E );
168	  return vec;
169	
170	}
171	
172	TMatrixD* TFitParticleEMomDev::getDerivative() {
173	  // returns derivative dP/dy with P=(p,E) and y=(r, theta, phi, d) 
174	  // the free parameters of the fit. The columns of the matrix contain 
175	  // (dP/dr, dP/dtheta, dP/dphi, dP/dd).
176	
177	  TMatrixD* DerivativeMatrix = new TMatrixD(4,4);
178	  (*DerivativeMatrix) *= 0.;
179	
180	  //1st column: dP/dr
181	  (*DerivativeMatrix)(0,0)=_pini.P()*_u1.X();
182	  (*DerivativeMatrix)(1,0)=_pini.P()*_u1.Y();
183	  (*DerivativeMatrix)(2,0)=_pini.P()*_u1.Z();
184	  (*DerivativeMatrix)(3,0)=0.;
185	
186	  //2nd column: dP/dtheta
187	  (*DerivativeMatrix)(0,1)=_u2.X();
188	  (*DerivativeMatrix)(1,1)=_u2.Y();
189	  (*DerivativeMatrix)(2,1)=_u2.Z();
190	  (*DerivativeMatrix)(3,1)=0.;
191	
192	   //3rd column: dP/dphi
193	  (*DerivativeMatrix)(0,2)=_u3.X();
194	  (*DerivativeMatrix)(1,2)=_u3.Y();
195	  (*DerivativeMatrix)(2,2)=_u3.Z();
196	  (*DerivativeMatrix)(3,2)=0.;
197	
198	   //4th column: dP/dm
199	  (*DerivativeMatrix)(0,3)=0.;
200	  (*DerivativeMatrix)(1,3)=0.;
201	  (*DerivativeMatrix)(2,3)=0.;
202	  (*DerivativeMatrix)(3,3)=_pini.E();
203	
204	  return DerivativeMatrix;  
205	}
206	
207	TMatrixD* TFitParticleEMomDev::transform(const TLorentzVector& vec) {
208	  // Returns the parameters corresponding to the given 
209	  // 4vector wrt. to the current base vectors u_r, u_theta, and u_phi
210	
211	  // construct rotation matrix
212	  TRotation rot;
213	  rot.RotateAxes( _u1, _u2, _u3 );
214	  rot.Invert();
215	
216	  // rotate vector
217	  TVector3 vec3( vec.Vect() );
218	  vec3.Transform( rot );
219	
220	  // retrieve parameters
221	  TMatrixD* tparams = new TMatrixD( _nPar, 1 );
222	  (*tparams)(0,0) = vec3.x()/_pini.P();
223	  (*tparams)(1,0) = vec3.y();
224	  (*tparams)(2,0) = vec3.z();
225	  (*tparams)(3,0) = vec.E()/_pini.E();
226	
227	  return tparams;
228	
229	}

