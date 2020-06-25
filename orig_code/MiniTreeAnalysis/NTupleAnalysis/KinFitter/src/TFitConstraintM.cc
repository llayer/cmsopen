1	// Classname: TFitConstraintM
2	// Author: Jan E. Sundermann, Verena Klose (TU Dresden)      
3	
4	
5	//________________________________________________________________
6	// 
7	// TFitConstraintM::
8	// --------------------
9	//
10	// Fit constraint: mass conservation ( m_i - m_j - MassConstraint == 0 )
11	//
12	
13	#include <iostream>
14	#include <iomanip>
15	#include <sstream>
16	#include "../interface/TFitConstraintM.h"
17	// #include "FWCore/MessageLogger/interface/MessageLogger.h"
18	#include "TLorentzVector.h"
19	#include "TClass.h"
20	
21	
22	//----------------
23	// Constructor --
24	//----------------
25	TFitConstraintM::TFitConstraintM()
26	  : TAbsFitConstraint() 
27	  ,_ParList1(0)
28	  ,_ParList2(0)
29	  ,_TheMassConstraint(0)
30	{
31	
32	}
33	
34	TFitConstraintM::TFitConstraintM(std::vector<TAbsFitParticle*>* ParList1,
35	                                 std::vector<TAbsFitParticle*>* ParList2, Double_t Mass)
36	  : TAbsFitConstraint() 
37	  ,_ParList1(0)
38	  ,_ParList2(0)
39	{
40	  // ParList1: Vector containing first list of constrained particles 
41	  //           ( sum[ m_i ] - sum[ m_j ] - MassConstraint == 0 )
42	  // ParList2: Vector containing second list of constrained particles 
43	  //           ( sum[ m_i ] - sum[ m_j ]  - MassConstraint == 0 )
44	
45	  if (ParList1) {
46	    _ParList1 = (*ParList1);
47	  }
48	  if (ParList2) {
49	    _ParList2 = (*ParList2);
50	  }
51	  if (Mass >= 0) {
52	    _TheMassConstraint = Mass;
53	  }
54	  else if(Mass < 0) {
55	    std::cout <<"NegativeMassConstr"
56	      << "Mass constraint in TFitConstraintM cannot be set to a negative value, will be set to 0."<<std::endl;
57	    _TheMassConstraint = 0.;
58	  }
59	}
60	
61	TFitConstraintM::TFitConstraintM(const TString &name, const TString &title,
62	                                 std::vector<TAbsFitParticle*>* ParList1,
63	                                 std::vector<TAbsFitParticle*>* ParList2, Double_t Mass)
64	  : TAbsFitConstraint(name, title) 
65	  ,_ParList1(0)
66	  ,_ParList2(0) 
67	{
68	  // ParList1: Vector containing first list of constrained particles 
69	  //           ( sum[ m_i ] - sum[ m_j ] - MassConstraint == 0 )
70	  // ParList2: Vector containing second list of constrained particles 
71	  //           ( sum[ m_i ] - sum[ m_j ] - MassConstraint == 0 )
72	
73	  if (ParList1) {
74	    _ParList1 = (*ParList1);
75	  }
76	  if (ParList2) {
77	    _ParList2 = (*ParList2);
78	  }  
79	  if (Mass >= 0) {
80	    _TheMassConstraint = Mass;
81	  }
82	  else if(Mass < 0) {
83	    std::cout<<"NegativeMassConstr"
84	      << "Mass constraint in TFitConstraintM cannot be set to a negative value, will be set to 0.";
85	    _TheMassConstraint = 0.;
86	  }
87	}
88	void TFitConstraintM::addParticle1( TAbsFitParticle* particle ) {
89	  // Add one constrained particle to first list of particles
90	  // ( sum[ m_i ] - sum[ m_j ] - MassConstraint == 0 )
91	
92	  _ParList1.push_back( particle );
93	
94	}
95	
96	void TFitConstraintM::addParticle2( TAbsFitParticle* particle ) {
97	  // Add one constrained particle to second list of particles
98	  // ( sum[ m_i ] - sum[ m_j ] - MassConstraint == 0 )
99	
100	  _ParList2.push_back( particle );
101	
102	}
103	
104	void TFitConstraintM::addParticles1( TAbsFitParticle* p1, TAbsFitParticle* p2, TAbsFitParticle* p3, TAbsFitParticle* p4,
105	                                     TAbsFitParticle* p5, TAbsFitParticle* p6, TAbsFitParticle* p7, TAbsFitParticle* p8,
106	                                     TAbsFitParticle* p9, TAbsFitParticle* p10) {
107	  // Add many constrained particle to first list of particles
108	  // ( sum[ m_i ] - sum[ m_j ] - MassConstraint == 0 )
109	
110	  if (p1) addParticle1( p1 );
111	  if (p2) addParticle1( p2 );
112	  if (p3) addParticle1( p3 );
113	  if (p4) addParticle1( p4 );
114	  if (p5) addParticle1( p5 );
115	  if (p6) addParticle1( p6 );
116	  if (p7) addParticle1( p7 );
117	  if (p8) addParticle1( p8 );
118	  if (p9) addParticle1( p9 );
119	  if (p10) addParticle1( p10 );
120	
121	}
122	
123	void TFitConstraintM::addParticles2( TAbsFitParticle* p1, TAbsFitParticle* p2, TAbsFitParticle* p3, TAbsFitParticle* p4,
124	                                     TAbsFitParticle* p5, TAbsFitParticle* p6, TAbsFitParticle* p7, TAbsFitParticle* p8,
125	                                     TAbsFitParticle* p9, TAbsFitParticle* p10) {
126	  // Add many constrained particle to second list of particles
127	  // ( sum[ m_i ] - sum[ m_j ] - MassConstraint == 0 )
128	
129	  if (p1) addParticle2( p1 );
130	  if (p2) addParticle2( p2 );
131	  if (p3) addParticle2( p3 );
132	  if (p4) addParticle2( p4 );
133	  if (p5) addParticle2( p5 );
134	  if (p6) addParticle2( p6 );
135	  if (p7) addParticle2( p7 );
136	  if (p8) addParticle2( p8 );
137	  if (p9) addParticle2( p9 );
138	  if (p10) addParticle2( p10 );
139	
140	}
141	
142	
143	//--------------
144	// Destructor --
145	//--------------
146	TFitConstraintM::~TFitConstraintM() {
147	
148	}
149	
150	//--------------
151	// Operations --
152	//--------------
153	TMatrixD* TFitConstraintM::getDerivative( TAbsFitParticle* particle ) {
154	  // returns derivative df/dP with P=(p,E) and f the constraint (f=0).
155	  // The matrix contains one row (df/dp, df/dE).
156	
157	  TMatrixD* DerivativeMatrix = new TMatrixD(1,4);
158	  (*DerivativeMatrix) *= 0.;
159	
160	  // Pf[4] is the 4-Mom (p,E) of the sum of particles on 
161	  // the list particle is part of 
162	  
163	  Double_t Factor = 0.;
164	  TLorentzVector Pf(0., 0., 0., 0.);
165	
166	  if( OnList( &_ParList1, particle) ) {
167	    UInt_t Npart = _ParList1.size();
168	    for (unsigned int i=0; i<Npart; i++) {
169	      const TLorentzVector* FourVec = (_ParList1[i])->getCurr4Vec();
170	      Pf += (*FourVec);
171	    }
172	    if( Pf.M() == 0. ) {
173	      std::cout << "KinFitter"
174	        << "Division by zero in "
175	        << IsA()->GetName() << " (named " << GetName() << ", titled " << GetTitle()
176	        << ") will lead to Inf in derivative matrix for particle "
177	        << particle->GetName() << ".";
178	    }
179	    Factor = 1./ Pf.M();
180	  } else if (OnList( &_ParList2, particle) ) {
181	    UInt_t Npart = _ParList2.size();
182	    for (unsigned int i=0; i<Npart; i++) {
183	      const TLorentzVector* FourVec = (_ParList2[i])->getCurr4Vec();
184	      Pf += (*FourVec);
185	    }
186	    if( Pf.M() == 0. ) {
187	      std::cout <<"KinFitter"
188	        << "Division by zero in "
189	        << IsA()->GetName() << " (named " << GetName() << ", titled " << GetTitle()
190	        << ") will lead to Inf in derivative matrix for particle "
191	        << particle->GetName() << ".";
192	    }
193	    Factor = -1./Pf.M();
194	  } else {
195	    Factor = 0.; 
196	  }
197	  
198	  (*DerivativeMatrix)(0,0) = -Pf[0] ;
199	  (*DerivativeMatrix)(0,1) = -Pf[1];
200	  (*DerivativeMatrix)(0,2) = -Pf[2];
201	  (*DerivativeMatrix)(0,3) = +Pf[3];
202	  (*DerivativeMatrix) *= Factor;
203	
204	  return DerivativeMatrix;
205	
206	}
207	
208	
209	Double_t TFitConstraintM::getInitValue() {
210	  // Get initial value of constraint (before the fit)
211	
212	  Double_t InitValue(0) ;   
213	  InitValue = CalcMass(&_ParList1,true) - CalcMass(&_ParList2,true) - _TheMassConstraint ;
214	  return InitValue;
215	}
216	
217	Double_t TFitConstraintM::getCurrentValue() {
218	  // Get value of constraint after the fit
219	
220	  Double_t CurrentValue(0);
221	  CurrentValue= CalcMass(&_ParList1,false) - CalcMass(&_ParList2,false) - _TheMassConstraint;
222	  return CurrentValue;
223	}
224	 
225	
226	Bool_t TFitConstraintM::OnList(std::vector<TAbsFitParticle*>* List,
227	                               TAbsFitParticle* particle) {
228	  // Checks whether list contains given particle
229	
230	  Bool_t ok(false);
231	  UInt_t Npart = List->size();
232	  for (unsigned int i=0;i<Npart;i++) {
233	    ok = (particle == (*List)[i]);
234	    if (ok) break;
235	  }
236	  return ok;
237	}
238	
239	Double_t TFitConstraintM::CalcMass(std::vector<TAbsFitParticle*>* List, Bool_t IniVal) {
240	  // Calculates initial/current invariant mass of provided list of particles
241	
242	  TLorentzVector P(0., 0., 0., 0.);
243	  UInt_t Npart = List->size();
244	  for (unsigned int i=0;i<Npart;i++) {
245	    const TLorentzVector* FourVec = 0;
246	    if (IniVal)
247	      FourVec = ((*List)[i])->getIni4Vec();
248	    else      
249	      FourVec = ((*List)[i])->getCurr4Vec();
250	    P += (*FourVec);
251	  }
252	  return P.M();
253	}
254	
255	TString TFitConstraintM::getInfoString() {
256	  // Collect information to be used for printout
257	
258	  std::stringstream info;
259	  info << std::scientific << std::setprecision(6);
260	
261	  info << "__________________________" << std::endl
262	       << std::endl;
263	  info <<"OBJ: " << IsA()->GetName() << "\t" << GetName() << "\t" << GetTitle() << std::endl;
264	
265	  info << "initial value: " << getInitValue() << std::endl;
266	  info << "current value: " << getCurrentValue() << std::endl;
267	  info << "mass: " << _TheMassConstraint << std::endl;
268	
269	  return info.str();
270	
271	}
272	
273	void TFitConstraintM::print() {
274	  // Print constraint contents
275	
276	  std::cout << "KinFitter " << this->getInfoString() << std::endl;
277	
278	}

