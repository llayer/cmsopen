1	#ifndef TAbsFitConstraint_hh
2	#define TAbsFitConstraint_hh
3	
4	#include <vector>
5	#include "TAbsFitParticle.h"
6	#include "TMatrixD.h"
7	#include "TNamed.h"
8	#include "TString.h"
9	
10	class TAbsFitConstraint : public TNamed {
11	
12	public :
13	
14	  TAbsFitConstraint();
15	  TAbsFitConstraint(const TString &name, const TString &title);
16	  virtual ~TAbsFitConstraint();
17	
18	  // returns derivative df/dP with P=(p,E) and f the constraint f=0.
19	  // The matrix contains one row (df/dp, df/dE).
20	  virtual TMatrixD* getDerivative( TAbsFitParticle* particle ) = 0 ;
21	  virtual Double_t getInitValue() = 0;
22	  virtual Double_t getCurrentValue() = 0;
23	
24	  // new ---  additional parameters
25	  Int_t getNPar() { return _nPar; } 
26	
27	  virtual TMatrixD* getDerivativeAlpha() { return 0; }
28	  
29	  virtual const TMatrixD* getCovMatrix() const { return &_covMatrix; }
30	  virtual void setCovMatrix(const TMatrixD* theCovMatrix);
31	
32	  virtual const TMatrixD* getCovMatrixFit() const { return &_covMatrixFit; }
33	  virtual void setCovMatrixFit(const TMatrixD* theCovMatrixFit);
34	
35	  virtual const TMatrixD* getCovMatrixDeltaAlpha();
36	  
37	  const TMatrixD* getParIni() { return &_iniparameters; }
38	  void  setParIni(const TMatrixD* parini);
39	  virtual void applyDeltaAlpha(TMatrixD* corrMatrix);
40	  const TMatrixD* getParCurr(){ return &_parameters; }
41	
42	  virtual TString getInfoString();
43	  virtual void print(); 
44	  virtual void reset();
45	
46	protected :
47	
48	  void calcCovMatrixDeltaAlpha();
49	
50	  Int_t _nPar;
51	
52	  TMatrixD _covMatrix;      // covariance matrix
53	  TMatrixD _covMatrixFit;   // fitted covariance matrix
54	  TMatrixD _covMatrixDeltaAlpha;  // V(deltaAlpha) == V(alpha_meas) - V(alpha_fit)
55	  TMatrixD _iniparameters;  // initialized parameters (parameters values before the fit)
56	  TMatrixD _parameters;     // fitted parameters
57	
58	
59	};
60	
61	#endif
