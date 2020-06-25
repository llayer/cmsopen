1	#ifndef TopObjetcResolutionsTau_h
2	#define TopObjetcResolutionsTau_h
3	
4	// the beta,gamme,delta version ...
5	   
6	namespace res{
7	  class HelperTau {
8	
9	  public:
10	    enum Flavor {kUds, kB};
11	
12	    HelperTau(){};
13	    ~HelperTau(){};
14	
15	    inline double eta(double pt, double eta);
16	    inline double phi(double pt, double eta);
17	    inline double et(double pt, double eta);
18	   
19	   };
20	}
21	
22	
23	inline double res::HelperTau::phi(double pt, double eta)
24	{
25	  double res = 5.06349e-03;
26	  if (fabs(eta)>1.5) res = 5.73417e-03;
27	  return res;
28	}
29	        
30	inline double res::HelperTau::et(double pt, double eta)
31	{
32	  double res = 1.03727e+00;
33	  if (fabs(eta)>1.5) res = 1.86995e+00;
34	  return res;
35	}
36	        
37	inline double res::HelperTau::eta(double pt, double eta)
38	{
39	  double res = 5.03359e-03;
40	  if (fabs(eta)>1.5) res = 5.36924e-03 ;
41	  return res;
42	}
43	
44	#endif
