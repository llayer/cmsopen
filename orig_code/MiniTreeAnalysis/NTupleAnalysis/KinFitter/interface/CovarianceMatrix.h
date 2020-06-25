1	#ifndef CovarianceMatrix_h
2	#define CovarianceMatrix_h
3	
4	//#include "FWCore/ParameterSet/interface/ParameterSet.h"
5	
6	//#include "StringObjectFunction.h"
7	//#include "StringCutObjectSelector.h"
8	
9	/*
10	#include "DataFormats/PatCandidates/interface/PATObject.h"
11	#include "DataFormats/PatCandidates/interface/Electron.h"
12	#include "DataFormats/PatCandidates/interface/Muon.h"
13	#include "DataFormats/PatCandidates/interface/Jet.h"
14	#include "DataFormats/PatCandidates/interface/MET.h"*/
15	
16	#include "TLorentzVector.h"
17	#include "MET.h"
18	#include "Jet.h"
19	#include "Muon.h"
20	#include "Electron.h"
21	#include "Tau.h"
22	
23	#include "../interface/TopKinFitter.h"
24	
25	class CovarianceMatrix {
26	    
27	 private:
28	
29	  /// vector of strings for the binning of the resolutions
30	  std::vector<std::string> binsUdsc_, binsB_, binsLep_, binsMet_;
31	  /// vectors for the resolution functions
32	  std::vector<std::string> funcEtUdsc_ , funcEtB_ , funcEtLep_ , funcEtMet_;
33	  std::vector<std::string> funcEtaUdsc_, funcEtaB_, funcEtaLep_, funcEtaMet_;
34	  std::vector<std::string> funcPhiUdsc_, funcPhiB_, funcPhiLep_, funcPhiMet_;
35	
36	 public:
37	  
38	  CovarianceMatrix(){};
39	  /*
40	  //AC change ParameterSet dependence
41	  CovarianceMatrix(const std::vector<edm::ParameterSet> udscResolutions, const std::vector<edm::ParameterSet> bResolutions){
42	    for(std::vector<edm::ParameterSet>::const_iterator iSet = udscResolutions.begin(); iSet != udscResolutions.end(); ++iSet){
43	      if(iSet->exists("bin")) binsUdsc_.push_back(iSet->getParameter<std::string>("bin"));
44	      else if(udscResolutions.size()==1) binsUdsc_.push_back("");
45	      else throw cms::Exception("WrongConfig") << "Parameter 'bin' is needed if more than one PSet is specified!\n";
46	      
47	      funcEtUdsc_.push_back(iSet->getParameter<std::string>("et"));
48	      funcEtaUdsc_.push_back(iSet->getParameter<std::string>("eta"));
49	      funcPhiUdsc_.push_back(iSet->getParameter<std::string>("phi"));
50	    }
51	    for(std::vector<edm::ParameterSet>::const_iterator iSet = bResolutions.begin(); iSet != bResolutions.end(); ++iSet){
52	      if(iSet->exists("bin")) binsB_.push_back(iSet->getParameter<std::string>("bin"));
53	      else if(bResolutions.size()==1) binsB_.push_back("");
54	      else throw cms::Exception("WrongConfig") << "Parameter 'bin' is needed if more than one PSet is specified!\n";
55	    
56	      funcEtB_.push_back(iSet->getParameter<std::string>("et"));
57	      funcEtaB_.push_back(iSet->getParameter<std::string>("eta"));
58	      funcPhiB_.push_back(iSet->getParameter<std::string>("phi"));
59	    }
60	  };
61	  
62	  //AC change ParameterSet dependence
63	  CovarianceMatrix(const std::vector<edm::ParameterSet> udscResolutions, const std::vector<edm::ParameterSet> bResolutions, const std::vector<edm::ParameterSet> lepResolutions, const std::vector<edm::ParameterSet> metResolutions){
64	    for(std::vector<edm::ParameterSet>::const_iterator iSet = udscResolutions.begin(); iSet != udscResolutions.end(); ++iSet){
65	      if(iSet->exists("bin")) binsUdsc_.push_back(iSet->getParameter<std::string>("bin"));
66	      else if(udscResolutions.size()==1) binsUdsc_.push_back("");
67	      else throw cms::Exception("WrongConfig") << "Parameter 'bin' is needed if more than one PSet is specified!\n";
68	
69	      funcEtUdsc_.push_back(iSet->getParameter<std::string>("et"));
70	      funcEtaUdsc_.push_back(iSet->getParameter<std::string>("eta"));
71	      funcPhiUdsc_.push_back(iSet->getParameter<std::string>("phi"));
72	    }
73	    for(std::vector<edm::ParameterSet>::const_iterator iSet = bResolutions.begin(); iSet != bResolutions.end(); ++iSet){
74	      if(iSet->exists("bin")) binsB_.push_back(iSet->getParameter<std::string>("bin"));
75	      else if(bResolutions.size()==1) binsB_.push_back("");
76	      else throw cms::Exception("WrongConfig") << "Parameter 'bin' is needed if more than one PSet is specified!\n";
77	
78	      funcEtB_.push_back(iSet->getParameter<std::string>("et"));
79	      funcEtaB_.push_back(iSet->getParameter<std::string>("eta"));
80	      funcPhiB_.push_back(iSet->getParameter<std::string>("phi"));
81	    }
82	    for(std::vector<edm::ParameterSet>::const_iterator iSet = lepResolutions.begin(); iSet != lepResolutions.end(); ++iSet){
83	      if(iSet->exists("bin")) binsLep_.push_back(iSet->getParameter<std::string>("bin"));
84	      else if(lepResolutions.size()==1) binsLep_.push_back("");
85	      else throw cms::Exception("WrongConfig") << "Parameter 'bin' is needed if more than one PSet is specified!\n";
86	
87	      funcEtLep_.push_back(iSet->getParameter<std::string>("et"));
88	      funcEtaLep_.push_back(iSet->getParameter<std::string>("eta"));
89	      funcPhiLep_.push_back(iSet->getParameter<std::string>("phi"));
90	    }
91	    for(std::vector<edm::ParameterSet>::const_iterator iSet = metResolutions.begin(); iSet != metResolutions.end(); ++iSet){
92	      if(iSet->exists("bin")) binsMet_.push_back(iSet->getParameter<std::string>("bin"));
93	      else if(metResolutions.size()==1) binsMet_.push_back("");
94	      else throw cms::Exception("WrongConfig") << "Parameter 'bin' is needed if more than one PSet is specified!\n";
95	
96	      funcEtMet_.push_back(iSet->getParameter<std::string>("et"));
97	      funcEtaMet_.push_back(iSet->getParameter<std::string>("eta"));
98	      funcPhiMet_.push_back(iSet->getParameter<std::string>("phi"));
99	    }
100	   };*/
101	   ~CovarianceMatrix(){};
102	 
103	  //AC template <class ObjectType>
104	  //TMatrixD setupMatrix(const pat::PATObject<ObjectType>& object, TopKinFitter::Param param, std::string resolutionProvider);
105	    TMatrixD setupMatrix(const TLorentzVector& object, std::string objectType, TopKinFitter::Param param, std::string resolutionProvider);
106	
107	  //AC template <class ObjectType>
108	  //double getResolution(const pat::PATObject<ObjectType>& object, const std::string whichResolution, bool isBJet);
109	    double getResolution(const TLorentzVector& object, std::string objectType, const std::string whichResolution, bool isBJet);
110	};
111	
112	/*
113	//AC template <class ObjectType>
114	double CovarianceMatrix::getResolution(const TLorentzVector& object, std::string objectType, const std::string whichResolution = "", bool isBJet = false)
115	{
116	  std::vector<std::string> * bins_, * funcEt_, * funcEta_, * funcPhi_;
117	
118	  if( objectType == "jet" && !isBJet ) {
119	    bins_    = &binsUdsc_;
120	    funcEt_  = &funcEtUdsc_;
121	    funcEta_ = &funcEtaUdsc_;
122	    funcPhi_ = &funcPhiUdsc_;
123	  }
124	  else if(objectType == "jet"  && isBJet ) {
125	    bins_    = &binsB_;
126	    funcEt_  = &funcEtB_;
127	    funcEta_ = &funcEtaB_;
128	    funcPhi_ = &funcPhiB_;
129	  }
130	  else if( objectType == "muon" ||  objectType == "electron") {
131	    bins_    = &binsLep_;
132	    funcEt_  = &funcEtLep_;
133	    funcEta_ = &funcEtaLep_;
134	    funcPhi_ = &funcPhiLep_;
135	  }
136	  else if( objectType == "met" ) {
137	    bins_    = &binsMet_;
138	    funcEt_  = &funcEtMet_;
139	    funcEta_ = &funcEtaMet_;
140	    funcPhi_ = &funcPhiMet_;
141	  }
142	  else{
143	    std::cout  << "The object given is not supported!"<< std::endl;
144	  }
145	
146	  int selectedBin=-1;
147	  reco::LeafCandidate candidate;
148	  for(unsigned int i=0; i<bins_->size(); ++i){
149	    StringCutObjectSelector<reco::LeafCandidate> select_(bins_->at(i));
150	    candidate = reco::LeafCandidate( 0, reco::LeafCandidate::LorentzVector(object.Px(), object.Py(), object.Pz(), object.E()) );
151	    if(select_(candidate)){
152	      selectedBin = i;
153	      break;
154	    }
155	  }
156	  if(selectedBin>=0){
157	    if(whichResolution == "et")       return StringObjectFunction<reco::LeafCandidate>(funcEt_ ->at(selectedBin)).operator()(candidate);
158	    else if(whichResolution == "eta") return StringObjectFunction<reco::LeafCandidate>(funcEta_->at(selectedBin)).operator()(candidate);
159	    else if(whichResolution == "phi") return StringObjectFunction<reco::LeafCandidate>(funcPhi_->at(selectedBin)).operator()(candidate);
160	    else std::cout << "Only 'et', 'eta' and 'phi' resolutions supported!"<<std::endl;
161	  }
162	  return 0;
163	}
164	*/
165	
166	//AC template <class ObjectType>
167	TMatrixD CovarianceMatrix::setupMatrix(const TLorentzVector& object, std::string objectType, TopKinFitter::Param param, std::string resolutionProvider = "")
168	{
169	  TMatrixD CovM3 (3,3); CovM3.Zero();
170	  TMatrixD CovM4 (4,4); CovM4.Zero();
171	  TMatrixD* CovM = &CovM3;
172	  
173	  /*AC // This part is for pat objects with resolutions embedded
174	    if(object.hasKinResolution())
175	    {
176	      switch(param){
177	      case TopKinFitter::kEtEtaPhi :
178	        CovM3(0,0) = pow(object.resolEt(resolutionProvider) , 2);
179	        if( dynamic_cast<const reco::MET*>(&object) ) CovM3(1,1) = pow(9999., 2);
180	        else CovM3(1,1) = pow(object.resolEta(resolutionProvider), 2);
181	        CovM3(2,2) = pow(object.resolPhi(resolutionProvider), 2);
182	        CovM = &CovM3;
183	        break;
184	      case TopKinFitter::kEtThetaPhi :
185	        CovM3(0,0) = pow(object.resolEt(resolutionProvider)   , 2);
186	        CovM3(1,1) = pow(object.resolTheta(resolutionProvider), 2);
187	        CovM3(2,2) = pow(object.resolPhi(resolutionProvider)  , 2);
188	        CovM = &CovM3;
189	        break;
190	      case TopKinFitter::kEMom :
191	        CovM4(0,0) = pow(1, 2);
192	        CovM4(1,1) = pow(1, 2);
193	        CovM4(2,2) = pow(1, 2);
194	        CovM4(3,3) = pow(1, 2);
195	        CovM = &CovM4;
196	        break;
197	      }
198	    }
199	  // This part is for objects without resolutions embedded
200	  else
201	    { */
202	      double pt = object.Pt(), eta = object.Eta();
203	      // if object is a jet
204	      //if( dynamic_cast<const reco::Jet*>(&object) ) {
205	        if( objectType == "jet" ) {
206	        res::HelperJet jetRes;
207	        switch(param){
208	        case TopKinFitter::kEMom :
209	          if(resolutionProvider == "bjets") {
210	            CovM4(0,0) = pow(jetRes.a (pt, eta, res::HelperJet::kB  ), 2); 
211	            CovM4(1,1) = pow(jetRes.b (pt, eta, res::HelperJet::kB  ), 2); 
212	            CovM4(2,2) = pow(jetRes.c (pt, eta, res::HelperJet::kB  ), 2);
213	            CovM4(3,3) = pow(jetRes.d (pt, eta, res::HelperJet::kB  ), 2);
214	          }
215	          else {
216	            CovM4(0,0) = pow(jetRes.a (pt, eta, res::HelperJet::kUds), 2);
217	            CovM4(1,1) = pow(jetRes.b (pt, eta, res::HelperJet::kUds), 2);
218	            CovM4(2,2) = pow(jetRes.c (pt, eta, res::HelperJet::kUds), 2);
219	            CovM4(3,3) = pow(jetRes.d (pt, eta, res::HelperJet::kUds), 2);
220	          }
221	          CovM = &CovM4;
222	          break;
223	        case TopKinFitter::kEtEtaPhi : 
224	          if(resolutionProvider == "bjets") {
225	            if(!binsB_.size()){
226	              CovM3(0,0) = pow(jetRes.et (pt, eta, res::HelperJet::kB  ), 2); 
227	              CovM3(1,1) = pow(jetRes.eta(pt, eta, res::HelperJet::kB  ), 2); 
228	              CovM3(2,2) = pow(jetRes.phi(pt, eta, res::HelperJet::kB  ), 2);
229	            }
230	            else{
231	              std::cout <<"not supported..." << std::endl;
232	              /*
233	              CovM3(0,0) = pow(getResolution(object, "jet", "et" , true), 2); 
234	              CovM3(1,1) = pow(getResolution(object, "jet", "eta", true), 2); 
235	              CovM3(2,2) = pow(getResolution(object, "jet", "phi", true), 2);*/
236	            }
237	          }
238	          else {
239	            if(!binsUdsc_.size()){
240	              CovM3(0,0) = pow(jetRes.et (pt, eta, res::HelperJet::kUds), 2);
241	              CovM3(1,1) = pow(jetRes.eta(pt, eta, res::HelperJet::kUds), 2);
242	              CovM3(2,2) = pow(jetRes.phi(pt, eta, res::HelperJet::kUds), 2);
243	            }
244	            else{
245	              std::cout <<"not supported..." << std::endl;
246	              /*
247	              CovM3(0,0) = pow(getResolution(object, "jet", "et") , 2); 
248	              CovM3(1,1) = pow(getResolution(object, "jet", "eta"), 2); 
249	              CovM3(2,2) = pow(getResolution(object, "jet", "phi"), 2);*/
250	            }
251	          }       
252	          CovM = &CovM3;
253	          break;
254	        case TopKinFitter::kEtThetaPhi :
255	          if(resolutionProvider == "bjets") {
256	            CovM3(0,0) = pow(jetRes.et   (pt, eta, res::HelperJet::kB  ), 2); 
257	            CovM3(1,1) = pow(jetRes.theta(pt, eta, res::HelperJet::kB  ), 2); 
258	            CovM3(2,2) = pow(jetRes.phi  (pt, eta, res::HelperJet::kB  ), 2);
259	          }
260	          else {
261	            CovM3(0,0) = pow(jetRes.et   (pt, eta, res::HelperJet::kUds), 2);
262	            CovM3(1,1) = pow(jetRes.theta(pt, eta, res::HelperJet::kUds), 2);
263	            CovM3(2,2) = pow(jetRes.phi  (pt, eta, res::HelperJet::kUds), 2);
264	          }
265	          CovM = &CovM3;
266	          break;
267	        } 
268	      }
269	      // if object is an electron
270	      //else if( dynamic_cast<const reco::GsfElectron*>(&object) ) {
271	        else if ( objectType == "electron") {
272	        res::HelperElectron elecRes;
273	        switch(param){
274	        case TopKinFitter::kEMom :
275	          CovM3(0,0) = pow(elecRes.a (pt, eta), 2);
276	          CovM3(1,1) = pow(elecRes.b (pt, eta), 2); 
277	          CovM3(2,2) = pow(elecRes.c (pt, eta), 2);
278	          CovM = &CovM3;
279	          break;
280	        case TopKinFitter::kEtEtaPhi :
281	          if(!binsLep_.size()){
282	            CovM3(0,0) = pow(elecRes.et (pt, eta), 2);
283	            CovM3(1,1) = pow(elecRes.eta(pt, eta), 2); 
284	            CovM3(2,2) = pow(elecRes.phi(pt, eta), 2);
285	          }
286	          else{
287	            std::cout <<"not supported..." << std::endl;
288	            /*
289	            CovM3(0,0) = pow(getResolution(object, "electron", "et") , 2);
290	            CovM3(1,1) = pow(getResolution(object, "electron", "eta"), 2);
291	            CovM3(2,2) = pow(getResolution(object, "electron", "phi"), 2);*/
292	          }
293	          CovM = &CovM3;
294	          break;
295	        case TopKinFitter::kEtThetaPhi :
296	          CovM3(0,0) = pow(elecRes.et   (pt, eta), 2);
297	          CovM3(1,1) = pow(elecRes.theta(pt, eta), 2); 
298	          CovM3(2,2) = pow(elecRes.phi  (pt, eta), 2);
299	          CovM = &CovM3;
300	          break;
301	        }
302	      }
303	      // if object is a muon
304	      //else if( dynamic_cast<const reco::Muon*>(&object) ) {
305	        else if ( objectType == "muon" ) {
306	        res::HelperMuon muonRes;
307	        switch(param){
308	        case TopKinFitter::kEMom :
309	          CovM3(0,0) = pow(muonRes.a (pt, eta), 2);
310	          CovM3(1,1) = pow(muonRes.b (pt, eta), 2); 
311	          CovM3(2,2) = pow(muonRes.c (pt, eta), 2);
312	          CovM = &CovM3;
313	          break;
314	        case TopKinFitter::kEtEtaPhi :
315	          if(!binsLep_.size()){
316	            CovM3(0,0) = pow(muonRes.et (pt, eta), 2);
317	            CovM3(1,1) = pow(muonRes.eta(pt, eta), 2); 
318	            CovM3(2,2) = pow(muonRes.phi(pt, eta), 2);
319	          }
320	          else{ 
321	            std::cout <<"not supported..." << std::endl;
322	            /*    
323	            CovM3(0,0) = pow(getResolution(object, "muon", "et") , 2);
324	            CovM3(1,1) = pow(getResolution(object, "muon", "eta"), 2);
325	            CovM3(2,2) = pow(getResolution(object, "muon", "phi"), 2);*/
326	          }
327	          CovM = &CovM3;
328	          break;
329	        case TopKinFitter::kEtThetaPhi :
330	          CovM3(0,0) = pow(muonRes.et   (pt, eta), 2);
331	          CovM3(1,1) = pow(muonRes.theta(pt, eta), 2); 
332	          CovM3(2,2) = pow(muonRes.phi  (pt, eta), 2);
333	          CovM = &CovM3;
334	          break;
335	        }
336	      }
337	      /*
338	      // if object is a hadronic tau
339	      else if ( objectType == "tau" ) {
340	        res::HelperTau tauRes;
341	        switch(param){
342	        case TopKinFitter::kEMom :
343	          std::cout <<"not supported..." << std::endl;
344	          break;
345	        case TopKinFitter::kEtEtaPhi :
346	          if(!binsLep_.size()){
347	            CovM3(0,0) = pow(tauRes.et (pt, eta), 2);
348	            CovM3(1,1) = pow(tauRes.eta(pt, eta), 2); 
349	            CovM3(2,2) = pow(tauRes.phi(pt, eta), 2);
350	          }
351	          else{ 
352	            std::cout <<"not supported..." << std::endl;
353	          }
354	          CovM = &CovM3;
355	          break;
356	        case TopKinFitter::kEtThetaPhi :
357	          std::cout <<"not supported..." << std::endl;
358	          break;
359	        }
360	      }*/
361	      // if object is a hadronic tau
362	      else if ( objectType == "tau" ) {
363	        res::HelperTau jetRes;
364	        switch(param){
365	        case TopKinFitter::kEMom :
366	          std::cout <<"not supported..." << std::endl;
367	          break;
368	        case TopKinFitter::kEtEtaPhi :
369	          if(!binsLep_.size()){
370	            CovM3(0,0) = pow(jetRes.et (pt, eta), 2);
371	            CovM3(1,1) = pow(jetRes.eta(pt, eta), 2); 
372	            CovM3(2,2) = pow(jetRes.phi(pt, eta), 2);
373	          }
374	          else{ 
375	            std::cout <<"not supported..." << std::endl;
376	          }
377	          CovM = &CovM3;
378	          break;
379	        case TopKinFitter::kEtThetaPhi :
380	          std::cout <<"not supported..." << std::endl;
381	          break;
382	        }
383	       }
384	      // if object is met
385	      //else if( dynamic_cast<const reco::MET*>(&object) ) {
386	        else if( objectType == "met" ) {
387	        res::HelperMET metRes;
388	        switch(param){
389	        case TopKinFitter::kEMom :
390	          CovM3(0,0) = pow(metRes.a(pt), 2);
391	          CovM3(1,1) = pow(metRes.b(pt), 2);
392	          CovM3(2,2) = pow(metRes.c(pt), 2);
393	          CovM = &CovM3;
394	          break;
395	        case TopKinFitter::kEtEtaPhi :
396	          if(!binsMet_.size()){
397	            CovM3(0,0) = pow(metRes.et(pt) , 2);
398	            CovM3(1,1) = pow(        9999. , 2);
399	            CovM3(2,2) = pow(metRes.phi(pt), 2);
400	          }
401	          else{std::cout <<"not supported..." << std::endl;
402	            /*
403	            CovM3(0,0) = pow(getResolution(object, "met", "et") , 2);
404	            CovM3(1,1) = pow(getResolution(object, "met", "eta"), 2);
405	            CovM3(2,2) = pow(getResolution(object, "met", "phi"), 2);*/
406	          }
407	          CovM = &CovM3;
408	          break;
409	        case TopKinFitter::kEtThetaPhi :
410	          CovM3(0,0) = pow(metRes.et(pt) , 2);
411	          CovM3(1,1) = pow(        9999. , 2);
412	          CovM3(2,2) = pow(metRes.phi(pt), 2);
413	          CovM = &CovM3;
414	          break;
415	        }
416	      }
417	    //}
418	  return *CovM;
419	}
420	
421	#endif
