1	#ifndef TopObjetcResolutionsMuon_h
2	#define TopObjetcResolutionsMuon_h
3	
4	namespace res{
5	  class HelperMuon {
6	
7	  public:
8	    HelperMuon(){};
9	    ~HelperMuon(){};
10	
11	    inline double pt (double pt, double eta);
12	    inline double eta(double pt, double eta);
13	    inline double phi(double pt, double eta);
14	    inline double a(double pt, double eta);
15	    inline double b(double pt, double eta);
16	    inline double c(double pt, double eta);
17	    inline double d(double pt, double eta);
18	    inline double et(double pt, double eta);
19	    inline double theta(double pt, double eta);
20	  };
21	}
22	
23	inline double res::HelperMuon::pt(double pt, double eta)
24	{
25	  double res = 1.5e-4*(pt*pt)+0.534*fabs(eta)+1.9e-2;
26	  return res;
27	}
28	
29	inline double res::HelperMuon::a(double pt, double eta)
30	{
31	  double res = 1000.;
32	        if(fabs(eta)<0.17) res = -0.00163044+0.00921744*exp(-(-0.00517804*pt));
33	        else if(fabs(eta)<0.35) res = -38.9811+38.9892*exp(-(-1.58728e-06*pt));
34	        else if(fabs(eta)<0.5) res = -17.254+17.2634*exp(-(-2.86961e-06*pt));
35	        else if(fabs(eta)<0.7) res = 0.00651163+0.0038473*exp(-(-0.00716166*pt));
36	        else if(fabs(eta)<0.9) res = -14.9098+14.9207*exp(-(-3.61147e-06*pt));
37	        else if(fabs(eta)<1.15) res = -0.0130723+0.028881*exp(-(-0.00143687*pt));
38	        else if(fabs(eta)<1.3) res = 0.0102039+0.00629269*exp(-(-0.00659229*pt));
39	        else if(fabs(eta)<1.6) res = -26.1501+26.1657*exp(-(-2.75489e-06*pt));
40	        else if(fabs(eta)<1.9) res = -26.1006+26.1168*exp(-(-2.65457e-06*pt));
41	        else if(fabs(eta)<2.5) res = -110.342+110.361*exp(-(-1.3011e-06*pt));
42	  return res;
43	}
44	
45	inline double res::HelperMuon::b(double pt, double eta)
46	{
47	  double res = 1000.;
48	        if(fabs(eta)<0.17) res = -25.2075+25.2104*exp(-(-1.10483e-05*pt));
49	        else if(fabs(eta)<0.35) res = -117.663+117.666*exp(-(-1.71416e-06*pt));
50	        else if(fabs(eta)<0.5) res = -21.8718+21.8742*exp(-(-8.13178e-06*pt));
51	        else if(fabs(eta)<0.7) res = -35.0557+35.0573*exp(-(-5.75421e-06*pt));
52	        else if(fabs(eta)<0.9) res = -6.37721+6.38003*exp(-(-2.51376e-05*pt));
53	        else if(fabs(eta)<1.15) res = -73.9844+73.9873*exp(-(-1.75066e-06*pt));
54	        else if(fabs(eta)<1.3) res = -32.7368+32.7402*exp(-(-3.28819e-06*pt));
55	        else if(fabs(eta)<1.6) res = -46.9103+46.9132*exp(-(-2.64771e-06*pt));
56	        else if(fabs(eta)<1.9) res = -63.3183+63.3218*exp(-(-1.56186e-06*pt));
57	        else if(fabs(eta)<2.5) res = -13.913+13.9174*exp(-(-6.62559e-06*pt));
58	  return res;
59	}
60	
61	inline double res::HelperMuon::c(double pt, double eta)
62	{
63	  double res = 1000.;
64	        if(fabs(eta)<0.17) res = -0.00784191+0.0107731*exp(-(-0.00262573*pt));
65	        else if(fabs(eta)<0.35) res = -4.16489+4.16781*exp(-(-8.18221e-06*pt));
66	        else if(fabs(eta)<0.5) res = -0.00758502+0.0109898*exp(-(-0.0023199*pt));
67	        else if(fabs(eta)<0.7) res = -0.0190548+0.0222179*exp(-(-0.00140706*pt));
68	        else if(fabs(eta)<0.9) res = -7.74332+7.74699*exp(-(-3.61138e-06*pt));
69	        else if(fabs(eta)<1.15) res = 0.00239644+0.00222721*exp(-(-0.00729116*pt));
70	        else if(fabs(eta)<1.3) res = -50.7222+50.7266*exp(-(-5.72416e-07*pt));
71	        else if(fabs(eta)<1.6) res = -9.91368+9.91795*exp(-(-3.8653e-06*pt));
72	        else if(fabs(eta)<1.9) res = 0.000103356+0.00464674*exp(-(-0.00750142*pt));
73	        else if(fabs(eta)<2.5) res = -0.00425536+0.00971869*exp(-(-0.00546016*pt));
74	  return res;
75	}
76	
77	inline double res::HelperMuon::d(double pt, double eta)
78	{
79	  double res = 1000.;
80	        if(fabs(eta)<0.17) res = -0.00146069+0.00904233*exp(-(-0.00524103*pt));
81	        else if(fabs(eta)<0.35) res = -34.455+34.4631*exp(-(-1.78448e-06*pt));
82	        else if(fabs(eta)<0.5) res = -15.3417+15.3511*exp(-(-3.23796e-06*pt));
83	        else if(fabs(eta)<0.7) res = 0.00639493+0.00393752*exp(-(-0.00707984*pt));
84	        else if(fabs(eta)<0.9) res = -12.855+12.866*exp(-(-4.16994e-06*pt));
85	        else if(fabs(eta)<1.15) res = -0.00526993+0.0211877*exp(-(-0.00186356*pt));
86	        else if(fabs(eta)<1.3) res = 0.0105021+0.00600376*exp(-(-0.00677709*pt));
87	        else if(fabs(eta)<1.6) res = -23.5742+23.5898*exp(-(-3.0526e-06*pt));
88	        else if(fabs(eta)<1.9) res = -27.317+27.3331*exp(-(-2.56587e-06*pt));
89	        else if(fabs(eta)<2.5) res = -112.151+112.17*exp(-(-1.25452e-06*pt));
90	  return res;
91	}
92	
93	inline double res::HelperMuon::theta(double pt, double eta)
94	{
95	  double res = 1000.;
96	        if(fabs(eta)<0.17) res = 0.000327129+0.000101907*exp(-(0.0172489*pt));
97	        else if(fabs(eta)<0.35) res = 0.00025588+9.99049e-05*exp(-(0.0189315*pt));
98	        else if(fabs(eta)<0.5) res = 0.000215701+0.000109968*exp(-(0.0406069*pt));
99	        else if(fabs(eta)<0.7) res = 0.000196151+5.66998e-05*exp(-(0.0263501*pt));
100	        else if(fabs(eta)<0.9) res = 0.000106579+0.000113065*exp(-(0.00377145*pt));
101	        else if(fabs(eta)<1.15) res = 0.000120697+0.0001408*exp(-(0.0520758*pt));
102	        else if(fabs(eta)<1.3) res = 9.57227e-05+0.000205436*exp(-(0.0633277*pt));
103	        else if(fabs(eta)<1.6) res = 8.56706e-05+0.000117908*exp(-(0.0582652*pt));
104	        else if(fabs(eta)<1.9) res = 6.62861e-05+0.000110841*exp(-(0.0640963*pt));
105	        else if(fabs(eta)<2.5) res = 6.3783e-05+0.000122656*exp(-(0.0974097*pt));
106	  return res;
107	}
108	
109	inline double res::HelperMuon::phi(double pt, double eta)
110	{
111	  double res = 1000.;
112	        if(fabs(eta)<0.17) res = 7.21523e-05+0.000293781*exp(-(0.0518546*pt));
113	        else if(fabs(eta)<0.35) res = 7.15456e-05+0.000290324*exp(-(0.0496431*pt));
114	        else if(fabs(eta)<0.5) res = 7.25417e-05+0.000322288*exp(-(0.0497559*pt));
115	        else if(fabs(eta)<0.7) res = 7.24273e-05+0.000301504*exp(-(0.0480936*pt));
116	        else if(fabs(eta)<0.9) res = 7.23791e-05+0.0003355*exp(-(0.0454216*pt));
117	        else if(fabs(eta)<1.15) res = 8.13896e-05+0.000432844*exp(-(0.0480919*pt));
118	        else if(fabs(eta)<1.3) res = 7.93329e-05+0.000333341*exp(-(0.0367028*pt));
119	        else if(fabs(eta)<1.6) res = 9.34279e-05+0.000372581*exp(-(0.0429296*pt));
120	        else if(fabs(eta)<1.9) res = 0.000112312+0.000479423*exp(-(0.0513205*pt));
121	        else if(fabs(eta)<2.5) res = 0.000144398+0.000432592*exp(-(0.0400788*pt));
122	  return res;
123	}
124	
125	inline double res::HelperMuon::et(double pt, double eta)
126	{
127	  double res = 1000.; 
128	        if(fabs(eta)<0.17) res = -0.0552605+0.0115814*pt;
129	        else if(fabs(eta)<0.35) res = -0.05039+0.0122729*pt;
130	        else if(fabs(eta)<0.5) res = -0.0435167+0.0128949*pt;
131	        else if(fabs(eta)<0.7) res = -0.038473+0.0129088*pt;
132	        else if(fabs(eta)<0.9) res = -0.0333693+0.0140788*pt;
133	        else if(fabs(eta)<1.15) res = -0.0102406+0.0180217*pt;
134	        else if(fabs(eta)<1.3) res = -0.0244845+0.0190696*pt;
135	        else if(fabs(eta)<1.6) res = -0.055785+0.0205605*pt;
136	        else if(fabs(eta)<1.9) res = -0.0457006+0.0204167*pt;
137	        else if(fabs(eta)<2.5) res = -0.0399952+0.027388*pt;
138	  return res;
139	}
140	
141	inline double res::HelperMuon::eta(double pt, double eta)
142	{
143	  double res = 1000.;
144	        if(fabs(eta)<0.17) res = 0.000322451+0.000107167*exp(-(0.0156347*pt));
145	        else if(fabs(eta)<0.35) res = 0.000271279+0.000106197*exp(-(0.0235732*pt));
146	        else if(fabs(eta)<0.5) res = 0.000235783+0.000146536*exp(-(0.0491182*pt));
147	        else if(fabs(eta)<0.7) res = 0.000241657+0.00012571*exp(-(0.0681321*pt));
148	        else if(fabs(eta)<0.9) res = 0.000218419+0.000380006*exp(-(0.0749053*pt));
149	        else if(fabs(eta)<1.15) res = 0.000184903+0.000217257*exp(-(0.0520574*pt));
150	        else if(fabs(eta)<1.3) res = 0.000169888+0.000490542*exp(-(0.0747289*pt));
151	        else if(fabs(eta)<1.6) res = 0.000179318+0.000290048*exp(-(0.0613212*pt));
152	        else if(fabs(eta)<1.9) res = 0.000144481+0.000260476*exp(-(0.0392984*pt));
153	        else if(fabs(eta)<2.5) res = 0.000158888+0.000442249*exp(-(0.0537515*pt));
154	  return res;
155	}
156	
157	#endif
