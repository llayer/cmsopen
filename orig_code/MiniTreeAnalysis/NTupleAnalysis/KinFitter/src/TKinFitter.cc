1	// Classname: TKinFitter
2	// Author: Jan E. Sundermann, Verena Klose (TU Dresden)      
3	
4	
5	//________________________________________________________________
6	// 
7	// TKinFitter::
8	// --------------------
9	//
10	// Class to perform kinematic fit with non-linear constraints
11	//
12	
13	#include <iostream>
14	#include <iomanip>
15	#include "../interface/TKinFitter.h"
16	#include "../interface/TAbsFitParticle.h"
17	#include "../interface/TAbsFitConstraint.h"
18	
19	
20	TKinFitter::TKinFitter():
21	  TNamed("UnNamed", "UnTitled"),
22	  _A(1, 1),
23	  _AT(1, 1),
24	  _B(1, 1),
25	  _BT(1, 1),
26	  _V(1, 1),
27	  _Vinv(1, 1),
28	  _VB(1, 1),
29	  _VBinv(1, 1),
30	  _VA(1, 1),
31	  _VAinv(1, 1),
32	  _c(1, 1),
33	  _C11(1, 1),
34	  _C11T(1, 1),
35	  _C21(1, 1),
36	  _C21T(1, 1),
37	  _C22(1, 1),
38	  _C22T(1, 1),
39	  _C31(1, 1),
40	  _C31T(1, 1),
41	  _C32(1, 1),
42	  _C32T(1, 1),
43	  _C33(1, 1),
44	  _C33T(1, 1),
45	  _deltaA(1, 1),
46	  _deltaY(1, 1),
47	  _deltaAstar(1, 1),
48	  _deltaYstar(1, 1),
49	  _lambda(1, 1),
50	  _lambdaT(1, 1),
51	  _lambdaVFit(1, 1),
52	  _yaVFit(1, 1),
53	  _constraints(0),
54	  _measParticles(0),
55	  _unmeasParticles(0)
56	{
57	
58	  reset();
59	
60	}
61	
62	TKinFitter::TKinFitter(const TString &name, const TString &title):
63	  TNamed(name, title),
64	  _A(1, 1),
65	  _AT(1, 1),
66	  _B(1, 1),
67	  _BT(1, 1),
68	  _V(1, 1),
69	  _Vinv(1, 1),
70	  _VB(1, 1),
71	  _VBinv(1, 1),
72	  _VA(1, 1),
73	  _VAinv(1, 1),
74	  _c(1, 1),
75	  _C11(1, 1),
76	  _C11T(1, 1),
77	  _C21(1, 1),
78	  _C21T(1, 1),
79	  _C22(1, 1),
80	  _C22T(1, 1),
81	  _C31(1, 1),
82	  _C31T(1, 1),
83	  _C32(1, 1),
84	  _C32T(1, 1),
85	  _C33(1, 1),
86	  _C33T(1, 1),
87	  _deltaA(1, 1),
88	  _deltaY(1, 1),
89	  _deltaAstar(1, 1),
90	  _deltaYstar(1, 1),
91	  _lambda(1, 1),
92	  _lambdaT(1, 1),
93	  _lambdaVFit(1, 1),
94	  _yaVFit(1, 1),
95	  _constraints(0),
96	  _measParticles(0),
97	  _unmeasParticles(0)
98	{
99	
100	  reset();
101	
102	}
103	
104	void TKinFitter::reset() {
105	  // reset all internal parameters of the fitter
106	
107	  _status = -1;
108	  _nbIter = 0;
109	  _nParA = 0;
110	  _nParB = 0;
111	  _verbosity = 1;
112	  _A.ResizeTo(1, 1);
113	  _AT.ResizeTo(1, 1);
114	  _B.ResizeTo(1, 1);
115	  _BT.ResizeTo(1, 1);
116	  _V.ResizeTo(1, 1);
117	  _Vinv.ResizeTo(1, 1);
118	  _VB.ResizeTo(1, 1);
119	  _VBinv.ResizeTo(1, 1);
120	  _VA.ResizeTo(1, 1);
121	  _VAinv.ResizeTo(1, 1);
122	  _c.ResizeTo(1, 1);
123	  _C11.ResizeTo(1, 1);
124	  _C11T.ResizeTo(1, 1);
125	  _C21.ResizeTo(1, 1);
126	  _C21T.ResizeTo(1, 1);
127	  _C22.ResizeTo(1, 1);
128	  _C22T.ResizeTo(1, 1);
129	  _C31.ResizeTo(1, 1);
130	  _C31T.ResizeTo(1, 1);
131	  _C32.ResizeTo(1, 1);
132	  _C32T.ResizeTo(1, 1);
133	  _C33.ResizeTo(1, 1);
134	  _C33T.ResizeTo(1, 1);
135	  _deltaA.ResizeTo(1, 1);
136	  _deltaY.ResizeTo(1, 1);
137	  _deltaAstar.ResizeTo(1, 1);
138	  _deltaYstar.ResizeTo(1, 1);
139	  _lambda.ResizeTo(1, 1);
140	  _lambdaT.ResizeTo(1, 1);
141	  _lambdaVFit.ResizeTo(1, 1);
142	  _yaVFit.ResizeTo(1, 1);
143	
144	  _constraints.clear();
145	  _measParticles.clear();
146	  _unmeasParticles.clear();
147	
148	  // Set to default values
149	  _maxNbIter = 50;
150	  _maxDeltaS = 5e-3;
151	  _maxF =  1e-4;
152	
153	}
154	
155	void TKinFitter::resetStatus() {
156	  // reset status of the fit
157	
158	  _status = -1;
159	  _nbIter = 0;
160	
161	}
162	
163	void TKinFitter::resetParams() {
164	  // reset all particles to their initial parameters
165	
166	  for (unsigned int iP = 0; iP < _measParticles.size(); iP++) {
167	    TAbsFitParticle* particle = _measParticles[iP];
168	    particle->reset();
169	  }
170	  for (unsigned int iP = 0; iP < _unmeasParticles.size(); iP++) {
171	    TAbsFitParticle* particle = _unmeasParticles[iP];
172	    particle->reset();
173	  }
174	  for (unsigned int iC = 0; iC < _constraints.size(); iC++) {
175	    TAbsFitConstraint* constraint = _constraints[iC];
176	    constraint->reset();
177	  }
178	
179	}
180	
181	TKinFitter::~TKinFitter() {
182	
183	}
184	
185	void TKinFitter::countMeasParams() {
186	  // count number of measured parameters
187	
188	  _nParB = 0;
189	  for (unsigned int indexParticle = 0; indexParticle < _measParticles.size(); indexParticle++) {
190	    _nParB += _measParticles[indexParticle]->getNPar();
191	  }
192	  for (unsigned int indexConstraint = 0; indexConstraint < _constraints.size(); indexConstraint++) {
193	    _nParB += _constraints[indexConstraint]->getNPar();
194	  }
195	
196	}
197	
198	void TKinFitter::countUnmeasParams() {
199	  // count number of unmeasured parameters
200	
201	  _nParA = 0;
202	  for (unsigned int indexParticle = 0; indexParticle < _unmeasParticles.size(); indexParticle++) {
203	    _nParA += _unmeasParticles[indexParticle]->getNPar();
204	  }
205	
206	}
207	
208	void TKinFitter::addMeasParticle( TAbsFitParticle* particle ) {
209	  // add one measured particle
210	
211	  resetStatus();
212	
213	  if ( particle != 0 ) {
214	    _measParticles.push_back( particle );
215	  } else {
216	    std::cout <<"NullPointer : " << "Measured particle points to NULL. It will not be added to the KinFitter."<<std::endl;
217	  }
218	
219	  countMeasParams();
220	
221	}
222	
223	void TKinFitter::addMeasParticles( TAbsFitParticle* p1, TAbsFitParticle* p2, TAbsFitParticle* p3, 
224	                              TAbsFitParticle* p4, TAbsFitParticle* p5, TAbsFitParticle* p6,
225	                              TAbsFitParticle* p7, TAbsFitParticle* p8, TAbsFitParticle* p9) {
226	  // add many measured particles
227	
228	  resetStatus();
229	
230	  if ( p1 != 0 ) _measParticles.push_back( p1 );
231	  if ( p2 != 0 ) _measParticles.push_back( p2 );
232	  if ( p3 != 0 ) _measParticles.push_back( p3 );
233	  if ( p4 != 0 ) _measParticles.push_back( p4 );
234	  if ( p5 != 0 ) _measParticles.push_back( p5 );
235	  if ( p6 != 0 ) _measParticles.push_back( p6 );
236	  if ( p7 != 0 ) _measParticles.push_back( p7 );
237	  if ( p8 != 0 ) _measParticles.push_back( p8 );
238	  if ( p9 != 0 ) _measParticles.push_back( p9 );
239	
240	  countMeasParams();
241	
242	}
243	
244	void TKinFitter::addUnmeasParticle( TAbsFitParticle* particle ) {
245	  // add one unmeasured particle
246	
247	  resetStatus();
248	
249	  if ( particle != 0 ) {
250	    _unmeasParticles.push_back( particle );
251	  } else {
252	    std::cout << "NullPointer : " << "Unmeasured particle points to NULL. It will not be added to the KinFitter."<< std::endl;
253	  }
254	
255	  countUnmeasParams();
256	
257	}
258	
259	void TKinFitter::addUnmeasParticles( TAbsFitParticle* p1, TAbsFitParticle* p2, TAbsFitParticle* p3, 
260	                                     TAbsFitParticle* p4, TAbsFitParticle* p5, TAbsFitParticle* p6,
261	                                     TAbsFitParticle* p7, TAbsFitParticle* p8, TAbsFitParticle* p9) {
262	  // add many unmeasured particles
263	
264	  resetStatus();
265	
266	  if ( p1 != 0 ) _unmeasParticles.push_back( p1 );
267	  if ( p2 != 0 ) _unmeasParticles.push_back( p2 );
268	  if ( p3 != 0 ) _unmeasParticles.push_back( p3 );
269	  if ( p4 != 0 ) _unmeasParticles.push_back( p4 );
270	  if ( p5 != 0 ) _unmeasParticles.push_back( p5 );
271	  if ( p6 != 0 ) _unmeasParticles.push_back( p6 );
272	  if ( p7 != 0 ) _unmeasParticles.push_back( p7 );
273	  if ( p8 != 0 ) _unmeasParticles.push_back( p8 );
274	  if ( p9 != 0 ) _unmeasParticles.push_back( p9 );
275	
276	  countUnmeasParams();
277	
278	}
279	
280	void TKinFitter::addConstraint( TAbsFitConstraint* constraint ) {
281	  // add a constraint
282	
283	  resetStatus();
284	
285	  if ( constraint != 0 ) {
286	    _constraints.push_back( constraint );
287	  }
288	
289	  countMeasParams();
290	
291	}
292	
293	void TKinFitter::setVerbosity( Int_t verbosity ) { 
294	  // Set verbosity of the fitter:
295	  // 0: quiet
296	  // 1: print information before and after the fit
297	  // 2: print output for every iteration of the fit
298	  // 3: print debug information
299	
300	
301	  if ( verbosity < 0 ) verbosity = 0;
302	  if ( verbosity > 3 ) verbosity = 3;
303	  _verbosity = verbosity;
304	
305	}
306	
307	
308	Int_t TKinFitter::fit() {
309	  // Perform the fit
310	  // Returns:
311	  // 0: converged
312	  // 1: not converged
313	
314	  resetParams();
315	  resetStatus();
316	
317	  _nbIter = 0;
318	  Bool_t isConverged = false;
319	  Double_t prevF;
320	  Double_t currF = getF();
321	  Double_t prevS;
322	  Double_t currS = 0.;
323	
324	  // Calculate covariance matrix V
325	  calcV();
326	
327	  // print status
328	  if ( _verbosity >= 1 ) {
329	    print();
330	  }
331	
332	  do {
333	
334	    // Reset status to "RUNNING"
335	    // (if it was not aborted already due to singular covariance matrix)
336	    if ( _status != -10 ) {
337	      _status = 10;
338	    }
339	
340	    // Calculate matrices
341	    calcB();
342	    calcVB();
343	    if ( _nParA > 0 ) {
344	      calcA();
345	      calcVA();
346	      calcC32();
347	    }
348	    calcC31();
349	    calcC33();
350	    calcC();
351	
352	    // Calculate corretion for a, y, and lambda
353	    if ( _nParA > 0 ){
354	      calcDeltaA();
355	    }
356	    calcDeltaY();
357	    calcLambda();
358	   
359	    if( _verbosity >= 3 ) {
360	      printMatrix( _A      , "A"      );
361	      printMatrix( _B      , "B"      );
362	      printMatrix( _VBinv  , "VBinv"  );
363	      printMatrix( _VB     , "VB"     );
364	      printMatrix( _V      , "V"      );
365	      printMatrix( _deltaY , "deltaY" );
366	      printMatrix( _deltaA , "deltaA" );
367	      printMatrix( _C32T   , "C32T"   );
368	      printMatrix( _c      , "C"      );
369	    }
370	
371	    if( _verbosity >= 2 ) {
372	      print();   
373	    }
374	
375	    // Apply calculated corrections to measured and unmeasured particles
376	    if ( _nParA > 0 ) {
377	      applyDeltaA();
378	    }
379	    applyDeltaY();
380	
381	    _nbIter++;
382	    
383	    //calculate F and S
384	    prevF = currF;
385	    currF = getF();
386	    prevS = currS;
387	    currS = getS();
388	
389	    if( TMath::IsNaN(currF) ) {
390	      std::cout << "KinFitter : " << "The current value of F is NaN. Fit will be aborted."<<std::endl;
391	      _status = -10;
392	    }
393	    if( TMath::IsNaN(currS) ) {
394	      std::cout << "KinFitter : " << "The current value of S is NaN. Fit will be aborted."<<std::endl;
395	      _status = -10;
396	    }
397	
398	    // Reduce step width if F is not getting smaller
399	    Int_t nstep = 0;
400	    while (currF >= prevF) {
401	      nstep++;
402	      if (nstep == 10) break;
403	      if (_nParA > 0) _deltaA -= (0.5) * (_deltaA - _deltaAstar);
404	      _deltaY -= (0.5) * (_deltaY - _deltaYstar);
405	      _lambda *= 0.5;
406	      _lambdaT *= 0.5;
407	      if (_nParA > 0) applyDeltaA();
408	      applyDeltaY();
409	      currF = getF();
410	      currS = getS();
411	    }
412	    
413	    // Test convergence
414	    isConverged = converged(currF, prevS, currS);
415	
416	 
417	  } while ( (! isConverged) && (_nbIter < _maxNbIter) && (_status != -10) );
418	
419	  // Calculate covariance matrices
420	  calcB();
421	  calcVB();
422	  
423	  if ( _nParA > 0 ) {
424	    calcA();
425	    calcVA();
426	    calcC21();
427	    calcC22();
428	    calcC32();
429	  }
430	  calcC11();
431	  calcC31();
432	  calcC33();
433	  calcVFit();
434	  applyVFit();
435	  
436	  // Set status information
437	  if (isConverged) {
438	    _status = 0;
439	  }
440	  else if (_status != -10) {
441	    _status = 1;
442	  }
443	  
444	  // print status
445	  if ( _verbosity >= 1 ) {
446	    print();
447	  }
448	
449	  return _status;
450	
451	}
452	
453	void TKinFitter::setCovMatrix( TMatrixD &V ) {
454	  // Set the covariance matrix of the measured particles
455	
456	  if ( (V.GetNrows() != _nParB) || (V.GetNcols() != _nParB) ) {
457	    std::cout << "WrongMatrixSize " 
458	      << "Covariance matrix of measured particles needs to be a " << _nParB << "x" << _nParB << " matrix."<<std::endl;
459	  } else {
460	    _V.ResizeTo( V );
461	    _V = V;
462	  }
463	
464	}
465	
466	Bool_t TKinFitter::calcV() {
467	  // Combines the covariance matrices of all measured particles
468	
469	  _V.ResizeTo( _nParB, _nParB );
470	  _V.Zero();
471	
472	  Int_t offsetP = 0;
473	  for (unsigned int iP = 0; iP < _measParticles.size(); iP++) {
474	    TAbsFitParticle* particle = _measParticles[iP];
475	    Int_t nParP = particle->getNPar();
476	    const TMatrixD* covMatrix =  particle->getCovMatrix();
477	
478	    for (int iX = offsetP; iX < offsetP + nParP; iX++) {
479	      for (int iY = offsetP; iY < offsetP + nParP; iY++) {
480	
481	        _V(iX, iY) = (*covMatrix)(iX-offsetP, iY-offsetP);
482	
483	      }
484	    }
485	
486	    offsetP += nParP;
487	  }
488	
489	  for (unsigned int iC = 0; iC < _constraints.size(); iC++) {
490	    TAbsFitConstraint* constraint = _constraints[iC];
491	    Int_t nParP = constraint->getNPar();
492	    const TMatrixD* covMatrix =  constraint->getCovMatrix();
493	
494	    for (int iX = offsetP; iX < offsetP + nParP; iX++) {
495	      for (int iY = offsetP; iY < offsetP + nParP; iY++) {
496	
497	        _V(iX, iY) = (*covMatrix)(iX-offsetP, iY-offsetP);
498	
499	      }
500	    }
501	
502	    offsetP += nParP;
503	  }
504	
505	  _Vinv.ResizeTo( _V );
506	  _Vinv = _V;
507	  try {
508	    _Vinv.Invert();
509	  } catch (std::exception) {
510	    std::cout << "KinFitter " << "Failed to invert covariance matrix V. Fit will be aborted."<<std::endl;
511	    _status = -10;
512	  }
513	
514	  return true;
515	
516	}
517	
518	Bool_t TKinFitter::calcA() {
519	  // Calculate the Jacobi matrix of unmeasured parameters df_i/da_i
520	  // Row i contains the derivatives of constraint f_i. Column q contains
521	  // the derivative wrt. a_q.
522	
523	  _A.ResizeTo( _constraints.size(), _nParA );
524	
525	  for (unsigned int indexConstr = 0; indexConstr < _constraints.size(); indexConstr++) {
526	
527	    int offsetParam = 0;
528	    for (unsigned int indexParticle = 0; indexParticle < _unmeasParticles.size(); indexParticle++) {
529	
530	      // Calculate matrix product  df/dP * dP/dy = (df/dr, df/dtheta, df/dphi, ...)
531	
532	      TAbsFitParticle* particle = _unmeasParticles[indexParticle];
533	      TMatrixD* derivParticle = particle->getDerivative();
534	      TMatrixD* derivConstr = _constraints[indexConstr]->getDerivative( particle );
535	      TMatrixD deriv( *derivConstr, TMatrixD::kMult, *derivParticle );
536	
537	      for (int indexParam = 0; indexParam < deriv.GetNcols(); indexParam++) {
538	        _A(indexConstr,indexParam+offsetParam) = deriv(0, indexParam);
539	      }
540	      offsetParam += deriv.GetNcols();
541	
542	      delete derivParticle;
543	      delete derivConstr;
544	
545	    }
546	  }
547	
548	  // Calculate transposed matrix
549	  TMatrixD AT(TMatrixD::kTransposed, _A);
550	  _AT.ResizeTo( AT );
551	  _AT = AT;
552	
553	  return true;
554	
555	}
556	
557	Bool_t TKinFitter::calcB() {
558	  // Calculate the Jacobi matrix of measured parameters df_i/da_i
559	  // Row i contains the derivatives of constraint f_i. Column q contains
560	  // the derivative wrt. a_q.
561	
562	  _B.ResizeTo( _constraints.size(), _nParB );
563	
564	  int offsetParam = 0;
565	  for (unsigned int indexConstr = 0; indexConstr < _constraints.size(); indexConstr++) {
566	
567	    offsetParam = 0;
568	    for (unsigned int indexParticle = 0; indexParticle < _measParticles.size(); indexParticle++) {
569	
570	      // Calculate matrix product  df/dP * dP/dy = (df/dr, df/dtheta, df/dphi, ...)
571	      TAbsFitParticle* particle = _measParticles[indexParticle];
572	      TMatrixD* derivParticle = particle->getDerivative();
573	      TMatrixD* derivConstr = _constraints[indexConstr]->getDerivative( particle );
574	      TMatrixD deriv( *derivConstr,  TMatrixD::kMult, *derivParticle );
575	      if (_verbosity >= 3) {
576	        TString matrixName = "B deriv: Particle -> ";
577	        matrixName += particle->GetName();
578	        printMatrix( *derivParticle, matrixName );
579	        matrixName = "B deriv: Constraint -> ";
580	        matrixName += _constraints[indexConstr]->GetName();
581	        matrixName += " , Particle -> ";
582	        matrixName += particle->GetName();
583	        printMatrix( *derivConstr, matrixName );
584	      } 
585	      for (int indexParam = 0; indexParam < deriv.GetNcols(); indexParam++) {
586	        _B(indexConstr,indexParam+offsetParam) = deriv(0, indexParam);
587	      }
588	      offsetParam += deriv.GetNcols();
589	
590	      delete derivParticle;
591	      delete derivConstr;
592	
593	    }
594	  }
595	
596	  for (unsigned int iC = 0; iC < _constraints.size(); iC++) {
597	
598	    TAbsFitConstraint* constraint = _constraints[iC];
599	    TMatrixD* deriv = constraint->getDerivativeAlpha();
600	
601	    if (deriv != 0) {
602	
603	      if (_verbosity >= 3) {
604	        TString matrixName = "B deriv alpha: Constraint -> ";
605	        matrixName += constraint->GetName();
606	        printMatrix( *deriv, matrixName );
607	      } 
608	      for (int indexParam = 0; indexParam < deriv->GetNcols(); indexParam++) {
609	        _B( iC, indexParam+offsetParam ) = (*deriv)(0, indexParam);
610	      }
611	      offsetParam += deriv->GetNcols();
612	      
613	      delete deriv;
614	    }
615	  }
616	
617	  TMatrixD BT( TMatrixD::kTransposed,  _B );
618	  _BT.ResizeTo( BT );
619	  _BT = BT;
620	
621	  return true;
622	
623	}
624	
625	Bool_t TKinFitter::calcVB() {
626	  // Calculate the matrix V_B = (B*V*B^T)^-1
627	
628	  TMatrixD BV( _B, TMatrixD::kMult, _V );
629	  TMatrixD VBinv( BV, TMatrixD::kMult, _BT );
630	  _VBinv.ResizeTo( VBinv );
631	  _VBinv = VBinv;
632	
633	  _VB.ResizeTo( _VBinv );
634	  _VB = _VBinv;
635	  try {
636	    _VB.Invert();
637	  } catch (std::exception) {
638	    std::cout << "KinFitter " << "Failed to invert matrix VB. Fit will be aborted."<<std::endl;
639	    _status = -10;
640	  }
641	
642	  return true;
643	
644	}
645	
646	Bool_t TKinFitter::calcVA() {
647	  // Calculate the matrix VA = (A^T*VB*A)
648	
649	  TMatrixD ATVB( _AT, TMatrixD::kMult, _VB );
650	  TMatrixD VA(ATVB, TMatrixD::kMult, _A);
651	  _VA.ResizeTo( VA );
652	  _VA = VA;
653	
654	  _VAinv.ResizeTo( _VA );
655	  _VAinv = _VA;
656	  try {
657	    _VAinv.Invert();
658	  } catch (std::exception) {
659	    std::cout <<"KinFitter " << "Failed to invert matrix VA. Fit will be aborted."<<std::endl;
660	    _status = -10;
661	  }
662	
663	  return true;
664	
665	}
666	
667	Bool_t TKinFitter::calcC11() {
668	  // Calculate the matrix C11 = V^(-1) - V^(-1)*BT*VB*B*V^(-1) + V^(-1)*BT*VB*A*VA^(-1)*AT*VB*B*V^(-1)
669	
670	  TMatrixD VBT( _V, TMatrixD::kMult, _BT );
671	  TMatrixD VBB( _VB, TMatrixD::kMult, _B );
672	  TMatrixD VBTVBB( VBT, TMatrixD::kMult, VBB );
673	  TMatrixD m2( VBTVBB,  TMatrixD::kMult, _V );
674	
675	  _C11.ResizeTo( _V );
676	  _C11 = _V;
677	  _C11 -= m2;
678	
679	  if ( _nParA > 0 ) {
680	    TMatrixD VBA( _VB, TMatrixD::kMult, _A );
681	    TMatrixD VBTVBA( VBT, TMatrixD::kMult, VBA );
682	    TMatrixD VAinvAT( _VAinv, TMatrixD::kMult, _AT );
683	    TMatrixD VBTVBAVAinvAT( VBTVBA, TMatrixD::kMult, VAinvAT );
684	    TMatrixD VBTVBAVAinvATVBB( VBTVBAVAinvAT, TMatrixD::kMult, VBB );
685	    TMatrixD m3( VBTVBAVAinvATVBB, TMatrixD::kMult, _V );
686	    _C11 += m3;
687	  }
688	
689	  TMatrixD C11T( TMatrixD::kTransposed,  _C11 );
690	  _C11T.ResizeTo( C11T );
691	  _C11T = C11T;
692	
693	  return true;
694	
695	}
696	
697	Bool_t TKinFitter::calcC21() {
698	  // Calculate the matrix  C21 = -VA^(-1)*AT*VB*B*V^(-1)
699	
700	  TMatrixD VAinvAT( _VAinv, TMatrixD::kMult, _AT );
701	  TMatrixD VBB( _VB, TMatrixD::kMult, _B );
702	  TMatrixD VAinvATVBB( VAinvAT, TMatrixD::kMult, VBB );
703	  TMatrixD C21( VAinvATVBB, TMatrixD::kMult, _V );
704	  C21 *= -1.;
705	  _C21.ResizeTo( C21 );
706	  _C21 = C21;
707	  
708	  TMatrixD C21T( TMatrixD::kTransposed,  _C21 );
709	  _C21T.ResizeTo( C21T );
710	  _C21T = C21T;
711	
712	  return true;
713	
714	}
715	
716	Bool_t TKinFitter::calcC22() {
717	  //  Calculate the matrix C22 = VA^(-1)
718	
719	  _C22.ResizeTo( _VAinv );
720	  _C22 = _VAinv;
721	
722	  TMatrixD C22T( TMatrixD::kTransposed,  _C22 );
723	  _C22T.ResizeTo( C22T );
724	  _C22T = C22T;
725	
726	  return true;
727	
728	}
729	
730	Bool_t TKinFitter::calcC31() {
731	  // Calculate the matrix  C31 = VB*B*V^(-1) - VB*A*VA^(-1)*AT*VB*B*V^(-1)
732	
733	  TMatrixD VbB(_VB, TMatrixD::kMult, _B);
734	  TMatrixD m1( VbB, TMatrixD::kMult, _V );
735	
736	  _C31.ResizeTo( m1 );
737	  _C31 = m1;
738	
739	  if ( _nParA > 0 ) {
740	    TMatrixD VbA(_VB, TMatrixD::kMult, _A);
741	    TMatrixD VAinvAT( _VAinv, TMatrixD::kMult, _AT );
742	    TMatrixD VbBV( VbB,  TMatrixD::kMult, _V );
743	    TMatrixD VbAVAinvAT(VbA, TMatrixD::kMult, VAinvAT); 
744	    TMatrixD m2(VbAVAinvAT, TMatrixD::kMult, VbBV);
745	
746	    _C31 -= m2;
747	  }
748	
749	  TMatrixD C31T( TMatrixD::kTransposed,  _C31 );
750	  _C31T.ResizeTo( C31T );
751	  _C31T = C31T;
752	
753	  return true;
754	
755	}
756	
757	Bool_t TKinFitter::calcC32() {
758	  // Calculate the matrix  C32 = VB*A*VA^(-1)
759	
760	  TMatrixD VbA( _VB, TMatrixD::kMult, _A );
761	  TMatrixD C32( VbA, TMatrixD::kMult, _VAinv );
762	  _C32.ResizeTo( C32 );
763	  _C32 = C32;
764	
765	  TMatrixD C32T( TMatrixD::kTransposed,  _C32 );
766	  _C32T.ResizeTo( C32T );
767	  _C32T = C32T;
768	
769	  return true;
770	
771	}
772	
773	Bool_t TKinFitter::calcC33() {
774	  // Calculate the matrix C33 = -VB + VB*A*VA^(-1)*AT*VB
775	
776	  _C33.ResizeTo( _VB );
777	  _C33 = _VB;
778	  _C33 *= -1.;
779	
780	  if ( _nParA > 0 ) {
781	    TMatrixD VbA(_VB, TMatrixD::kMult, _A );
782	    TMatrixD VAinvAT( _VAinv, TMatrixD::kMult, _AT );
783	    TMatrixD VbAVAinvAT( VbA, TMatrixD::kMult, VAinvAT );
784	    TMatrixD C33( VbAVAinvAT,  TMatrixD::kMult, _VB );
785	    _C33 += C33;
786	  }
787	
788	  TMatrixD C33T( TMatrixD::kTransposed,  _C33 );
789	  _C33T.ResizeTo( C33T );
790	  _C33T = C33T;
791	
792	  return true;
793	}
794	
795	Bool_t TKinFitter::calcC() {
796	  // Calculate the matrix c = A*deltaAStar + B*deltaYStar - fStar
797	
798	  int offsetParam = 0;
799	
800	  // calculate delta(a*), = 0 in the first iteration
801	  TMatrixD deltaastar( 1, 1 );
802	  if ( _nParA > 0 ) {
803	
804	    deltaastar.ResizeTo( _nParA, 1 );
805	    for (unsigned int indexParticle = 0; indexParticle < _unmeasParticles.size(); indexParticle++) {
806	    
807	      TAbsFitParticle* particle = _unmeasParticles[indexParticle];
808	      const TMatrixD* astar = particle->getParCurr();
809	      const TMatrixD* a = particle->getParIni();
810	      TMatrixD deltaastarpart(*astar);
811	      deltaastarpart -= *a;
812	      
813	      for (int indexParam = 0; indexParam < deltaastarpart.GetNrows(); indexParam++) {
814	        deltaastar(indexParam+offsetParam, 0) = deltaastarpart(indexParam, 0);
815	      }
816	      offsetParam += deltaastarpart.GetNrows();
817	      
818	    }
819	
820	    if ( _verbosity >= 3 ) {
821	      printMatrix( deltaastar, "deltaastar" );
822	    }
823	
824	  }
825	
826	  // calculate delta(y*), = 0 in the first iteration
827	  TMatrixD deltaystar( _nParB, 1 );
828	  offsetParam = 0;
829	  for (unsigned int indexParticle = 0; indexParticle < _measParticles.size(); indexParticle++) {
830	
831	    TAbsFitParticle* particle = _measParticles[indexParticle];
832	    const TMatrixD* ystar = particle->getParCurr();
833	    const TMatrixD* y = particle->getParIni();
834	    TMatrixD deltaystarpart(*ystar);
835	    deltaystarpart -= *y;
836	
837	    for (int indexParam = 0; indexParam < deltaystarpart.GetNrows(); indexParam++) {
838	      deltaystar(indexParam+offsetParam, 0) = deltaystarpart(indexParam, 0);
839	    }
840	    offsetParam += deltaystarpart.GetNrows();
841	
842	  } 
843	
844	  for (unsigned int iC = 0; iC < _constraints.size(); iC++) {
845	
846	    TAbsFitConstraint* constraint = _constraints[iC];
847	    
848	    if ( constraint->getNPar() > 0 ) {
849	
850	      const TMatrixD* alphastar = constraint->getParCurr();
851	      const TMatrixD* alpha = constraint->getParIni();
852	
853	      TMatrixD deltaalphastarpart(*alphastar);
854	      deltaalphastarpart -= *alpha;
855	
856	      for (int indexParam = 0; indexParam < deltaalphastarpart.GetNrows(); indexParam++) {
857	        deltaystar(indexParam+offsetParam, 0) = deltaalphastarpart(indexParam, 0);
858	      }
859	      offsetParam += deltaalphastarpart.GetNrows();
860	
861	    }
862	  }
863	
864	  if ( _verbosity >= 3 ) {
865	    printMatrix( deltaystar, "deltaystar" );
866	  }
867	
868	  // calculate f*
869	  TMatrixD fstar( _constraints.size(), 1 );
870	  for (unsigned int indexConstr = 0; indexConstr < _constraints.size(); indexConstr++) {
871	    fstar( indexConstr, 0 ) = _constraints[indexConstr]->getCurrentValue();
872	  }
873	
874	  // calculate c
875	  _c.ResizeTo( fstar );
876	  _c = fstar;
877	  _c *= (-1.);
878	  TMatrixD Bdeltaystar( _B, TMatrixD::kMult, deltaystar );
879	  _c += Bdeltaystar;
880	  if ( _nParA ) {
881	    TMatrixD Adeltaastar( _A, TMatrixD::kMult, deltaastar );
882	    _c += Adeltaastar;
883	  }
884	
885	  return true;
886	
887	}
888	
889	Bool_t TKinFitter::calcDeltaA() {
890	  // Calculate the matrix deltaA = C32T * c
891	  // (corrections to unmeasured parameters)
892	
893	  TMatrixD deltaA( _C32T, TMatrixD::kMult, _c );
894	  _deltaA.ResizeTo( deltaA );
895	
896	  if (_nbIter == 0) {
897	    _deltaAstar.ResizeTo(deltaA);
898	    _deltaAstar.Zero();
899	  } else
900	    _deltaAstar = _deltaA;
901	
902	  _deltaA = deltaA;
903	
904	  return true;
905	
906	}
907	
908	Bool_t TKinFitter::calcDeltaY() {
909	  // Calculate the matrix deltaY = C31T * c 
910	  // (corrections to measured parameters)
911	
912	  TMatrixD deltaY( _C31T, TMatrixD::kMult, _c );
913	  _deltaY.ResizeTo( deltaY );
914	
915	  if (_nbIter == 0) {
916	    _deltaYstar.ResizeTo(deltaY);
917	    _deltaYstar.Zero();
918	  } else
919	    _deltaYstar = _deltaY;
920	
921	  _deltaY = deltaY;
922	
923	  return true;
924	
925	}
926	
927	Bool_t TKinFitter::calcLambda() {
928	  // Calculate the matrix Lambda = C33 * c 
929	  // (Lagrange Multipliers)
930	
931	  TMatrixD lambda( _C33,  TMatrixD::kMult, _c );
932	  _lambda.ResizeTo( lambda );
933	  _lambda = lambda;
934	
935	  TMatrixD lambdaT( TMatrixD::kTransposed,  _lambda );
936	  _lambdaT.ResizeTo( lambdaT );
937	  _lambdaT = lambdaT;
938	
939	  return true;
940	
941	}
942	
943	Bool_t TKinFitter::calcVFit() {
944	  // Calculate the covariance matrix of fitted parameters
945	  //
946	  // Vfit(y) = ( C11  C21T )
947	  //     (a)   ( C21  C22  )
948	  //
949	  // Vfit(lambda) = (-C33)
950	  
951	  // Calculate covariance matrix of lambda
952	  _lambdaVFit.ResizeTo( _C33 );
953	  _lambdaVFit = _C33;
954	  _lambdaVFit *= -1.;
955	
956	
957	  // Calculate combined covariance matrix of y and a
958	  Int_t nbRows = _C11.GetNrows();
959	  Int_t nbCols = _C11.GetNcols();
960	  if ( _nParA > 0 ) {
961	    nbRows += _C21.GetNrows();
962	    nbCols += _C21T.GetNcols();
963	  }
964	  _yaVFit.ResizeTo( nbRows, nbCols );
965	
966	  for (int iRow = 0; iRow < nbRows; iRow++) {
967	    for (int iCol = 0; iCol < nbCols; iCol++) {
968	
969	      if (iRow >= _C11.GetNrows()) {
970	        if (iCol >= _C11.GetNcols()) {
971	          _yaVFit(iRow, iCol) = _C22(iRow-_C11.GetNrows(), iCol-_C11.GetNcols());
972	        } else {
973	          _yaVFit(iRow, iCol) = _C21(iRow-_C11.GetNrows(), iCol);
974	        }
975	      } else {
976	        if (iCol >= _C11.GetNcols()) {
977	          _yaVFit(iRow, iCol) = _C21T(iRow, iCol-_C11.GetNcols());
978	        } else {
979	          _yaVFit(iRow, iCol) = _C11(iRow, iCol);
980	        }
981	      }
982	
983	    }
984	  }
985	
986	  return true;
987	
988	}
989	
990	void TKinFitter::applyVFit() {
991	  // apply fit covariance matrix to measured and unmeasured  particles
992	
993	  int offsetParam = 0;
994	  for (unsigned int indexParticle = 0; indexParticle < _measParticles.size(); indexParticle++) {
995	    TAbsFitParticle* particle = _measParticles[indexParticle];
996	    Int_t nbParams = particle->getNPar();
997	    TMatrixD vfit( nbParams, nbParams );
998	    for (Int_t c = 0; c < nbParams; c++) {
999	      for (Int_t r = 0; r < nbParams; r++) {
1000	        vfit(r, c) = _yaVFit(r + offsetParam, c + offsetParam);
1001	      }
1002	    }
1003	    particle->setCovMatrixFit( &vfit );
1004	    offsetParam += nbParams;
1005	  }
1006	
1007	  for (unsigned int indexConstraint = 0; indexConstraint < _constraints.size(); indexConstraint++) {
1008	    TAbsFitConstraint* constraint = _constraints[indexConstraint];
1009	    Int_t nbParams = constraint->getNPar();
1010	    if (nbParams > 0) {
1011	      TMatrixD vfit( nbParams, nbParams );
1012	      for (Int_t c = 0; c < nbParams; c++) {
1013	        for (Int_t r = 0; r < nbParams; r++) {
1014	          vfit(r, c) = _yaVFit(r + offsetParam, c + offsetParam);
1015	        }
1016	      }
1017	      constraint->setCovMatrixFit( &vfit );
1018	      offsetParam += nbParams;
1019	    }
1020	  }
1021	
1022	  for (unsigned int indexParticle = 0; indexParticle < _unmeasParticles.size(); indexParticle++) {
1023	    TAbsFitParticle* particle = _unmeasParticles[indexParticle];
1024	    Int_t nbParams = particle->getNPar();
1025	    TMatrixD vfit( nbParams, nbParams );
1026	    for (Int_t c = 0; c < nbParams; c++) {
1027	      for (Int_t r = 0; r < nbParams; r++) {
1028	        vfit(r, c) = _yaVFit(r + offsetParam, c + offsetParam);
1029	      }
1030	    }
1031	    particle->setCovMatrixFit( &vfit );
1032	    offsetParam += nbParams;
1033	  }
1034	
1035	}
1036	
1037	Bool_t TKinFitter::applyDeltaA() {
1038	  //apply corrections to unmeasured particles
1039	
1040	  int offsetParam = 0;
1041	  for (unsigned int indexParticle = 0; indexParticle < _unmeasParticles.size(); indexParticle++) {
1042	
1043	    TAbsFitParticle* particle = _unmeasParticles[indexParticle];
1044	    Int_t nbParams = particle->getNPar();
1045	    TMatrixD params( nbParams, 1 );
1046	    for (Int_t index = 0; index < nbParams; index++) {
1047	      params(index, 0) = _deltaA(index+offsetParam, 0);
1048	    }
1049	    particle->applycorr( &params );
1050	    offsetParam+=nbParams;
1051	
1052	  }
1053	
1054	  return true;
1055	
1056	}
1057	
1058	Bool_t TKinFitter::applyDeltaY() {
1059	  //apply corrections to measured particles
1060	
1061	  int offsetParam = 0;
1062	  for (unsigned int indexParticle = 0; indexParticle < _measParticles.size(); indexParticle++) {
1063	
1064	    TAbsFitParticle* particle = _measParticles[indexParticle];
1065	    Int_t nbParams = particle->getNPar();
1066	    TMatrixD params( nbParams, 1 );
1067	    for (Int_t index = 0; index < nbParams; index++) {
1068	      params(index, 0) = _deltaY(index+offsetParam, 0);
1069	    }
1070	    particle->applycorr( &params );
1071	    offsetParam+=nbParams;
1072	
1073	  }
1074	
1075	  for (unsigned int indexConstraint = 0; indexConstraint < _constraints.size(); indexConstraint++) {
1076	
1077	    TAbsFitConstraint* constraint = _constraints[indexConstraint];
1078	    Int_t nbParams = constraint->getNPar();
1079	    if ( nbParams > 0 ) {
1080	      TMatrixD params( nbParams, 1 );
1081	      for (Int_t index = 0; index < nbParams; index++) {
1082	        params(index, 0) = _deltaY(index+offsetParam, 0);
1083	      }
1084	      constraint->applyDeltaAlpha( &params );
1085	      offsetParam+=nbParams;
1086	    }
1087	
1088	  }
1089	
1090	  return true;
1091	
1092	}
1093	  
1094	Double_t TKinFitter::getF() {
1095	  // calculate current absolut value of constraints
1096	  // F = Sum[ Abs(f_k( aStar, yStar)) ] 
1097	
1098	  Double_t F = 0.;
1099	  for (unsigned int indexConstr = 0; indexConstr < _constraints.size(); indexConstr++) {
1100	    F += TMath::Abs( _constraints[indexConstr]->getCurrentValue() );
1101	    //std::cout << "_constraints[indexConstr]->getCurrentValue() "<<_constraints[indexConstr]->getCurrentValue()<<std::endl;
1102	  }
1103	  
1104	  return F;
1105	
1106	}
1107	
1108	Double_t TKinFitter::getS() {
1109	  // calculate current value of Chi2
1110	  // S = deltaYT * V^-1 * deltaY
1111	
1112	  Double_t S = 0.;
1113	
1114	  if ( _nbIter > 0 ) {
1115	    
1116	    TMatrixD deltaYTVinv(_deltaY, TMatrixD::kTransposeMult, _Vinv);
1117	    TMatrixD S2(deltaYTVinv, TMatrixD::kMult, _deltaY);
1118	    S = S2(0,0);
1119	         
1120	  }
1121	
1122	  return S;
1123	
1124	}
1125	
1126	Bool_t TKinFitter::converged( Double_t F, Double_t prevS, Double_t currS ) {
1127	  // check whether convergence criteria are fulfilled
1128	
1129	  Bool_t isConverged = false;
1130	  
1131	  // calculate F, delta(a) and delta(y) already applied
1132	  isConverged = (F < _maxF);
1133	
1134	  // Calculate current Chi^2 and delta(S)
1135	  Double_t deltaS = currS - prevS;
1136	  isConverged = isConverged && (TMath::Abs(deltaS) < _maxDeltaS);
1137	
1138	  return isConverged;
1139	
1140	}
1141	
1142	TString TKinFitter::getStatusString() {
1143	
1144	  TString statusstring = "";
1145	
1146	  switch ( _status ) {
1147	      
1148	    case -1: {
1149	      statusstring = "NO FIT PERFORMED";
1150	      break;
1151	    }
1152	    case 10: {
1153	      statusstring = "RUNNING";
1154	      break;
1155	    }
1156	    case -10: {
1157	      statusstring = "ABORTED";
1158	      break;
1159	    }
1160	    case 0: {
1161	      statusstring = "CONVERGED";
1162	      break;
1163	    }
1164	    case 1: {
1165	      statusstring = "NOT CONVERGED";
1166	      break;
1167	    }
1168	  }
1169	    
1170	  return statusstring;
1171	
1172	}
1173	
1174	void TKinFitter::print() {
1175	
1176	  
1177	  // Print status of fit
1178	  std::cout << "Status: " << getStatusString()
1179	      << "   F=" << getF() << "   S=" << getS() << "   N=" << _nbIter << "   NDF=" << getNDF() << "\n";
1180	  // Print measured particles
1181	  std::cout << "measured particles: \n";
1182	  Int_t parIndex = 0;
1183	  for (unsigned int iP = 0; iP < _measParticles.size(); iP++) {
1184	    TAbsFitParticle* particle = _measParticles[iP];
1185	    Int_t nParP = particle->getNPar();
1186	    const TMatrixD* par = particle->getParCurr();
1187	    const TMatrixD* covP = particle->getCovMatrix();
1188	    std::cout << std::setw(3) << setiosflags(std::ios::right) << iP;
1189	    std::cout << std::setw(15) << setiosflags(std::ios::right) << particle->GetName();
1190	    std::cout << std::setw(3) << " ";
1191	    for (int iPar = 0; iPar < nParP; iPar++) {
1192	      if (iPar > 0) {
1193	        std::cout << setiosflags(std::ios::right) << std::setw(21) << " ";
1194	      }
1195	      TString colstr = "";
1196	      colstr += parIndex;
1197	      colstr += ":";
1198	      std::cout << std::setw(4) << colstr;
1199	      std::cout << std::setw(2) << " ";   
1200	      std::cout << setiosflags(std::ios::left) << setiosflags(std::ios::scientific) << std::setprecision(3);
1201	      std::cout << std::setw(15) << (*par)(iPar, 0);
1202	      if(_nbIter > 0 && _status < 10) {
1203	        std::cout << std::setw(15) << TMath::Sqrt( _yaVFit(iPar, iPar) );
1204	      } else {
1205	        std::cout << std::setw(15) << " ";
1206	      }
1207	      std::cout << std::setw(15) << TMath::Sqrt( (*covP)(iPar, iPar) );
1208	      std::cout << "\n";
1209	      parIndex++;
1210	    }
1211	    std::cout << particle->getInfoString();
1212	  }
1213	  // Print unmeasured particles
1214	  std::cout<< "unmeasured particles: \n";
1215	  parIndex = 0;
1216	  for (unsigned int iP = 0; iP < _unmeasParticles.size(); iP++) {
1217	    TAbsFitParticle* particle = _unmeasParticles[iP];
1218	    Int_t nParP = particle->getNPar();
1219	    const TMatrixD* par = particle->getParCurr();
1220	    std::cout << std::setw(3) << setiosflags(std::ios::right) << iP;
1221	    std::cout << std::setw(15) << particle->GetName();
1222	    std::cout << std::setw(3) << " ";
1223	    for (int iPar = 0; iPar < nParP; iPar++) {
1224	      if (iPar > 0) {
1225	        std::cout << setiosflags(std::ios::right) << std::setw(21) << " ";
1226	      }
1227	      TString colstr = "";
1228	      colstr += parIndex;
1229	      colstr += ":";
1230	      std::cout << std::setw(4) << colstr;
1231	      std::cout << std::setw(2) << " ";
1232	      std::cout << setiosflags(std::ios::left) << setiosflags(std::ios::scientific) << std::setprecision(3);
1233	      std::cout << std::setw(15) << (*par)(iPar, 0);
1234	      if(_nbIter > 0 && _status < 10) {
1235	        std::cout << std::setw(15) << TMath::Sqrt( _yaVFit(iPar+_nParB, iPar+_nParB) );
1236	      } else {
1237	        std::cout << std::setw(15) << " ";
1238	      }
1239	      std::cout << "\n";
1240	      parIndex++;
1241	    }
1242	    std::cout << particle->getInfoString();
1243	  }
1244	  std::cout << "\n";
1245	  // Print constraints
1246	  std::cout << "constraints: \n";
1247	  for (unsigned int indexConstr = 0; indexConstr < _constraints.size(); indexConstr++) {
1248	    std::cout << _constraints[indexConstr]->getInfoString();
1249	  }
1250	  std::cout << "\n";
1251	
1252	}
1253	
1254	void TKinFitter::printMatrix(const TMatrixD &matrix, const TString name) {
1255	  // produce a tabular printout for matrices
1256	  // this function is a modified version of Root's TMatrixTBase<Element>::Print method
1257	  // which could not be used together with the MessageLogger
1258	
1259	  if (!matrix.IsValid()) {
1260	    std::cout << "InvalidMatrix : " << "Matrix " << name << " is invalid."<<std::endl;
1261	    return;
1262	  }
1263	
1264	
1265	  const Int_t nCols  = matrix.GetNcols();
1266	  const Int_t nRows  = matrix.GetNrows();
1267	  const Int_t colsPerSheet = 5;
1268	  char topbar[100];
1269	  Int_t nk = 6+13*TMath::Min(colsPerSheet, nCols);
1270	  for(Int_t i = 0; i < nk; i++) topbar[i] = '-';
1271	  topbar[nk] = 0;
1272	
1273	  Int_t sw = (70-name.Length())/2;
1274	 
1275	  std::cout << std::setfill('=') << std::setw(sw) << "=  " << name << std::setw(sw) << std::left << "  ="  << "\n"
1276	      << std::setfill(' ') << std::right << "\n";
1277	
1278	  std::cout << nRows << "x" << nCols << " matrix is as follows \n";
1279	
1280	  for (Int_t iSheet = 0; iSheet < nCols; iSheet += colsPerSheet) {
1281	    std::cout << "\n"
1282	        << "     |";
1283	    for (Int_t iCol = iSheet; iCol < iSheet+colsPerSheet && iCol < nCols; iCol++)
1284	      std::cout << std::setw(8) << iCol << "    |";
1285	    std::cout << "\n"
1286	        << topbar << " \n";
1287	    for(Int_t iRow = 0; iRow < nRows; iRow++) {
1288	      std::cout << std::setw(4) << iRow << " |";
1289	      for (Int_t iCol = iSheet; iCol < iSheet+colsPerSheet && iCol < nCols; iCol++)
1290	        std::cout << std::setw(12) << matrix(iRow, iCol) << " ";
1291	      std::cout << "\n";
1292	    }
1293	  }
1294	
1295	}
