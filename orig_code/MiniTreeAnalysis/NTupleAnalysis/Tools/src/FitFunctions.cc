#include "../interface/FitFunctions.h"

using namespace FitFunctions;

double FitFunctions::BreitWigner (double *x, double *p) {
        double pi = TMath::Pi();
        Double_t value = p[2]/((p[0]*pi)*(1+pow((x[0]-p[1])/p[0],2)));
        return value;
}


double FitFunctions::AsymBreitWignerPlusCutEffect (double *x, double *p) {
        double pi = TMath::Pi();
        double value = 0.;
        if ( x[0]-p[1]<=0 ){
           value = p[7]*(TMath::Erf(x[0]-p[8])-TMath::Erf(x[0]-p[8]-p[9]))+
                   p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2)))) 
                      ;
        }
        if ( x[0]-p[1]>0 ){
           value = p[3] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) 
                      ;
        }
        return value;
}


double FitFunctions::BreitWignerPlusFixedContinuum(double *x, double *p) {
        double pi = TMath::Pi();
        double value = p[9] * (p[8]/((p[6]*pi)*(1+pow((x[0]-p[7])/p[6],2)))) +
                       p[10]* (p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]));
        return value;
}

double FitFunctions::BreitWignerPlusFixedContinuumPlusCutEffect(double *x, double *p) {
        double pi = TMath::Pi();
        double value = p[13]*(TMath::Erf(x[0]-p[11])-TMath::Erf(x[0]-p[11]-p[12]))+
                       p[9] * (p[8]/((p[6]*pi)*(1+pow((x[0]-p[7])/p[6],2)))) +
                       p[10]* (p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]));
        return value;
}

double FitFunctions::Continuum(double *x, double *p) {
        Double_t value = p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]);
        return value;
}

double FitFunctions::AsymBreitWignerPlusFixedContinuum(double *x, double *p) {
        double pi = TMath::Pi();
        double value = 0.;
        if ( x[0]-p[7]<=0 ){
           value = p[9] * (p[8]/((p[6]*pi)*(1+p[11]*pow((x[0]-p[7])/p[6],2)))) +
                       p[10]* (p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]));
        }
        if ( x[0]-p[7]>0 ){
           value = p[9] * (p[8]/((p[6]*pi)*(1+p[12]*pow((x[0]-p[7])/p[6],2)))) +
                       p[10]* (p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]));
        }
        return value;
}

double FitFunctions::AsymBreitWignerPlusFixedContinuumPlusCutEffect(double *x, double *p) {
        double pi = TMath::Pi();
        double value = 0.;
        if ( x[0]-p[7]<=0 ){
           value = p[15]*(TMath::Erf(x[0]-p[13])-TMath::Erf(x[0]-p[13]-p[14]))+
                   p[9] * (p[8]/((p[6]*pi)*(1+p[11]*pow((x[0]-p[7])/p[6],2)))) +
                       p[10]* (p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]));
        }
        if ( x[0]-p[7]>0 ){
           value = p[9] * (p[8]/((p[6]*pi)*(1+p[12]*pow((x[0]-p[7])/p[6],2)))) +
                       p[10]* (p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]));
        }
        return value;
}
