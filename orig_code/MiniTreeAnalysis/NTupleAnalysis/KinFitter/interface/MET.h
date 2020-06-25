1	#ifndef TopObjetcResolutionsMET_h
2	#define TopObjetcResolutionsMET_h
3	
4	namespace res{
5	  class HelperMET {
6	    
7	  public:
8	    HelperMET(){};
9	    ~HelperMET(){};
10	    
11	    inline double met(double met);
12	    inline double a(double pt);
13	    inline double b(double pt);
14	    inline double c(double pt);
15	    inline double d(double pt);
16	    inline double theta(double pt);
17	    inline double phi(double pt);
18	    inline double et(double pt);
19	    inline double eta(double pt);
20	  };
21	}
22	
23	inline double res::HelperMET::met(double met)
24	{
25	  return 1.14*exp(-2.16e-3*met)+0.258;
26	}
27	
28	inline double res::HelperMET::a(double pt)
29	{
30	        double res = 0.241096+0.790046*exp(-(0.0248773*pt));
31	  return res;
32	}
33	
34	inline double res::HelperMET::b(double pt)
35	{
36	        double res = -141945+141974*exp(-(-1.20077e-06*pt));
37	  return res;
38	}
39	
40	inline double res::HelperMET::c(double pt)
41	{
42	        double res = 21.5615+1.13958*exp(-(-0.00921408*pt));
43	  return res;
44	}
45	
46	inline double res::HelperMET::d(double pt)
47	{
48	        double res = 0.376192+15.2485*exp(-(0.116907*pt));
49	  return res;
50	}
51	
52	inline double res::HelperMET::theta(double pt)
53	{
54	        double res = 1000000.;
55	  return res;
56	}
57	
58	inline double res::HelperMET::phi(double pt)
59	{
60	        double res = 0.201336+1.53501*exp(-(0.0216707*pt));
61	  return res;
62	}
63	
64	inline double res::HelperMET::et(double pt)
65	{
66	        double res = 11.7801+0.145218*pt;
67	  return res;
68	}
69	
70	inline double res::HelperMET::eta(double pt)
71	{
72	        double res = 1000000.;
73	  return res;
74	}
75	
76	#endif
