#ifndef Structures_h
#define Structure_h

#include <string>
#include <vector>

using namespace std;

struct Parameter{
        string name;
        // "Lumi" - "SignalXsection" - "BR_"channelName - "Xsection_"process - uncertainties
        double value;
        double PEValue; // only used for Pseudo-Experiments
        double estimation_minuit; 
        double estimation_stat;
        double estimation_syst;
        bool isNormalization;
        bool isCommon; // this means that it is shared by all channels
        bool isZeroCentered;
        bool isANuisanceParameter;
        bool isAnEstimator;
        bool isGlobal; // means that there is a k-factor to be applied to all signal distribution (depending on the channel), but not deformation of the distribution (so no histos related to)
        bool WithGraph;// use a graph to describe the evolution of effect as function of the parameter - otherwise it's just 3 points: -1sig, nominal, +1sig
        double ExpectedValue;
        pair<double,double> uncertainty;//used if it's just a fixed parameter - defined at the minimum of the PLR
        pair<double,double> uncertainty_lik; // defined when minimizing a simple likelihood
        pair<double,double> StatError;
        pair<double,double> SystError;
        double Error_minuit;
        pair<double,double> CombinedError;
        //used by tminuit
        double step;
        //PLR
        int NofPoints;
        //For Pseudo-exp
        int nbins;
        pair<double,double> range;
        pair<double,double> ErrRange;
        bool fixed;//this means that it is neither a nuisance parameter or a estimator but just a constant
};

struct PLREstimation{
        string name;
        int NofPoints;
        double* X_PLR;
        double* Y_PLR;
        double X_min;   
        double X_max;   
        double Y_min;
};

//Structure which represents a process
// DD means Data Driven

struct DDEstimation{
        string name;//Process-name
        bool isDDEstimated; // DD estimated[true] or from MC[false]
        bool isASyst; // Do we need to consider it has an uncertainty in the likelihood ?
        double NofEvts;
        // Errors on the NofEvts 
        double ErrNeg;  
        double ErrPos;
};

//Structure which represents a systematic uncertainty [only for signal]
struct Systematic{
        string name;    
        string channel;
        double effm1s; //factor (compare to 1) at -1 sigma
        double effp1s; //factor (compare to 1) at +1 sigma
};

#endif
