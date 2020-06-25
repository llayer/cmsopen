1	#ifndef TopObjetcResolutionsElectron_h
2	#define TopObjetcResolutionsElectron_h
3	
4	namespace res{
5	  class HelperElectron {
6	
7	  public:
8	    HelperElectron(){};
9	    ~HelperElectron(){};
10	
11	    inline double pt (double pt, double eta);
12	    inline double eta(double pt, double eta);
13	    inline double phi(double pt, double eta);
14	          inline double a(double pt, double eta);
15	    inline double b(double pt, double eta);
16	    inline double c(double pt, double eta);
17	    inline double d(double pt, double eta);
18	    inline double et(double pt, double eta);
19	    inline double theta(double pt, double eta);
20	  };
21	}
22	
23	inline double res::HelperElectron::pt(double pt, double eta)
24	{
25	  double res=0.2*sqrt(pt);
26	  if( fabs(eta)<=0.8 ) 
27	    res+=-0.28*fabs(eta)+0.54;
28	  else if( 0.8<fabs(eta) && fabs(eta)<=1.4 ) 
29	    res+= 1.52*fabs(eta)-1.07;
30	  else  
31	    res+=-0.158*eta*eta +0.97;
32	  return res;
33	}
34	
35	inline double res::HelperElectron::a(double pt, double eta)
36	{
37	  double res = 1000.;
38	        if(fabs(eta)<0.17) res = 0.0114228+0.1135*exp(-(0.111787*pt));
39	        else if(fabs(eta)<0.35) res = 0.010297+0.0163219*exp(-(0.0248655*pt));
40	        else if(fabs(eta)<0.5) res = 0.0081634+0.0230718*exp(-(0.0250963*pt));
41	        else if(fabs(eta)<0.7) res = 0.0122627+0.0228103*exp(-(0.0420525*pt));
42	        else if(fabs(eta)<0.9) res = 0.0124118+0.0384051*exp(-(0.0545988*pt));
43	        else if(fabs(eta)<1.15) res = 0.0133398+0.0307574*exp(-(0.0316605*pt));
44	        else if(fabs(eta)<1.3) res = 0.0144956+0.0355239*exp(-(0.0273916*pt));
45	        else if(fabs(eta)<1.6) res = -13.9017+13.9315*exp(-(7.01823e-06*pt));
46	        else if(fabs(eta)<1.9) res = 0.0106309+0.0230149*exp(-(0.00792017*pt));
47	        else if(fabs(eta)<2.5) res = 0.0187822+0.119922*exp(-(0.140598*pt));
48	  return res;
49	}
50	
51	inline double res::HelperElectron::b(double pt, double eta)
52	{
53	  double res = 1000.;
54	        if(fabs(eta)<0.17) res = -36.5147+36.516*exp(-(-9.36847e-06*pt));
55	        else if(fabs(eta)<0.35) res = -0.132294+0.134289*exp(-(-0.0021212*pt));
56	        else if(fabs(eta)<0.5) res = -96.9689+96.9704*exp(-(-2.83158e-06*pt));
57	        else if(fabs(eta)<0.7) res = -122.755+122.757*exp(-(-1.90859e-06*pt));
58	        else if(fabs(eta)<0.9) res = -226.455+226.457*exp(-(-8.91395e-07*pt));
59	        else if(fabs(eta)<1.15) res = -249.279+249.281*exp(-(-7.57645e-07*pt));
60	        else if(fabs(eta)<1.3) res = -16.7465+16.7481*exp(-(-1.17848e-05*pt));
61	        else if(fabs(eta)<1.6) res = -128.535+128.537*exp(-(-1.52236e-06*pt));
62	        else if(fabs(eta)<1.9) res = -66.1731+66.1762*exp(-(-2.49121e-06*pt));
63	        else if(fabs(eta)<2.5) res = -0.0199509+0.0237796*exp(-(-0.00591733*pt));
64	  return res;
65	}
66	
67	inline double res::HelperElectron::c(double pt, double eta)
68	{
69	  double res = 1000.;
70	        if(fabs(eta)<0.17) res = -68.6101+68.6161*exp(-(-7.88064e-07*pt));
71	        else if(fabs(eta)<0.35) res = -130.219+130.224*exp(-(-8.39696e-07*pt));
72	        else if(fabs(eta)<0.5) res = -262.825+262.83*exp(-(-4.26008e-07*pt));
73	        else if(fabs(eta)<0.7) res = -47.898+47.9041*exp(-(-2.07568e-06*pt));
74	        else if(fabs(eta)<0.9) res = -178.795+178.799*exp(-(-5.78263e-07*pt));
75	        else if(fabs(eta)<1.15) res = -249.26+249.267*exp(-(-3.16408e-07*pt));
76	        else if(fabs(eta)<1.3) res = -173.603+173.606*exp(-(-1.58982e-06*pt));
77	        else if(fabs(eta)<1.6) res = -98.726+98.7326*exp(-(-2.67151e-06*pt));
78	        else if(fabs(eta)<1.9) res = -82.6028+82.6122*exp(-(-2.79483e-06*pt));
79	        else if(fabs(eta)<2.5) res = -119.94+119.95*exp(-(-1.69882e-06*pt));
80	  return res;
81	}
82	
83	inline double res::HelperElectron::d(double pt, double eta)
84	{
85	  double res = 1000.;
86	        if(fabs(eta)<0.17) res = 0.0114197+0.081753*exp(-(0.0969625*pt));
87	        else if(fabs(eta)<0.35) res = 0.0097562+0.0137737*exp(-(0.0187112*pt));
88	        else if(fabs(eta)<0.5) res = 0.00844899+0.021595*exp(-(0.0241679*pt));
89	        else if(fabs(eta)<0.7) res = 0.0124758+0.0243678*exp(-(0.0459914*pt));
90	        else if(fabs(eta)<0.9) res = 0.0117518+0.0357218*exp(-(0.0463595*pt));
91	        else if(fabs(eta)<1.15) res = 0.0133048+0.0292511*exp(-(0.0305812*pt));
92	        else if(fabs(eta)<1.3) res = 0.0144949+0.0337369*exp(-(0.0269872*pt));
93	        else if(fabs(eta)<1.6) res = 0.0137634+0.0343427*exp(-(0.0208025*pt));
94	        else if(fabs(eta)<1.9) res = 0.00572644+0.0270719*exp(-(0.00536847*pt));
95	        else if(fabs(eta)<2.5) res = 0.0189177+0.169591*exp(-(0.152597*pt));
96	  return res;
97	}
98	
99	inline double res::HelperElectron::theta(double pt, double eta)
100	{
101	  double res = 1000.;
102	        if(fabs(eta)<0.17) res = 0.000282805+0.000157786*exp(-(0.0343273*pt));
103	        else if(fabs(eta)<0.35) res = 0.000184362+4.34076e-05*exp(-(-0.0131909*pt));
104	        else if(fabs(eta)<0.5) res = 0.000249332+5.83114e-05*exp(-(0.0508729*pt));
105	        else if(fabs(eta)<0.7) res = -6.56357e-05+0.000325051*exp(-(0.00177319*pt));
106	        else if(fabs(eta)<0.9) res = 0.000182277+0.000125324*exp(-(0.0581923*pt));
107	        else if(fabs(eta)<1.15) res = 0.000140771+0.000407914*exp(-(0.0971668*pt));
108	        else if(fabs(eta)<1.3) res = 0.000125551+0.001266*exp(-(0.180176*pt));
109	        else if(fabs(eta)<1.6) res = 0.000107631+101920*exp(-(1.17024*pt));
110	        else if(fabs(eta)<1.9) res = 8.33927e-05+158936*exp(-(1.20127*pt));
111	        else if(fabs(eta)<2.5) res = 6.55271e-05+0.12459*exp(-(0.437044*pt));
112	  return res;
113	}
114	
115	inline double res::HelperElectron::phi(double pt, double eta)
116	{
117	  double res = 1000.;
118	        if(fabs(eta)<0.17) res = 0.000175676+0.000471783*exp(-(0.0383161*pt));
119	        else if(fabs(eta)<0.35) res = 0.000202185+0.00048635*exp(-(0.0373331*pt));
120	        else if(fabs(eta)<0.5) res = 0.000150868+0.000444216*exp(-(0.0268835*pt));
121	        else if(fabs(eta)<0.7) res = 0.000243624+0.00182347*exp(-(0.0850746*pt));
122	        else if(fabs(eta)<0.9) res = 0.000254463+0.000431233*exp(-(0.0446507*pt));
123	        else if(fabs(eta)<1.15) res = 0.000309592+0.000918965*exp(-(0.0555677*pt));
124	        else if(fabs(eta)<1.3) res = 0.000502204+0.000277996*exp(-(0.076721*pt));
125	        else if(fabs(eta)<1.6) res = 0.000361181+0.000655126*exp(-(0.0238519*pt));
126	        else if(fabs(eta)<1.9) res = 0.000321587+0.00155721*exp(-(0.0337709*pt));
127	        else if(fabs(eta)<2.5) res = 0.000819101+0.00205336*exp(-(0.0992806*pt));
128	  return res;
129	}
130	
131	inline double res::HelperElectron::et(double pt, double eta)
132	{
133	  double res = 1000.; 
134	        if(fabs(eta)<0.17) res = 0.326238+0.00760789*pt;
135	        else if(fabs(eta)<0.35) res = 0.40493+0.00659958*pt;
136	        else if(fabs(eta)<0.5) res = 0.369785+0.00690331*pt;
137	        else if(fabs(eta)<0.7) res = 0.437539+0.00703808*pt;
138	        else if(fabs(eta)<0.9) res = 0.456138+0.0078252*pt;
139	        else if(fabs(eta)<1.15) res = 0.518685+0.00907836*pt;
140	        else if(fabs(eta)<1.3) res = 0.733672+0.00953255*pt;
141	        else if(fabs(eta)<1.6) res = 1.02678+0.0116056*pt;
142	        else if(fabs(eta)<1.9) res = 0.948368+0.00977619*pt;
143	        else if(fabs(eta)<2.5) res = 0.418302+0.0127816*pt;
144	  return res;
145	}
146	
147	inline double res::HelperElectron::eta(double pt, double eta)
148	{
149	  double res = 1000.;
150	        if(fabs(eta)<0.17) res = 0.000266154+0.000104322*exp(-(0.0140464*pt));
151	        else if(fabs(eta)<0.35) res = -0.251539+0.251791*exp(-(-7.37147e-07*pt));
152	        else if(fabs(eta)<0.5) res = 0.000290074+1.54664e-12*exp(-(-0.115541*pt));
153	        else if(fabs(eta)<0.7) res = 8.37182e-05+0.000233453*exp(-(0.00602386*pt));
154	        else if(fabs(eta)<0.9) res = 0.000229422+0.000114253*exp(-(0.0188935*pt));
155	        else if(fabs(eta)<1.15) res = 0.000191525+0.000404238*exp(-(0.0554545*pt));
156	        else if(fabs(eta)<1.3) res = 0.000195461+1.43699e-07*exp(-(-0.0315088*pt));
157	        else if(fabs(eta)<1.6) res = 0.000223422+2.05169e+07*exp(-(1.41408*pt));
158	        else if(fabs(eta)<1.9) res = -0.867114+0.867336*exp(-(4.08639e-07*pt));
159	        else if(fabs(eta)<2.5) res = -0.866567+0.866746*exp(-(-8.62871e-07*pt));
160	  return res;
161	}
162	
163	#endif
