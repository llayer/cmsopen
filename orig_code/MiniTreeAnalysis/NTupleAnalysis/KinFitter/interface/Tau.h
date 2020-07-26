#ifndef TopObjetcResolutionsTau_h
#define TopObjetcResolutionsTau_h

// the beta,gamme,delta version ...
   
namespace res{
  class HelperTau {

  public:
    enum Flavor {kUds, kB};

    HelperTau(){};
    ~HelperTau(){};

    inline double eta(double pt, double eta);
    inline double phi(double pt, double eta);
    inline double et(double pt, double eta);
   
   };
}


inline double res::HelperTau::phi(double pt, double eta)
{
  double res = 5.06349e-03;
  if (fabs(eta)>1.5) res = 5.73417e-03;
  return res;
}
        
inline double res::HelperTau::et(double pt, double eta)
{
  double res = 1.03727e+00;
  if (fabs(eta)>1.5) res = 1.86995e+00;
  return res;
}
        
inline double res::HelperTau::eta(double pt, double eta)
{
  double res = 5.03359e-03;
  if (fabs(eta)>1.5) res = 5.36924e-03 ;
  return res;
}

#endif
