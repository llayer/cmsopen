1	#ifndef TFitConstraintM_hh
2	#define TFitConstraintM_hh
3	
4	#include "TAbsFitConstraint.h"
5	#include <vector>
6	
7	#include "TMatrixD.h"
8	
9	class TAbsFitParticle;
10	
11	class TFitConstraintM: public TAbsFitConstraint {
12	
13	public :
14	
15	  TFitConstraintM();
16	  TFitConstraintM(std::vector<TAbsFitParticle*>* ParList1,
17	                  std::vector<TAbsFitParticle*>* ParList2,
18	                  Double_t Mass = 0);
19	  TFitConstraintM(const TString &name, const TString &title,
20	                  std::vector<TAbsFitParticle*>* ParList1,
21	                  std::vector<TAbsFitParticle*>* ParList2,
22	                  Double_t Mass = 0);
23	
24	  virtual ~TFitConstraintM();
25	
26	  void addParticle1( TAbsFitParticle* particle );
27	  void addParticle2( TAbsFitParticle* particle );
28	  void addParticles1( TAbsFitParticle* p1, TAbsFitParticle* p2 = 0, TAbsFitParticle* p3 = 0, TAbsFitParticle* p4 = 0,
29	                      TAbsFitParticle* p5 = 0, TAbsFitParticle* p6 = 0, TAbsFitParticle* p7 = 0, TAbsFitParticle* p8 = 0,
30	                      TAbsFitParticle* p9 = 0, TAbsFitParticle* p10 = 0);
31	  void addParticles2( TAbsFitParticle* p1, TAbsFitParticle* p2 = 0, TAbsFitParticle* p3 = 0, TAbsFitParticle* p4 = 0,
32	                      TAbsFitParticle* p5 = 0, TAbsFitParticle* p6 = 0, TAbsFitParticle* p7 = 0, TAbsFitParticle* p8 = 0,
33	                      TAbsFitParticle* p9 = 0, TAbsFitParticle* p10 = 0);
34	  void setMassConstraint(Double_t Mass) { _TheMassConstraint = Mass; }
35	
36	  // returns derivative df/dP with P=(p,E) and f the constraint f=0 for 
37	  // one particle. The matrix contains one row (df/dp, df/dE).
38	  virtual TMatrixD* getDerivative( TAbsFitParticle* particle );
39	  virtual Double_t getInitValue();
40	  virtual Double_t getCurrentValue();
41	
42	  Bool_t OnList(std::vector<TAbsFitParticle*>* List, TAbsFitParticle* particle);
43	  Double_t CalcMass(std::vector<TAbsFitParticle*>* List, Bool_t IniVal);
44	
45	  virtual TString getInfoString();
46	  virtual void print(); 
47	
48	protected :
49	  
50	  std::vector<TAbsFitParticle*> _ParList1;   // Vector containing first list of constrained particles ( sum[ m_i ] - sum[ m_j ] == 0 )
51	  std::vector<TAbsFitParticle*> _ParList2;   // Vector containing second list of constrained particles ( sum[ m_i ] - sum[ m_j ] == 0 )
52	  Double_t _TheMassConstraint;
53	  
54	};
55	
56	#endif
57	

