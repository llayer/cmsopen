
#ifndef FUNCCB
#define FUNCCB

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class FuncCB : public RooAbsReal {
public:
  FuncCB() {} ; 
  FuncCB(const char *name, const char *title,
              RooAbsReal& _m,
              RooAbsReal& _m0,
              RooAbsReal& _sigma,
              RooAbsReal& _alpha,
              RooAbsReal& _n,
              RooAbsReal& _norm);
  FuncCB(const FuncCB& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new FuncCB(*this,newname); }
  inline virtual ~FuncCB() { }

protected:
  Double_t ApproxErf(Double_t arg) const ;
  RooRealProxy m ;
  RooRealProxy m0 ;
  RooRealProxy sigma ;
  RooRealProxy alpha ;
  RooRealProxy n ;
  RooRealProxy norm ;
  
  Double_t evaluate() const ;

private:

  ClassDef(FuncCB,1) // Your description goes here...
};
 
#endif


