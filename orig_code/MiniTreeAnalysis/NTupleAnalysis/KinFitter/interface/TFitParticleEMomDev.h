1	#ifndef TFitParticleEMomDev_hh
2	#define TFitParticleEMomDev_hh
3	
4	
5	#include "TAbsFitParticle.h"
6	#include "TLorentzVector.h"
7	#include "TMatrixD.h"
8	
9	
10	class TFitParticleEMomDev: public TAbsFitParticle {
11	
12	public :
13	
14	  TFitParticleEMomDev();
15	  TFitParticleEMomDev( const TFitParticleEMomDev& fitParticle );
16	  TFitParticleEMomDev(TLorentzVector* pini, const TMatrixD* theCovMatrix);
17	  TFitParticleEMomDev(const TString &name, const TString &title, 
18	               TLorentzVector* pini,
19	               const TMatrixD* theCovMatrix);
20	  virtual ~TFitParticleEMomDev();
21	  virtual TAbsFitParticle* clone( TString newname = "" ) const;
22	
23	  // returns derivative dP/dy with P=(p,E) and y=(par1, par2, par3, ...) 
24	  // the free parameters of the fit. The columns of the matrix contain 
25	  // (dP/dpar1, dP/dpar2, ...).
26	  virtual TMatrixD* getDerivative();
27	  virtual TMatrixD* transform(const TLorentzVector& vec);
28	  virtual void setIni4Vec(const TLorentzVector* pini);
29	  virtual TLorentzVector* calc4Vec( const TMatrixD* params );
30	
31	protected :
32	
33	  void init(TLorentzVector* pini, const TMatrixD* theCovMatrix);
34	
35	
36	private:
37	  
38	};
39	
40	#endif
