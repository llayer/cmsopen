
1	#ifndef FUNCCB
2	#define FUNCCB
3	
4	#include "RooAbsReal.h"
5	#include "RooRealProxy.h"
6	#include "RooCategoryProxy.h"
7	#include "RooAbsReal.h"
8	#include "RooAbsCategory.h"
9	 
10	class FuncCB : public RooAbsReal {
11	public:
12	  FuncCB() {} ; 
13	  FuncCB(const char *name, const char *title,
14	              RooAbsReal& _m,
15	              RooAbsReal& _m0,
16	              RooAbsReal& _sigma,
17	              RooAbsReal& _alpha,
18	              RooAbsReal& _n,
19	              RooAbsReal& _norm);
20	  FuncCB(const FuncCB& other, const char* name=0) ;
21	  virtual TObject* clone(const char* newname) const { return new FuncCB(*this,newname); }
22	  inline virtual ~FuncCB() { }
23	
24	protected:
25	  Double_t ApproxErf(Double_t arg) const ;
26	  RooRealProxy m ;
27	  RooRealProxy m0 ;
28	  RooRealProxy sigma ;
29	  RooRealProxy alpha ;
30	  RooRealProxy n ;
31	  RooRealProxy norm ;
32	  
33	  Double_t evaluate() const ;
34	
35	private:
36	
37	  ClassDef(FuncCB,1) // Your description goes here...
38	};
39	 
40	#endif
41	
42	
