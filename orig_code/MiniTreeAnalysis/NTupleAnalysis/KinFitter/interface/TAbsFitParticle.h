
1	#ifndef TAbsFitParticle_hh
2	#define TAbsFitParticle_hh
3	
4	#include "TMatrixD.h"
5	#include "TLorentzVector.h"
6	#include "TVector3.h"
7	#include "TNamed.h"
8	
9	class TAbsFitParticle : public TNamed {
10	
11	public :
12	
13	  TAbsFitParticle();
14	  TAbsFitParticle(const TString &name, const TString &title);
15	  virtual ~TAbsFitParticle();
16	  virtual TAbsFitParticle* clone( TString newname = "" ) const = 0;
17	
18	  // returns derivative dP/dy with P=(p,E) and y=(par1, par2, par3, ...) 
19	  // the free parameters of the fit. The columns of the matrix contain 
20	  // (dP/dpar1, dP/dpar2, ...).
21	  virtual TMatrixD* getDerivative() = 0;
22	
23	  virtual void applycorr(TMatrixD* corrMatrix);
24	  virtual TMatrixD* transform(const TLorentzVector& vec) = 0;
25	  virtual void setCovMatrix(const TMatrixD* theCovMatrix);
26	  virtual const TMatrixD* getCovMatrix() const { return &_covMatrix; }
27	  virtual void setCovMatrixFit(const TMatrixD* theCovMatrixFit);
28	  virtual const TMatrixD* getCovMatrixFit() const { return &_covMatrixFit; }
29	  virtual const TMatrixD* getCovMatrixDeltaY();  
30	  virtual const TMatrixD* getPull();
31	
32	  Int_t getNPar() const { return _nPar; }
33	  const TLorentzVector* getCurr4Vec() { return &_pcurr; }
34	  const TLorentzVector* getIni4Vec() { return &_pini; }
35	  virtual TLorentzVector* calc4Vec( const TMatrixD* params ) = 0;
36	  virtual void setIni4Vec(const TLorentzVector* pini) = 0;
37	  const TMatrixD* getParIni() { return &_iniparameters; }
38	  void  setParIni(const TMatrixD* parini);
39	  const TMatrixD* getParCurr(){ return &_parameters; }
40	  const TVector3* getu1()    { return &_u1; }
41	  const TVector3* getu2(){ return &_u2; }
42	  const TVector3* getu3()  { return &_u3; }
43	
44	  TString getInfoString();
45	  virtual void print(); 
46	  virtual void reset();
47	
48	protected :
49	
50	  void calcCovMatrixDeltaY();
51	
52	  Int_t _nPar;        // Number of free parameters
53	  TVector3 _u1;       // base vector 1
54	  TVector3 _u2;       // base vector 2
55	  TVector3 _u3;       // base vector 3
56	
57	  TMatrixD _covMatrix;      // covariance matrix
58	  TMatrixD _covMatrixFit;   // fitted covariance matrix
59	  TMatrixD _covMatrixDeltaY;  // V(deltaY) == V(y_meas) - V(y_fit)
60	  TMatrixD _pull;           // pull values for all parameters
61	  TMatrixD _iniparameters;  // initialized parameters (parameters values before the fit)
62	  TMatrixD _parameters;     // fitted parameters
63	  TLorentzVector _pini;     // measured 4vector
64	  TLorentzVector _pcurr;    // fitted 4vector
65	  
66	};
67	
68	#endif

