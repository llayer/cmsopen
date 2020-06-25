
1	#ifndef TFitParticleEScaledMomDev_hh
2	#define TFitParticleEScaledMomDev_hh
3	
4	#include "../interface/TAbsFitParticle.h"
5	#include "TLorentzVector.h"
6	#include "TMatrixD.h"
7	
8	
9	class TFitParticleEScaledMomDev: public TAbsFitParticle {
10	
11	public :
12	
13	  TFitParticleEScaledMomDev();
14	  TFitParticleEScaledMomDev( const TFitParticleEScaledMomDev& fitParticle );
15	  TFitParticleEScaledMomDev(TLorentzVector* pini, const TMatrixD* theCovMatrix);
16	  TFitParticleEScaledMomDev(const TString &name, const TString &title,
17	                      TLorentzVector* pini, const TMatrixD* theCovMatrix);
18	  virtual ~TFitParticleEScaledMomDev();
19	  virtual TAbsFitParticle* clone( TString newname = "" ) const;
20	
21	  // returns derivative dP/dy with P=(p,E) and y=(r, theta, phi, ...) 
22	  // the free parameters of the fit. The columns of the matrix contain 
23	  // (dP/dr, dP/dtheta, ...).
24	  virtual TMatrixD* getDerivative();
25	  virtual TMatrixD* transform(const TLorentzVector& vec);
26	  virtual void setIni4Vec(const TLorentzVector* pini);
27	  virtual TLorentzVector* calc4Vec( const TMatrixD* params );
28	
29	protected :
30	
31	  void init(TLorentzVector* pini, const TMatrixD* theCovMatrix);
32	
33	};
34	
35	#endif
