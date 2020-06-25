1	// Setup a style
2	  using namespace RooFit;
3	  #include "TStyle.h"
4	
5	// tdrGrid: Turns the grid lines on (true) or off (false)
6	void tdrGrid(bool gridOn) {
7	  tdrStyle->SetPadGridX(gridOn);
8	  tdrStyle->SetPadGridY(gridOn);
9	}
10	
11	// fixOverlay: Redraws the axis
12	void fixOverlay() {
13	  gPad->RedrawAxis();
14	}
15	
16	void setTDRStyle() {
17	  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
18	
19	// For the canvas:
20	  tdrStyle->SetCanvasBorderMode(0);
21	  tdrStyle->SetCanvasColor(kWhite);
22	  tdrStyle->SetCanvasDefH(600); //Height of canvas
23	  tdrStyle->SetCanvasDefW(600); //Width of canvas
24	  tdrStyle->SetCanvasDefX(0);   //POsition on screen
25	  tdrStyle->SetCanvasDefY(0);
26	
27	// For the Pad:
28	  tdrStyle->SetPadBorderMode(0);
29	  // tdrStyle->SetPadBorderSize(Width_t size = 1);
30	  tdrStyle->SetPadColor(kWhite);
31	  tdrStyle->SetPadGridX(false);
32	  tdrStyle->SetPadGridY(false);
33	  tdrStyle->SetGridColor(0);
34	  tdrStyle->SetGridStyle(3);
35	  tdrStyle->SetGridWidth(1);
36	
37	// For the frame:
38	  tdrStyle->SetFrameBorderMode(0);
39	  tdrStyle->SetFrameBorderSize(1);
40	  tdrStyle->SetFrameFillColor(0);
41	  tdrStyle->SetFrameFillStyle(0);
42	  tdrStyle->SetFrameLineColor(1);
43	  tdrStyle->SetFrameLineStyle(1);
44	  tdrStyle->SetFrameLineWidth(1);
45	
46	// For the histo:
47	  // tdrStyle->SetHistFillColor(1);
48	  // tdrStyle->SetHistFillStyle(0);
49	  tdrStyle->SetHistLineColor(1);
50	  tdrStyle->SetHistLineStyle(0);
51	  tdrStyle->SetHistLineWidth(1);
52	  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
53	  // tdrStyle->SetNumberContours(Int_t number = 20);
54	
55	/*  tdrStyle->SetEndErrorSize(2);
56	//  tdrStyle->SetErrorMarker(20);
57	  tdrStyle->SetErrorX(0.);
58	  
59	  tdrStyle->SetMarkerStyle(20);*/
60	
61	//For the fit/function:
62	  tdrStyle->SetOptFit(1);
63	  tdrStyle->SetFitFormat("5.4g");
64	  tdrStyle->SetFuncColor(2);
65	  tdrStyle->SetFuncStyle(1);
66	  tdrStyle->SetFuncWidth(1);
67	
68	//For the date:
69	  tdrStyle->SetOptDate(0);
70	  // tdrStyle->SetDateX(Float_t x = 0.01);
71	  // tdrStyle->SetDateY(Float_t y = 0.01);
72	
73	// For the statistics box:
74	  tdrStyle->SetOptFile(0);
75	  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
76	  tdrStyle->SetStatColor(kWhite);
77	  tdrStyle->SetStatFont(42);
78	  tdrStyle->SetStatFontSize(0.025);
79	  tdrStyle->SetStatTextColor(1);
80	  tdrStyle->SetStatFormat("6.4g");
81	  tdrStyle->SetStatBorderSize(1);
82	  tdrStyle->SetStatH(0.1);
83	  tdrStyle->SetStatW(0.15);
84	  // tdrStyle->SetStatStyle(Style_t style = 1001);
85	  // tdrStyle->SetStatX(Float_t x = 0);
86	  // tdrStyle->SetStatY(Float_t y = 0);
87	
88	// Margins:
89	  tdrStyle->SetPadTopMargin(0.05);
90	  tdrStyle->SetPadBottomMargin(0.13);
91	  tdrStyle->SetPadLeftMargin(0.16);
92	  tdrStyle->SetPadRightMargin(0.02);
93	
94	// For the Global title:
95	
96	  tdrStyle->SetOptTitle(0);
97	  tdrStyle->SetTitleFont(42);
98	  tdrStyle->SetTitleColor(1);
99	  tdrStyle->SetTitleTextColor(1);
100	  tdrStyle->SetTitleFillColor(10);
101	  tdrStyle->SetTitleFontSize(0.05);
102	  // tdrStyle->SetTitleH(0); // Set the height of the title box
103	  // tdrStyle->SetTitleW(0); // Set the width of the title box
104	  // tdrStyle->SetTitleX(0); // Set the position of the title box
105	  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
106	  // tdrStyle->SetTitleStyle(Style_t style = 1001);
107	  // tdrStyle->SetTitleBorderSize(2);
108	
109	// For the axis titles:
110	
111	  tdrStyle->SetTitleColor(1, "XYZ");
112	  tdrStyle->SetTitleFont(42, "XYZ");
113	  tdrStyle->SetTitleSize(0.06, "XYZ");
114	  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
115	  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
116	  tdrStyle->SetTitleXOffset(0.9);
117	  tdrStyle->SetTitleYOffset(1.25);
118	  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset
119	
120	// For the axis labels:
121	
122	  tdrStyle->SetLabelColor(1, "XYZ");
123	  tdrStyle->SetLabelFont(42, "XYZ");
124	  tdrStyle->SetLabelOffset(0.007, "XYZ");
125	  tdrStyle->SetLabelSize(0.05, "XYZ");
126	
127	// For the axis:
128	
129	  tdrStyle->SetAxisColor(1, "XYZ");
130	  tdrStyle->SetStripDecimals(kTRUE);
131	  tdrStyle->SetTickLength(0.03, "XYZ");
132	  tdrStyle->SetNdivisions(510, "XYZ");
133	  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
134	  tdrStyle->SetPadTickY(1);
135	
136	// Change for log plots:
137	 tdrStyle->SetOptLogx(0);
138	  tdrStyle->SetOptLogy(0);
139	 tdrStyle->SetOptLogz(0);
140	
141	// Postscript options:
142	  tdrStyle->SetPaperSize(20.,20.);
143	  // tdrStyle->SetLineScalePS(Float_t scale = 3);
144	  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
145	  // tdrStyle->SetHeaderPS(const char* header);
146	  // tdrStyle->SetTitlePS(const char* pstitle);
147	
148	  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
149	  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
150	  // tdrStyle->SetPaintTextFormat(const char* format = "g");
151	  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
152	  // tdrStyle->SetTimeOffset(Double_t toffset);
153	  // tdrStyle->SetHistMinimumZero(kTRUE);
154	
155	  tdrStyle->cd();
156	
157	}
158	
159	void StackPlot_2_2011B_FR() {
160	   
161	   setTDRStyle();
162	
163	// Which histogram ?  
164	
165	  int i = 22;//1,2,6,12,13,14,16,17//15
166	             //9 8 18 19 20
167	  
168	  TH1F*  hist0;
169	  TH1F*  hist1;
170	  TH1F*  hist2;
171	  TH1F*  hist3;
172	  TH1F*  hist4;
173	  TH1F*  hist5;
174	  TH1F*  hist6;
175	  TH1F*  hist7;
176	  TH1F*  hist8;
177	  TH1F*  hist9;
178	  TH1F*  hist10;
179	  TH1F*  hist11;
180	  TH1F*  histDATA;
181	  TH1F*  histERROR;
182	  TH1F*  histTOT;
183	  
184	  TH1F *hist0 = (TH1F*)gDirectory->Get("hist0");
185	  TH1F *hist1 = (TH1F*)gDirectory->Get("hist1");
186	  TH1F *hist2 = (TH1F*)gDirectory->Get("hist2");
187	  TH1F *hist3 = (TH1F*)gDirectory->Get("hist3");
188	  TH1F *hist4 = (TH1F*)gDirectory->Get("hist4");
189	  TH1F *hist6 = (TH1F*)gDirectory->Get("hist6");
190	  TH1F *hist7 = (TH1F*)gDirectory->Get("hist7");
191	  TH1F *hist8 = (TH1F*)gDirectory->Get("hist8");
192	  TH1F *hist9 = (TH1F*)gDirectory->Get("hist9");
193	  TH1F *hist10 = (TH1F*)gDirectory->Get("hist10");
194	  TH1F *histERROR = (TH1F*)gDirectory->Get("histERROR");
195	  TH1F *histTOT = (TH1F*)gDirectory->Get("histTOT");
196	  //histERROR->Sumw2();
197	  
198	  
199	// Grab PDF's from histos
200	  
201	  THStack hs("hs","test stacked histograms");
202	   
203	  TFile *_file_TTbar ;
204	  // _file_TTbar = TFile::Open("~/SemiLeptonicTauXsectionMeas_WZT_3-5fb_medtau_met15_medbtag_NPV");
205	  _file_TTbar = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_WZT_Fermi.root");
206	
207	  _file_TTbar->cd();
208	  
209	  if(i==22)
210	  { 
211	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/M3_NN5_30_50_350");
212	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/M3_NN5_30_50_350");
213	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/M3_NN5_30_50_350");
214	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/M3_NN5_30_50_350");
215	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/M3_NN5_30_50_350");
216	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/M3_NN5_30_50_350");
217	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/M3_NN5_30_50_350");
218	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/M3_NN5_30_50_350");
219	   hist0->Rebin(1);
220	   hist1->Rebin(1);
221	   hist2->Rebin(1);
222	   hist3->Rebin(1);
223	   hist4->Rebin(1);   
224	   hist6->Rebin(1);   
225	   hist7->Rebin(1);   
226	   hist8->Rebin(1);  
227	   }
228	  if(i==0)
229	  { 
230	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
231	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/NNOutput");
232	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/NNOutput");
233	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/NNOutput");
234	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/NNOutput");
235	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/NNOutput");
236	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/NNOutput");
237	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/NNOutput"); 
238	   }
239	  if(i==1)
240	  { 
241	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Ht");
242	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Ht");
243	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Ht");
244	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Ht");
245	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Ht");
246	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Ht");
247	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Ht");
248	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Ht"); 
249	   hist0->Rebin(1);
250	   hist1->Rebin(1);
251	   hist2->Rebin(1);
252	   hist3->Rebin(1);
253	   hist4->Rebin(1);   
254	   hist6->Rebin(1);   
255	   hist7->Rebin(1);   
256	   hist8->Rebin(1);   
257	   }
258	  if(i==2)
259	  { 
260	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Aplanarity");
261	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Aplanarity");
262	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Aplanarity");
263	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Aplanarity");
264	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Aplanarity");
265	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Aplanarity");
266	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Aplanarity");
267	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Aplanarity"); 
268	   }
269	  if(i==3)
270	  { 
271	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Sphericity");
272	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Sphericity");
273	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Sphericity");
274	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Sphericity");
275	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Sphericity");
276	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Sphericity");
277	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Sphericity");
278	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Sphericity"); 
279	   }
280	  if(i==4)
281	  { 
282	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/C");
283	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/C");
284	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/C");
285	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/C");
286	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/C");
287	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/C");
288	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/C");
289	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/C"); 
290	   }
291	  if(i==5)
292	  { 
293	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/D");
294	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/D");
295	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/D");
296	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/D");
297	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/D");
298	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/D");
299	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/D");
300	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/D"); 
301	   }
302	  if(i==6)
303	  { 
304	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarTauHQQ/Et");
305	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarQQQQ/Et");
306	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarLQQ/Et");
307	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarQQQQ/Et");
308	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarLQQ/Et");
309	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarETauH/Et");
310	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarMTauH/Et");
311	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarTauHTauH/Et"); 
312	   }
313	  if(i==7)
314	  { 
315	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/decayMode");
316	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/decayMode");
317	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/decayMode");
318	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/decayMode");
319	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/decayMode");
320	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/decayMode");
321	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/decayMode");
322	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/decayMode"); 
323	   }
324	  if(i==8)
325	  { 
326	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/Pt");
327	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/Pt");
328	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/Pt");
329	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/Pt");
330	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/Pt");
331	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/Pt");
332	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/Pt");
333	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/Pt"); 
334	   hist0->Rebin(1);
335	   hist1->Rebin(1);
336	   hist2->Rebin(1);
337	   hist3->Rebin(1);
338	   hist4->Rebin(1); hist6->Rebin(1); hist7->Rebin(1); hist8->Rebin(1); 
339	  }
340	  if(i==9)
341	  { 
342	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/q*Eta");
343	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/q*Eta");
344	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/q*Eta");
345	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/q*Eta");
346	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/q*Eta");
347	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/q*Eta");
348	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/q*Eta");
349	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/q*Eta"); 
350	   hist0->Rebin(1);
351	   hist1->Rebin(1);
352	   hist2->Rebin(1);
353	   hist3->Rebin(1);
354	   hist4->Rebin(1); 
355	  }
356	  if(i==10)
357	  { 
358	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/leadTrackPt");
359	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/leadTrackPt");
360	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/leadTrackPt");
361	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/leadTrackPt");
362	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/leadTrackPt");
363	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/leadTrackPt");
364	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/leadTrackPt");
365	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/leadTrackPt"); 
366	  }
367	  if(i==11)
368	  { 
369	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/emFraction");
370	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/emFraction");
371	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/emFraction");
372	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/emFraction");
373	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/emFraction");
374	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/emFraction");
375	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/emFraction");
376	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/emFraction"); 
377	  }
378	  if(i==12)
379	  { 
380	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Multiplicity");
381	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Multiplicity");
382	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Multiplicity");
383	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Multiplicity");
384	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Multiplicity");
385	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Multiplicity");
386	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Multiplicity");
387	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Multiplicity"); 
388	  }
389	 if(i==13)
390	  { 
391	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/MT");
392	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/MT");
393	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/MT");
394	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/MT");
395	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/MT");
396	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/MT");
397	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/MT");
398	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/MT"); 
399	  }
400	 if(i==14)
401	  { 
402	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/DeltaPhiTauMet");
403	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/DeltaPhiTauMet");
404	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/DeltaPhiTauMet");
405	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/DeltaPhiTauMet");
406	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/DeltaPhiTauMet");
407	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/DeltaPhiTauMet");
408	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/DeltaPhiTauMet");
409	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/DeltaPhiTauMet"); 
410	  }
411	  if(i==15)
412	  { 
413	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Pt");
414	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Pt");
415	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Pt");
416	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Pt");
417	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Pt");
418	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Pt");
419	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Pt");
420	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Pt"); 
421	  }
422	  if(i==16)
423	  { 
424	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/sqrts");
425	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/sqrts");
426	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/sqrts");
427	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/sqrts");
428	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/sqrts");
429	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/sqrts");
430	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/sqrts");
431	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/sqrts"); 
432	   hist0->Rebin(4);
433	   hist1->Rebin(4);
434	   hist2->Rebin(4);
435	   hist3->Rebin(4);
436	   hist4->Rebin(4);hist6->Rebin(4); hist7->Rebin(4);hist8->Rebin(4);}
437	  if(i==17)
438	  { 
439	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/kinFitChi2");
440	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/kinFitChi2");
441	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/kinFitChi2");
442	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/kinFitChi2");
443	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/kinFitChi2");
444	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/kinFitChi2");
445	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/kinFitChi2");
446	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/kinFitChi2"); 
447	  }
448	  if(i==18)
449	  { 
450	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Eta");
451	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Eta");
452	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Eta");
453	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Eta");
454	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Eta");
455	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Eta");
456	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Eta");
457	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Eta"); 
458	  }
459	  if(i==19)
460	  { 
461	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/Eta");
462	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/Eta");
463	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/Eta");
464	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/Eta");
465	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/Eta");
466	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/Eta");
467	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/Eta");
468	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/Eta");  
469	  }
470	  if(i==20)
471	  { 
472	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/NPV");
473	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/NPV");
474	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/NPV");
475	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/NPV");
476	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/NPV");
477	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/NPV");
478	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/NPV");
479	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/NPV");  
480	  }
481	
482	   TFile *_file_sTop ;
483	   //  _file_sTop = TFile::Open("~/SemiLeptonicTauXsectionMeas_singletop_3-5fb_medtau_met15_medbtag_NPV.root");
484	   _file_sTop = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_singletop_Fermi.root");
485	   _file_sTop->cd();
486	  
487	  if(i==22)
488	  { 
489	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/M3_NN5_30_50_350");
490	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/M3_NN5_30_50_350");
491	   hist9->Rebin(1);
492	   hist10->Rebin(1);
493	   
494	   }
495	  if(i==0)
496	  { 
497	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/NNOutput");
498	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/NNOutput");
499	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/NNOutput");
500	   }
501	  if(i==1)
502	  { 
503	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Ht");
504	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Ht");
505	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/Ht");
506	   hist9->Rebin(1);
507	   hist10->Rebin(1);
508	   //hist11->Rebin(1);
509	   }
510	  if(i==2)
511	  { 
512	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Aplanarity");
513	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Aplanarity");
514	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/Aplanarity");
515	  }
516	  if(i==3)
517	  { 
518	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Sphericity");
519	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Sphericity");
520	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/Sphericity");
521	  }
522	  if(i==4)
523	  { 
524	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/C");
525	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/C");
526	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/C");
527	   }
528	  if(i==5)
529	  { 
530	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/D");
531	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/D");
532	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/D");
533	   }
534	  if(i==6)
535	  { 
536	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletoptW/Et");
537	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletopBartW/Et");
538	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletopT/Et");
539	  }
540	  if(i==7)
541	  { 
542	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/decayMode");
543	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/decayMode");
544	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/decayMode");
545	  }
546	  if(i==8)
547	  { 
548	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/Pt");
549	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/Pt");
550	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/Pt");
551	   hist9->Rebin(1);
552	   hist10->Rebin(1);
553	  // hist11->Rebin(1);
554	  }
555	  if(i==9)
556	  { 
557	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/q*Eta");
558	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/q*Eta");
559	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/q*Eta");
560	   hist9->Rebin(1);
561	   hist10->Rebin(1); 
562	   //hist11->Rebin(1);
563	  }
564	  if(i==10)
565	  { 
566	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/leadTrackPt");
567	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/leadTrackPt");
568	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/leadTrackPt");
569	  }
570	  if(i==11)
571	  { 
572	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/emFraction");
573	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/emFraction");
574	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/emFraction");
575	  }
576	  if(i==12)
577	  { 
578	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Multiplicity");
579	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Multiplicity");
580	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopT/Multiplicity");
581	 }
582	 if(i==13)
583	  { 
584	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/MT");
585	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/MT");
586	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/MT");
587	   }
588	 if(i==14)
589	  { 
590	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/DeltaPhiTauMet");
591	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/DeltaPhiTauMet");
592	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/DeltaPhiTauMet");
593	  }
594	  if(i==15)
595	  { 
596	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Pt");
597	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Pt");
598	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopT/Pt");
599	  }
600	  if(i==16)
601	  { 
602	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/sqrts");
603	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/sqrts");
604	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/sqrts");
605	   hist9->Rebin(4);
606	   hist10->Rebin(4);
607	   //hist11->Rebin(4);
608	   }
609	  if(i==17)
610	  { 
611	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/kinFitChi2");
612	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/kinFitChi2");
613	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopT/kinFitChi2");
614	  }
615	  if(i==18)
616	  { 
617	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Eta");
618	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Eta");
619	  // hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopT/Eta");
620	  }
621	  if(i==19)
622	  { 
623	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/Eta");
624	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/Eta");
625	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/Eta");
626	  }
627	  if(i==20)
628	  { 
629	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/NPV");
630	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/NPV");
631	   //hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopT/NPV");
632	  }
633	  
634	  // Get MultiJet histo
635	  
636	  TFile *_fileMULTIJET ;
637	  // _fileMULTIJET = TFile::Open("~/SemiLeptonicTauXsectionMeas_multijet_3-5fb_medtau_met15_medbtag_NPV.root");
638	  _fileMULTIJET = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_multijet_check.root");
639	  _fileMULTIJET->cd();
640	    
641	  if(i==22){
642	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
643	   hist5->Rebin(1);}
644	  if(i==0)
645	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
646	  if(i==1){
647	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Ht");
648	   hist5->Rebin(1);}
649	  if(i==2)
650	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Aplanarity");
651	  if(i==3)
652	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Sphericity");
653	  if(i==4)
654	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/C");
655	  if(i==5)
656	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/D");
657	  if(i==6)
658	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/MET/all/met/DATA/Et");
659	  if(i==7)
660	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/decayMode");
661	  if(i==8){
662	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Pt");
663	   hist5->Rebin(1);}
664	  if(i==9){
665	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/q*Eta");
666	   hist5->Rebin(1);}
667	  if(i==10)
668	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/leadTrackPt");
669	  if(i==11)
670	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/emFraction");
671	  if(i==12)
672	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Multiplicity");
673	  if(i==13)
674	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/MT");
675	  if(i==14)
676	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/DeltaPhiTauMet");
677	  if(i==15)
678	    hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Pt");
679	  if(i==16){
680	    hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/sqrts");
681	    hist5->Rebin(4);}
682	  if(i==17)
683	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/kinFitChi2"); 
684	  if(i==18){
685	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Eta");
686	  }  
687	  if(i==19){
688	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Eta");
689	  }
690	  if(i==20){
691	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/NPV");
692	  }
693	  // Get Data histo
694	  
695	  TFile *_fileDATA ;
696	  // _fileDATA = TFile::Open("~/SemiLeptonicTauXsectionMeas_data_3-5fb_medtau_met15_medbtag_NPV.root");
697	  _fileDATA = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_searchsample.root");
698	 
699	  //_fileDATA = TFile::Open("~/SemiLeptonicTauXsectionMeas_Data_1-991fb_relaxed4thjet_looseTau_rebinNN_3pfjet40_NPV.root");
700	  _fileDATA->cd();
701	  
702	  if(i==22){
703	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
704	   histDATA->Rebin(1);}
705	  if(i==0)
706	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
707	  if(i==1){
708	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Ht");
709	   histDATA->Rebin(1);}
710	  if(i==2)
711	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Aplanarity");
712	  if(i==3)
713	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Sphericity");
714	  if(i==4)
715	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/C");
716	  if(i==5)
717	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/D");
718	  if(i==6)
719	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/MET/all/met/DATA/Et");
720	  if(i==7)
721	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/decayMode");
722	  if(i==8){
723	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Pt"); 
724	   histDATA->Rebin(1);}
725	  if(i==9){
726	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/q*Eta"); 
727	   histDATA->Rebin(1);}
728	  if(i==10)
729	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/leadTrackPt");
730	  if(i==11)
731	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/emFraction");
732	  if(i==12)
733	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Multiplicity");
734	  if(i==13)
735	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/MT");
736	  if(i==14)
737	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/DeltaPhiTauMet");
738	  if(i==15)
739	    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Pt");
740	  if(i==16){
741	    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/sqrts");
742	    histDATA->Rebin(4);}
743	  if(i==17)
744	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/kinFitChi2");
745	  if(i==18)
746	    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Eta");
747	  if(i==19){
748	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Eta"); 
749	   }
750	  if(i==20){
751	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/NPV"); 
752	   }
753	  if(i==22){
754	    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
755	 }
756	   
757	  
758	  hist0->SetFillColor(2);hist0->SetLineColor(2);
759	  hist1->SetFillColor(4);hist1->SetLineColor(4);
760	  hist2->SetFillColor(4);hist2->SetLineColor(4);
761	  hist3->SetFillColor(5);hist3->SetLineColor(5);
762	  hist4->SetFillColor(5);hist4->SetLineColor(5);
763	  hist5->SetFillColor(7);hist5->SetLineColor(7);
764	  hist6->SetFillColor(4);hist6->SetLineColor(4); 
765	  hist7->SetFillColor(4);hist7->SetLineColor(4);
766	  hist8->SetFillColor(4);hist8->SetLineColor(4);
767	  hist9->SetFillColor(6);hist9->SetLineColor(6);
768	  hist10->SetFillColor(6);hist10->SetLineColor(6);
769	 
770	  histDATA->SetMarkerStyle(20);
771	 
772	  // Scaling from the fit
773	 
774	  //loose tau  met 15
775	  //hist5->Scale(3.14697);
776	  //hist0->Scale(1.11073);
777	  
778	  //loose tau no b reweight met 15
779	  //hist5->Scale(0.252091);
780	  //hist0->Scale(1.30616);
781	  
782	  //med tau met 15
783	  //hist5->Scale(3.07762);
784	  //hist0->Scale(1.04014);
785	  
786	  //loose tau met25
787	  //hist5->Scale(3.22745); 
788	  //hist0->Scale(1.11232);
789	  
790	  //med tau med btag for multijet met 15
791	  hist5->Scale(4.65478); 
792	  hist0->Scale(0.919743);
793	  
794	  //med tau med btag for multijet met 15 no b reweight
795	  //hist5->Scale(0.2469); 
796	  //hist0->Scale(1.157);
797	 
798	  // For legend and plotting purposes
799	  
800	  TH1F*  hcolor4;
801	  TH1F*  hcolor5;
802	  TH1F*  hcolor6;
803	  TH1F*  hmc;
804	  
805	  hcolor4 = (TH1F*) hist1->Clone();
806	  hcolor4->Add(hist2);
807	  hcolor4->Add(hist6);
808	  hcolor4->Add(hist7);
809	  hcolor4->Add(hist8);
810	  
811	  hcolor5 = (TH1F*) hist3->Clone();
812	  hcolor5->Add(hist4);
813	  
814	  hcolor6 = (TH1F*) hist9->Clone();
815	  hcolor6->Add(hist10);
816	  //hcolor6->Add(hist11);
817	  
818	  hmc = (TH1F*) hist0->Clone(); hmc->Sumw2();
819	  hmc->Add(hcolor4);
820	  hmc->Add(hcolor5);
821	  hmc->Add(hcolor6);
822	  //hmc->Add(hist0);
823	  hmc->Add(hist5);
824	 
825	  
826	   TH1F* hmcHatchedArea = (TH1F*) hmc->Clone();
827	   hmcHatchedArea->SetLineColor(1);
828	   hmcHatchedArea->SetFillStyle(3004);
829	   hmcHatchedArea->SetFillColor(1);
830	 
831	  for(int j=0; j<hmcHatchedArea->GetNbinsX(); j++)
832	  {
833	    hmcHatchedArea->SetBinError(j+1,sqrt(pow(hmc->GetBinError(j+1),2)+pow(hmc->GetBinContent(j+1)*0.13,2)));}
834	    
835	 
836	  //TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
837	  TCanvas *c1 = new TCanvas("c1", "c1",10,54,782,552);
838	  c1->cd();
839	   
840	  //TPad *canvas_1 = new TPad("canvas_1", "canvas_1",0,0.5,1.0,0.98);
841	  TPad *canvas_1 = new TPad("canvas_1", "canvas_1",0,0.25,1.0,0.98);
842	  canvas_1->Range(-8.307211,-9.283388,357.5235,53.01629);
843	  canvas_1->SetLeftMargin(0.159383);
844	  canvas_1->SetRightMargin(0.02056555);
845	  canvas_1->SetTopMargin(0.06148698);
846	  canvas_1->SetBottomMargin(0.1490118);
847	  
848	  
849	  int logy = 0;
850	  canvas_1->SetLogy(logy);
851	  canvas_1->Draw();
852	  canvas_1->cd(); 
853	  
854	  histDATA->GetYaxis()->SetTitleFont(42);
855	  histDATA->GetXaxis()->SetLabelSize(0);
856	  histDATA->GetYaxis()->SetLabelFont(42);
857	  histDATA->GetYaxis()->SetTitleOffset( 0.8 );
858	  histDATA->GetYaxis()->SetTitleSize( 0.05);
859	  histDATA->GetYaxis()->SetLabelSize(0.05); 
860	 // histDATA->SetMinimum(0);
861	 // histDATA->SetMaximum(histDATA->GetMaximum()+15);
862	  histDATA->SetMaximum(50);
863	  //histDATA->GetXaxis()->SetNdivisions(0);
864	 histDATA->Draw("e"); 
865	  //hist0->GetXaxis()->SetNdivisions(0);
866	  //histDATA->SetMinimum(1);
867	  
868	  if(i==0)histDATA->GetYaxis()->SetTitle("Events / 0.1");  
869	  else histDATA->GetYaxis()->SetTitle("Events / 10 GeV");
870	  histDATA->GetYaxis()->SetNdivisions(5,1,0,kFALSE);
871	  hs.Add(hist5); 
872	  hs.Add(hist1);
873	  hs.Add(hist2);
874	  hs.Add(hist6); 
875	  hs.Add(hist7);
876	  hs.Add(hist8);
877	  hs.Add(hist3);
878	  hs.Add(hist4);
879	  hs.Add(hist9);
880	  hs.Add(hist10);
881	  hs.Add(hist0);
882	 
883	  hs.Draw("histosame");hmcHatchedArea->Draw("e2same");
884	 //histDATA->Draw("esameaxis");
885	 histDATA->Draw("esame");
886	 //histDATA->Draw("sameaxis");
887	  
888	  TLegend* qw = 0;
889	  qw = new TLegend(0.6580977,0.507163,0.9498715,0.9306703,NULL,"brNDC");
890	  //qw = new TLegend(0.75,0.70,0.98,0.98);
891	  
892	  qw->AddEntry(histDATA, "Data" ,             "p");
893	  qw->AddEntry(hmcHatchedArea,  "stat.+syst. uncertainty" ,"f");
894	  qw->AddEntry(hist0,    "t#bar{t} #tau_{h}+jets" ,"f");
895	  qw->AddEntry(hcolor6,  "single top" ,"f");
896	  qw->AddEntry(hcolor5,  "W/Z + jets" ,"f");
897	  qw->AddEntry(hcolor4,  "t#bar{t} background" ,"f");
898	  qw->AddEntry(hist5,    "multijet "          ,"f");
899	  
900	  qw->SetFillColor(0);
901	  qw->SetTextFont(42);
902	  qw->SetLineWidth(0);
903	  qw->SetBorderSize(0);
904	  
905	  qw->Draw();
906	    
907	  text1 = new TLatex(0.15,0.9,"CMS #sqrt{s}=7 TeV, 3.9 fb^{-1}");
908	  text1->SetNDC();
909	  text1->SetTextAlign(12);
910	  text1->SetX(0.18);
911	  //text1->SetY(0.92);
912	  //text1->SetLineWidth(2);
913	  text1->SetTextFont(42);
914	  text1->SetTextSize(0.05);
915	  //text1->SetTextSizePixels(24);// dflt=28
916	  text1->Draw();
917	  canvas_1->RedrawAxis() ;
918	  
919	  //
920	 
921	  c1->cd();
922	
923	  
924	  TPad *canvas_2 = new TPad("canvas_2", "canvas_2",0,0.,1.0,0.34);
925	 // canvas_2->Range(-8.307211,-1.381443,357.5235,2.291959);
926	  canvas_2->Draw();
927	  canvas_2->cd();
928	  gPad->SetBottomMargin(0.375);
929	  gPad->SetGridy();
930	  gPad->SetGridx();
931	
932	  histo_ratio = (TH1F*) histDATA->Clone();
933	  
934	  //histDATA->GetXaxis()->SetNdivisions(0);
935	 //histDATA->GetXaxis()->SetRange(100,300);
936	 //histDATA->GetXaxis()->SetTicks(0);
937	 
938	  histo_ratio->SetName("histo_ratio");
939	  histo_ratio->SetTitle("");
940	  histo_ratio->Divide(hmc);
941	
942	  histo_ratio->SetMarkerStyle(20);
943	  histo_ratio->SetMarkerSize(0.7);
944	  //histo_ratio->GetXaxis()->SetRange(4,28);
945	  histo_ratio->GetYaxis()->SetTitle("Data/MC");
946	  histo_ratio->GetYaxis()->SetTitleFont(42);
947	  histo_ratio->GetYaxis()->SetLabelFont(42);
948	  histo_ratio->GetXaxis()->SetTitleFont(42);
949	  histo_ratio->GetXaxis()->SetLabelFont(42);
950	  histo_ratio->GetXaxis()->SetLabelSize(0.1);
951	
952	  histo_ratio->GetYaxis()->SetTitleOffset( 0.4 );
953	  histo_ratio->GetYaxis()->SetTitleSize( 0.1 );
954	  histo_ratio->GetYaxis()->SetLabelSize(0.1);
955	  histo_ratio->GetYaxis()->SetNdivisions( 505 );
956	  histo_ratio->GetXaxis()->SetTitleSize( 0.15 );
957	  histo_ratio->GetXaxis()->SetTitle( " M_{3} (GeV), D_{NN}>0.5" );
958	
959	  histo_ratio->SetMinimum(0.);
960	  histo_ratio->SetMaximum(2.5);
961	  histo_ratio->Draw("E1X0");
962	
963	  c1->cd();
964	   
965	  
966	 // Printing
967	  
968	  TString name1 = "c1_topmeeting_medtau_met15_medbtag_log";
969	  TString name2 = Form("%d", i);
970	  TString name3 = ".pdf";
971	  TString name4 = ".png";
972	  TString name5 = Form("%d", logy);
973	  TString name6 = "_";
974	  TString name_pdf = name1+name5+name6+name2+name3;
975	  TString name_png = name1+name5+name6+name2+name4;
976	  
977	  c1->SaveAs(name_pdf.Data());
978	  //c1->SaveAs(name_png.Data());
979	
980	}

