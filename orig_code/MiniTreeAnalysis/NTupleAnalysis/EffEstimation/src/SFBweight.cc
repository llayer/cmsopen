1	#include "../interface/SFBweight.h"
2	
3	SFBweight::SFBweight(){
4	  btag_algo_=-1;
5	  btag_discri_=-999.;
6	  n_bjets_= 0;
7	  method_origin1_="default";
8	  method_origin2_="default";
9	
10	  histo_sfvalb_=0;
11	  histo_sferrb_=0;
12	  histo_sfvall_=0;
13	  histo_sferrl_=0;
14	
15	  histo_effvalb_=0;
16	  histo_efferrb_=0;
17	  histo_effvall_=0;         
18	  histo_efferrl_=0;
19	
20	  histo_effmcb_=0;
21	  histo_effmcc_=0;
22	  histo_effmcl_=0;
23	  histo_errmcb_=0;
24	  histo_errmcc_=0;
25	  histo_errmcl_=0;
26	
27	  map_effmcb_.clear();
28	  map_effmcc_.clear();
29	  map_effmcl_.clear();
30	  map_errmcb_.clear();
31	  map_errmcc_.clear();
32	  map_errmcl_.clear();
33	
34	}
35	
36	
37	SFBweight::SFBweight(const SFBweight& w){
38	  cout<<"Call the copy constructor of SFBweight"<<endl;
39	  btag_algo_= w.btag_algo_;
40	  btag_discri_= w.btag_discri_;
41	  n_bjets_= w.n_bjets_;
42	  method_origin1_= w.method_origin1_;
43	  method_origin2_= w.method_origin2_;
44	
45	  if(histo_sfvalb_!=0) histo_sfvalb_ = (TH2D*)  w.histo_sfvalb_->Clone("");
46	  if(histo_sferrb_!=0) histo_sferrb_ = (TH2D*)  w.histo_sferrb_->Clone("");
47	  if(histo_sfvall_!=0) histo_sfvall_ = (TH2D*)  w.histo_sfvall_->Clone("");
48	  if(histo_sferrl_!=0) histo_sferrl_ = (TH2D*)  w.histo_sferrl_->Clone("");
49	
50	  if(histo_effvalb_!=0) histo_effvalb_ = (TH2D*)  w.histo_effvalb_->Clone("");
51	  if(histo_efferrb_!=0) histo_efferrb_ = (TH2D*)  w.histo_efferrb_->Clone("");
52	  if(histo_effvall_!=0) histo_effvall_ = (TH2D*)  w.histo_effvall_->Clone("");         
53	  if(histo_efferrl_!=0) histo_efferrl_ = (TH2D*)  w.histo_efferrl_->Clone("");
54	
55	  if(histo_effmcb_!=0) histo_effmcb_ = (TH2D*)  w.histo_effmcb_->Clone("");
56	  if(histo_effmcc_!=0) histo_effmcc_ = (TH2D*)  w.histo_effmcc_->Clone("");
57	  if(histo_effmcl_!=0) histo_effmcl_ = (TH2D*)  w.histo_effmcl_->Clone("");
58	  if(histo_errmcb_!=0) histo_errmcb_ = (TH2D*)  w.histo_errmcb_->Clone("");
59	  if(histo_errmcc_!=0) histo_errmcc_ = (TH2D*)  w.histo_errmcc_->Clone("");
60	  if(histo_errmcl_!=0) histo_errmcl_ = (TH2D*)  w.histo_errmcl_->Clone("");
61	 
62	  //clear maps
63	  map_effmcc_.clear(); 
64	  map_effmcb_.clear(); 
65	  map_effmcl_.clear(); 
66	  map_errmcc_.clear(); 
67	  map_errmcb_.clear(); 
68	  map_errmcl_.clear(); 
69	
70	
71	  //normaly a copy of the map is needed ....
72	  map<string,TH2D*>::iterator it;
73	  
74	  //for ( it=w.map_effmcc_.begin() ; it != w.map_effmcc_.end(); it++ ) cout<<"toto"<<endl;//map_effmcc_.insert(pair<string,TH2D*>(it->first,(TH2D*) it->second->Clone("")));
75	  /*
76	  for ( it=w.map_effmcc_.begin() ; it != w.map_effmcc_.end(); it++ ) map_effmcc_.insert(pair<string,TH2D*>(it->first,(TH2D*) it->second->Clone("")));
77	  for ( it=w.map_effmcb_.begin() ; it != w.map_effmcb_.end(); it++ ) map_effmcb_.insert(pair<string,TH2D*>(it->first,(TH2D*) it->second->Clone("")));
78	  for ( it=w.map_effmcl_.begin() ; it != w.map_effmcl_.end(); it++ ) map_effmcl_.insert(pair<string,TH2D*>(it->first,(TH2D*) it->second->Clone("")));
79	  for ( it=w.map_effmcc_.begin() ; it != w.map_effmcc_.end(); it++ ) map_effmcc_.insert(pair<string,TH2D*>(it->first,(TH2D*) it->second->Clone("")));
80	  for ( it=w.map_effmcb_.begin() ; it != w.map_effmcb_.end(); it++ ) map_effmcb_.insert(pair<string,TH2D*>(it->first,(TH2D*) it->second->Clone("")));
81	  for ( it=w.map_effmcl_.begin() ; it != w.map_effmcl_.end(); it++ ) map_effmcl_.insert(pair<string,TH2D*>(it->first,(TH2D*) it->second->Clone("")));
82	  */
83	        
84	}
85	
86	SFBweight::SFBweight(int btag_algo, float btag_discri, int n_bjets){
87	         SFBinit(btag_algo,btag_discri,n_bjets);
88	}
89	
90	
91	SFBweight::~SFBweight(){
92	}
93	
94	void SFBweight::SFBinit(int btag_algo, float btag_discri, int n_bjets){
95	
96	         std::cout << " initialisation of SFBinit " << std::endl;
97	         btag_algo_=btag_algo;
98	         btag_discri_=btag_discri;
99	         n_bjets_=n_bjets;
100	         std::string algoname;
101	        //is it usefull if methodb=2 ??
102	         std ::cout << " ALGO " <<  btag_algo << " & DISCRI CUT " << btag_discri << std::endl;
103	         if (btag_algo==0) {
104	             // TrackCounting
105	             algoname="TCHE";
106	             if (btag_discri>1.69 &&  btag_discri<1.71) {
107	                    method_origin1_="BTAG"+algoname+"L";
108	                    method_origin2_="MISTAG"+algoname+"L";
109	             }
110	             else if (btag_discri>3.29 && btag_discri<3.31) {
111	                    method_origin1_="BTAG"+algoname+"M";
112	                    method_origin2_="MISTAG"+algoname+"M";
113	             }
114	             else if (btag_discri>10.19 && btag_discri<10.21) {
115	                    //cout << " version hackee par Caro car TCHET pas dans DB " << endl;
116	                    method_origin1_="BTAG"+algoname+"M";
117	                    method_origin2_="MISTAG"+algoname+"M";
118	             }
119	             else {
120	                    method_origin1_="default";
121	                    method_origin2_="default";
122	             }
123	         }
124	         else if (btag_algo==1) {
125	              // SecondaryVertex
126	              algoname="SSVHE";
127	              if (btag_discri>1.73 && btag_discri<1.75) {
128	                    method_origin1_="BTAG"+algoname+"M";
129	                    method_origin2_="MISTAG"+algoname+"M";
130	              }
131	              else if (btag_discri>3.04 && btag_discri<3.06) {
132	                    //cout << " version hackee par Caro car SSVHET pas dans DB " << endl;
133	                    method_origin1_="BTAG"+algoname+"M";
134	                    method_origin2_="MISTAG"+algoname+"M";
135	              }
136	              else {
137	                    method_origin1_="default";
138	                    method_origin2_="default";
139	              }
140	         }
141	         else if (btag_algo==2) {
142	             // TrackCounting
143	             algoname="TCHP";
144	             if (btag_discri>1.92 &&  btag_discri<1.94) {
145	                    method_origin1_="BTAG"+algoname+"M";
146	                    method_origin2_="MISTAG"+algoname+"M";
147	             }
148	             else if (btag_discri>3.40 && btag_discri<3.42) {
149	                    method_origin1_="BTAG"+algoname+"T";
150	                    method_origin2_="MISTAG"+algoname+"T";
151	              }
152	             else if (btag_discri>1.18 && btag_discri<1.20) {
153	                    //cout << " version hackee par Caro car TCHPL pas dans DB " << endl;
154	                    method_origin1_="BTAG"+algoname+"M";
155	                    method_origin2_="MISTAG"+algoname+"M";
156	              }
157	              else {
158	                    method_origin1_="default";
159	                    method_origin2_="default";
160	              }
161	         }
162	         else if (btag_algo==3) {
163	             // SecondaryVertex
164	              algoname="SSVHP";
165	             if (btag_discri>1.99 && btag_discri<2.01) {
166	                    method_origin1_="BTAG"+algoname+"T";
167	                    method_origin2_="MISTAG"+algoname+"T";
168	              }
169	              else {
170	                    method_origin1_="default";
171	                    method_origin2_="default";
172	              }
173	         }
174	         else if (btag_algo==4) {
175	             // JP
176	              algoname="JP";
177	             if (btag_discri>0.274 && btag_discri<0.276) {
178	                    method_origin1_="BTAG"+algoname+"L";
179	                    method_origin2_="MISTAG"+algoname+"L";
180	             }
181	             else if (btag_discri>0.544 && btag_discri<0.546) {
182	                    method_origin1_="BTAG"+algoname+"M";
183	                    method_origin2_="MISTAG"+algoname+"M";
184	             }      
185	             else if (btag_discri>0.78 && btag_discri<0.8) {
186	                    method_origin1_="BTAG"+algoname+"T";
187	                    method_origin2_="MISTAG"+algoname+"T";
188	              }     
189	              else {
190	                    method_origin1_="default";
191	                    method_origin2_="default";
192	              }
193	         }   
194	         else if (btag_algo==5) { 
195	             // JBP 
196	              algoname="JBP";
197	             if (btag_discri>1.32 && btag_discri<1.34) {
198	                    method_origin1_="BTAG"+algoname+"L";
199	                    method_origin2_="MISTAG"+algoname+"L";
200	             }      
201	             else if (btag_discri>2.54 && btag_discri<2.56) {
202	                    method_origin1_="BTAG"+algoname+"M";
203	                    method_origin2_="MISTAG"+algoname+"M";
204	             }      
205	             else if (btag_discri>3.73 && btag_discri<3.75) {
206	                    method_origin1_="BTAG"+algoname+"T";
207	                    method_origin2_="MISTAG"+algoname+"T";
208	              }
209	              else {
210	                    method_origin1_="default";
211	                    method_origin2_="default";
212	              }     
213	         }    
214	         else if (btag_algo==6) {
215	             // CSV 
216	              algoname="CSV";
217	             if (btag_discri>0.243 && btag_discri<0.245) {
218	                    method_origin1_="BTAG"+algoname+"L";
219	                    method_origin2_="MISTAG"+algoname+"L";
220	             }
221	             else if (btag_discri>0.678 && btag_discri<0.68) {
222	                    method_origin1_="BTAG"+algoname+"M";
223	                    method_origin2_="MISTAG"+algoname+"M";
224	             }
225	             else if (btag_discri>0.897 && btag_discri<0.899) {
226	                    method_origin1_="BTAG"+algoname+"T";
227	                    method_origin2_="MISTAG"+algoname+"T";
228	              }
229	              else {
230	                    method_origin1_="default";
231	                    method_origin2_="default";
232	              }
233	         }
234	         else {
235	             std::cout << " WHICH B-TAG ALGO DO YOU WANT??? " << std::endl;
236	             method_origin1_="default";
237	             method_origin2_="default";
238	         }
239	         //std::cout << " --> ALGO : " << algoname << ", METHODS for SF&Eff : " << method_origin1_ << " and " << method_origin2_ << std::endl;
240	
241	
242	}
243	
244	void SFBweight::LoadInfo(){
245	
246	
247	
248	        //TFile* f = TFile::Open(" /opt/sbg/data/data1/cms/ccollard/CMSSW/fichier_root2011/efficacite_btag.root");
249	        TFile* f = TFile::Open("/opt/sbg/data/data1/cms/ccollard/CMSSW/CMSSW_4_2_5/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/efficacite_btag.root");
250	
251	        std::string histo_name1= "h_" + method_origin1_ + "_BTAG" + "B" +"EFFCORR";
252	        std::string histo_name2= "h_" + method_origin1_ + "_BTAG" + "B" +"ERRCORR";
253	//        std::string histo_name3= "h_" + method_origin1_ + "_BTAG" + "B" +"EFF";  // no Eff in DB for b-quark
254	//        std::string histo_name4= "h_" + method_origin1_ + "_BTAG" + "B" +"ERR";  // no Err(Eff) in DB for b-quark
255	        std::string histo_name3= "h_" + method_origin1_ + "_BTAG" + "B" +"EFFCORR";
256	        std::string histo_name4= "h_" + method_origin1_ + "_BTAG" + "B" +"ERRCORR";
257	        std::string histo_name5= "h_" + method_origin2_ + "_BTAG" + "L" +"EFFCORR";
258	        std::string histo_name6= "h_" + method_origin2_ + "_BTAG" + "L" +"ERRCORR";
259	        std::string histo_name7= "h_" + method_origin2_ + "_BTAG" + "L" +"EFF";
260	        std::string histo_name8= "h_" + method_origin2_ + "_BTAG" + "L" +"ERR";
261	 
262	        if (method_origin1_ !="default" && method_origin2_!="default") {
263	         histo_sfvalb_    = (TH2D*) gDirectory->Get( histo_name1.c_str() )->Clone("") ;
264	         histo_sferrb_    = (TH2D*) gDirectory->Get( histo_name2.c_str() )->Clone("") ;
265	         histo_effvalb_   = (TH2D*) gDirectory->Get( histo_name3.c_str() )->Clone("") ;
266	         histo_efferrb_   = (TH2D*) gDirectory->Get( histo_name4.c_str() )->Clone("") ;
267	         histo_sfvall_    = (TH2D*) gDirectory->Get( histo_name5.c_str() )->Clone("") ;
268	         histo_sferrl_    = (TH2D*) gDirectory->Get( histo_name6.c_str() )->Clone("") ;
269	         histo_effvall_   = (TH2D*) gDirectory->Get( histo_name7.c_str() )->Clone("") ;
270	         histo_efferrl_   = (TH2D*) gDirectory->Get( histo_name8.c_str() )->Clone("") ;
271	
272	
273	
274	         histo_sfvalb_->SetDirectory(0); // 
275	         histo_sferrb_->SetDirectory(0); // 
276	         histo_effvalb_->SetDirectory(0); // 
277	         histo_efferrb_->SetDirectory(0); // 
278	         histo_sfvall_->SetDirectory(0); // 
279	         histo_sferrl_->SetDirectory(0); // 
280	         histo_effvall_->SetDirectory(0); // 
281	         histo_efferrl_->SetDirectory(0); // 
282	
283	         std::cout << " loaded histo: " <<  histo_name1.c_str() << std::endl;
284	         std::cout << " loaded histo: " <<  histo_name2.c_str() << std::endl;
285	         std::cout << " loaded histo: " <<  histo_name3.c_str() << std::endl;
286	         std::cout << " loaded histo: " <<  histo_name4.c_str() << std::endl;
287	         std::cout << " loaded histo: " <<  histo_name5.c_str() << std::endl;
288	         std::cout << " loaded histo: " <<  histo_name6.c_str() << std::endl;
289	         std::cout << " loaded histo: " <<  histo_name7.c_str() << std::endl;
290	         std::cout << " loaded histo: " <<  histo_name8.c_str() << std::endl;
291	        }
292	        else {
293	         std::cout << " no histogram loaded : not the correct methods! " << std::endl;
294	        }
295	
296	        f->Close();
297	}
298	
299	
300	
301	
302	void SFBweight::LoadInfo2(){
303	
304	  bool debug = true;
305	
306	  if(debug) cout<<"SFBweight::LoadInfo2"<<endl;
307	  
308	  //////////////////////////////////////
309	  // Reinitialize the maps
310	  //////////////////////////////////////
311	  map_effmcb_.clear();
312	  map_effmcc_.clear();
313	  map_effmcl_.clear();
314	  map_errmcb_.clear();
315	  map_errmcc_.clear();
316	  map_errmcl_.clear();
317	
318	        string filename; 
319	        //filename = string("/opt/sbg/data/data1/cms/ccollard/CMSSW/fichier_root2011/eff_from_ttmadgraph_summer11_multiwp.root");
320	        filename = string("/opt/sbg/data/data1/cms/echabert/RootFiles/eff_from_ttmadgraph_AllTaggers.root");
321	        if(debug) cout<<"SFBweight::LoadInfo2:: Loading the file "<<filename<<endl;
322	        TFile* f2 = TFile::Open(filename.c_str());
323	
324	        TDirectoryFile* newdir = 0;
325	        for(int i=0;i<f2->GetListOfKeys()->GetSize();i++){
326	                if(f2->Get(f2->GetListOfKeys()->At(i)->GetName())->ClassName()==string("TDirectoryFile")){
327	                        newdir = (TDirectoryFile*) f2->Get(f2->GetListOfKeys()->At(i)->GetName());
328	                        string keyname = string(newdir->GetName());
329	                        if(debug) cout<<"Loading histograms from the directory "<<keyname<<endl;
330	                        if(newdir!=0){
331	                                map_effmcb_.insert(pair<string,TH2D*>(keyname,(TH2D*) newdir->Get( "h_eff_bq" )->Clone("") ));
332	                                map_effmcc_.insert(pair<string,TH2D*>(keyname,(TH2D*) newdir->Get( "h_eff_cq" )->Clone("") ));
333	                                map_effmcl_.insert(pair<string,TH2D*>(keyname,(TH2D*) newdir->Get( "h_eff_lq" )->Clone("") ));
334	                                map_errmcb_.insert(pair<string,TH2D*>(keyname,(TH2D*) newdir->Get( "h_err_bq" )->Clone("") ));
335	                                map_errmcc_.insert(pair<string,TH2D*>(keyname,(TH2D*) newdir->Get( "h_err_cq" )->Clone("") ));
336	                                map_errmcl_.insert(pair<string,TH2D*>(keyname,(TH2D*) newdir->Get( "h_err_lq" )->Clone("") ));
337	                        }
338	                }
339	        }
340	
341	        //SetDirectory(0);
342	        //This is needed before closing the file
343	
344	        map<string,TH2D*>::iterator it;
345	        for ( it=map_effmcb_.begin() ; it != map_effmcb_.end(); it++ ) it->second->SetDirectory(0);
346	        for ( it=map_effmcc_.begin() ; it != map_effmcc_.end(); it++ ) it->second->SetDirectory(0);
347	        for ( it=map_effmcl_.begin() ; it != map_effmcl_.end(); it++ ) it->second->SetDirectory(0);
348	        for ( it=map_errmcb_.begin() ; it != map_errmcb_.end(); it++ ) it->second->SetDirectory(0);
349	        for ( it=map_errmcc_.begin() ; it != map_errmcc_.end(); it++ ) it->second->SetDirectory(0);
350	        for ( it=map_errmcl_.begin() ; it != map_errmcl_.end(); it++ ) it->second->SetDirectory(0);
351	        
352	        f2->Close();
353	        delete f2;
354	}
355	
356	void SFBweight::InitAlgoAndWP(int algo, float wp){
357	
358	        bool debug = false;
359	
360	        if(debug) cout<<"SFBweight::InitAlgoAndWP"<<endl;
361	        
362	        stringstream ss (stringstream::in | stringstream::out);
363	        string str_algo;
364	        string str_discri;
365	        ss << btag_discri_;
366	        ss << " ";
367	        ss << btag_algo_;
368	        ss >> str_discri;
369	        ss >> str_algo;
370	        //solve the problem of 2 which should be 2.0
371	        if(str_discri.find(".")>str_discri.size()) str_discri+=".0";
372	
373	        if(debug) cout<<"Algo: "<<str_algo<<" DiscriCut: "<<str_discri<<endl;
374	        string keyname = string("algo_")+str_algo+string("_discri_")+str_discri;
375	        if(debug) cout<<"Try to access: "<<keyname;
376	        if (map_effmcb_.find(keyname) == map_effmcb_.end()) {
377	                if(debug) cout<<": not found "<<endl;
378	                keyname +="0";
379	                if(debug) cout<<"Try to access: "<<keyname;
380	                if (map_effmcb_.find(keyname) == map_effmcb_.end()) {
381	                        if(debug) cout<<": not found "<<endl;
382	                        keyname +="0";
383	                        if(debug) cout<<"Try to access: "<<keyname;
384	                        if (map_effmcb_.find(keyname) == map_effmcb_.end()){ 
385	                                if(debug) cout<<": not found"<<endl;
386	                        }
387	                        else if(debug) cout<<" found !"<<endl;
388	                }
389	                else if(debug) cout<<" found !"<<endl;
390	        } 
391	        else if(debug) cout<<" found !"<<endl;
392	        if (map_effmcb_.find(keyname) == map_effmcb_.end()) {
393	          cerr<<"SFBweight::LoadInfo2:: No btag efficiency computed for the given b-tagging algo & W.P. "<< keyname <<" --> use TCHEL info by default"<<endl;
394	          keyname = map_effmcb_.begin()->first;
395	        }
396	        if(debug) cout << "We will access to " << keyname << endl;
397	
398	        //////////////////////////////////////////////////////////////////////////////////
399	        //Change the pointers: now point to the indicated algo and working point 
400	        //////////////////////////////////////////////////////////////////////////////////
401	
402	         if(map_effmcb_.find(keyname)!=map_effmcb_.end()) histo_effmcb_ = map_effmcb_.find(keyname)->second;
403	         else cerr<<"SFweight::InitAlgoAndWP:: histo effmcb not loaded for "<<keyname<<endl;
404	         if(map_effmcc_.find(keyname)!=map_effmcb_.end()) histo_effmcc_ = map_effmcc_.find(keyname)->second;
405	         else cerr<<"SFweight::InitAlgoAndWP:: histo effmcc not loaded for "<<keyname<<endl;
406	         if(map_effmcl_.find(keyname)!=map_effmcb_.end()) histo_effmcl_ = map_effmcl_.find(keyname)->second;
407	         else cerr<<"SFweight::InitAlgoAndWP:: histo effmcl not loaded for "<<keyname<<endl;
408	
409	         if(map_errmcb_.find(keyname)!=map_effmcb_.end()) histo_errmcb_ = map_errmcb_.find(keyname)->second;
410	         else cerr<<"SFweight::InitAlgoAndWP:: histo errmcb not loaded for "<<keyname<<endl;
411	         if(map_errmcc_.find(keyname)!=map_effmcb_.end()) histo_errmcc_ = map_errmcc_.find(keyname)->second;
412	         else cerr<<"SFweight::InitAlgoAndWP:: histo errmcc not loaded for "<<keyname<<endl;
413	         if(map_errmcl_.find(keyname)!=map_effmcb_.end()) histo_errmcl_ = map_errmcl_.find(keyname)->second;
414	         else cerr<<"SFweight::InitAlgoAndWP:: histo errmcl not loaded for "<<keyname<<endl;
415	}
416	
417	float SFBweight::GetWeight_BTV003(int info,  int quarkorigin, float pt, float eta, int sys) const
418	{
419	// So far only TCHEM supported !!
420	// sys : 0,central value, +1 or -1, for +/-1 sigma, +/- 0.04 for SF_b +/-0.08 for SF_c
421	
422	float weight =-1;
423	float discr = 3.3; //TCHEM
424	
425	if (pt<20)  pt=20.;
426	if (pt>670) pt=670.;
427	
428	if (quarkorigin==5 || quarkorigin==4) 
429	  {         
430	   // returns SF_b  or SF_c
431	   if (info==0)
432	   {
433	    float unc = 0.;
434	    if (quarkorigin==5)      unc = 0.04;
435	    else if (quarkorigin==4) unc = 0.08;
436	    
437	    if (sys==0)       weight =  0.00152129076412*discr +  0.95678084337;
438	    else if (sys==-1) weight = (0.00152129076412*discr +  0.95678084337)*(1.-unc);
439	    else if (sys==1)  weight = (0.00152129076412*discr +  0.95678084337)*(1.+unc);
440	   
441	   }
442	  }
443	          
444	if (quarkorigin==0) 
445	  {
446	   //returns SF_l 
447	   if (info==0)
448	    {
449	     if( fabs(eta)<=0.8 )
450	     {                      
451	      if( sys==0  ) weight = (1.2875*((1+(-0.000356371*pt))+(1.08081e-07*(pt*pt))))+(-6.89998e-11*(pt*(pt*(pt/(1+(-0.0012139*pt))))));
452	      if( sys==-1 ) weight = (1.11418*((1+(-0.000442274*pt))+(1.53463e-06*(pt*pt))))+(-4.93683e-09*(pt*(pt*(pt/(1+(0.00152436*pt))))));
453	      if( sys==1  ) weight = (1.47515*((1+(-0.000484868*pt))+(2.36817e-07*(pt*pt))))+(-2.05073e-11*(pt*(pt*(pt/(1+(-0.00142819*pt))))));
454	      }
455	     if(fabs(eta)>0.8 && fabs(eta)<=1.6)
456	     {
457	      if(  sys==0  ) weight = (1.24986*((1+(-0.00039734*pt))+(5.37486e-07*(pt*pt))))+(-1.74023e-10*(pt*(pt*(pt/(1+(-0.00112954*pt))))));
458	      if(  sys==-1 ) weight = (1.08828*((1+(-0.000208737*pt))+(1.50487e-07*(pt*pt))))+(-2.54249e-11*(pt*(pt*(pt/(1+(-0.00141477*pt))))));
459	      if(  sys==1  ) weight = (1.41211*((1+(-0.000559603*pt))+(9.50754e-07*(pt*pt))))+(-5.81148e-10*(pt*(pt*(pt/(1+(-0.000787359*pt))))));
460	      }
461	     if(fabs(eta)>1.6 && fabs(eta)<=2.4)
462	     {
463	      if(  sys==0  ) weight = (1.10763*((1+(-0.000105805*pt))+(7.11718e-07*(pt*pt))))+(-5.3001e-10*(pt*(pt*(pt/(1+(-0.000821215*pt))))));
464	      if(  sys==-1 ) weight = (0.958079*((1+(0.000327804*pt))+(-4.09511e-07*(pt*pt))))+(-1.95933e-11*(pt*(pt*(pt/(1+(-0.00143323*pt))))));
465	      if(  sys==1  ) weight = (1.26236*((1+(-0.000524055*pt))+(2.08863e-06*(pt*pt))))+(-2.29473e-09*(pt*(pt*(pt/(1+(-0.000276268*pt))))));
466	      }
467	    }
468	   
469	   //returns eff_l  
470	   if (info==2 )
471	    {      
472	     if(fabs(eta)<=0.8)                  weight = ( 0.000919586+(0.00026266*pt))+(-1.75723e-07*(pt*pt));
473	     if(fabs(eta)>0.8 && fabs(eta)<=1.6) weight = (-0.00364137+(0.000350371*pt))+(-1.89967e-07*(pt*pt));
474	     if(fabs(eta)>1.6 && fabs(eta)<=2.4) weight = (-0.00483904+(0.000367751*pt))+(-1.36152e-07*(pt*pt));
475	     }
476	   }
477	
478	if (info !=0 && info!=2) std::cout <<"not supported SF or eff for BTV-003!!"<< std::endl;         
479	if (quarkorigin!=0 && quarkorigin!=4 && quarkorigin!=5) std::cout <<"not supported flavour for BTV-003!!"<< std::endl;    
480	if (sys!=-1 && sys!=1 && sys!=0) std::cout <<"sys opt in BTV_003 not supported" << std::endl;
481	
482	return weight;
483	
484	}
485	float SFBweight::GetWeight_JP_BTV003(int info,  int quarkorigin, float pt, float eta, int sys) const
486	{
487	// So far only JPM supported !!
488	// sys : 0,central value, +1 or -1, for +/-1 sigma, 
489	
490	  float weight =-1;
491	  //float discr = 0.545; //TCHEM
492	
493	  if (pt<20)  pt=20.;
494	  if (pt>670) pt=670.;
495	
496	  if (quarkorigin==5 || quarkorigin==4) 
497	  {         
498	   // returns SF_b  or SF_c
499	   if (info==0)
500	   {
501	    float abs_unc = 0.;
502	    
503	    if (pt<30)             {pt = 30.; abs_unc = 0.12;}
504	    if (pt>30 && pt <40)   abs_unc = 0.0352594;
505	    if (pt>40 && pt <50)   abs_unc = 0.0353008;
506	    if (pt>50 && pt <60)   abs_unc = 0.0299008;
507	    if (pt>60 && pt <70)   abs_unc = 0.0276606;
508	    if (pt>70 && pt <80)   abs_unc = 0.0292312;
509	    if (pt>80 && pt <100)  abs_unc = 0.0336607;
510	    if (pt>100 && pt <120) abs_unc = 0.0284701;
511	    if (pt>120 && pt <160) abs_unc = 0.029544;
512	    if (pt>160 && pt <210) abs_unc = 0.0358872;
513	    if (pt>210 && pt <260) abs_unc = 0.0367869;
514	    if (pt>260 && pt <320) abs_unc = 0.0375048;
515	    if (pt>320 && pt <400) abs_unc = 0.0597367;
516	    if (pt>400 && pt <500) abs_unc = 0.0653152;
517	    if (pt>500 && pt <670) abs_unc = 0.074242 ;
518	    
519	    if(quarkorigin==4) abs_unc *=2.;
520	       
521	    if (sys==0)       weight =  0.90806*((1.+(0.000236997*pt))/(1.+(5.49455e-05*pt)));
522	    else if (sys==-1) weight =  0.90806*((1.+(0.000236997*pt))/(1.+(5.49455e-05*pt)))-abs_unc;
523	    else if (sys==1)  weight =  0.90806*((1.+(0.000236997*pt))/(1.+(5.49455e-05*pt)))+abs_unc;
524	    //std::cout <<"BTV003 weight " << weight<< std::endl;
525	   }
526	  }
527	          
528	  if (quarkorigin==0) 
529	  {
530	   //returns SF_l 
531	   if (info==0)
532	    {
533	    if( fabs(eta)<=0.8)
534	    {
535	    if(sys==0 )  weight = ((0.970028+(0.00118179*pt))+(-4.23119e-06*(pt*pt)))+(3.61065e-09*(pt*(pt*pt)));
536	    if(sys==-1)  weight = ((0.840326+(0.000626372*pt))+(-2.08293e-06*(pt*pt)))+(1.57604e-09*(pt*(pt*pt)));
537	    if(sys==1 )  weight = ((1.09966+(0.00173739*pt))+(-6.37946e-06*(pt*pt)))+(5.64527e-09*(pt*(pt*pt)));
538	    }
539	     //if( Atagger == "JPM" && sEtamin == "0.0" && sEtamapt == "2.4")
540	     //{
541	     //if( meanminmapt == "mean" ) tmpSFl = new TF1("SFlight","((0.871294+(0.00215201*pt))+(-6.77675e-06*(pt*pt)))+(5.79197e-09*(pt*(pt*pt)))", 20.,670.);
542	     //if( meanminmapt == "min" )  tmpSFl = new TF1("SFlightMin","((0.7654+(0.00149792*pt))+(-4.47192e-06*(pt*pt)))+(3.67664e-09*(pt*(pt*pt)))", 20.,670.);
543	     //if( meanminmapt == "mapt" )  tmpSFl = new TF1("SFlightMapt","((0.977076+(0.00280638*pt))+(-9.08158e-06*(pt*pt)))+(7.9073e-09*(pt*(pt*pt)))", 20.,670.);
544	     //}
545	    if( fabs(eta)>0.8 && fabs(eta)<=1.6)
546	    {
547	    if(sys==0 )  weight = ((0.918387+(0.000898595*pt))+(-2.00643e-06*(pt*pt)))+(1.26486e-09*(pt*(pt*pt)));
548	    if(sys==-1)  weight = ((0.790843+(0.000548016*pt))+(-6.70941e-07*(pt*pt)))+(1.90355e-11*(pt*(pt*pt)));
549	    if(sys==1 )  weight = ((1.0459+(0.00124924*pt))+(-3.34192e-06*(pt*pt)))+(2.51068e-09*(pt*(pt*pt)));
550	    }
551	    if( fabs(eta)>1.6 && fabs(eta)<=2.4)
552	    {
553	    if(sys==0 )  weight = ((0.790103+(0.00117865*pt))+(-2.07334e-06*(pt*pt)))+(1.42608e-09*(pt*(pt*pt)));
554	    if(sys==-1)  weight = ((0.667144+(0.00105593*pt))+(-1.43608e-06*(pt*pt)))+(5.24039e-10*(pt*(pt*pt)));
555	    if(sys==1 )  weight = ((0.913027+(0.00130143*pt))+(-2.71061e-06*(pt*pt)))+(2.32812e-09*(pt*(pt*pt)));
556	    }
557	   }
558	   
559	   //returns eff_l  
560	   if (info==2 )
561	    {      
562	     if( fabs(eta)<=0.8 ) weight = (0.00727084+(4.48901e-05*pt))+(-4.42894e-09*(pt*pt));
563	     //if( Atagger == "JPM" && sEtamin == "0.0" && sEtamax == "2.4")
564	     //{
565	     //if(sys==0) tmpMistag = new TF1("Mistag","(0.00461602+(6.31828e-05*pt))+(-1.22468e-08*(pt*pt))", 20.,670.);//}
566	     if( fabs(eta)>0.8 && fabs(eta)<=1.6) weight = (0.00389156+(6.35508e-05*pt))+(1.54183e-08*(pt*pt));
567	     if( fabs(eta)>1.6 && fabs(eta)<=2.4) weight = (0.0032816+(4.18867e-05*pt))+(7.44912e-08*(pt*pt));
568	     }
569	   }
570	
571	if (info !=0 && info!=2) std::cout <<"not supported SF or eff for BTV-003!!"<< std::endl;         
572	if (quarkorigin!=0 && quarkorigin!=4 && quarkorigin!=5) std::cout <<"not supported flavour for BTV-003!!"<< std::endl;    
573	if (sys!=-1 && sys!=1 && sys!=0) std::cout <<"sys opt in BTV_003 not supported" << std::endl;
574	
575	return weight;
576	
577	}
578	
579	float SFBweight::GetWeight(int info,  int quarkorigin, float pt, float eta) const{
580	
581	
582	
583	         //std::cout << "debug " << info << " " << quarkorigin << "--> " ;
584	         //if(info<3) std::cout << "debug " << info << " " << quarkorigin << "--> "<<endl; 
585	
586	         std::string whichquark;
587	         if (quarkorigin==5) {
588	           whichquark="B";
589	         }
590	         else if (quarkorigin==0) {
591	           whichquark="L";
592	         }
593	         else if (quarkorigin==4) {
594	              whichquark="C";
595	         }
596	         else {
597	            whichquark="N";
598	            std::cout << " Choose another value for quarkorigin (5 =b or 0= light or 4=c) " << quarkorigin << std::endl;
599	            return -1.;
600	        }
601	
602	//        std::cout << " Extraction of weight for info = " << info << ", pt = " << pt << ", eta = " << eta 
603	//                  << " (" << whichquark << "-jet) " << std::endl;
604	
605	        if (info<4 && method_origin1_=="default" && method_origin2_=="default") {
606	            std::cout << " NOT THE CORRECT METHODS ==> return -1!" << std::endl;
607	            return -1.;
608	        }
609	
610	        if (info<4 && (
611	             (btag_algo_==0 && btag_discri_>10.19 && btag_discri_<10.21) 
612	             || (btag_algo_==1 && btag_discri_>3.04 && btag_discri_<3.06)
613	             || (btag_algo_==2 && btag_discri_>1.18 && btag_discri_<1.20)  )
614	           ) {
615	            //cout << " print de verif pour Eric de Caro : NORMALEMENT TU NE DEVRAIS JAMAIS LIRE CETTE LIGNE " << endl;
616	            // si tu la vois apparaitre dans ton log, c'est que les info SF de la database sont utilisees quelque part
617	            // alors que j'etais sure que ca n'etait pas le cas!!! probleme avec le hackage dans SFBinit...
618	        }
619	 
620	        TH2D* histo_local = 0;
621	
622	        if (quarkorigin==5 || quarkorigin==4) {
623	          if (info==0){ histo_local= histo_sfvalb_; }
624	          else if (info==1) { histo_local= histo_sferrb_; }
625	          else if (info==2) { histo_local= histo_effvalb_; }
626	          else if (info==3) { histo_local= histo_efferrb_; }
627	          else if (info==4) {
628	               if (quarkorigin==5) histo_local= histo_effmcb_; 
629	               else  histo_local= histo_effmcc_; 
630	          }
631	          else if (info==5) {
632	               if (quarkorigin==5) histo_local= histo_errmcb_; 
633	               else  histo_local= histo_errmcc_; 
634	          }
635	          else { std::cout << "This option for info doesn't exist ! Choose another info value : [0,4]"<< std::endl;
636	                return -1.;
637	          }
638	        }
639	        else {
640	          // case : quarkorigin==0
641	          if (info==0){ histo_local= histo_sfvall_; }   
642	          else if (info==1) { histo_local=  histo_sferrl_; }
643	          else if (info==2) { histo_local=  histo_effvall_; }
644	          else if (info==3) { histo_local=  histo_efferrl_; }
645	          else if (info==4) { histo_local= histo_effmcl_; }
646	          else if (info==5) { histo_local= histo_errmcl_; }
647	          else { std::cout << "This option for info doesn't exist ! Choose another info value : [0,4]"<< std::endl;
648	                return -1.;
649	          }
650	        }
651	
652	        float aa = -1.;
653	
654	        if (quarkorigin==5 && pt>=240) pt=239;  // because limit in DB
655	        if (quarkorigin==0 && pt>=520) pt=519;  // because limit in DB
656	        aa= histo_local->GetBinContent( histo_local->FindBin(pt,eta) );
657	
658	        histo_local= new TH2D("aa","",10,0,1,10,0,1);
659	        histo_local->Delete();
660	
661	        return aa;
662	}
663	vector<float> SFBweight::GetWeigth4BSel(int method_b,  int syst_b, const std::vector<NTJet> &  selJets) const{
664	      return GetWeigth4BSel(method_b,  syst_b, selJets, -1., -1.);
665	}
666	
667	vector<float> SFBweight::GetWeigth4BSel(int method_b,  int syst_b, const std::vector<NTJet> &  selJets_, float sf_val_for_b, float sf_val_for_l) const{
668	       vector<TopTree::NTJet> selJets = selJets_;
669	       //using a copy is needed otherwise it's a const NTJet and there are problem to access GetDiscri method 
670	       //useless: has to be changed in the next version
671	
672	       std::vector<float> proba_jetb;
673	       vector<float> weight;
674	       weight.push_back(1); // weightb of the event
675	       weight.push_back(0); // probability to get 0 jet
676	       weight.push_back(0); // probability to get 1 jet
677	       weight.push_back(0); // probability to get 2 jet
678	       weight.push_back(0); // probability to get at least 3 jet
679	       //std::cout <<"--------------------------"<< std::endl;
680	       for(unsigned int j=0;j<selJets.size();j++){
681	             // only quark from a defined quarkorigin_
682	             int sectectedflavour=0;
683	             int quarkorigin=-1;
684	             // LIGHT QUARKS 
685	             if (abs(selJets[j].partonFlavour)==1) sectectedflavour=1; //d quark
686	             if (abs(selJets[j].partonFlavour)==2) sectectedflavour=1; //u quark
687	             if (abs(selJets[j].partonFlavour)==3) sectectedflavour=1; //s quark
688	             if (abs(selJets[j].partonFlavour)==21) sectectedflavour=1; //gluon 
689	             if (abs(selJets[j].partonFlavour)==21) sectectedflavour=1; //gluon    
690	             if (selJets[j].partonFlavour==-999)  sectectedflavour=1; //data, assume light jet (special case for tau+jets analysis) 
691	          
692	             if (sectectedflavour==1) quarkorigin=0;
693	             // B QUARKS
694	             if (abs(selJets[j].partonFlavour)==5) {
695	                  sectectedflavour=1;
696	                  quarkorigin=5;
697	             }
698	             // C QUARKS 
699	             else if (abs(selJets[j].partonFlavour)==4) {
700	                  sectectedflavour=1;
701	                  quarkorigin=4;
702	
703	             }
704	             // NOT RECOGNIZED --> LIGHT QUARKS, for the moment
705	             else if (selJets[j].partonFlavour ==0) {
706	                  sectectedflavour=1;
707	                  quarkorigin=0;
708	             }
709	
710	             if (sectectedflavour==0) {
711	                //DEBUG
712	                std::cout << " partonFlavour " << selJets[j].partonFlavour << std::endl;
713	             }
714	             float discri_val=0.;
715	             switch(btag_algo_){
716	                        case 0 :
717	                                discri_val =  selJets[j].GetDiscri(string("trackCountingHighEffBJetTags"));
718	                                break;
719	                        case 1 :
720	                                discri_val =  selJets[j].GetDiscri(string("simpleSecondaryVertexHighEffBJetTags"));
721	                                break;
722	                        case 2 :
723	                                discri_val =  selJets[j].GetDiscri(string("trackCountingHighPurBJetTags"));
724	                                break;
725	                        case 3 :
726	                                discri_val =  selJets[j].GetDiscri(string("simpleSecondaryVertexHighPurBJetTags"));
727	                                break;
728	                        case 4 :
729	                                discri_val =  selJets[j].GetDiscri(string("jetProbabilityBJetTags"));
730	                                break;
731	                        case 5 :
732	                                discri_val =  selJets[j].GetDiscri(string("jetBProbabilityBJetTags"));
733	                                break;
734	                        case 6 :
735	                                discri_val =  selJets[j].GetDiscri(string("combinedSecondaryVertexBJetTags"));
736	                                break;
737	                        default:
738	                                 cerr << "btagAlgo doesn't exist ** ! ["<<btag_algo_<<"]"<<endl;
739	                                 break;
740	
741	              }
742	
743	
744	              float pt_val_jet = selJets[j].p4.Pt();
745	              if (pt_val_jet>1000.) pt_val_jet=997.;
746	              float eta_val_jet = selJets[j].p4.Eta();
747	              if (eta_val_jet<0) eta_val_jet*=-1.;
748	              if (eta_val_jet>=2.4) eta_val_jet=2.399;
749	
750	              float pt_val_jet_orig = selJets[j].p4.Pt();
751	              float eta_val_jet_orig = selJets[j].p4.Eta();
752	
753	              int info_b=method_b;
754	              if (method_b>=2) info_b=0;
755	              
756	              // Compute the weight, depending of method_b 
757	              // method_b = 0 : SF
758	              // method_b = 1 : Eff_Data
759	              // method_b = 2 : SF*Eff_MC
760	              // method_b = 3 : SF*Eff_MC with error depending on SF and Eff
761	
762	              float weight_jet=0;
763	              //impose value for SF from outside
764	              if      (info_b==0 && quarkorigin==5 && sf_val_for_b>-1.) weight_jet=sf_val_for_b;  //b
765	              else if (info_b==0 && quarkorigin==4 && sf_val_for_b>-1.) weight_jet=sf_val_for_b;  //c
766	              else if (info_b==0 && quarkorigin==0 && sf_val_for_l>-1.) weight_jet=sf_val_for_l;  //udsg
767	              // take value from root files
768	              
769	              // this is the line used in tau+jets analysis !!
770	              // returns eff_l if method_b==1 in config file and quarkorigin=0 (multijet sample)
771	              // returns sf_b  if method_b==2 in config file and quarkorigin=4,5, sf_l if quarkorigin=0 (MC sample)
772	              //else weight_jet=GetWeight(info_b*2,quarkorigin,pt_val_jet,eta_val_jet);
773	              
774	              //else weight_jet=GetWeight_JP_BTV003(info_b*2,quarkorigin,pt_val_jet,eta_val_jet,0);
775	              else weight_jet=GetWeight_JP_BTV003(info_b*2,quarkorigin,pt_val_jet,eta_val_jet,0);
776	              
777	              float ptvaleff=pt_val_jet;
778	              if (pt_val_jet>200.) ptvaleff=199.;
779	              float eff_mc=GetWeight(4,quarkorigin,ptvaleff,eta_val_jet);
780	              float sf1=weight_jet;
781	              float ersf1=0;
782	              float eref1=0;
783	              float errw=0;
784	              if (method_b==3 && syst_b>0) {
785	                          ersf1=GetWeight(1,quarkorigin,pt_val_jet,eta_val_jet);
786	                          eref1=GetWeight(5,quarkorigin,ptvaleff,eta_val_jet);
787	                          errw=sqrt(ersf1*ersf1*eff_mc*eff_mc + sf1*sf1*eref1*eref1)/eff_mc;  // from SF and Eff
788	              }
789	              if (method_b==4 && syst_b>0) {
790	                          eref1=GetWeight(5,quarkorigin,ptvaleff,eta_val_jet);  // from Eff only
791	              }
792	
793	
794	              if (syst_b==1) { //+ all
795	                    if (method_b==3) {
796	                          weight_jet+=errw;
797	                    }
798	                    else if (method_b==4) {
799	                          eff_mc+=eref1;
800	                    }
801	                    else {
802	                       weight_jet+=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
803	                    }
804	              }
805	              else if (syst_b==2) { //- all
806	                    if (method_b==3) {
807	                          weight_jet-=errw;
808	                    }
809	                    else if (method_b==4) {
810	                          eff_mc-=eref1;
811	                    }
812	                    else {
813	                       weight_jet-=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
814	                    }
815	              }
816	              else if (syst_b==3) { //+ only b!
817	                  if (quarkorigin==4|| quarkorigin==5) {
818	                    if (method_b==3) {
819	                          weight_jet+=errw;
820	                    }
821	                    else if (method_b==4) {
822	                          eff_mc+=eref1;
823	                    }
824	                    else {
825	                         weight_jet+=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
826	                    }
827	                  }
828	              }
829	              else if (syst_b==4) { //- only b!
830	                  if (quarkorigin==4|| quarkorigin==5) {
831	                    if (method_b==3) {
832	                          weight_jet-=errw;
833	                    }
834	                    else if (method_b==4) {
835	                          eff_mc-=eref1;
836	                    }
837	                    else {
838	                      weight_jet-=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
839	                    }
840	                  }
841	              }
842	              else if (syst_b==5) { //+ only light!
843	                  if (quarkorigin==0) {
844	                    if (method_b==3) {
845	                          weight_jet+=errw;
846	                    }
847	                    else if (method_b==4) {
848	                          eff_mc+=eref1;
849	                    }
850	                    else {
851	                      weight_jet+=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
852	                    }
853	                  }
854	              }
855	              else if (syst_b==6) { //- only light!
856	                  if (quarkorigin==0) {
857	                    if (method_b==3) {
858	                          weight_jet-=errw;
859	                    }
860	                    else if (method_b==4) {
861	                          eff_mc-=eref1;
862	                    }
863	                    else {
864	                       weight_jet-=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
865	                    }
866	                  }
867	              }
868	              else if (syst_b==7) { //+ b and - light!
869	                  if (quarkorigin==4|| quarkorigin==5) {
870	                    if (method_b==3) {
871	                          weight_jet+=errw;
872	                    }
873	                    else if (method_b==4) {
874	                          eff_mc+=eref1;
875	                    }
876	                    else {
877	                          weight_jet+=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
878	                    }
879	                  }
880	                  else if (quarkorigin==0) {
881	                    if (method_b==3) {
882	                          weight_jet-=errw;
883	                    }
884	                    else if (method_b==4) {
885	                          eff_mc-=eref1;
886	                    }
887	                    else {
888	                         weight_jet-=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
889	                    }
890	                  }
891	              }
892	              else if (syst_b==8) { //- b and + light!
893	                  if (quarkorigin==4|| quarkorigin==5) {
894	                    if (method_b==3) {
895	                          weight_jet-=errw;
896	                    }
897	                    else if (method_b==4) {
898	                          eff_mc-=eref1;
899	                    }
900	                    else {
901	                         weight_jet-=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
902	                    }
903	                  }
904	                  else if (quarkorigin==0) {
905	                    if (method_b==3) {
906	                          weight_jet+=errw;
907	                    }    
908	                    else if (method_b==4) {
909	                          eff_mc+=eref1;
910	                    }
911	                    else {
912	                          weight_jet+=GetWeight(info_b*2+1,quarkorigin,pt_val_jet,eta_val_jet);
913	                    }
914	                  }
915	              }
916	
917	
918	
919	              if (method_b==0)  {
920	                // APPLY THE SCALE FACTOR
921	                if (discri_val>=btag_discri_) {
922	                   weight[0]*=weight_jet;
923	                   
924	                }
925	                else {
926	                   //SF (notag) = (1-Eff_data)/(1-Eff_MC) = (1- SF * Eff_MC)/(1-Eff_MC)
927	                   if (eff_mc<1) weight[0]*=(1 - weight_jet*eff_mc)/(1-eff_mc);
928	                   else weight[0]*=1.;
929	                }
930	              }
931	              else {//tau+jets analysis continues here !!
932	                 // APPLY THE EFFICIENCY or SF*EFF
933	                 // ATTENTION : in that case, the Selection should stop at Step 6
934	                 if (method_b>=2) {
935	                    weight_jet*=eff_mc;
936	                   if (weight_jet>1.) weight_jet=1.;
937	                   if (weight_jet<0.) weight_jet=0.;
938	                 }
939	                 proba_jetb.push_back(weight_jet);
940	                 if (weight_jet<0) std::cout << " weight_jet " << weight_jet << " quark "<< quarkorigin 
941	                                   <<" eta " << eta_val_jet_orig << " pt " << pt_val_jet_orig
942	                                   << " eff_mc " << eff_mc << std::endl;
943	                 if (weight_jet>1) std::cout << " weight_jet " << weight_jet << " eta " << eta_val_jet << " pt " << pt_val_jet << std::endl;
944	
945	              }
946	
947	
948	
949	       }
950	       //         btag_nbjet;
951	
952	       if (method_b>=1) {
953	
954	           // combinatory
955	           float proba_0jet = 1.;
956	           float proba_1jet = 0.;
957	           float proba_2jet = 0.;
958	           float proba_atleast3jet = 0.;
959	           for (unsigned int ijet=0; ijet<proba_jetb.size(); ijet++) {
960	               proba_0jet *= (1-proba_jetb[ijet]);
961	               float pp1 = 1;
962	               for (unsigned int kjet=0; kjet<proba_jetb.size(); kjet++) {
963	                  if (kjet!=ijet) {
964	                      pp1 *= (1-proba_jetb[kjet]);
965	                   }
966	
967	                   if (kjet>ijet) {
968	                       float pp2 = 1;
969	                       for (unsigned int ljet=0; ljet<proba_jetb.size(); ljet++) {
970	                          if (ljet!=ijet && ljet!=kjet) {
971	                             pp2 *= (1-proba_jetb[ljet]);
972	                           }
973	                        }
974	                        proba_2jet+=proba_jetb[ijet]*proba_jetb[kjet]*pp2;
975	                   }
976	               }
977	               proba_1jet+=proba_jetb[ijet]*pp1;
978	            }
979	            if(proba_jetb.size()>2) proba_atleast3jet= 1- proba_0jet - proba_1jet - proba_2jet;
980	            if (n_bjets_==0) {
981	              weight[0]*=1.;
982	            }
983	            else if (n_bjets_==1) {
984	              weight[0]*=1 - proba_0jet;
985	            }
986	            else if (n_bjets_==2) {
987	              weight[0]*=1 - proba_0jet - proba_1jet;
988	            }
989	            if (weight[0]<0)  {
990	                 // not good! :(
991	                 if (n_bjets_>0 && (proba_0jet<0 || proba_0jet>1 )) std::cout << " P0 " << proba_0jet << std::endl;
992	                 if (n_bjets_>1 && (proba_1jet<0 || proba_1jet>1 )) {
993	                       std::cout << " P1 " << proba_1jet << std::endl;
994	                 }
995	            }
996	            weight[1]=proba_0jet;
997	            weight[2]=proba_1jet;
998	            weight[3]=proba_2jet;
999	            weight[4]=proba_atleast3jet;
1000	
1001	            if (weight[3]>1.) {
1002	                 // not good! :(
1003	                 std::cout << " weight[3]>1!!!??? " <<std::endl;
1004	            }
1005	
1006	       }
1007	
1008	       //std::cout <<"weight[0] " << weight[0]<< std::endl;
1009	       return weight;
1010	
1011	
1012	}
1013	
1014	
1015	const TH2D* SFBweight::GetHistoSFB() const{
1016	    return histo_sfvalb_;
1017	}
1018	
1019	const TH2D* SFBweight::GetHistoEffMCb() const{
1020	        return histo_effmcb_;
1021	}
1022	
1023	const TH2D* SFBweight::GetHistoEffMCc() const{
1024	        return histo_effmcc_;
1025	}
1026	
1027	const TH2D* SFBweight::GetHistoEffMCl() const{
1028	        return histo_effmcl_;
1029	}
