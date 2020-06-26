#include "math.h"

//Interpolation methods

//par[0] = mean
//par[1] = value @ -1 sigma
//par[2] = value @ +1 sigma


double LinearInterpol(double x, double *par);
        
double CubicInterpol(double x, double *par);
        
double ExpoInterpol(double x, double *par);
