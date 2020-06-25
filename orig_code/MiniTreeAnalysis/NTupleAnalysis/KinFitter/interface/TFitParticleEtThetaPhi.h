1	#ifndef TFitParticleEtThetaPhi_hh
2	#define TFitParticleEtThetaPhi_hh
3	
4	
5	#include "../interface/TAbsFitParticle.h"
6	#include "TLorentzVector.h"
7	#include "TMatrixD.h"
8	
9	
10	class TFitParticleEtThetaPhi: public TAbsFitParticle {
11	
12	public :
13	
14	  TFitParticleEtThetaPhi();
15	  TFitParticleEtThetaPhi( const TFitParticleEtThetaPhi& fitParticle );
16	  TFitParticleEtThetaPhi(TLorentzVector* pini, const TMatrixD* theCovMatrix);
17	  TFitParticleEtThetaPhi(const TString &name, const TString &title, 
18	               TLorentzVector* pini,
19	               const TMatrixD* theCovMatrix);
20	  virtual ~TFitParticleEtThetaPhi();
21	  virtual TAbsFitParticle* clone( TString newname = "" ) const;
22	
23	  // returns derivative dP/dy with P=(p,E) and y=(et, theta, phi) 
24	  // the free parameters of the fit. The columns of the matrix contain 
25	  // (dP/d(et), dP/d(theta), dP/d(phi)).
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
