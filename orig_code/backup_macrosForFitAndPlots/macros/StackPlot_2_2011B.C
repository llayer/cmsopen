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
55	  tdrStyle->SetEndErrorSize(2);
56	//  tdrStyle->SetErrorMarker(20);
57	  tdrStyle->SetErrorX(0.);
58	  
59	  tdrStyle->SetMarkerStyle(20);
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
137	  tdrStyle->SetOptLogx(0);
138	  tdrStyle->SetOptLogy(0);
139	  tdrStyle->SetOptLogz(0);
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
154	  tdrStyle->SetLegendBorderSize(0); 
155	
156	  tdrStyle->cd();
157	
158	}
159	
160	void StackPlot_2_2011B() {
161	   
162	   setTDRStyle();
163	
164	// Which histogram ?  
165	
166	  int i = 0;//1,2,6,12,13,14,16,17,15
167	             //9 8 18 19 20,21,22
168	  
169	  TH1F*  hist0;
170	  TH1F*  hist1;
171	  TH1F*  hist2;
172	  TH1F*  hist3;
173	  TH1F*  hist4;
174	  TH1F*  hist5;
175	  TH1F*  hist6;
176	  TH1F*  hist7;
177	  TH1F*  hist8;
178	  TH1F*  hist9;
179	  TH1F*  hist10;
180	  TH1F*  hist11;
181	  TH1F*  histDATA;
182	  
183	  
184	  
185	// Grab PDF's from histos
186	  
187	  THStack hs("hs","test stacked histograms");
188	   
189	  TFile *_file_TTbar ;
190	  _file_TTbar = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_WZT_Fermi.root");
191	  _file_TTbar->cd();
192	  
193	  
194	  if(i==0)
195	  { 
196	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/NNOutput");
197	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/NNOutput");
198	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/NNOutput");
199	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/NNOutput");
200	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/NNOutput");
201	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/NNOutput");
202	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/NNOutput");
203	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/NNOutput"); 
204	   }
205	  if(i==1)
206	  { 
207	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Ht");
208	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Ht");
209	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Ht");
210	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Ht");
211	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Ht");
212	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Ht");
213	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Ht");
214	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Ht"); 
215	   hist0->Rebin(2);
216	   hist1->Rebin(2);
217	   hist2->Rebin(2);
218	   hist3->Rebin(2);
219	   hist4->Rebin(2);   
220	   hist6->Rebin(2);   
221	   hist7->Rebin(2);   
222	   hist8->Rebin(2);   
223	   }
224	  if(i==2)
225	  { 
226	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Aplanarity");
227	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Aplanarity");
228	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Aplanarity");
229	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Aplanarity");
230	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Aplanarity");
231	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Aplanarity");
232	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Aplanarity");
233	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Aplanarity"); 
234	   }
235	  if(i==3)
236	  { 
237	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/Sphericity");
238	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/Sphericity");
239	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/Sphericity");
240	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/Sphericity");
241	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/Sphericity");
242	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/Sphericity");
243	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/Sphericity");
244	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/Sphericity"); 
245	   }
246	  if(i==4)
247	  { 
248	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/C");
249	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/C");
250	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/C");
251	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/C");
252	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/C");
253	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/C");
254	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/C");
255	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/C"); 
256	   }
257	  if(i==5)
258	  { 
259	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/D");
260	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/D");
261	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/D");
262	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/D");
263	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/D");
264	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/D");
265	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/D");
266	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/D"); 
267	   }
268	  if(i==6)
269	  { 
270	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarTauHQQ/Et");
271	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarQQQQ/Et");
272	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarLQQ/Et");
273	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/WJetsToLNu/Et");
274	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/DYJetsToLL/Et");
275	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarETauH/Et");
276	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarMTauH/Et");
277	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/MET/all/met/TTbarTauHTauH/Et"); 
278	   }
279	  if(i==7)
280	  { 
281	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/decayMode");
282	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/decayMode");
283	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/decayMode");
284	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/decayMode");
285	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/decayMode");
286	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/decayMode");
287	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/decayMode");
288	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/decayMode"); 
289	   }
290	  if(i==8)
291	  { 
292	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/Pt");
293	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/Pt");
294	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/Pt");
295	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/Pt");
296	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/Pt");
297	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/Pt");
298	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/Pt");
299	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/Pt"); 
300	   hist0->Rebin(2); hist1->Rebin(2); hist2->Rebin(2); hist3->Rebin(2);
301	   hist4->Rebin(2); hist6->Rebin(2); hist7->Rebin(2); hist8->Rebin(2); 
302	  }
303	  if(i==9)
304	  { 
305	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/q*Eta");
306	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/q*Eta");
307	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/q*Eta");
308	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/q*Eta");
309	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/q*Eta");
310	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/q*Eta");
311	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/q*Eta");
312	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/q*Eta"); 
313	   hist0->Rebin(1);
314	   hist1->Rebin(1);
315	   hist2->Rebin(1);
316	   hist3->Rebin(1);
317	   hist4->Rebin(1); 
318	  }
319	  if(i==10)
320	  { 
321	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/leadTrackPt");
322	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/leadTrackPt");
323	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/leadTrackPt");
324	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/leadTrackPt");
325	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/leadTrackPt");
326	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/leadTrackPt");
327	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/leadTrackPt");
328	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/leadTrackPt"); 
329	  }
330	  if(i==11)
331	  { 
332	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/emFraction");
333	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/emFraction");
334	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/emFraction");
335	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/emFraction");
336	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/emFraction");
337	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/emFraction");
338	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/emFraction");
339	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/emFraction"); 
340	  }
341	  if(i==12)
342	  { 
343	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Multiplicity");
344	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Multiplicity");
345	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Multiplicity");
346	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Multiplicity");
347	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Multiplicity");
348	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Multiplicity");
349	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Multiplicity");
350	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Multiplicity"); 
351	  }
352	 if(i==13)
353	  { 
354	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/MT");
355	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/MT");
356	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/MT");
357	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/MT");
358	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/MT");
359	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/MT");
360	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/MT");
361	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/MT"); 
362	  }
363	 if(i==14)
364	  { 
365	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/DeltaPhiTauMet");
366	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/DeltaPhiTauMet");
367	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/DeltaPhiTauMet");
368	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/DeltaPhiTauMet");
369	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/DeltaPhiTauMet");
370	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/DeltaPhiTauMet");
371	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/DeltaPhiTauMet");
372	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/DeltaPhiTauMet"); 
373	  }
374	  if(i==15)
375	  { 
376	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Pt");
377	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Pt");
378	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Pt");
379	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Pt");
380	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Pt");
381	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Pt");
382	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Pt");
383	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Pt"); 
384	   hist0->Rebin(2);hist1->Rebin(2);hist2->Rebin(2); hist3->Rebin(2);
385	   hist4->Rebin(2);hist6->Rebin(2); hist7->Rebin(2);hist8->Rebin(2);}
386	  if(i==16)
387	  { 
388	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/sqrtsNN");
389	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/sqrtsNN");
390	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/sqrtsNN");
391	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/sqrtsNN");
392	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/sqrtsNN");
393	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/sqrtsNN");
394	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/sqrtsNN");
395	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/sqrtsNN"); 
396	   hist0->Rebin(4);
397	   hist1->Rebin(4);
398	   hist2->Rebin(4);
399	   hist3->Rebin(4);
400	   hist4->Rebin(4);hist6->Rebin(4); hist7->Rebin(4);hist8->Rebin(4);}
401	  if(i==17)
402	  { 
403	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/kinFitChi2");
404	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/kinFitChi2");
405	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/kinFitChi2");
406	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/kinFitChi2");
407	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/kinFitChi2");
408	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/kinFitChi2");
409	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/kinFitChi2");
410	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/kinFitChi2"); 
411	  }
412	  if(i==18)
413	  { 
414	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHQQ/Eta");
415	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarQQQQ/Eta");
416	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarLQQ/Eta");
417	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/WJetsToLNu/Eta");
418	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DYJetsToLL/Eta");
419	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarETauH/Eta");
420	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarMTauH/Eta");
421	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Jets/all/met/TTbarTauHTauH/Eta"); 
422	  }
423	  if(i==19)
424	  { 
425	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/Eta");
426	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/Eta");
427	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/Eta");
428	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/Eta");
429	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/Eta");
430	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/Eta");
431	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/Eta");
432	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/Eta");  
433	  }
434	  if(i==20)
435	  { 
436	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHQQ/NPV");
437	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarQQQQ/NPV");
438	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarLQQ/NPV");
439	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/WJetsToLNu/NPV");
440	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DYJetsToLL/NPV");
441	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarETauH/NPV");
442	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarMTauH/NPV");
443	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/Taus/all/met/TTbarTauHTauH/NPV");  
444	  }
445	 if(i==21)
446	  { 
447	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/kinFitProba");
448	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/kinFitProba");
449	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/kinFitProba");
450	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/kinFitProba");
451	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/kinFitProba");
452	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/kinFitProba");
453	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/kinFitProba");
454	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/kinFitProba"); 
455	  }
456	  if(i==22)
457	  { 
458	   hist0 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHQQ/M3_NN5_30_50_350");
459	   hist1 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarQQQQ/M3_NN5_30_50_350");
460	   hist2 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarLQQ/M3_NN5_30_50_350");
461	   hist3 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/WJetsToLNu/M3_NN5_30_50_350");
462	   hist4 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DYJetsToLL/M3_NN5_30_50_350");
463	   hist6 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarETauH/M3_NN5_30_50_350");
464	   hist7 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarMTauH/M3_NN5_30_50_350");
465	   hist8 = (TH1F*)_file_TTbar->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/TTbarTauHTauH/M3_NN5_30_50_350"); 
466	   
467	   /*hist0->Rebin(2);
468	   hist1->Rebin(2);
469	   hist2->Rebin(2);
470	   hist3->Rebin(2);
471	   hist4->Rebin(2);
472	   hist7->Rebin(2);
473	   hist8->Rebin(2);
474	   hist6->Rebin(2);*/
475	   }
476	  
477	   TFile *_file_sTop ;
478	  _file_sTop = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_singletop_Fermi.root");
479	  _file_sTop->cd();
480	  
481	  if(i==0)
482	  { 
483	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/NNOutput");
484	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/NNOutput");
485	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/NNOutput");
486	   }
487	  if(i==1)
488	  { 
489	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Ht");
490	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Ht");
491	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/Ht");
492	   hist9->Rebin(2);
493	   hist10->Rebin(2);
494	   hist11->Rebin(2);
495	   }
496	  if(i==2)
497	  { 
498	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Aplanarity");
499	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Aplanarity");
500	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/Aplanarity");
501	  }
502	  if(i==3)
503	  { 
504	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/Sphericity");
505	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/Sphericity");
506	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/Sphericity");
507	  }
508	  if(i==4)
509	  { 
510	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/C");
511	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/C");
512	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/C");
513	   }
514	  if(i==5)
515	  { 
516	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/D");
517	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/D");
518	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/D");
519	   }
520	  if(i==6)
521	  { 
522	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletoptW/Et");
523	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/SingletopBartW/Et");
524	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/MET/all/met/Singletopt/Et");
525	  }
526	  if(i==7)
527	  { 
528	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/decayMode");
529	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/decayMode");
530	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/decayMode");
531	  }
532	  if(i==8)
533	  { 
534	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/Pt");
535	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/Pt");
536	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/Pt");
537	   hist9->Rebin(2);
538	   hist10->Rebin(2);
539	   hist11->Rebin(2);
540	  }
541	  if(i==9)
542	  { 
543	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/q*Eta");
544	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/q*Eta");
545	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/q*Eta");
546	   hist9->Rebin(1);
547	   hist10->Rebin(1); 
548	   hist11->Rebin(1);
549	  }
550	  if(i==10)
551	  { 
552	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/leadTrackPt");
553	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/leadTrackPt");
554	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/leadTrackPt");
555	  }
556	  if(i==11)
557	  { 
558	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/emFraction");
559	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/emFraction");
560	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/emFraction");
561	  }
562	  if(i==12)
563	  { 
564	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Multiplicity");
565	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Multiplicity");
566	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/Singletopt/Multiplicity");
567	 }
568	 if(i==13)
569	  { 
570	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/MT");
571	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/MT");
572	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/MT");
573	   }
574	 if(i==14)
575	  { 
576	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/DeltaPhiTauMet");
577	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/DeltaPhiTauMet");
578	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/DeltaPhiTauMet");
579	  }
580	  if(i==15)
581	  { 
582	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Pt");
583	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Pt");
584	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/Singletopt/Pt");
585	   hist9->Rebin(2);
586	   hist10->Rebin(2);
587	   hist11->Rebin(2);
588	  }
589	  if(i==16)
590	  { 
591	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/sqrtsNN");
592	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/sqrtsNN");
593	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/sqrtsNN");
594	   hist9->Rebin(4);
595	   hist10->Rebin(4);
596	   hist11->Rebin(4);
597	   }
598	  if(i==17)
599	  { 
600	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/kinFitChi2");
601	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/kinFitChi2");
602	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/kinFitChi2");
603	  }
604	  if(i==18)
605	  { 
606	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletoptW/Eta");
607	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/SingletopBartW/Eta");
608	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Jets/all/met/Singletopt/Eta");
609	  }
610	  if(i==19)
611	  { 
612	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/Eta");
613	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/Eta");
614	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/Eta");
615	  }
616	  if(i==20)
617	  { 
618	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletoptW/NPV");
619	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/SingletopBartW/NPV");
620	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/Taus/all/met/Singletopt/NPV");
621	  }
622	  if(i==21)
623	  { 
624	   hist9  = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/kinFitProba");
625	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/kinFitProba");
626	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/kinFitProba");
627	  }
628	  if(i==22)
629	  { 
630	   hist9 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletoptW/M3_NN5_30_50_350");
631	   hist10 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/SingletopBartW/M3_NN5_30_50_350");
632	   hist11 = (TH1F*)_file_sTop->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/Singletopt/M3_NN5_30_50_350");
633	   //hist9->Rebin(2);hist10->Rebin(2);hist11->Rebin(2);
634	   }
635	  // Get MultiJet histo
636	  
637	  TFile *_fileMULTIJET ;
638	  _fileMULTIJET =
639	  TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_multijet_check.root");
640	  _fileMULTIJET->cd();
641	    
642	  if(i==0)
643	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
644	  if(i==1){
645	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Ht");
646	   hist5->Rebin(2);
647	   }
648	  if(i==2)
649	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Aplanarity");
650	  if(i==3)
651	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Sphericity");
652	  if(i==4)
653	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/C");
654	  if(i==5)
655	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/D");
656	  if(i==6)
657	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/MET/all/met/DATA/Et");
658	  if(i==7)
659	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/decayMode");
660	  if(i==8){
661	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Pt");
662	   hist5->Rebin(2);
663	   }
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
677	  if(i==15){
678	    hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Pt"); hist5->Rebin(2);}
679	  if(i==16){
680	    hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/sqrtsNN");
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
693	   if(i==21)
694	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/kinFitProba"); 
695	// Get Data histo
696	  if(i==22){
697	   hist5 = (TH1F*)_fileMULTIJET->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
698	   //hist5->Rebin(2);
699	   }
700	   
701	  TFile *_fileDATA ;
702	  _fileDATA = TFile::Open("~/Final_rootfiles/SemiLeptonicTauXsectionMeas_searchsample.root");
703	  //_fileDATA = TFile::Open("~/SemiLeptonicTauXsectionMeas_Data_1-991fb_relaxed4thjet_looseTau_rebinNN_3pfjet40.root");
704	  _fileDATA->cd();
705	  
706	  if(i==0)
707	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/NNOutput");
708	  if(i==1){
709	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Ht");
710	   histDATA->Rebin(2);
711	   }
712	  if(i==2)
713	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Aplanarity");
714	  if(i==3)
715	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/Sphericity");
716	  if(i==4)
717	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/C");
718	  if(i==5)
719	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/D");
720	  if(i==6)
721	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/MET/all/met/DATA/Et");
722	  if(i==7)
723	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/decayMode");
724	  if(i==8){
725	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Pt"); 
726	   histDATA->Rebin(2);
727	   }
728	  if(i==9){
729	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/q*Eta"); 
730	   histDATA->Rebin(1);}
731	  if(i==10)
732	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/leadTrackPt");
733	  if(i==11)
734	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/emFraction");
735	  if(i==12)
736	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Multiplicity");
737	  if(i==13)
738	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/MT");
739	  if(i==14)
740	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/DeltaPhiTauMet");
741	  if(i==15){
742	    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Pt");histDATA->Rebin(2);}
743	  if(i==16){
744	    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/sqrtsNN");
745	    histDATA->Rebin(4);}
746	  if(i==17)
747	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/kinFitChi2");
748	  if(i==18)
749	    histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Jets/all/met/DATA/Eta");
750	  if(i==19){
751	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/Eta"); 
752	   }
753	  if(i==20){
754	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/Taus/all/met/DATA/NPV"); 
755	   }
756	  if(i==21)
757	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/kinFitProba");
758	  if(i==22){
759	   histDATA = (TH1F*)_fileDATA->Get("SemiLeptonicTauAnaPlots/EventShapes/all/met/DATA/M3_NN5_30_50_350");
760	   //histDATA->Rebin(2);
761	   }
762	   
763	  hist0->SetFillColor(2);hist0->SetLineColor(2);
764	  hist1->SetFillColor(4);hist1->SetLineColor(4);
765	  hist2->SetFillColor(4);hist2->SetLineColor(4);
766	  hist3->SetFillColor(5);hist3->SetLineColor(5);
767	  hist4->SetFillColor(5);hist4->SetLineColor(5);
768	  hist5->SetFillColor(7);hist5->SetLineColor(7);
769	  hist6->SetFillColor(4);hist6->SetLineColor(4); 
770	  hist7->SetFillColor(4);hist7->SetLineColor(4);
771	  hist8->SetFillColor(4);hist8->SetLineColor(4);
772	  hist9->SetFillColor(6);hist9->SetLineColor(6);
773	  hist10->SetFillColor(6);hist10->SetLineColor(6);
774	  hist11->SetFillColor(6);hist11->SetLineColor(6);
775	 
776	  histDATA->SetMarkerStyle(20);
777	 
778	  // Scaling from the fit
779	 
780	  //loose tau  met 15
781	  //hist5->Scale(3.14697);
782	  //hist0->Scale(1.11073);
783	  
784	  //loose tau no b reweight met 15
785	  //hist5->Scale(0.252091);
786	  //hist0->Scale(1.30616);
787	  
788	  //med tau met 15
789	  //hist5->Scale(3.07762);
790	  //hist0->Scale(1.04014);
791	  
792	  //loose tau met25
793	  //hist5->Scale(3.22745); 
794	  //hist0->Scale(1.11232);
795	  
796	  //med tau med btag for multijet met 15
797	  //hist5->Scale(3.0836); 
798	  //hist0->Scale(1.01974);
799	 
800	  //med tau med btag for multijet met 15, 20 bins
801	  //hist5->Scale(3.07959); 
802	  //hist0->Scale(1.03221);
803	  
804	  //med tau med btag for multijet met 15 no b reweight
805	  //hist5->Scale(0.2469); 
806	  //hist0->Scale(1.157);
807	  
808	  //med tau med btag for multijet met 15 BTV-003
809	  //hist5->Scale(2.96986); 
810	  //hist0->Scale(0.991392);
811	  
812	  //med tau med btag for multijet met 15 BTV-003 +new lumi
813	  //hist5->Scale(2.9181); 
814	  //hist0->Scale(0.86);
815	  
816	  //med tau med btag for multijet met 15 BTV-003 +new lumi+no reweight
817	  //hist5->Scale(0.247054); 
818	  //hist0->Scale(0.97698);
819	  
820	  //med tau med btag for multijet met 20 BTV-003 +old lumi 428p7 + new tauleg
821	 // hist5->Scale(3.198); 
822	  //hist0->Scale(0.902743);
823	 
824	  //med tau med btag for multijet met 20 BTV-003 +old lumi 428p7 + new tauleg
825	  //hist5->Scale(4.66801); 
826	  //hist0->Scale(1.01858);
827	   
828	  // med tau med btag for multijet met 20 BTV-003 +old lumi 428p7 + new tauleg +noreweight
829	  //hist5->Scale(0.186228); 
830	  //hist0->Scale(1.108);
831	  
832	  //med tau med btag for multijet met 20 BTV-003 +new lumi 428p7 + new tauleg
833	  hist5->Scale(4.65478); 
834	  hist0->Scale(0.919743);
835	  
836	  //med tau med btag for multijet met 20 BTV-003 +new lumi 428p7 + new tauleg+noreweight
837	  //hist5->Scale(0.185867); 
838	  //hist0->Scale(1.0299);
839	 
840	  // For legend and plotting purposes
841	  
842	  TH1F*  hcolor4;
843	  TH1F*  hcolor5;
844	  TH1F*  hcolor6;
845	  TH1F*  hmc;
846	  
847	  hcolor4 = (TH1F*) hist1->Clone();
848	  hcolor4->Add(hist2);
849	  hcolor4->Add(hist6);
850	  hcolor4->Add(hist7);
851	  hcolor4->Add(hist8);
852	  
853	  hcolor5 = (TH1F*) hist3->Clone();
854	  hcolor5->Add(hist4);
855	  
856	  hcolor6 = (TH1F*) hist9->Clone();
857	  hcolor6->Add(hist10);
858	  hcolor6->Add(hist11);
859	  
860	  hmc = (TH1F*) hist0->Clone();
861	  hmc->Add(hcolor4);
862	  hmc->Add(hcolor5);
863	  hmc->Add(hcolor6);
864	  //hmc->Add(hist0);
865	  hmc->Add(hist5);
866	  
867	 
868	  // Error band
869	  
870	 /* TH1F * lumiband = (TH1F*) hmc->Clone();
871	    for (int ilum=0; ilum<lumiband->GetNbinsX(); ilum++) {
872	     lumiband->SetBinError(ilum+1,lumiband->GetBinContent(ilum+1)*0.06);
873	    }
874	    TGraphErrors *thegraph = new TGraphErrors(lumiband);
875	    thegraph->SetFillStyle(3005);
876	    thegraph->SetFillColor(0);*/
877	
878	  //
879	  
880	  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
881	  c1->cd();
882	   
883	  //TPad *canvas_1 = new TPad("canvas_1", "canvas_1",0,0.5,1.0,0.98);
884	  TPad *canvas_1 = new TPad("canvas_1", "canvas_1",0,0.25,1.0,0.98);
885	  
886	  canvas_1->Draw();
887	  canvas_1->cd(); 
888	  int logy = 1;
889	  canvas_1->SetLogy(logy);
890	  
891	  histDATA->GetYaxis()->SetTitleFont(42);
892	  histDATA->GetYaxis()->SetLabelFont(42);
893	  histDATA->GetYaxis()->SetTitleOffset(0.8);
894	  histDATA->GetYaxis()->SetTitleSize(0.05);
895	  histDATA->GetYaxis()->SetLabelSize(0.05); 
896	  
897	  if(i==12) histDATA->GetXaxis()->SetTitle("N. of jets"); 
898	  if(i==6 ) histDATA->GetXaxis()->SetTitle("MET (GeV)"); 
899	  if(i==16 ) histDATA->GetXaxis()->SetTitle("M(#tau,jets) (GeV)"); 
900	  
901	  histDATA->Draw("e"); 
902	  //histDATA->SetMinimum(1);
903	  
904	  if(i==0 && logy==0)histDATA->SetMaximum(histDATA->GetMaximum()+90);
905	  else if(i==0 && logy==1)histDATA->SetMaximum(histDATA->GetMaximum()+11000);
906	  else if(i==1) histDATA->SetMaximum(histDATA->GetMaximum()+2000); 
907	  else if(i==2) histDATA->SetMaximum(histDATA->GetMaximum()+3000); 
908	  else if(i==6) histDATA->SetMaximum(histDATA->GetMaximum()+2000); 
909	  else if(i==9) histDATA->SetMaximum(histDATA->GetMaximum()+18000); 
910	  else if(i==14) histDATA->SetMaximum(histDATA->GetMaximum()+16000); 
911	  else if(i==22) histDATA->SetMaximum(histDATA->GetMaximum()+10); 
912	  else if(i==18) histDATA->SetMaximum(histDATA->GetMaximum()+18000); 
913	  else if(i==19) histDATA->SetMaximum(histDATA->GetMaximum()+14000); 
914	  else if(i==12) histDATA->SetMaximum(histDATA->GetMaximum()+6000); 
915	  else if(i==16) histDATA->SetMaximum(histDATA->GetMaximum()+5000); 
916	  else if(i==13) histDATA->SetMaximum(histDATA->GetMaximum()+3000); 
917	  else if(i==17) histDATA->SetMaximum(histDATA->GetMaximum()+3000); 
918	  else if (i==8 || i==15) histDATA->SetMaximum(histDATA->GetMaximum()+5000);
919	  
920	  if (logy==0) histDATA->SetMinimum(0.);
921	  else         histDATA->SetMinimum(0.5);
922	  
923	  if(i==0) histDATA->GetYaxis()->SetTitle("Events / 0.1");  
924	  else if (i==1) histDATA->GetYaxis()->SetTitle("events / 60 GeV");  
925	  else if (i==2) histDATA->GetYaxis()->SetTitle("events / 0.02");  
926	  else if (i==6) histDATA->GetYaxis()->SetTitle("events / 4 GeV");  
927	  else if (i==9) histDATA->GetYaxis()->SetTitle("events / 0.3");   
928	  else if (i==13) histDATA->GetYaxis()->SetTitle("events / 10 GeV");  
929	  else if (i==14) histDATA->GetYaxis()->SetTitle("events / 0.18");  
930	  else if (i==8 || i==15) histDATA->GetYaxis()->SetTitle("events / 4 GeV");  
931	  else if (i==16) histDATA->GetYaxis()->SetTitle("events / 80 GeV");  
932	  else if (i==17) histDATA->GetYaxis()->SetTitle("events / 1");  
933	  else if (i==21) histDATA->GetYaxis()->SetTitle("events / 0.02");  
934	  else if (i==22) histDATA->GetYaxis()->SetTitle("events / 7 GeV");   
935	  else if (i==18 ||i==19) histDATA->GetYaxis()->SetTitle("events / 0.1");  
936	  else histDATA->GetYaxis()->SetTitle("events");
937	
938	  //if (logy==0){
939	  hs.Add(hist5);
940	  hs.Add(hist1);
941	  hs.Add(hist2);
942	  hs.Add(hist6); 
943	  hs.Add(hist7);
944	  hs.Add(hist8);
945	  hs.Add(hist3);
946	  hs.Add(hist4);
947	  hs.Add(hist9);
948	  hs.Add(hist10);
949	  hs.Add(hist11);
950	  hs.Add(hist0);
951	  /*}
952	  else {
953	  hs.Add(hist1);
954	  hs.Add(hist2);
955	  hs.Add(hist6); 
956	  hs.Add(hist7);
957	  hs.Add(hist8);
958	  hs.Add(hist3);
959	  hs.Add(hist4);
960	  hs.Add(hist9);
961	  hs.Add(hist10);
962	  hs.Add(hist11);
963	  hs.Add(hist0); 
964	  hs.Add(hist5);
965	  }*/
966	 
967	  
968	
969	  hs.Draw("histosame");
970	  histDATA->Draw("e1same"); histDATA->Draw("sameaxis");
971	  //thegraph->Draw("esame");
972	  
973	  std::cout << histDATA->Chi2Test(hmc,"UWPCHI2/NDF") <<std::endl;//CHI2 CHI2/NDF //NORM
974	  
975	  // Legend
976	  
977	  TLegend* qw = 0;
978	  //qw = new TLegend(0.77,0.63,0.95,0.95);  
979	  qw = new TLegend(0.71,0.57,0.97,0.92);  
980	  //qw = new TLegend(0.6580977,0.507163,0.9498715,0.9306703,NULL,"brNDC");
981	
982	  /*if(logy==1){
983	  qw->AddEntry(histDATA, "Data" ,             "p");
984	  qw->AddEntry(hist5,    "MultiJet "          ,"f");
985	  qw->AddEntry(hist0,    "t#bar{t} #tau+jets" ,"f");
986	  qw->AddEntry(hcolor6,  "single top" ,"f");
987	  qw->AddEntry(hcolor5,  "W/Z + jets" ,"f");
988	  qw->AddEntry(hcolor4,  "t#bar{t} background" ,"f");
989	  qw->SetFillColor(0);
990	  qw->SetTextFont(42);
991	  qw->SetLineWidth(0);}
992	  else {*/
993	  qw->AddEntry(histDATA, "Data" ,              "p");
994	  qw->AddEntry(hist0,    "t#bar{t} #tau_{h}+jets" ,"f");
995	  qw->AddEntry(hcolor6,  "single top" ,"f");
996	  qw->AddEntry(hcolor5,  "W/Z + jets" ,"f");
997	  qw->AddEntry(hcolor4,  "t#bar{t} background" ,"f");
998	  qw->AddEntry(hist5,    "multijet "          ,"f");
999	  qw->SetFillColor(0);
1000	  qw->SetTextFont(42);
1001	  qw->SetLineWidth(0);//}
1002	  qw->Draw();
1003	    
1004	  text1 = new TLatex(0.18,0.88,"CMS #sqrt{s}=7 TeV, 3.9 fb^{-1}");
1005	  text1->SetNDC();
1006	  //text1->SetTextAlign(13);
1007	  //text1->SetX(0.18);
1008	  //text1->SetY(0.92);
1009	  //text1->SetLineWidth(2);
1010	  text1->SetTextFont(42);
1011	  text1->SetTextSize(0.05);
1012	  //text1->SetTextSizePixels(24);// dflt=28
1013	  text1->Draw();
1014	  
1015	  //
1016	  c1->cd();
1017	
1018	  
1019	  TPad *canvas_2 = new TPad("canvas_2", "canvas_2",0,0.,1.0,0.34);
1020	  canvas_2->Draw();
1021	  canvas_2->cd();
1022	  gPad->SetBottomMargin(0.375);
1023	  gPad->SetGridy();
1024	  gPad->SetGridx();
1025	
1026	  histo_ratio = (TH1F*) histDATA->Clone();
1027	  histo_ratio->SetName("histo_ratio");
1028	  histo_ratio->SetTitle("");
1029	  histo_ratio->Divide(hmc);
1030	
1031	  histo_ratio->SetMarkerStyle(20);
1032	  histo_ratio->SetMarkerSize(0.7);
1033	  histo_ratio->GetYaxis()->SetTitle("Data/MC");
1034	  histo_ratio->GetYaxis()->SetTitleFont(42);
1035	  histo_ratio->GetYaxis()->SetLabelFont(42);
1036	  histo_ratio->GetXaxis()->SetTitleFont(42);
1037	  histo_ratio->GetXaxis()->SetLabelFont(42);
1038	  histo_ratio->GetXaxis()->SetLabelSize(0.1);
1039	
1040	  histo_ratio->GetYaxis()->SetTitleOffset( 0.4 );
1041	  histo_ratio->GetYaxis()->SetTitleSize( 0.1 );
1042	  histo_ratio->GetYaxis()->SetLabelSize(0.1);
1043	  histo_ratio->GetYaxis()->SetNdivisions( 505 );
1044	  histo_ratio->GetXaxis()->SetTitleSize( 0.15 );
1045	  histo_ratio->GetXaxis()->SetTitle( "D_{NN}" );
1046	
1047	  histo_ratio->SetMinimum(0.);
1048	  histo_ratio->SetMaximum(2.);
1049	  histo_ratio->Draw("E1X0");
1050	
1051	  c1->cd();
1052	   
1053	 
1054	 // Printing
1055	  
1056	  TString name1 = "c1_topmeeting_medtau_met20_medbtag_BTV003_newLumi_newTauLeg_428p7_JP_log";
1057	  TString name2 = Form("%d", i);
1058	  TString name3 = ".pdf";
1059	  TString name4 = ".png";
1060	  TString name5 = Form("%d", logy);
1061	  TString name6 = "_";
1062	  TString name_pdf = name1+name5+name6+name2+name3;
1063	  TString name_png = name1+name5+name6+name2+name4;
1064	  
1065	  c1->SaveAs(name_pdf.Data());
1066	  //c1->SaveAs(name_png.Data());
1067	
1068	}
