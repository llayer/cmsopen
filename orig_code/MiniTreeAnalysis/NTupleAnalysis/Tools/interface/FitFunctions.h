
#ifndef FitFuctions_h
#define FitFunctions_h


// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include "TH1F.h"
#include "THStack.h"
#include "TF1.h"
#include <math.h>
#include "TMath.h"


using namespace std;

namespace FitFunctions {

   double BreitWigner (double *x, double *p);
   double AsymBreitWignerPlusCutEffect (double *x, double *p);
   double BreitWignerPlusFixedContinuum(double *x, double *p);
   double BreitWignerPlusFixedContinuumPlusCutEffect(double *x, double *p);
   double Continuum(double *x, double *p);
   double AsymBreitWignerPlusFixedContinuum(double *x, double *p);
   double AsymBreitWignerPlusFixedContinuumPlusCutEffect(double *x, double *p);
}

#endif

