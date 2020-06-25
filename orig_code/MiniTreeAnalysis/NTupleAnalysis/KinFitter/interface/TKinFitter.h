1	#ifndef TKinFitter_h
2	#define TKinFitter_h
3	
4	#include <vector>
5	#include "TMatrixD.h"
6	#include "TNamed.h"
7	#include "TAbsFitParticle.h"
8	
9	//class TAbsFitParticle;
10	class TAbsFitConstraint;
11	class TH1D;
12	class TLorentzVector;
13	
14	class TKinFitter : public TNamed {
15	
16	public :
17	
18	  TKinFitter();
19	  TKinFitter(const TString &name, const TString &title);  
20	  ~TKinFitter();
21	  void reset();         
22	  void resetStatus();   
23	
24	  Int_t fit();
25	
26	  void addMeasParticle( TAbsFitParticle* particle );
27	  void addMeasParticles( TAbsFitParticle* p1, TAbsFitParticle* p2 = 0, TAbsFitParticle* p3 = 0, 
28	                         TAbsFitParticle* p4 = 0, TAbsFitParticle* p5 = 0, TAbsFitParticle* p6 = 0,
29	                         TAbsFitParticle* p7 = 0, TAbsFitParticle* p8 = 0, TAbsFitParticle* p9 = 0);
30	  void addUnmeasParticle( TAbsFitParticle* particle );
31	  void addUnmeasParticles( TAbsFitParticle* p1, TAbsFitParticle* p2 = 0, TAbsFitParticle* p3 = 0, 
32	                           TAbsFitParticle* p4 = 0, TAbsFitParticle* p5 = 0, TAbsFitParticle* p6 = 0,
33	                           TAbsFitParticle* p7 = 0, TAbsFitParticle* p8 = 0, TAbsFitParticle* p9 = 0);
34	  void addConstraint( TAbsFitConstraint* constraint );
35	
36	  Int_t getNDF() { return  (_constraints.size() - _nParA); }
37	  Int_t getNParA() { return _nParA; }
38	  Int_t getNParB() { return _nParB; }
39	  void setMaxNbIter( Int_t maxNbIter ) { _maxNbIter = maxNbIter; }
40	  Int_t getMaxNumberIter() { return _maxNbIter; }
41	  Int_t getNbIter() { return _nbIter; }
42	  Int_t getStatus() { return _status; }
43	  void setMaxDeltaS( Double_t maxDeltaS ) { _maxDeltaS = TMath::Abs( maxDeltaS ); }
44	  Double_t getMaxDeltaS() { return _maxDeltaS; }
45	  void setMaxF( Double_t maxF ) { _maxF = TMath::Abs( maxF ); }
46	  Double_t getMaxF() { return _maxF; }
47	  const TMatrixD* getCovMatrix() { return &_V; }
48	  void setCovMatrix( TMatrixD &V );
49	  const TMatrixD* getCovMatrixFit() { return &_yaVFit; }
50	  Double_t getS();
51	  Double_t getF();
52	  void setVerbosity( Int_t verbosity = 1 );
53	  Int_t getVerbosity( ) { return _verbosity; }
54	
55	  Int_t nbMeasParticles() { return _measParticles.size(); }
56	  const TAbsFitParticle* getMeasParticle( Int_t index ) { return _measParticles[index]; }
57	
58	  const TLorentzVector* Get4vec( Int_t index ) { 
59	
60	    //TAbsFitParticle *tmp = _measParticles[index];
61	    return (_measParticles[index])->getCurr4Vec(); 
62	  }
63	
64	  Int_t nbUnmeasParticles() { return _unmeasParticles.size(); }
65	  const TAbsFitParticle* getUnmeasParticle( Int_t index ) { return _unmeasParticles[index]; }
66	  Int_t nbConstraints() { return _constraints.size(); }
67	
68	  void print();
69	
70	protected:
71	
72	  Bool_t calcA();
73	  Bool_t calcB();
74	  Bool_t calcVA();
75	  Bool_t calcVB();
76	  Bool_t calcC();
77	
78	  Bool_t calcC11();
79	  Bool_t calcC21();
80	  Bool_t calcC22();
81	  Bool_t calcC31();
82	  Bool_t calcC32();
83	  Bool_t calcC33();
84	
85	  Bool_t calcDeltaA();
86	  Bool_t calcDeltaY();
87	  Bool_t calcLambda();
88	
89	  Bool_t calcV();
90	  Bool_t calcVFit();
91	
92	  Bool_t applyDeltaA();
93	  Bool_t applyDeltaY();
94	  void applyVFit();
95	
96	  Bool_t converged(Double_t F, Double_t prevS, Double_t currS);
97	
98	  TString getStatusString();
99	  void countMeasParams();
100	  void countUnmeasParams();
101	  void resetParams();
102	
103	  void printMatrix(const TMatrixD &matrix, const TString name = "");
104	
105	private :
106	
107	  Int_t _maxNbIter;       // Maximum number of iterations
108	  Double_t _maxDeltaS;    // Convergence criterium for deltaS
109	  Double_t _maxF;       // Convergence criterium for F
110	  Int_t _verbosity;       // Verbosty of the fitter 0: quiet, 1: print result, 2: print iterations, 3: print also matrices
111	
112	  TMatrixD _A;      // Jacobi Matrix of unmeasured parameters
113	  TMatrixD _AT;     // Transposed Jacobi Matrix of unmeasured parameters
114	  TMatrixD _B;      // Jacobi Matrix of measured parameters
115	  TMatrixD _BT;     // Transposed Jacobi Matrix of measured parameters
116	  TMatrixD _V;      // Covariance matrix
117	  TMatrixD _Vinv;   // Inverse covariance matrix
118	  TMatrixD _VB;     // VB    = ( B*V*BT )^(-1)
119	  TMatrixD _VBinv;  // VBinv = ( B*V*BT )
120	  TMatrixD _VA;     // VA    = ( AT*VB*A )
121	  TMatrixD _VAinv;  // VAinv = ( AT*VB*A )^(-1)
122	  TMatrixD _c;      // Vector c = A*delta(a*) + B*delta(y*) - f*
123	
124	  TMatrixD _C11;     // Matrix C11
125	  TMatrixD _C11T;    // Matrix C11T
126	  TMatrixD _C21;     // Matrix C21
127	  TMatrixD _C21T;    // Matrix C21T
128	  TMatrixD _C22;     // Matrix C22
129	  TMatrixD _C22T;    // Matrix C22T
130	  TMatrixD _C31;     // Matrix C31
131	  TMatrixD _C31T;    // Matrix C31T
132	  TMatrixD _C32;     // Matrix C32
133	  TMatrixD _C32T;    // Matrix C32T
134	  TMatrixD _C33;     // Matrix C33
135	  TMatrixD _C33T;    // Matrix C33T
136	
137	  TMatrixD _deltaA;  // The correction vector deltaA for unmeasured particles of the current iteration
138	  TMatrixD _deltaY;  // The correction vector deltaY for measured particles of the current iteration
139	  TMatrixD _deltaAstar; // The correction vector deltaA for unmeasured particles of the previous iteration
140	  TMatrixD _deltaYstar; // The correction vector deltaY for measured particles of the previous iteration
141	  TMatrixD _lambda;  // The column vector of Lagrange multiplicators (likelihood L = S + 2 sum_i lambda_i * f_i)
142	  TMatrixD _lambdaT; // The row vector of Lagrange multiplicators (likelihood L = S + 2 sum_i lambda_i * f_i)
143	
144	  TMatrixD _lambdaVFit;   // Covariance matrix of lambda after the fit
145	  TMatrixD _yaVFit;       // Combined covariance matrix of y and a after the fit
146	
147	  Int_t _nParA;     // Number of unmeasured parameters
148	  Int_t _nParB;     // Number of measured parameters
149	
150	  std::vector<TAbsFitConstraint*> _constraints;    // vector with constraints
151	  std::vector<TAbsFitParticle*> _measParticles;    // vector with measured particles
152	  std::vector<TAbsFitParticle*> _unmeasParticles;  // vector with unmeasured particles
153	
154	  Int_t _status;        // Status of the last fit;_
155	  Int_t _nbIter;        // number of iteration performed in the fit
156	
157	};
158	
159	#endif
