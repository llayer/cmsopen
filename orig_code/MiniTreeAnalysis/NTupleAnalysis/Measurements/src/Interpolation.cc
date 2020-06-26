#include "../interface/Interpolation.h"
#include <iostream>

using namespace std;


//////////// INTERPOLATION //////////////////////////////

double ExpoInterpol(double x, double* par){
        cout<<"expo"<<std::endl;
        double f = 0;
        if(x>=0){
                if(par[2]<0) f = -pow(fabs(par[2]), fabs(x));
                else f = pow(par[2], fabs(x));
        }
        else{
                if(par[1]<0) f = -pow(fabs(par[1]), fabs(x));
                else f = pow(par[1], fabs(x));
        }
        return f;
}


double CubicInterpol(double x, double *par)
{
  double C0 = par[0];
  double C1 = 0.5*(par[2]-par[1]);
  double C2 = (par[1]+par[2]-2*par[0]);
  double C3 = 0.5*C2;
  double f = 0.;
   if ( x>=0 && x<1) {
     f = C0 - C3*x*x*x + C2*x*x +C1*x;
   }
   if ( x>=1) {
     f = (-3*C3+2*C2+C1)*x + C0+2*C3-C2 ;
   }
   if ( x<0 && x>-1) {
     f = C0 + C3*x*x*x + C2*x*x +C1*x;
   }
   if ( x<=-1) {
     f = (3*C3-2*C2+C1) * x + 2*C3-C2+C0;
   }
   return f;
}

double LinearInterpol(double x, double *par)
{
   double f = 0.;
   if ( x>=0 ) {
     f = (par[2]-par[0])*x+ par[0];
   }
   if ( x<0 ) {
     f = (par[0]-par[1])*x+ par[0];
   }
   return f;
}
