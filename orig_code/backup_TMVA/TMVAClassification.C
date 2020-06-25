1	
2	// @(#)root/tmva $Id: TMVAClassification.C,v 1.36 2009-04-14 13:08:13 andreas.hoecker Exp $
3	/**********************************************************************************
4	 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
5	 * Package   : TMVA                                                               *
6	 * Root Macro: TMVAClassification                                                 *
7	 *                                                                                *
8	 * This macro provides examples for the training and testing of the               *
9	 * TMVA classifiers.                                                              *
10	 *                                                                                *
11	 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
12	 * and linearly correlated input variables.                                       *
13	 *                                                                                *
14	 * The methods to be 
15	#include "TMVAGui.C"used can be switched on and off by means of booleans, or     *
16	 * via the prompt command, for example:                                           *
17	 *                                                                                *
18	 *    root -l TMVAClassification.C\(\"Fisher,Likelihood\"\)                       *
19	 *                                                                                *
20	 * (note that the backslashes are mandatory)                                      *
21	 * If no method given, a default set is used.                                     *
22	 *                                                                                *
23	 * The output file "TMVA.root" can be analysed with the use of dedicated          *
24	 * macros (simply say: root -l <macro.C>), which can be conveniently              *
25	 * invoked through a GUI that will appear at the end of the run of this macro.    *
26	 **********************************************************************************/
27	
28	#include <cstdlib>
29	#include <iostream> 
30	#include <map>
31	#include <string>
32	
33	#include "TChain.h"
34	#include "TFile.h"
35	#include "TTree.h"
36	#include "TString.h"
37	#include "TObjString.h"
38	#include "TSystem.h"
39	#include "TROOT.h"
40	#include "TPluginManager.h"
41	
42	#include "TMVAGui.C"
43	
44	#ifndef __CINT__
45	#include "TMVA/Tools.h"
46	#include "TMVA/Factory.h"
47	#endif
48	
49	using namespace TMVA;
50	
51	// read input data file with ascii format (otherwise ROOT) ?
52	Bool_t ReadDataFromAsciiIFormat = kFALSE;
53	   
54	void TMVAClassification( TString myMethodList = "" ) 
55	{
56	   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
57	   // if you use your private .rootrc, or run from a different directory, please copy the 
58	   // corresponding lines from .rootrc
59	
60	   // methods to be processed can be given as an argument; use format:
61	   //
62	   // mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
63	   //
64	   // if you like to use a method via the plugin mechanism, we recommend using
65	   // 
66	   // mylinux~> root -l TMVAClassification.C\(\"P_myMethod\"\)
67	   // (an example is given for using the BDT as plugin (see below),
68	   // but of course the real application is when you write your own
69	   // method based)
70	
71	   // this loads the library
72	   TMVA::Tools::Instance();
73	
74	   //---------------------------------------------------------------
75	   // default MVA methods to be trained + tested
76	   std::map<std::string,int> Use;
77	
78	   Use["Cuts"]            =0;
79	   Use["CutsD"]           =0;
80	   Use["CutsPCA"]         =0;
81	   Use["CutsGA"]          =0;
82	   Use["CutsSA"]          =0;
83	   // ---
84	   Use["Likelihood"]      =0;
85	   Use["LikelihoodD"]     =0; // the "D" extension indicates decorrelated input variables (see option strings)
86	   Use["LikelihoodPCA"]   =0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
87	   Use["LikelihoodKDE"]   =0;
88	   Use["LikelihoodMIX"]   =0;
89	   // ---
90	   Use["PDERS"]           =0;
91	   Use["PDERSD"]          =0;
92	   Use["PDERSPCA"]        =0;
93	   Use["PDERSkNN"]        =0; // depreciated until further notice
94	   Use["PDEFoam"]         =0;
95	   // --
96	   Use["KNN"]             =0;
97	   // ---
98	   Use["HMatrix"]         =0;
99	   Use["Fisher"]          =0;
100	   Use["FisherG"]         =0;
101	   Use["BoostedFisher"]   =0;
102	   Use["LD"]              =0;
103	   // ---
104	   Use["FDA_GA"]          =0;
105	   Use["FDA_SA"]          =0;
106	   Use["FDA_MC"]          =0;
107	   Use["FDA_MT"]          =0;
108	   Use["FDA_GAMT"]        =0;
109	   Use["FDA_MCMT"]        =0;
110	   // ---
111	   Use["MLP"]             = 1; // this is the recommended ANN
112	   Use["MLPBFGS"]         = 0; // recommended ANN with optional training method
113	   Use["CFMlpANN"]        =0; // *** missing
114	   Use["TMlpANN"]         =0; 
115	   // ---
116	   Use["SVM"]             =0;
117	   // ---
118	   Use["BDT"]             =0;
119	   Use["BDTD"]            =0;
120	   Use["BDTG"]            =0;
121	   Use["BDTB"]            =0;
122	   // ---
123	   Use["RuleFit"]         =0;
124	   // ---
125	   Use["Plugin"]          =0;
126	   // ---------------------------------------------------------------
127	
128	   std::cout << std::endl;
129	   std::cout << "==> Start TMVAClassification" << std::endl;
130	
131	   if (myMethodList != "") {
132	      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;
133	
134	      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
135	      for (UInt_t i=0; i<mlist.size(); i++) {
136	         std::string regMethod(mlist[i]);
137	
138	         if (Use.find(regMethod) == Use.end()) {
139	            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
140	            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
141	            std::cout << std::endl;
142	            return;
143	         }
144	         Use[regMethod] = 1;
145	      }
146	   }
147	
148	   // Create a new root output file.
149	   TString outfileName( "TMVA_output_428p7_BTV003_newPU_newLumi_AC.root" );
150	   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
151	
152	   // Create the factory object. Later you can choose the methods
153	   // whose performance you'd like to investigate. The factory will
154	   // then run the performance analysis for you.
155	   //
156	   // The first argument is the base of the name of all the
157	   // weightfiles in the directory weight/ 
158	   //
159	   // The second argument is the output file for the training results
160	   // All TMVA output can be suppressed by removing the "!" (not) in 
161	   // front of the "Silent" argument in the option string
162	   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile, 
163	                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );
164	
165	   // If you wish to modify default settings 
166	   // (please check "src/Config.h" to see all available global options)
167	   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
168	   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
169	
170	   // Define the input variables that shall be used for the MVA training
171	   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
172	   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]
173	 //   factory->AddVariable( "myvar1 := var1+var2", 'F' );
174	//    factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );
175	//    factory->AddVariable( "var3",                "Variable 3", "units", 'F' );
176	//    factory->AddVariable( "var4",                "Variable 4", "units", 'F' );
177	
178	   factory->AddVariable( "Aplanarity := aplanarity", 'F' );
179	   factory->AddVariable( "chargeEta := chargeEta",  'F' );
180	   factory->AddVariable( "MET := Met",  'F' );
181	   factory->AddVariable( "MtauJet := MtauJet",  'F' );
182	   factory->AddVariable( "HT := Ht",  'F' );
183	   factory->AddVariable( "Chi2 := kinFitChi2",  'F' );
184	   factory->AddVariable( "DeltaPhiTauMET := DeltaPhiTauMet",  'F' );
185	   //factory->AddVariable( "Mt := Mt",  'F' );
186	
187	
188	
189	   // You can add so-called "Spectator variables", which are not used in the MVA training, 
190	   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the 
191	   // input variables, the response values of all trained MVAs, and the spectator variables
192	   //  factory->AddSpectator( "spec1:=var1*2",  "Spectator 1", "units", 'F' );
193	   // factory->AddSpectator( "spec2:=var1*3",  "Spectator 2", "units", 'F' );
194	
195	   // read training and test data
196	   if (ReadDataFromAsciiIFormat) {
197	      // load the signal and background event samples from ascii files
198	      // format in file must be:
199	      // var1/F:var2/F:var3/F:var4/F
200	      // 0.04551   0.59923   0.32400   -0.19170
201	      // ...
202	
203	      TString datFileS = "tmva_example_sig.dat";
204	      TString datFileB = "tmva_example_bkg.dat";
205	
206	      factory->SetInputTrees( datFileS, datFileB );
207	   }
208	   else {
209	
210	  
211	    TFile* f0 = new TFile("/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/TMVA_sig_newLumi.root");
212	    TFile* f1 = new TFile("/opt/sbg/data/data1/cms/lebihan/clean_january_2012_2/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/TopTauJets/TMVA_bkg_newLumi.root");
213	  
214	    TTree *signal     = (TTree*)f0->Get("Ttree");
215	    TTree *background = (TTree*)f1->Get("Ttree");
216	
217	    Double_t backgroundWeight = 1.0;
218	    Double_t signalWeight     = 1.0;
219	    // ====== register trees ====================================================
220	    //
221	    // the following method is the prefered one:
222	    // you can add an arbitrary number of signal or background trees
223	
224	    factory->AddSignalTree    ( signal,     signalWeight     );
225	    factory->AddBackgroundTree( background, backgroundWeight );
226	
227	     //   factory->AddSignalTree    ( signal );
228	     //factory->AddBackgroundTree( background );
229	
230	
231	      // To give different trees for training and testing, do as follows:
232	      //    factory->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
233	      //    factory->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );
234	
235	      // Use the following code instead of the above two or four lines to add signal and background 
236	      // training and test events "by hand"
237	      // NOTE that in this case one should not give expressions (such as "var1+var2") in the input 
238	      //      variable definition, but simply compute the expression before adding the event
239	      // 
240	      //    // --- begin ----------------------------------------------------------
241	      //    std::vector<Double_t> vars( 4 ); // vector has size of number of input variables
242	      //    Float_t  treevars[4];
243	      //    for (Int_t ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
244	      //    for (Int_t i=0; i<signal->GetEntries(); i++) {
245	      //       signal->GetEntry(i);
246	      //       for (Int_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
247	      //       // add training and test events; here: first half is training, second is testing
248	      //       // note that the weight can also be event-wise   
249	      //       if (i < signal->GetEntries()/2) factory->AddSignalTrainingEvent( vars, signalWeight ); 
250	      //       else                            factory->AddSignalTestEvent    ( vars, signalWeight ); 
251	      //    }
252	      //
253	      //    for (Int_t ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
254	      //    for (Int_t i=0; i<background->GetEntries(); i++) {
255	      //       background->GetEntry(i); 
256	      //       for (Int_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
257	      //       // add training and test events; here: first half is training, second is testing
258	      //       // note that the weight can also be event-wise   
259	      //       if (i < background->GetEntries()/2) factory->AddBackgroundTrainingEvent( vars, backgroundWeight ); 
260	      //       else                                factory->AddBackgroundTestEvent    ( vars, backgroundWeight ); 
261	      //    }
262	      //    // --- end ------------------------------------------------------------
263	      //
264	      // ====== end of register trees ==============================================
265	   }
266	   
267	   // This would set individual event weights (the variables defined in the 
268	   // expression need to exist in the original TTree)
269	   //    for signal    : factory->SetSignalWeightExpression("weight1*weight2");
270	   //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
271	   factory->SetBackgroundWeightExpression("weight_BTAG");
272	   factory->SetSignalWeightExpression("weight*weight_BTAG");
273	   // Apply additional cuts on the signal and background samples (can be different)
274	  
275	   // TCut mycuts = "MHt >=0  && MMTauJet >=0 && MM3 >= 0"; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
276	   // TCut mycutb = "MHt >=0  && MMTauJet >=0 && MM3 >= 0"; // for example: TCut mycutb = "abs(var1)<0.5";
277	   TCut mycuts = "Met>=20 "; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
278	   TCut mycutb = "Met>=20 "; // for example: TCut mycutb = "abs(var1)<0.5";
279	   
280	   // tell the factory to use all remaining events in the trees after training for testing:
281	   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
282	                                        "nTrain_Signal=3000:nTrain_Background=5000:SplitMode=Random:NormMode=NumEvents:!V" );
283	
284	   // If no numbers of events are given, half of the events in the tree are used for training, and 
285	   // the other half for testing:
286	   //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );  
287	   // To also specify the number of testing events, use:
288	   //    factory->PrepareTrainingAndTestTree( mycut, 
289	   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );  
290	
291	   // ---- Book MVA methods
292	   //
293	   // please lookup the various method configuration options in the corresponding cxx files, eg:
294	   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
295	   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
296	   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable
297	
298	   // Cut optimisation
299	   if (Use["Cuts"])
300	      factory->BookMethod( TMVA::Types::kCuts, "Cuts", 
301	                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
302	
303	   if (Use["CutsD"])
304	      factory->BookMethod( TMVA::Types::kCuts, "CutsD", 
305	                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );
306	
307	   if (Use["CutsPCA"])
308	      factory->BookMethod( TMVA::Types::kCuts, "CutsPCA", 
309	                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );
310	
311	   if (Use["CutsGA"])
312	      factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
313	                           "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );
314	   
315	   if (Use["CutsSA"])
316	      factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
317	                           "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
318	   
319	   // Likelihood
320	   if (Use["Likelihood"])
321	      factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", 
322	                           "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" ); 
323	
324	   // test the decorrelated likelihood
325	   if (Use["LikelihoodD"])
326	      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD", 
327	                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" ); 
328	
329	   if (Use["LikelihoodPCA"])
330	      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA", 
331	                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 
332	 
333	   // test the new kernel density estimator
334	   if (Use["LikelihoodKDE"])
335	      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE", 
336	                           "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 
337	
338	   // test the mixed splines and kernel density estimator (depending on which variable)
339	   if (Use["LikelihoodMIX"])
340	      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX", 
341	                           "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 
342	
343	   // test the multi-dimensional probability density estimator
344	   // here are the options strings for the MinMax and RMS methods, respectively:
345	   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );   
346	   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );   
347	   if (Use["PDERS"])
348	      factory->BookMethod( TMVA::Types::kPDERS, "PDERS", 
349	                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );
350	
351	   if (Use["PDERSkNN"])
352	      factory->BookMethod( TMVA::Types::kPDERS, "PDERSkNN", 
353	                           "!H:!V:VolumeRangeMode=kNN:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );
354	
355	   if (Use["PDERSD"])
356	      factory->BookMethod( TMVA::Types::kPDERS, "PDERSD", 
357	                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );
358	
359	   if (Use["PDERSPCA"])
360	      factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA", 
361	                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );
362	
363	   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
364	   if (Use["PDEFoam"])
365	      factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam", 
366	                           "H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0333:nActiveCells=500:nSampl=2000:nBin=5:CutNmin=T:Nmin=100:Kernel=None:Compress=T" );
367	
368	   // K-Nearest Neighbour classifier (KNN)
369	   if (Use["KNN"])
370	      factory->BookMethod( TMVA::Types::kKNN, "KNN", 
371	                           "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );
372	   // H-Matrix (chi2-squared) method
373	   if (Use["HMatrix"])
374	      factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V" ); 
375	
376	   // Fisher discriminant   
377	   if (Use["Fisher"])
378	      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=60:NsmoothMVAPdf=10" );
379	
380	   // Fisher with Gauss-transformed input variables
381	   if (Use["FisherG"])
382	      factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );
383	
384	   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
385	   if (Use["BoostedFisher"])
386	      factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2");
387	
388	   // Linear discriminant (same as Fisher)
389	   if (Use["LD"])
390	      factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None" );
391	
392	        // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
393	   if (Use["FDA_MC"])
394	      factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
395	                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );
396	   
397	   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
398	      factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
399	                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );
400	
401	   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
402	      factory->BookMethod( TMVA::Types::kFDA, "FDA_SA",
403	                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
404	
405	   if (Use["FDA_MT"])
406	      factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
407	                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );
408	
409	   if (Use["FDA_GAMT"])
410	      factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
411	                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );
412	
413	   if (Use["FDA_MCMT"])
414	      factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT",
415	                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );
416	
417	   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
418	   if (Use["MLP"])
419	      factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );
420	
421	   if (Use["MLPBFGS"])
422	      factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS" );
423	
424	
425	   // CF(Clermont-Ferrand)ANN
426	   if (Use["CFMlpANN"])
427	      factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  
428	  
429	   // Tmlp(Root)ANN
430	   if (Use["TMlpANN"])
431	      factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...
432	  
433	   // Support Vector Machine
434	   if (Use["SVM"])
435	      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
436	   
437	   // Boosted Decision Trees
438	   if (Use["BDTG"]) // Gradient Boost
439	      factory->BookMethod( TMVA::Types::kBDT, "BDTG", 
440	                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5" );
441	
442	   if (Use["BDT"])  // Adaptive Boost
443	      factory->BookMethod( TMVA::Types::kBDT, "BDT", 
444	                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
445	   
446	   if (Use["BDTB"]) // Bagging
447	      factory->BookMethod( TMVA::Types::kBDT, "BDTB", 
448	                           "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
449	
450	   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
451	      factory->BookMethod( TMVA::Types::kBDT, "BDTD", 
452	                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );
453	   
454	   // RuleFit -- TMVA implementation of Friedman's method
455	   if (Use["RuleFit"])
456	      factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
457	                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );
458	
459	   // --------------------------------------------------------------------------------------------------
460	
461	   // As an example how to use the ROOT plugin mechanism, book BDT via
462	   // plugin mechanism
463	   if (Use["Plugin"]) {
464	         //
465	         // first the plugin has to be defined, which can happen either through the following line in the local or global .rootrc:
466	         //
467	         // # plugin handler          plugin name(regexp) class to be instanciated library        constructor format
468	         // Plugin.TMVA@@MethodBase:  ^BDT                TMVA::MethodBDT          TMVA.1         "MethodBDT(TString,TString,DataSet&,TString)"
469	         // 
470	         // or by telling the global plugin manager directly
471	      gPluginMgr->AddHandler("TMVA@@MethodBase", "BDT", "TMVA::MethodBDT", "TMVA.1", "MethodBDT(TString,TString,DataSet&,TString)");
472	      factory->BookMethod( TMVA::Types::kPlugins, "BDT",
473	                           "!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=50" );
474	   }
475	
476	   // --------------------------------------------------------------------------------------------------
477	
478	   // ---- Now you can tell the factory to train, test, and evaluate the MVAs
479	
480	   // Train MVAs using the set of training events
481	   factory->TrainAllMethodsForClassification();
482	
483	   // ---- Evaluate all MVAs using the set of test events
484	   factory->TestAllMethods();
485	
486	   // ----- Evaluate and compare performance of all configured MVAs
487	   factory->EvaluateAllMethods();    
488	
489	   // --------------------------------------------------------------
490	   
491	   // Save the output
492	   outputFile->Close();
493	
494	   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
495	   std::cout << "==> TMVAClassification is done!" << std::endl;      
496	
497	   delete factory;
498	
499	   // Launch the GUI for the root macros
500	   if (!gROOT->IsBatch()) TMVAGui( outfileName );
501	}

